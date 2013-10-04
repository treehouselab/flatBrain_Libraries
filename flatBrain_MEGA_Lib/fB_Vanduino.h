#ifndef FB_RELAY_H
#define FB_RELAY_H

#include "fB_Header.h"

#define IGTN		0x0001	//  Ignition switch on
#define ALTN		0x0002	//  Alternator on
#define EXTN		0x0004  //  External (shore,solar) charge on
#define BT2			0x0008  //  Batt2 exists
#define BT3			0X0010  //  Batt3 exists
#define AT2			0x0020	//  Alt charging Batt2
#define AT3			0x0040	//  Alt charging Batt3
#define XT1			0x0080	//  Ext charging Batt1
#define XT3			0x0100	//  Ext charging Batt3
//#define 		0x0200
#define RT1		0x0400  //  Relay 1 ON
#define RT2		0x0800	
#define RT3		0x1000	
#define RT4		0x2000	
#define RT5		0x4000
#define RT6		0x8000
#define RMASK	0xFC00


#define VOLTSEXIST	ptVEXS->dVal->value	// Volts > this indicates: BATTERY EXISTS
#define VOLTSALT	ptVALT->dVal->value	// Volts > this indicates: ALTERNATOR ACTIVE

#define CHGLOV1		ptCHLO->dVal->value 	// Volts < this indicates: READY TO CHARGE
#define CHGLOV2		ptCHLO->dVal->value	
#define CHGLOV3		ptCHLO->dVal->value

#define CHGHIV1		ptCHHI->dVal->value	// Volts > this indicates: STOP CHARGING
#define CHGHIV2		ptCHHI->dVal->value
#define CHGHIV3		ptCHHI->dVal->value

#define LVDLOV1		ptDLO1->dVal->value	// Volts < this indicates: LOW VOLTAGE DISCONNECT 
#define LVDLOV2		ptDLO2->dVal->value
#define LVDLOV3		ptDLO3->dVal->value

#define LVDHIV1		ptDHI1->dVal->value		// Volts > this indicates: LVD RECOVER
#define LVDHIV2		ptDHI2->dVal->value
#define LVDHIV3		ptDHI3->dVal->value

#define sALT		(state.flags & ALTN?1:0)	// Alternator active
#define sIGN		(state.flags & IGTN?1:0)    // Ignition switch on (Accesory switch ?)
#define sEXT		(state.flags & EXTN?1:0)	// External charge active (Solar, shore)
#define sB2			(state.flags & BT2?1:0)		// Battery 2 exists
#define sB3			(state.flags & BT3?1:0)		// Battery 3 exists
#define sA2			(state.flags & AT2?1:0)		// Alt charging Battery 2 
#define sA3			(state.flags & AT3?1:0)    	// Alt charging Battery 3 
#define sX1			(state.flags & XT1?1:0)		// Ext charging Battery 1 
#define sX3			(state.flags & XT3?1:0)		// Ext charging Battery 3 
#define sR1			(state.flags & RT1?1:0)		// Relay 1  
#define sR2			(state.flags & RT2?1:0)		// Relay 2  
#define sR3			(state.flags & RT3?1:0)		// Relay 3  
#define sR4			(state.flags & RT4?1:0)		// Relay 4  
#define sR5			(state.flags & RT5?1:0)		// Relay 5  
#define sR6			(state.flags & RT6?1:0)		// Relay 6  
#define sRELAYS		(state.flags & RMASK)		// Relay 6  

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
#define nALT		(next.flags & ALTN?1:0)	// Alternator active
#define nR1			(next.flags & RT1?1:0)		// Relay 1  
#define nR2			(next.flags & RT2?1:0)		// Relay 2  
#define nR3			(next.flags & RT3?1:0)		// Relay 3  
#define nR4			(next.flags & RT4?1:0)		// Relay 4  
#define nR5			(next.flags & RT5?1:0)		// Relay 5  
#define nR6			(next.flags & RT6?1:0)		// Relay 6  
#define nRELAYS		(next.flags & RMASK)		// Relay 6  


