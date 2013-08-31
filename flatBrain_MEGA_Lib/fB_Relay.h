#ifndef FB_RELAY_H
#define FB_RELAY_H

#include "fB_Header.h"


class fB_Relay {
	public:
		double Volts;
		uint8_t  state;

		uint16_t tTag;
		uint8_t  pTag;
		uint16_t sTag;
		uint16_t vTag;
		fB_Tag* pT;
		fB_Tag* pS;
		fB_Tag* pV;
		
		void setTags(uint16_t triggerTag, uint16_t stateTag, uint16_t  voltTag, uint8_t  pageTag);
		void update();
		void turn(uint8_t newState);
		void updateVolts();
		void updateState();
		fB_Relay(uint16_t triggerTag, uint16_t stateTag, uint16_t  voltTag, uint8_t  pageTag);

 } ;



 
fB_Relay::fB_Relay(uint16_t triggerTag, uint16_t stateTag, uint16_t  voltTag, uint8_t  pageTag) {
	 tTag = triggerTag;
	 sTag = stateTag;
	 vTag = voltTag;
	 pTag = pageTag;
	 pT = Tag(triggerTag);
	 pS = Tag(stateTag);
	 pV = Tag(voltTag);
	 //dbug(F("bset pageTag:%d %P "),pageTag,Tag(pageTag)->Ptitle);
 }

 void fB_Relay::update() {
	updateState();
	if(vTag) updateVolts();
 }
 void fB_Relay::updateState() {
	uint8_t oldState = state;
	uint8_t onval = pS->getOnVal();
	pS->read();
	if(pS->iVal) {
		if(onval) state = ON;
		else state = OFF;
	}
	else {
		if(onval) state = OFF;
		else state = ON;
	}
	if(oldState != state && curr.pageTag == pTag) menu.refreshRow(sTag);
	
 }
 void fB_Relay::updateVolts() {
	double oldVolts = Volts;
	pV->read();
//dbug(F("uVS %P val:%f"),pV->Ptitle,pV->dVal->value);
	if(pV->dVal->value <0) pV->dVal->value = 0;
	Volts = pV->dVal->value;
	if((fabs(oldVolts-Volts)> 0.1) && curr.pageTag == pTag) menu.refreshRow(vTag);
 }
 void fB_Relay::turn(uint8_t newState) {
	 updateState();
	 if(state != newState) {
		 pT->YshiftPulse(PULSEMSECS);
	 }
	 updateState();
	 if(state != newState) {
		 alarm.play(ALARM_2);
	 }
 }
 //dbug(F("uS pTag:%d currpt:%d "),pTag,curr.pageTag);


#endif
