#ifndef FB_RELAY_H
#define FB_RELAY_H

#include "fB_Header.h"

#define IGTN		0x0001	//  Ignition switch on
#define ALTN		0x0002	//  Alternator on
#define EXTN		0x0004  //  External (shore,solar) charge on
#define BT2			0x0008  //  Batt2 exists
#define BT3			0X0010  //  Batt3 exists
#define CT2			0x0020	//  Alt charging Batt2
#define CT3			0x0040	//  Alt charging Batt3
#define XT1			0x0080	//  Ext charging Batt1
#define XT3			0x0100	//  Ext charging Batt3
//#define 		0x0200
#define RT1		0x0400  //  Relay 1 ON
#define RT2		0x0800	
#define RT3		0x1000	
#define RT4		0x2000	
#define RT5		0x4000
#define RT6		0x8000

#define VOLTSEXIST		9.0		// Volts > this indicates: BATTERY EXISTS
#define VOLTSALT		13.8	// Volts > this indicates: ALTERNATOR ACTIVE

#define CHGLOV1			12.6	// Volts < this indicates: READY TO CHARGE
#define CHGLOV2			12.6
#define CHGLOV3			12.6

#define CHGHIV1		13.9	// Volts > this indicates: STOP CHARGING
#define CHGHIV2		13.9
#define CHGHIV3		13.9

#define LVDLOV1		12.5	// Volts < this indicates: LOW VOLTAGE DISCONNECT 
#define LVDLOV2		11.0
#define LVDLOV3		10.0

#define LVDHIV1		13		// Volts > this indicates: LVD RECOVER
#define LVDHIV2		12.0
#define LVDHIV3		11.0

#define sALT		(state.flags & ALTN?1:0)	// Alternator active
#define sIGN		(state.flags & IGTN?1:0)// Ignition switch on (Accesory switch ?)
#define sEXT		(state.flags & EXTN?1:0)	// External charge active (Solar, shore)
#define sB2			(state.flags & BT2?1:0)		// Battery 2 exists
#define sB3			(state.flags & BT3?1:0)		// Battery 3 exists
#define sC2			(state.flags & CT2?1:0)		// Alt charging Battery 2 
#define sC3			(state.flags & CT3?1:0)    		// Alt charging Battery 3 
#define sX1			(state.flags & XT1?1:0)		// Ext charging Battery 1 
#define sX3			(state.flags & XT3?1:0)		// Ext charging Battery 3 
#define sR1			(state.flags & RT1?1:0)		// Relay 1  
#define sR2			(state.flags & RT2?1:0)		// Relay 2  
#define sR3			(state.flags & RT3?1:0)		// Relay 3  
#define sR4			(state.flags & RT4?1:0)		// Relay 4  
#define sR5			(state.flags & RT5?1:0)		// Relay 5  
#define sR6			(state.flags & RT6?1:0)		// Relay 6  

/*
#define nALT		(next.flags & ALTN)		// Alternator active
#define nIGN		(next.flags & IGTN)		// Ignition switch on (Accesory switch ?)
#define nEXT		(next.flags & EXTN)		// External charge active (Solar, shore)
#define nB2			(next.flags & BT2)		// Battery 2 exists
#define nB3			(next.flags & BT3)		// Battery 3 exists
#define nC2			(next.flags & CT2)		// Alt charging Battery 2 
#define nC3			(next.flags & CT3)		// Alt charging Battery 3 
#define nX1			(next.flags & XT1)		// Ext charging Battery 1 
#define nX3			(next.flags & XT3)		// Ext charging Battery 3 
*/
#define nR1			(next.flags & RT1?1:0)		// Relay 1  
#define nR2			(next.flags & RT2?1:0)		// Relay 2  
#define nR3			(next.flags & RT3?1:0)		// Relay 3  
#define nR4			(next.flags & RT4?1:0)		// Relay 4  
#define nR5			(next.flags & RT5?1:0)		// Relay 5  
#define nR6			(next.flags & RT6?1:0)		// Relay 6  


#define sV1			state.dV1	// V1 voltage
#define sV2			state.dV2	// V2 voltage
#define sV3			state.dV3	// V3 voltage
#define sCL			state.dCL	// Load Amps
#define sCX			state.dCX	// EXT Charging Amps