#define sV0			state.dV0	// V0 voltage
#define sV1			state.dV1	// V1 voltage
#define sV2			state.dV2	// V2 voltage
#define sV3			state.dV3	// V3 voltage
#define sCL			state.dCL	// Load Amps
#define sCX			state.dCX	// EXT Charging Amps

#define pALT		(prevShow.flags & ALTN?1:0)	// Alternator active
#define pV0			prevShow.dV0	// V1 voltage
#define pV1			prevShow.dV1	// V1 voltage
#define pV2			prevShow.dV2	// V2 voltage
#define pV3			prevShow.dV3	// V3 voltage
#define pCL			prevShow.dCL	// Load Amps
#define pCX			prevShow.dCX	// EXT Charging Amps

#define gV1			log.dV1	// V1 voltage
#define gV2			log.dV2	// V2 voltage
#define gV3			log.dV3	// V3 voltage
#define gCL			log.dCL	// Load Amps
#define gCX			log.dCX	// EXT Charging Amps

#define R1_ON			relay1.turn(ON)	
#define R1_OFF			relay1.turn(OFF)	
#define R2_ON			relay2.turn(ON)	
#define R2_OFF			relay2.turn(OFF)	
#define R3_ON			relay3.turn(ON)	
#define R3_OFF			relay3.turn(OFF)	
#define R4_ON			relay4.turn(ON)	
#define R4_OFF			relay4.turn(OFF)	
#define R5_ON			relay5.turn(ON)	
#define R5_OFF			relay5.turn(OFF)	
#define R6_ON			relay6.turn(ON)	
#define R6_OFF			relay6.turn(OFF)
#define RL_OFF			relay4.turn(OFF);relay5.turn(OFF);relay6.turn(OFF)


		
///////////////////////////////////////////////////////////////////////////////////////
class V_State {
	public:
		uint16_t flags;
		double dCL, dCX, dV0,dV1, dV2, dV3;
		uint16_t msgIndex;
		char	*msgText;
		void copyAllTo(V_State* pState);
		void copyMsgTo(V_State* pState);
		void copyRelaysTo(V_State* pState);
		void setMsg(uint16_t msgIndex,char *msgText);
		void setBit(uint16_t bitVal, bool logic);
		V_State();
 } ;

 V_State::V_State() {
	 dCL = dCX = dV0 =dV1 = dV2 = dV3 = 0;
	 flags = 0;
 }

void V_State::copyAllTo(V_State* pState) { 
	pState->dCL = dCL;
	pState->dCX = dCX;
	pState->dV1 = dV1;
	pState->dV2 = dV2;
	pState->dV3 = dV3;
	pState->flags = flags;
	copyMsgTo(pState); 
} 
void V_State::copyRelaysTo(V_State* pState) { 
	pState->flags |= flags & RMASK;
} 

void V_State::copyMsgTo(V_State* pState) { 
	pState->msgIndex = msgIndex;
	pState->msgText = msgText;
} 

void V_State::setBit(uint16_t bitVal, bool logic) { 
	if(logic) flags |= bitVal ;
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
	 if(getRstate() != newState)  alarm.playTag(_TALRMFL);
 }

////////////////////////////////////////////////////////////////////////////////////////////
class fB_Vanduino {
	public:
		V_State		prevShow;
		V_State		next;
		V_State		state;
		V_State		log;
		V_Relay		relay1, relay2, relay3, relay4, relay5, relay6;
		fB_Tag		*ptCL, *ptCX, *ptIGN, *ptALT, *ptV0,  *ptV1, *ptV2, *ptV3, *ptVALT, *ptVEXS;
		fB_Tag		*ptCHLO, *ptCHHI, *ptDLO1, *ptDLO2, *ptDLO3, *ptDHI1, *ptDHI2, *ptDHI3;
		fB_Tag		*ptYSHFT, *ptLED, *ptCPSEC, *ptBKSEC, *ptLGMIN;
		uint16_t	pageTag;
		uint8_t 	manOver;	
		uint8_t 	LEDonVal;
		uint8_t		logTimerIndex;
		uint8_t		logTimerFlag;
		uint8_t		bootFlag;


