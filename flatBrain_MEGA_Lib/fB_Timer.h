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

#ifndef FB_TIMER_H
#define FB_TIMER_H


#include "fB_Header.h"

//#define _TIMER_MAX_EVENTS (10)      // defined in fB_usr_Define.h

//#define	_TIMER_FREEINDEXSTART	0 // defined in fB_SYS_Define.h

#define TIMER_NOT_AN_EVENT (-2)
#define NO_TIMER_AVAILABLE (-1)

#define EVENT_NONE				0
#define EVENT_EVERY				1
#define EVENT_OSCILLATE_PIN		2
#define EVENT_OSCILLATE_TAG		3

class fB_Event
{

public:
  fB_Event(void);
  uint8_t eventType;
  bool update(void);
  unsigned long period;
  unsigned long lastEventTime;
  int repeatCount;
  uint16_t count;
  fB_Tag *pT;
  uint16_t arg16;
  uint8_t pin;
  uint8_t currState;
  uint8_t startState;
  void (*callback)(uint16_t);
};

class fB_Timer
{

public:
  fB_Timer(void);

  int perpetual(uint8_t index,unsigned long period, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int perpetual(unsigned long period, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int repeat(uint8_t index,unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int repeat(unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int after(uint8_t index,unsigned long duration, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int after(unsigned long duration, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int oscillate(uint8_t pin, unsigned long period, uint16_t startingValue);
  int oscillate(uint8_t pin, unsigned long period,uint16_t startingValue, int  repeatCount);
  int oscillateTag(uint8_t index,uint16_t tag, unsigned long period, uint16_t startingValue, int repeatCount);
  
  /**
   * This method will generate a pulse of !startingValue, occuring period after the
   * call of this method and lasting for period. The Pin will be left in !startingValue.
   */
  int pulse(uint8_t pin, unsigned long period, uint16_t startingValue);
  
  /**
   * This method will generate a pulse of pulseValue, starting immediately and of
   * length period. The pin will be left in the !pulseValue state
   */
  int pulseImmediate(uint8_t pin, unsigned long period, uint16_t pulseValue);
  int pulseTag(uint8_t index,uint16_t tag, unsigned long period, uint16_t pulseValue);
  void stop(uint8_t id);
  void update(void);
  bool update(uint8_t index);
  void updateWarn();
  void setPeriod(uint8_t index, unsigned long period);
  uint8_t writeLog(uint8_t logTag,uint8_t mode);
  uint8_t scheduleLog(uint8_t logTag,uint8_t mode,double minutes = NULL);


protected:
  int every(uint8_t index,unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16=NULL);
  int every(unsigned long period, int repeatCount, void (*callback)(uint16_t), uint16_t arg16=NULL);
 fB_Event _events[_TIMER_MAX_EVENTS];
 int findFreeEventIndex(void);

};

#endif