#define pV1			prev.dV1	// V1 voltage
#define pV2			prev.dV2	// V2 voltage
#define pV3			prev.dV3	// V3 voltage
#define pCL			prev.dCL	// Load Amps
#define pCX			prev.dCX	// EXT Charging Amps

#define R1_ON			Relay[0].turn(ON)	
#define R1_OFF			Relay[0].turn(OFF)	
#define R2_ON			Relay[1].turn(ON)	
#define R2_OFF			Relay[1].turn(OFF)	
#define R3_ON			Relay[2].turn(ON)	
#define R3_OFF			Relay[2].turn(OFF)	
#define R4_ON			Relay[3].turn(ON)	
#define R4_OFF			Relay[3].turn(OFF)	
#define R5_ON			Relay[4].turn(ON)	
#define R5_OFF			Relay[4].turn(OFF)	
#define R6_ON			Relay[5].turn(ON)	
#define R6_OFF			Relay[5].turn(OFF)
#define RL_OFF			Relay[3].turn(OFF);Relay[4].turn(OFF);Relay[5].turn(OFF)


		
///////////////////////////////////////////////////////////////////////////////////////
class V_State {
	public:
		uint16_t flags;
		double dCL, dCX, dV1, dV2, dV3;
		uint16_t msgIndex;
		char	*msgText;
		void copyTo(V_State* pState);
		void setMsg(uint16_t msgIndex,char *msgText);
		void setBit(uint16_t bitVal, uint16_t ival);
		V_State();
 } ;

 V_State::V_State() {
	 dCL = dCX = dV1 = dV2 = dV3 = 0;
	 flags = 0;
 }

void V_State::copyTo(V_State* pState) { 
	pState->flags = flags;
	pState->dCL = dCL;
	pState->dCX = dCX;
	pState->dV1 = dV1;
	pState->dV2 = dV2;
	pState->dV3 = dV3;
	pState->msgIndex = msgIndex;
	pState->msgText = msgText;
} 

void V_State::setBit(uint16_t bitVal, uint16_t ival) { 
	if(ival) flags |= bitVal ;
	else	 flags &= ~bitVal;
}

void V_State::setMsg(uint16_t index,char* text = NULL){
	msgIndex = index;
	msgText = text;
}
/////////////////////////////////////////////////////////////////////////////////////////
class V_Relay {
	public:
		uint16_t tTag;
		uint16_t sTag;
		fB_Tag* pT;
		fB_Tag* pS;
		
		uint16_t getRstate();
		//void putRstate(uint8_t state);
		void turn(uint8_t newState);
		void setTags(uint16_t triggerTag, uint16_t stateTag) ;

 };

 void V_Relay::setTags(uint16_t triggerTag, uint16_t stateTag) {
	 tTag = triggerTag;
	 pT = Tag(tTag);
	 sTag = stateTag;
	 pS = Tag(sTag);
 }

uint16_t V_Relay::getRstate() {
	uint16_t bState = 0;

	pS->read();
	if(pS->getOnVal() ){
		if(pS->iVal) bState = ON;
		else bState = OFF;
	}
	else {
		if(pS->iVal) bState = OFF;
		else bState = ON;
	}
	return bState;
}

void V_Relay::turn(uint8_t newState) {
	//dbug(F("vRelay %P Rstate:%d, newstate:%d"),pT->Ptitle,getRstate(),newState);

	 if(getRstate()!= newState)  pT->YshiftPulse(PULSEMSECS);
	 //if(getRstate() != newState)  alarm.play(ALARM_2);
 }

////////////////////////////////////////////////////////////////////////////////////////////
class fB_Vanduino {
	public:
		V_State		prev;
		V_State		next;
		V_State		state;
		V_Relay		Relay[6];
		fB_Tag		*ptCL, *ptCX, *ptIGN, *ptALT, *ptV1, *ptV2, *ptV3;
		uint16_t	pTag;
		uint8_t		refresh;

		void getState();
		void showState();
		void nextState() ;
		void setState() ;
		void action();
		void setBit(uint16_t bit, uint16_t value);
		void setNext(uint16_t bit, uint16_t value);
		void setMsg(uint16_t bit, uint16_t value);
		void init(uint8_t pageTag);
		uint8_t priorityShutdownLoad(uint16_t relay);
		uint8_t checkVolts(uint16_t relay);
		uint8_t switchShutdown(uint8_t index,uint8_t relaySrc, uint8_t relayDst );
		//void alarmLVD();