		void logTimer();
		void getState();
		double getStateAnalogTag(uint16_t tag);
		uint8_t getStateRelayIndex(uint8_t index) ;
		void getStateRelays() { for(int i=1;i<7;i++) getStateRelayIndex(i); }
		void getStateAnalog() ;
		void setRelaysNext() ;
		void showState();
		void nextState() ;
		void buildNextState();
		//void action();
		void setMsg(uint16_t bit, uint16_t value);
		void init(uint16_t pageTag);
		uint8_t priorityShutdownLoad(uint16_t relay);
		uint8_t switchShutdown(uint8_t index,uint16_t relaySrc, uint16_t relayDst );
		uint8_t checkVolts(uint16_t relay);

		fB_Vanduino();
};

fB_Vanduino::fB_Vanduino() { 
	bootFlag = 1;
}

void fB_Vanduino::init(uint16_t pTag) { 

	//analogReference(EXTERNAL);
	pageTag  =  pTag; 
	manOver = 0;

	relay1.setTags(Y1,Y1S);
	relay2.setTags(Y2,Y2S);
	relay3.setTags(Y3,Y3S);
	relay4.setTags(Y4,Y4S);
	relay5.setTags(Y5,Y5S);
	relay6.setTags(Y6,Y6S);

	ptCPSEC = Tag(CPSEC);
	ptBKSEC = Tag(BKSEC);
	ptLGMIN = Tag(LGMIN);
	ptCL = Tag(CL);
	ptCX = Tag(CX);
	ptV0 = Tag(V0);
	ptV1 = Tag(V1);
	ptV2 = Tag(V2);
	ptV3 = Tag(V3);
	ptIGN = Tag(IGN);
	ptALT = Tag(ALT);
	ptVEXS = Tag(VEXS);
	ptVALT = Tag(VALT);
	ptCHLO = Tag(CHLO);
	ptCHHI = Tag(CHHI);
	ptDLO1 = Tag(DLO1);
	ptDLO2 = Tag(DLO2);
	ptDLO3 = Tag(DLO3);
	ptDHI1 = Tag(DHI1);
	ptDHI2 = Tag(DHI2);
	ptDHI3 = Tag(DHI3);
	ptYSHFT = Tag(YSHFT);
	ptLED =   Tag(_TALRMLED);
	LEDonVal = ptLED->getOnVal(); 	
	getStateRelays();
	state.copyAllTo(&next); // save original relays
	//dbug(F("V INIT0 sB2:%d, sB3:%d, "),sB2,sB3);

	// find BT2, BT3, should be constant for this session
	getStateAnalogTag(V2);
	if(!sR2) state.setBit(BT2,sV2 > VOLTSEXIST);
	else {
		if(sR3) R3_OFF;
		if(sR1) R1_OFF;
		getStateAnalogTag(V2);
		state.setBit(BT2,sV2 > VOLTSEXIST);
	}
	getStateRelayIndex(3); 
	//dbug(F("V INIT1 sB2:%d, sB3:%d, "),sB2,sB3);
	getStateAnalogTag(V3);
	if(!sR3) state.setBit(BT3,sV3 > VOLTSEXIST);
	else {
		if(sR1) R1_OFF;
		getStateAnalogTag(V3);
		state.setBit(BT3,sV3 > VOLTSEXIST);
	}
	
	// reset original relays
	setRelaysNext();
	next.copyRelaysTo(&state);
	if(_bootStatus & _SD){
		timer.writeLog(VGLOG,_APPENDDIF);
		if(bootFlag) {
			Tag(LBOOT)->iVal = HIGH;;
			timer.writeLog(VDATA,_APPENDLOG);
			bootFlag = 0;
			Tag(LBOOT)->iVal = LOW;
		}
		logTimerIndex = timer.scheduleLog(VDATA,_APPENDLOG, ptLGMIN->dVal->value);
	}
	if(_bootMsgIndex) state.setMsg(_bootMsgIndex);
}


