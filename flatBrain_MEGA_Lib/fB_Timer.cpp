/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Code by Simon Monk
 http://www.simonmonk.org
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "fB_Include.h"

fB_Event::fB_Event(void)
{
	eventType = EVENT_NONE;
	arg16 = 0;
	pT = 0;
}

void fB_Timer::stop(uint8_t id){
	if (id >= 0 && id < _TIMER_MAX_EVENTS) {
		_events[id].eventType = EVENT_NONE;
		// in case repeatCount == -1 (forever) , stop at start value
		if(_events[id].repeatCount == -1 &&_events[id].eventType == EVENT_OSCILLATE_TAG) _events[id].pT->write(_events[id].startState);
	}
}

void fB_Timer::update(void){
	for (int i = 0; i < _TIMER_MAX_EVENTS; i++)	{
		if (_events[i].eventType != EVENT_NONE)	_events[i].update();
	}
}

bool fB_Timer::update(uint8_t index){
	bool action = false;
	if (_events[index].eventType != EVENT_NONE)	action = _events[index].update();
	return action;
}
void fB_Timer::updateWarn() {

        timer.update(_TIMER_WARNDELAY);
        timer.update(_TIMER_WARN);
        timer.update(_TIMER_ALARM);
}

int fB_Timer::findFreeEventIndex(void)
{
	for (int i = _TIMER_FREEINDEXSTART; i < _TIMER_MAX_EVENTS; i++)
	{
		if (_events[i].eventType == EVENT_NONE)	return i;
	}
	return NO_TIMER_AVAILABLE;
}



bool fB_Event::update(void)
{
	bool action = false;
	unsigned long now = millis();
	//dbug(F("Event update a16:%d eventyp:%d, now:%d, let:%d, p:%d"),arg16 ,eventType,(uint16_t) now,(uint16_t) lastEventTime,(uint16_t) period);
	//Serial.println(now);
	//Serial.println(lastEventTime);
	//Serial.println(period);
	if (now - lastEventTime >= period)
	{
		action = true;
		switch (eventType)
		{
			case EVENT_EVERY:
	//dbug(F("*****EventY typ:%d repeat:%d , count:%d"),eventType,repeatCount,count);

				(*callback)(arg16);
				break;

			case EVENT_OSCILLATE_PIN:
				currState = ! currState;
				digitalWrite(pin, currState);
				break;

			case EVENT_OSCILLATE_TAG:
				currState = ! currState;
				pT->write(currState);
				break;
		}
	//dbug(F("EventX "));
	//Serial.println(now);
	//Serial.println(lastEventTime);
	    lastEventTime = now;
		count++;
	}
	if (repeatCount > -1 && count >= repeatCount) eventType = EVENT_NONE;
	return action;
}

fB_Timer::fB_Timer(void){}

void fB_Timer::setPeriod(uint8_t index, unsigned long period){
	_events[index].period = period;
}

int fB_Timer::every(uint8_t index, unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16){
	//dbug(F("TIMER every index:%d period:%d rC:%d  a16:%d"),index,(uint16_t) period,repeatCount,arg16);

	_events[index].eventType = EVENT_EVERY;
	_events[index].arg16 = arg16;
	_events[index].period = period;
	_events[index].repeatCount = repeatCount;
	_events[index].callback = callback;
	_events[index].lastEventTime = millis();
	_events[index].count = 0;
	return index;
}
int fB_Timer::every(unsigned long period,  int repeatCount, void (*callback)(uint16_t), uint16_t arg16){
	uint8_t i = findFreeEventIndex();
	if (i == -1) return -1;
	else return every(i, period,repeatCount, callback, arg16);
}

int fB_Timer::repeat(uint8_t index,unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16){
	return every(index, period,repeatCount, callback, arg16); 
}
int fB_Timer::repeat(unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16){
	return every(period, repeatCount, callback, arg16); 
}
int fB_Timer::perpetual(uint8_t index,unsigned long period, void (*callback)(uint16_t), uint16_t arg16){
	return every(index, period,-1, callback,  arg16); 
}
int fB_Timer::perpetual(unsigned long period, void (*callback)(uint16_t), uint16_t arg16){
	return every(period, -1,callback,  arg16); 
}

int fB_Timer::after(uint8_t index,unsigned long period, void (*callback)(uint16_t), uint16_t arg16)
{
	return every(index,period,  1,callback, arg16);
}
int fB_Timer::after(unsigned long period, void (*callback)(uint16_t), uint16_t arg16)
{
	return every(period,  1, callback, arg16);
}