		fB_Vanduino();
};

fB_Vanduino::fB_Vanduino() { 
	refresh = 1;
}

void fB_Vanduino::init(uint8_t pageTag) { 
	pTag = pageTag; 
	Relay[0].setTags(Y1,Y1S);
	Relay[1].setTags(Y2,Y2S);
	Relay[2].setTags(Y3,Y3S);
	Relay[3].setTags(Y4,Y4S);
	Relay[4].setTags(Y5,Y5S);
	Relay[5].setTags(Y6,Y6S);

	ptCL = Tag(CL);
	ptCX = Tag(CX);
	ptV1 = Tag(V1);
	ptV2 = Tag(V2);
	ptV3 = Tag(V3);
	ptIGN = Tag(IGN);
	ptALT = Tag(ALT);

	// check state of relays, record
	// turn off all relays
	//check existence of batts
	//check existence of EXT
	// set flags
	// restore relays
}

void fB_Vanduino::setBit(uint16_t bitVal, uint16_t ival) { 
	if(ival) state.flags |= bitVal ;
	else	 state.flags &= ~bitVal;
}
void fB_Vanduino::setNext(uint16_t bitVal, uint16_t ival) { 
	if(ival) next.flags |= bitVal ;
	else	 next.flags &= ~bitVal;
}


void fB_Vanduino::getState() {
	//uint16_t bState;
	uint8_t  ign;
	//V_Relay* pR;

	state.copyTo(&prev);

	state.setBit(RT1,Relay[0].getRstate());
	state.setBit(RT2,Relay[1].getRstate());
	state.setBit(RT3,Relay[2].getRstate());
	state.setBit(RT4,Relay[3].getRstate());
	state.setBit(RT5,Relay[4].getRstate());
	state.setBit(RT6,Relay[5].getRstate());

	ptCL->read();
	ptCX->read();
	ptV1->read();
	ptV2->read();
	ptV3->read();

	state.dCL = ptCL->dVal->value;
	state.dCX = ptCX->dVal->value;
	state.dV1 = ptV1->dVal->value;
	state.dV2 = ptV2->dVal->value;
	state.dV3 = ptV3->dVal->value;

	state.setBit(BT2,sV2 > VOLTSEXIST);
	state.setBit(BT3,sV3 > VOLTSEXIST);

	if(ptIGN->readInt() > 500) ign = 1;
	else ign = 0;
	state.setBit(IGTN,ign);
	if(ign && sV1 > VOLTSALT) {
		state.setBit(ALTN,1);
		ptALT->iVal = 1;
	}
	else {
		state.setBit(ALTN,0);
 		ptALT->iVal =0;
	}
	
	state.setBit(EXTN,!sALT && sCX > 0); // external source charging

//dbug(F("gstateS flags:0x%x "),state.flags);
//dbug(F("gstateS sr1:%d, sr2:%d, sr3:%d "),sR1,sR2,sR3);

	state.setBit(CT2, sR1 && sR2 && sALT);		// Alternator charging Batt 2
	state.setBit(CT3, sR1 && sR3 && sALT);		// Alternator charging Batt 3
	state.setBit(XT1, sR1 && sR2 && sEXT);		// External charging Batt 1
	state.setBit(XT3, sR3 && sR2 && sEXT);		// External charging Batt 3

	state.copyTo(&next);
}


uint8_t fB_Vanduino::checkVolts(uint16_t relay) {
	switch(relay) {
		case RT1:
			ptV1->read();
			sV1 = ptV1->dVal->value;
			if(sV1 > LVDLOV1) return 1;
			break;
		case RT2:
			ptV2->read();
			sV2 = ptV2->dVal->value;
			if(sV2 > LVDLOV2) return 1;
			break;
		case RT3:
			ptV3->read();
			sV3 = ptV3->dVal->value;
			if(sV3 > LVDLOV3) return 1;
			break;
	}
	return 0;
}