void fB_Vanduino::getStateAnalog() {
	//dbug(F("V GSA entry"));
	getStateAnalogTag(CL);
	getStateAnalogTag(CX);
	getStateAnalogTag(V0);
	getStateAnalogTag(V1);
	if(sB2)	getStateAnalogTag(V2);
	if(sB3) getStateAnalogTag(V3);
}

void fB_Vanduino::getState() {
	uint8_t  ign;
	double seconds;
	//dbug(F("V GetState entry"));

	if(warn.action == _WD_DELAY && ptLED){
		ptLED->write(~LEDonVal);	
		delay(50);
		ptLED->write(LEDonVal);	
		delay(50);
		ptLED->write(~LEDonVal);	
		delay(50);
		ptLED->write(LEDonVal);	
	}
	
	if(curr.pageTag == pageTag) seconds = min(_CPSECLIM,ptCPSEC->dVal->value);
	else seconds = min(_BKSECLIM,ptBKSEC->dVal->value);
	timer.setPeriod(_TIMER_UPDATE, (unsigned long) (1000 * seconds));

	getStateRelays();
	getStateAnalog();

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

//dbug(F("gstateS sr1:%d, sr2:%d, sr3:%d "),sR1,sR2,sR3);
	state.setBit(AT2, sR1 && sR2 && sALT);		// Alternator charging Batt 2
	state.setBit(AT3, sR1 && sR3 && sALT);		// Alternator charging Batt 3
	state.setBit(XT1, sR1 && sR2 && sEXT);		// External charging Batt 1
	state.setBit(XT3, sR3 && sR2 && sEXT);		// External charging Batt 3
	//dbug(F("*** V GSX sf:0x%x"),state.flags);
	//dbug(F("*** V GSX nf:0x%x"),next.flags);


}

double fB_Vanduino::getStateAnalogTag(uint16_t tag) {
	//dbug(F("V GSA entry"));

	switch(tag) {			
		case CL:	
				ptCL->read();
				return state.dCL = ptCL->dVal->value;
		case CX:	
				ptCX->read();
				return state.dCX = ptCX->dVal->value;
		case V0:	
				ptV0->read();
				return state.dV0 = ptV1->dVal->value;
		case V1:	
				ptV1->read();
				return state.dV1 = ptV1->dVal->value;
		case V2:	
				ptV2->read();
				return state.dV2 = ptV2->dVal->value;
		case V3:	
				ptV3->read();
				return state.dV3 = ptV3->dVal->value;
	}
	return 0;	

}

uint8_t fB_Vanduino::getStateRelayIndex(uint8_t index) {	
	//dbug(F("V GSRIx"));

	switch(index) {
		case 1: state.setBit(RT1,relay1.getRstate()); return sR1;
		case 2: state.setBit(RT2,relay2.getRstate()); return sR2;
		case 3: state.setBit(RT3,relay3.getRstate()); return sR3;
		case 4: state.setBit(RT4,relay4.getRstate()); return sR4;
		case 5: state.setBit(RT5,relay5.getRstate()); return sR5;
		case 6: state.setBit(RT6,relay6.getRstate()); return sR6;
	}
	return 0;
}



