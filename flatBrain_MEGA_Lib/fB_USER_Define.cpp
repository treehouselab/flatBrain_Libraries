#include "fB_Include.h"


void defineUser() {



	//////////// DEFINE MENU

	//  SYNTAX: Tags can be user-defined, place tag in fB_USER_Define.h
	//  defineHome(tag);					
	//  definePage(tag,parentTag)	
	//  defineRow(tag,targetTag,flags)	
	//////////////////////////////////////////

 	//////////// DEFINE CARDS AND PINS
		defineCard(YCRD,_X76,_C0,_B3);	
		definePin(Y1,YCRD,26,_L,_INPUT,LOW); 
		definePin(Y2,YCRD,26,_R,_INPUT,LOW); 
		definePin(Y3,YCRD,27,_R,_INPUT,LOW); 
		definePin(Y4,YCRD,27,_L,_INPUT,LOW); 
		definePin(Y5,YCRD,28,_R,_INPUT,LOW);
		definePin(Y6,YCRD,28,_L,_INPUT,LOW);
		definePin(YRST,YCRD,25,_L,_INPUT,LOW);
		definePin(YSHFT,YCRD,25,_R,_INPUT,LOW);

		definePin(Y1S,YCRD,18,_L,_INPUT,HIGH); 
		definePin(Y2S,YCRD,19,_R,_INPUT,HIGH); 
		definePin(Y3S,YCRD,19,_L,_INPUT,HIGH); 
		definePin(Y4S,YCRD,20,_R,_INPUT,HIGH); 
		definePin(Y5S,YCRD,20,_L,_INPUT,HIGH);
		definePin(Y6S,YCRD,18,_R,_INPUT,HIGH);
		definePin(_WARN_OVR,YCRD,17,_R,_INPUT,HIGH);
		definePin(_ALARM_LED,YCRD,17,_L,_OUTPUT,HIGH);

		definePin(IGN,YCRD,24,_L,_INPUT,_PGATE); 
		definePin(CL,YCRD,23,_R,_INPUT,_PGATE); 
		definePin(CZ,YCRD,21,_L,_INPUT,_PGATE); 
		definePin(CX,YCRD,23,_L,_INPUT,_PGATE);
		definePin(V0,YCRD,21,_R,_INPUT,_PGATE); 
		definePin(V1,YCRD,22,_L,_INPUT,_PGATE); 
		definePin(V2,YCRD,24,_R,_INPUT,_PGATE); 
		definePin(V3,YCRD,22,_R,_INPUT,_PGATE); 


	//////////////////////////////////////////
   	defineHome(FLATBRAIN);			// MUST HAVE A HOME
		defineJump(SYSTEM);			// SYSTEM Page created in fB_SYS_Defines.cpp
		// OPTIONAL User-Defined Rows start here
		defineJump(RSTATUS);
		defineJump(RPULSE);		
		defineJump(SENSORS);
	//////////////////////////////////////////

   	definePage(RSTATUS,HOME);
		defineRow(V1, FLOAT1 | _LOG);
		defineRow(V2, FLOAT1 | _LOG);
		defineRow(V3, FLOAT1 | _LOG);
		//defineRow(V0, FLOAT1 | _LOG);
		//defineRow(CZ,  FLOAT2 | _LOG);
		defineRow(CX,  FLOAT2 | _LOG);
		defineRow(CL,  FLOAT2 | _LOG);
		//defineRow(IGN,  BLAMP | LOG );
		defineRow(ALT,  BLAMP | _LOG );
		//defineTarget(IGN,VIGN);

   	definePage(RPULSE,HOME);
		defineRow(Y1,BLANK | SHFTPULSE);
		defineRow(Y2,BLANK | SHFTPULSE);
		defineRow(Y3,BLANK | SHFTPULSE);
		defineRow(Y4,BLANK | SHFTPULSE);
		defineRow(Y5,BLANK | SHFTPULSE);
		defineRow(Y6,BLANK | SHFTPULSE);
		defineRow(YRST,BLANK | SHFTPULSE);
		defineAlias(YRST,RESET ALL);

   	definePage(SENSORS,HOME);
		defineRow(V1, FLOAT1 | _LOG);
		defineRow(V2, FLOAT1 | _LOG);
		defineRow(V3, FLOAT1 | _LOG);
		//defineRow(CZ, FLOAT1 | _LOG);
		//defineRow(CC, FLOAT1 | _LOG);
		//defineRow(CL, FLOAT1 | _LOG);
 

 	//////////// DEFINE RECORDS

	defineRecord(V1,LOG2,NULL); // flags: [format] | _STOREE ( for eeprom )
	defineRecord(V2,LOG2,NULL);
	defineRecord(V3,LOG3,NULL);
	defineRecord(CZ,LOG1,NULL);
	defineRecord(CX,LOG1,NULL);
	defineRecord(CL,LOG1,NULL);

	defineAlias(RSTATUS,RELAY STATUS);
	defineAlias(RPULSE,RELAY SWITCH);
	defineAlias(V1,VOLTS B1);
	defineAlias(V2,VOLTS B2);
	defineAlias(V3,VOLTS B3);
	//defineAlias(Y1S,RELAY 1);
	//defineAlias(Y2S,RELAY 2);
	//defineAlias(Y3S,RELAY 3);
	//defineAlias(Y4S,INVERTER);
	//defineAlias(Y5S,LOAD);
	//defineAlias(Y6S,FAN);
	defineCalibrate(V0,posOnly,0.03676,-15.54);
	defineCalibrate(V1,posOnly,0.03676,-15.54);
	defineCalibrate(V2,posOnly,0.03676,-15.54);
	defineCalibrate(V3,posOnly,0.03676,-15.54);

	defineCalibrate(CZ,sysAmps,.02744,-.31);
	defineCalibrate(CX,posOnly,0.03789,-4.020);
	defineCalibrate(CL,loadAmps,0.5167,0);
	//defineCalibrate(IGN,posOnly,0.004883,0);

 }

	double posOnly(fB_Tag* pT,uint16_t intVal) { return max(intVal * pT->dVal->factor + pT->dVal->offset,0); }

	double sysAmps(fB_Tag* pT,uint16_t intVal) { return max((intVal - 512) * pT->dVal->factor + pT->dVal->offset,0); }

	double loadAmps(fB_Tag* pT,uint16_t intVal) {
		//int sign;
		if(intVal > 513 && intVal < 516) return 0;
		return ((double)intVal - 515 )* pT->dVal->factor + pT->dVal->offset;
		//return  sign * ( fabs((intVal - 534 )* pT->dVal->factor) + pT->dVal->offset);
	}