void fB_Vanduino::nextState() {
	uint8_t warnAction;

	next.setMsg(P_BLANK); 
	if(!sB3) next.setBit(RT3, OFF);												// if no batt 3, turn off relay 3
	if(sALT){																	// alternator on and charging voltage ( > VOLTSALT)
		dbug(F("NS sSalt"));
		//if(!sC3 && sV2 < CHGLOV2){ // alternator on and not charging batt 3 and batt 2 low
		if(sV2 < CHGLOV2){ // alternator on and not charging batt 3 and batt 2 low
			next.setBit(RT1, ON); 
			next.setBit(RT2, ON); 
			next.setBit(RT3, OFF); 
			next.setMsg(P_CHGALT,"B2"); 
			return; 
		}
		if(!sC2 && sV3 < CHGLOV3){ // alternator on and not charging batt 2 and batt 3 low
			next.setBit(RT1, ON); 
			next.setBit(RT3, ON); 
			next.setBit(RT2, OFF); 
			next.setMsg(P_CHGALT,"B3"); 
			return; 
		}
		if((sC2 && sV2 > CHGHIV2) || (sC3 && sV3 > CHGHIV3)){ // alternator on and ( charging batt 2 and batt 2  topped OR charging batt 3 and batt 3  topped)
			next.setBit(RT2, ON); 
			next.setBit(RT3, OFF); 
			next.setBit(RT1, OFF); 
			return; 
		}		
	}
	/////////////////////////////// NOT ALTERNATOR ON
	else {
		if(!sR1 && !sR2 && !sR3) {  // no Batt relays on, turn all load off
			next.setBit(RT4, OFF);   
			next.setBit(RT5, OFF);   
			next.setBit(RT6, OFF);  
		}

		//if(sC2 || sC3)  next.setBit(RT1, OFF);  
		dbug(F("NS !sSalt"));
		next.setBit(RT1, OFF);  
		if(sEXT) {								// external charge active
		dbug(F("NS sEXT"));
			if(sV2 > CHGHIV2) {					// batt2 charged
				if(sV1 < CHGHIV1 )	{			// charge batt 1 by external source
		dbug(F("NS sEXT1"));
					next.setBit(RT2, ON);   
					next.setBit(RT1, ON);   
					next.setBit(RT3, OFF);   
					next.setMsg(P_CHGEXT,"B1"); 
					return; 
				}	
				if(sB3 && sV3 < CHGHIV3 )	{	// charge batt 3 by external source
		dbug(F("NS sEXT2"));
					next.setBit(RT2, ON);   
					next.setBit(RT3, ON);   
					next.setBit(RT1, OFF);   
					next.setMsg(P_CHGEXT,"B3"); 
					return; 
				}
			}
			else if(sR2 && sV2 < CHGLOV2) {
		dbug(F("NS sEXT3"));
				next.setBit(RT1, OFF);   
				next.setBit(RT3, OFF);
			}
			next.setMsg(P_CHGEXT,"B1");
		}
		//if(state.msgIndex == P_CHGEXT) next.setMsg(P_BLANK);
		next.setMsg(P_BLANK);
	    // not EXT charging	
		if(sR1 && switchShutdown(1,RT1,RT2)) return;
		if(sR2 && switchShutdown(2,RT2,RT3)) return;
		if(sR3 && switchShutdown(3,RT3,NULL)) return;
	}
}