void fB_Vanduino::nextState() {
	//dbug(F("V NextState entry"));
	//dbug(F("*** V NS sf:0x%x"),state.flags);
	//dbug(F("*** V NS nf:0x%x"),next.flags);

	//if(pageTag != curr.pageTag) return; // for dbug
	uint16_t preBuildRelayFlags = sRELAYS;
	//dbug(F("_____V_NS	copyS->N"));
	state.copyAllTo(&next);

	if(_fBiK1 == INTK1) {
		if(warn.action == _WD_WARN) warn.startWarnDelay();
		else if(warn.action == _WD_DELAY) warn.stop();
		else if( !sR1 && !sR2 && !sR3) {
			next.setBit(RT2, ON); // turn on R2
			alarm.playTag(_TALRMAC);
		}
		_fBiK1 = 0;     // reset K1 interrupt flag
	}

	else if(_fBiK1 == INTK1SHFT) {
		//dbug(F("V_NS	INTR manual"));
		warn.stop();
		if(manOver) {
			manOver = 0; 
			if(ptLED) 	ptLED->write(~LEDonVal);
			next.setMsg(P_BLANK); 
		}
		else {
			manOver = 1; 
			if(ptLED) 	ptLED->write(LEDonVal);	
			next.setMsg(P_MANUAL); 
		}
		next.copyMsgTo(&state);
		_fBiK1 = 0;     // reset K1 interrupt flag
	}
	if(_fBiTFT || manOver) return;  // skip if TFT button pressed;
		//dbug(F("NS2"));

	//dbug(F("_____V_NS	buildNS"));
	buildNextState();
	//dbug(F("_____V_NS	getStateRelays"));
	getStateRelays();
	uint16_t postBuildRelayFlags = sRELAYS;
	if(postBuildRelayFlags == preBuildRelayFlags) { // if relays have not changed manually during build
		//dbug(F("_____V_NS	copyN->S"));
		//dbug(F("_____V_NS	setRelays"));
		setRelaysNext();
		state.flags = next.flags;
		//dbug(F("_____V_NS	getStateAnalog"));
		getStateAnalog();
		//dbug(F("_____V_NS	copyN->S"));
		next.copyMsgTo(&state);
	}

}

void fB_Vanduino::buildNextState() {
	//dbug(F("*** V BNS sf:0x%x"),state.flags);
	//dbug(F("*** V BNS nf:0x%x"),next.flags);

	if(_sdMsgIndex) next.setMsg(_sdMsgIndex);
	else next.setMsg(P_BLANK); 

	if(!sB3) next.setBit(RT3, OFF);						// if no batt 3, turn off relay 3
	if(!sR1 && !sR2 && !sR1) {							// if no batt relays on, turn off load relays
		next.setBit(RT4, OFF);												
		next.setBit(RT5, OFF);												
		next.setBit(RT6, OFF);	
	}
	else if(sR4) next.setMsg(P_INVERTER); 
	if(!sR4 && next.msgIndex == P_INVERTER) next.setMsg(P_BLANK); 

	if(sALT){											// alternator on and charging voltage ( > VOLTSALT)
		//if(!sC3 && sV2 < CHGLOV2){ // alternator on and not charging batt 3 and batt 2 low
		if(sV2 < CHGLOV2){ // alternator on and not charging batt 3 and batt 2 low
			next.setBit(RT1, ON); 
			next.setBit(RT2, ON); 
			next.setBit(RT3, OFF); 
			next.setMsg(P_CHGALT,"B2"); 
			return; 
		}
		if(!sA2 && sB3 && sV3 < CHGLOV3){ // alternator on and not charging batt 2 and batt 3 and batt 3 low
			next.setBit(RT1, ON); 
			next.setBit(RT3, ON); 
			next.setBit(RT2, OFF); 
			next.setMsg(P_CHGALT,"B3"); 
			return; 
		}
		if((sA2 && sV2 > CHGHIV2) || (sA3 && sB3 && sV3 > CHGHIV3)){ // alternator on and ( charging batt 2 and batt 2  topped OR charging batt 3 and batt 3  topped)
			next.setBit(RT2, ON); 
			next.setBit(RT3, OFF); 
			next.setBit(RT1, OFF); 
			return; 
		}		
	}
	/////////////////////////////// NOT ALTERNATOR ON
	else {
		if(sR1 && ( sR2 || sR3))  {   // alt off, r1 or r2 on , turn r1 off
			next.setBit(RT1, OFF);   
		}
		if(!sR1 && !sR2 && !sR3) {  // no Batt relays on, turn all load off
			next.setBit(RT4, OFF);   
			next.setBit(RT5, OFF);   
			next.setBit(RT6, OFF);  
		}

		//if(sA2 || sA3)  next.setBit(RT1, OFF);  
		//next.setBit(RT1, OFF);  
		if(sEXT) {								// external charge active
			//dbug(F("	VbNS EXT"));
			if(sV2 > CHGHIV2) {					// batt2 charged
				//dbug(F("	VbNS EXT V2>CHI2"));
				if(sV1 < CHGHIV1 )	{			// charge batt 1 by external source
					//dbug(F("	VbNS EXT V1<CHI1"));
					next.setBit(RT2, ON);   
					next.setBit(RT1, ON);   
					next.setBit(RT3, OFF);   
					next.setMsg(P_CHGEXT,"B1"); 
					return; 
				}	
				if(sB3 && sV3 < CHGHIV3 )	{	// charge batt 3 by external source
					//dbug(F("	VbNS EXT V3<CHI3"));
					next.setBit(RT2, ON);   
					next.setBit(RT3, ON);   
					next.setBit(RT1, OFF);   
					next.setMsg(P_CHGEXT,"B3"); 
					return; 
				}
			}
			else if(sR2 && sV2 < CHGLOV2) {
				//dbug(F("	VbNS EXT V2<CHI2"));
				if(sX1) next.setBit(RT1, OFF);   
				if(sX3) next.setBit(RT3, OFF);
			}
			next.setMsg(P_CHGEXT,"B2");
		}
		// not EXT charging	
		//dbug(F("    VbNS !EXT "));

		if(sX1) next.setBit(RT1, OFF);   
		if(sX3) next.setBit(RT3, OFF);

		// these calls do not switch relays without showing message
		if(sR1 && nR1 && switchShutdown(1,RT1,RT2)) return;
		if(sR2 && nR2 && switchShutdown(2,RT2,RT3)) return;
		if(sR3 && nR3 && switchShutdown(3,RT3,NULL)) return;
		warn.reset();
	}

}