int fB_Timer::oscillate(uint8_t pin, unsigned long period, uint16_t startingValue, int repeatCount)
{
	uint8_t i = findFreeEventIndex();
	if (i == NO_TIMER_AVAILABLE) return NO_TIMER_AVAILABLE;

	_events[i].eventType = EVENT_OSCILLATE_PIN;
	_events[i].pin = pin;
	_events[i].period = period;
	_events[i].currState = startingValue;
	digitalWrite(pin, startingValue);
	_events[i].repeatCount = repeatCount * 2; // full cycles not transitions
	_events[i].lastEventTime = millis();
	_events[i].count = 0;
	return i;
}
int fB_Timer::oscillateTag(uint8_t index,uint16_t tag, unsigned long period, uint16_t startingValue, int repeatCount)
{

	_events[index].eventType = EVENT_OSCILLATE_TAG;
	_events[index].pT = Tag(tag);
	_events[index].period = period;
	_events[index].startState = startingValue;
	_events[index].currState = startingValue;
	_events[index].pT->write(startingValue);
	_events[index].repeatCount = repeatCount * 2; // full cycles not transitions
	_events[index].lastEventTime = millis();
	_events[index].count = 0;
	return index;
}

int fB_Timer::oscillate(uint8_t pin, unsigned long period, uint16_t startingValue)
{
	return oscillate(pin, period, startingValue, -1); // forever
}

/**
 * This method will generate a pulse of !startingValue, occuring period after the
 * call of this method and lasting for period. The Pin will be left in !startingValue.
 */
int fB_Timer::pulse(uint8_t pin, unsigned long period, uint16_t startingValue)
{
	return oscillate(pin, period, startingValue, 1); // once
}

/**
 * This method will generate a pulse of startingValue, starting immediately and of
 * length period. The pin will be left in the !startingValue state
 */
int fB_Timer::pulseImmediate(uint8_t pin, unsigned long period, uint16_t pulseValue)
{
	int id(oscillate(pin, period, pulseValue, 1));
	// now fix the repeat count
	if (id >= 0 && id < _TIMER_MAX_EVENTS) {
		_events[id].repeatCount = 1;
	}
	return id;
}

int fB_Timer::pulseTag(uint8_t index,uint16_t tag, unsigned long period, uint16_t pulseValue){
	int id(oscillateTag(index,tag, period, pulseValue, 1));
	// now fix the repeat count
	if (id >= 0 && id < _TIMER_MAX_EVENTS) {
		_events[id].repeatCount = 1;
	}
	return id;
}


uint8_t fB_Timer::writeLog(uint8_t fTag,uint8_t mode) {
	scheduleLog(fTag,mode,NULL);
}

uint8_t fB_Timer::scheduleLog(uint8_t fTag,uint8_t mode, double minutes) {
	//dbug(F("T schedulLog entry tag:%d"),fTag);

	char* filename;
	uint8_t index;
	unsigned long msecs;			
	uint8_t writeHeader = 0;
	uint8_t writeTags   = 0;

	filename = rec.logGetFilename(fTag);
	if(!filename) return 0;
	switch(mode) {
		case _NEWLOG:
			if(rec.fileFind(filename)) rec.logArchive();
			rec.logCreate(fTag);
			break;
		case _APPENDLOG:
		case _APPENDDIF:
			if(!rec.fileFind(filename)) 	return 0;
	}

	switch(mode) {
		case _APPENDDIF: 
			{
				fB_Tag  bufTag, *pT;
				fB_Val  bufVal;
				bufTag.dVal = &bufVal;
				for(int i=0; i< logTagCount; i++) {
					if(logTagRay[i].fTag != fTag) continue;
					pT = rec.EEgetTag(bufTag,logTagRay[i].tag,BASEELOG);
					if(!pT) {
						writeHeader = 1;
						writeTags = 1;
						//dbug(F("T sl != tag:%d"),fTag);
					}
					else {
						if(pT->flag16 & _DUBL) {
							if(pT->dVal->value != bufTag.dVal->value) writeTags = 1;
						}
						else 	{
							if(pT->iVal != bufTag.iVal) writeTags = 1;
							//dbug(F("T sl iVal tag:%d"),fTag);
						}
					}
				}
			}
			break;
		default:
			writeHeader = 0;
			writeTags = 1;
	}
	if(writeHeader) rec.logHeader(fTag);
	if(writeTags) {
				//dbug(F("T sl wT tag:%d"),fTag);
		if(minutes) {
			msecs = (unsigned long) (minutes * (double)60000);
			index = timer.perpetual(msecs,logData,(uint16_t) fTag);
			return index;
		}
		else rec.logData(fTag);
	}
	if(writeHeader || writeTags) rec.EEwriteTags(BASEELOG);
	return 0;
}
/*
uint8_t fB_Timer::scheduleLog(uint8_t fTag,uint8_t mode, double minutes) {
	dbug(F("T schedulLog entry"));

	char* filename;
	uint8_t index;
	unsigned long msecs;
	filename = rec.logGetFilename(fTag);
	if(!filename) return 0;
	if(mode == _NEWLOG) {
		if(rec.fileFind(filename)) rec.logArchive();
		rec.logCreate(fTag);
	}
	else if(!rec.fileFind(filename)) return 0;
	if(minutes) {
		msecs = (unsigned long) (minutes * (double)60000);
		index = timer.perpetual(msecs,logData,(uint16_t) fTag);
		return index;
	}
	else rec.logWriteData(fTag);
	return 0;
}
*/