uint8_t fB_Vanduino::switchShutdown(uint8_t index,uint8_t relaySrc, uint8_t relayDst ) {  // source and dest are the ordinate relay numbers (eg., 1,2;  1,3;  2;3)
	uint8_t relayX, battX;
	double vSrc,vDst, LowSrc, LowDst;
	char *textSrc,*textDst;
		dbug(F("ssD entry"));

	switch(relaySrc) {
		case RT1:
			vSrc	 = sV1;
			LowSrc	 = LVDLOV1;
			textSrc  = "R1";
			break;
		case RT2:
			vSrc	= sV2;
			LowSrc = LVDLOV2;
			textSrc = "R2";
			break;
		case RT3:
			vSrc	= sV3;
			LowSrc = LVDLOV3;
			textSrc = "R3";
			break;
		default: return 0;
	}

	switch(relayDst) {
		case RT2:
			battX	= sB2; 
			vDst	= sV2;
			LowDst = LVDLOV2;
			textDst = "R2";
			break;
		case RT3:
			battX	= sB3; 
			vDst	= sV3;
			LowDst = LVDLOV3;
			textDst = "R3";
			break;
		case NULL:
			battX	= NULL; 
			vDst	= NULL;
			LowDst	= NULL;
			textDst = NULL;
			break;
		default: return 0;
	}

	if(relayX && vSrc < LowSrc) {							// Batt Src selected AND Batt Src below Vdiscon			
		if(battX && vDst > LowDst )  {						// Batt Src selected AND Batt Src below Vdiscon AND Batt 2 above Vdiscon
			// SYNTAX: warning(uint8_t id, uint8_t wdSecs,uint8_t wSecs, uint8_t aSecs) 
			switch ( warn.warning(index*2, 20,20, 4, WLED)) { 
				case _WD_WARN:
					next.setMsg(P_SWITCHTO,textDst);
					return 1;
				case _WD_DELAY:
					next.setMsg(P_DELAYSW2,textDst);
					return 1;
				case _WD_ACT: // switch to batt dest
					next.setBit(relayDst, ON); 
					next.setBit(relaySrc, OFF); 
					next.setMsg(P_BLANK);
					return 1;
			}	
		}
		else {												// Batt Src selected AND Batt Src below Vdiscon  AND !exist Batt Dest or Batt Dest below Vdiscon
			 if(warn.currID == index*2) {					// If Batt Dest voltage drops below Vdiscon while above warn active, clear warning
				warn.reset();
				next.setMsg(P_BLANK);
			}
			if(!priorityShutdownLoad(relaySrc)) {			// Start priority shutdown of Load to see if we can bring Batt Src above Vdiscon
				next.setMsg(P_BLANK);
				return 1;
			}
			switch ( warn.warning(index*2 +1, 20,20, 4, WLED)) { // Start shutdown of Relay Source
				case _WD_WARN:
					next.setMsg(P_SHUTDOWN,textSrc);
					return 1;
				case _WD_DELAY:
					next.setMsg(P_DELAYSHUT,textSrc);
					return 1;
				case _WD_ACT:									// shutdown relay src
					next.setBit(relaySrc, OFF); 
					next.setMsg(P_BLANK);
					return 1;
			}
		}
	}
	 if(warn.currID == index*2 || warn.currID == index*2 +1 ) {		// If Batt Src voltage above Vdiscon while either above warns active, clear warning
		warn.reset();
		next.setMsg(P_BLANK);
	}
	return 0;
}

uint8_t fB_Vanduino::priorityShutdownLoad(uint16_t relay) {

		if(sR4) {
			R4_OFF; 
			delay(1000);
			if(checkVolts(relay)) return 1;
		}
		if(sR6) {
			R6_OFF; 
			delay(1000);
			if(checkVolts(relay)) return 1;
		}
		if(sR5) {
			R5_OFF; 
			delay(1000);
			if(checkVolts(relay)) return 1;
		}
		return 0;
}


void fB_Vanduino::setState() {
		Relay[0].turn(nR1);
		Relay[1].turn(nR2);
		Relay[2].turn(nR3);
		Relay[3].turn(nR4);
		Relay[4].turn(nR5);
		Relay[5].turn(nR6);

		next.copyTo(&state);
}

 void fB_Vanduino::showState() {
// dbug(F("vSS flags:0x%x "),state.flags);
	 /*
	 if(fabs(state.sAL - prevState.sAL) > 0.1)  pAL->showRow(curr.row(AL));
	 if(fabs(state.sCX - prevState.sCX) > 0.1)  pCX->showRow(curr.row(CX));
	 if(fabs(state.sV1 - prevState.sV1) > 0.1)  pV1->showRow(curr.row(V1));
	 if(fabs(state.sV2 - prevState.sV2) > 0.1)  pV2->showRow(curr.row(V2));
	 if(fabs(state.sV3 - prevState.sV3) > 0.1)  pV3->showRow(curr.row(V3));
	 */
	 if(fabs(sCL -  pCL) > 0.1)  ptCL->showRow(curr.row(CL));
	 if(fabs(sCX -  pCX) > 0.1)  ptCX->showRow(curr.row(CX));

	 if(fabs(sV1 - pV1) > 0.1)   ptV1->showRow(curr.row(V1));
	 if(fabs(sV2 - pV2) > 0.1)   ptV2->showRow(curr.row(V2));
	 if(fabs(sV3 - pV3) > 0.1)   ptV3->showRow(curr.row(V3));

	 //if(sB3) pV3->showRow(curr.row(V3));
	 if((state.flags & ALTN) != (prev.flags & ALTN )) {
		 ptALT->showRow(curr.row(ALT));
	 }
	 if(prev.msgIndex != state.msgIndex && prev.msgText != state.msgText) menu.showMessage(state.msgIndex,state.msgText);
 }


#endif
