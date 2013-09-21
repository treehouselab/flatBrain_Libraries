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
		defineJump(VLIMITS);
		defineRow(DRST,_BLANK);
		defineTarget(DRST,YCRD);

	//////////////////////////////////////////

   	definePage(RSTATUS,HOME);
		defineRow(V1, _FLOAT1 | _LOG);
		defineRow(V2, _FLOAT1 | _LOG);
		defineRow(V3, _FLOAT1 | _LOG);
		//defineRow(V0, _FLOAT1 | _LOG);
		//defineRow(CZ,  _FLOAT2 | _LOG);
		defineRow(CX,  _FLOAT2 | _LOG);
		defineRow(CL,  _FLOAT2 | _LOG);
		//defineRow(IGN,  _BLAMP | LOG );
		defineRow(ALT,  _BLAMP | _LOG );
		//defineTarget(IGN,VIGN);

   	definePage(RPULSE,HOME);
		defineRow(Y1,_BLANK | SHFTPULSE);
		defineRow(Y2,_BLANK | SHFTPULSE);
		defineRow(Y3,_BLANK | SHFTPULSE);
		defineRow(Y4,_BLANK | SHFTPULSE);
		defineRow(Y5,_BLANK | SHFTPULSE);
		defineRow(Y6,_BLANK | SHFTPULSE);
		defineRow(YRST,_BLANK | SHFTPULSE);
		defineAlias(YRST,RESET ALL);

   	definePage(SENSORS,HOME);
		defineRow(V1, _FLOAT1 | _LOG);
		defineRow(V2, _FLOAT1 | _LOG);
		defineRow(V3, _FLOAT1 | _LOG);
		//defineRow(CZ, _FLOAT1 | _LOG);
		//defineRow(CC, _FLOAT1 | _LOG);
		//defineRow(CL, _FLOAT1 | _LOG);

	definePage(VLIMITS,HOME);
		defineRow(VEXS, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(VALT, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(CHLO, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(CHHI, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(DLO1, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(DLO2, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(DLO3, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(DHI1, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(DHI2, _FLOAT1 | _INCR | _LOADEE| _LOG);
		defineRow(DHI3, _FLOAT1 | _INCR | _LOADEE| _LOG);


 	//////////// DEFINE RECORDS

	defineRecord(V1,LOG2,NULL); // flags: [format] | _LOADEE( for eeprom )
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

	defineIncrement(VEXS,9.0,0.1);
	defineIncrement(VALT,13.8,0.1);
	defineIncrement(CHLO,12.5,0.1);
	defineIncrement(CHHI,13.6,0.1);
	defineIncrement(DLO1,12.5,0.1);
	defineIncrement(DLO2,11.0,0.1);
	defineIncrement(DLO3,10.0,0.1);
	defineIncrement(DHI1,13.0,0.1);
	defineIncrement(DHI2,12.0,0.1);
	defineIncrement(DHI3,11.0,0.1);

	defineAlias(VEXS,BATT EXST);
	defineAlias(VALT,ALTN ON);
	defineAlias(CHLO,CHG BEGIN);
	defineAlias(CHHI,CHG STOP);
	defineAlias(DLO1,LVDLO 1);
	defineAlias(DLO2,LVDLO 2);
	defineAlias(DLO3,LVDLO 3);
	defineAlias(DHI1,LVDHI 1);
	defineAlias(DHI2,LVDHI 2);
	defineAlias(DHI3,LVDHI 3);

	_pTiK1shft = Tag(YSHFT); // interrupt K1 shift key

 }


	double posOnly(fB_Tag* pT,uint16_t intVal) { return max(intVal * pT->dVal->factor + pT->dVal->offset,0); }

	double sysAmps(fB_Tag* pT,uint16_t intVal) { return max((intVal - 512) * pT->dVal->factor + pT->dVal->offset,0); }

	double loadAmps(fB_Tag* pT,uint16_t intVal) {
		//int sign;
		if(intVal > 513 && intVal < 516) return 0;
		return ((double)intVal - 515 )* pT->dVal->factor + pT->dVal->offset;
		//return  sign * ( fabs((intVal - 534 )* pT->dVal->factor) + pT->dVal->offset);
	}