uint8_t fB_Vanduino::switchShutdown(uint8_t index,uint16_t relaySrc, uint16_t relayDst ) {  // source and dest corres to ordinate relay numbers (eg., 1,2;  2,3;  3,0)
	uint8_t relayX, battX;
	double vSrc,vDst, LowSrc, LowDst;
	char *textSrc,*textDst;
	//dbug(F("Vssd i:%d, rs:%d , rdst:%d"), index,relaySrc,relayDst);


	switch(relaySrc) {
		case RT1:
			relayX	 = sR1;
			vSrc	 = sV1;
			LowSrc	 = LVDLOV1;
			textSrc  = "R1";
			break;
		case RT2:
			relayX	 = sR2;
			vSrc	= sV2;
			LowSrc = LVDLOV2;
			textSrc = "R2";
			break;
		case RT3:
			relayX	 = sR3;
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
	//dbug(F("Vssd i:%d, bx:%d, vsrc:%f , lovsrc:%f"), index,battX,vDst,LowDst);

	if(relayX && vSrc < LowSrc) {	
		//dbug(F("          Vssd 1"));
// Batt Src selected AND Batt Src below Vdiscon		
		if(battX && vDst > LowDst )  {						// Batt Src selected AND Batt Src below Vdiscon AND Batt 2 above Vdiscon
			//dbug(F("          Vssd 2"));
		// SYNTAX: warning(uint8_t id, uint8_t wdSecs,uint8_t wSecs, uint8_t aSecs) 
			//switch ( warn.warning(index*2, 20,20, 4, WLED)) { 
			switch ( warn.warning(index*2, Tag(_SECWD)->iVal,Tag(_SECWN)->iVal,Tag(_SECAL)->iVal, WLED)) { 
				case _WD_WARN:
					next.setMsg(P_SWITCHTO,textDst);
			//dbug(F("          Vssd _WD_WARN"));
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
		//dbug(F("          Vssd 3"));
		 if(warn.currID == index*2) {					// If Batt Dest voltage drops below Vdiscon while above warn active, clear warning
				warn.reset();
				next.setMsg(P_BLANK);
			}
			//dbug(F("          Vssd  psL "));
			if(priorityShutdownLoad(relaySrc)) {			// Start priority shutdown of Load to see if we can bring Batt Src above Vdiscon
				next.setMsg(P_BLANK);
				return 1;
			}
		//dbug(F("          Vssd 4"));
			switch ( warn.warning(index*2 +1, 20,20, 4, WLED)) { // Start shutdown of Relay Source
		//dbug(F("          Vssd 5"));
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

uint8_t fB_Vanduino::checkVolts(uint16_t relay) {
	switch(relay) {
		case RT1:
			ptV1->read();
			state.dV1 = ptV1->dVal->value;
			if(sV1 > LVDLOV1) return 1;
			break;
		case RT2:
			ptV2->read();
			state.dV2 = ptV2->dVal->value;
			if(sV2 > LVDLOV2) return 1;
			break;
		case RT3:
			ptV3->read();
			state.dV3 = ptV3->dVal->value;
			if(sV3 > LVDLOV3) return 1;
			break;
	}
	return 0;
}

void fB_Vanduino::setRelaysNext() {
	//dbug(F("*** V SRN sf:0x%x"),state.flags);
	//dbug(F("*** V SRN nf:0x%x"),next.flags);

	if(nR1 && nR2 && nR3) {  // failsafe
		alarm.playTag(_TALRMEG);
		next.setMsg(P_ALARM,"123");
	}
	/*
	else if(!sB3 &&  nR3) {  // failsafe
		alarm.playTag(_TALRMEG);
		next.setMsg(P_ALARM,"R3");
	}
	else if(!sB2 &&  nR2) {  // failsafe
		alarm.playTag(_TALRMEG);
		next.setMsg(P_ALARM,"R2");
	}
	*/
	else {	
		if(sR2) { // if R2 on, switch R2 last, else switch first
			relay1.turn(nR1);
			delay(100);
			relay3.turn(nR3);
			delay(100);
			relay4.turn(nR4);
			delay(100);
			relay5.turn(nR5);
			delay(100);
			relay6.turn(nR6);
			delay(100);
			relay2.turn(nR2);
		}
		else  { 
			relay2.turn(nR2);
			delay(100);
			relay1.turn(nR1);
			relay3.turn(nR3);
			delay(100);
			relay4.turn(nR4);
			delay(100);
			relay5.turn(nR5);
			delay(100);
			relay6.turn(nR6);
		}
	}
}

void fB_Vanduino::showState() {
	 uint8_t show = 0;
	 logTimerFlag= 0;
	 static int firstShow = 1;


 	//dbug(F("V ShowState"));

	 if(curr.pageTag == pageTag) show = 1;

	 if(show) {
		 if(firstShow) {
			 if(!sB2) {
				 ptV2->putFormat(_STRIKE);
				 ptV2->showRow(curr.row(V2));
			 }
			 if(!sB3) {
				 ptV3->putFormat(_STRIKE);
				 ptV3->showRow(curr.row(V3));
			 }
			 firstShow = 0;
		}
		 if(sALT !=  pALT) ptALT->showRow(curr.row(ALT));
		 if(fabs(sCL -  pCL) > .5) ptCL->showRow(curr.row(CL));
		 if(fabs(sCX -  pCX) > .2) ptCX->showRow(curr.row(CX));
		 if(fabs(sV0 -  pV0) > .1) ptV0->showRow(curr.row(V0));
		 if(fabs(sV1 -  pV1) > .1) ptV1->showRow(curr.row(V1));
		 if( sB2 && fabs(sV2 -  pV2) > .1) ptV2->showRow(curr.row(V2));
		 if( sB3 && fabs(sV3 -  pV3) > .1) ptV3->showRow(curr.row(V3));
		 if(prevShow.msgIndex != state.msgIndex || prevShow.msgText != state.msgText) menu.showMessage(state.msgIndex,state.msgText);
		 state.copyAllTo(&prevShow);
	 }

	 if(log.flags != state.flags) logTimerFlag= 1;
	 if(fabs(sCL -  gCL) > 1.0)  logTimerFlag = 2;
	 if(fabs(sCX -  gCX) > 0.2)  logTimerFlag = 3;
	 if(fabs(sV1 -  gV1) > 0.1)  logTimerFlag = 4;
	 if(fabs(sV2 -  gV2) > 0.1)  logTimerFlag = 5;
	 if(fabs(sV3 -  gV3) > 0.1)  logTimerFlag = 6;
 
 }

void fB_Vanduino::logTimer() {
	if(logTimerFlag && timer.update(logTimerIndex)) 	state.copyAllTo(&log);
}



#endif
