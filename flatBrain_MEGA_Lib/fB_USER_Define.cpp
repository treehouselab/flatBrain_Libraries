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
		definePin(_WARN_OVR,YCRD,17,_R,_INPUT,LOW);
		definePin(_TALRMLED,YCRD,17,_L,_OUTPUT,HIGH);

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
		defineJump(VSTATUS);
		defineJump(VGLOBALS);
		defineJump(LOGS);
		//defineJump(RPULSE);		
		//defineJump(SENSORS);
		defineRow(DRST,_BLANK);
		defineTarget(DRST,YCRD);

	//////////////////////////////////////////

   	definePage(VSTATUS,HOME);
		defineRow(V0, _FLOAT1 );
		defineRow(V1, _FLOAT1 | _UPDATE);
		defineRow(V2, _FLOAT1 | _UPDATE );
		defineRow(V3, _FLOAT1 | _UPDATE );
		//defineRow(CZ,  _FLOAT2 );
		defineRow(CX,  _FLOAT1 | _UPDATE );
		defineRow(CL,  _FLOAT1 |  _UPDATE);
		//defineRow(IGN,  _BLAMP | LOG );
		defineRow(ALT,  _BLAMP  | _NOACT );
		defineJump(VGLOBALS);
		defineJump(LOGS);
		//defineTarget(IGN,VIGN);

 /*  	definePage(RPULSE,HOME);
		defineRow(Y1,_BLANK | SHFTPULSE);
		defineRow(Y2,_BLANK | SHFTPULSE);
		defineRow(Y3,_BLANK | SHFTPULSE);
		defineRow(Y4,_BLANK | SHFTPULSE);
		defineRow(Y5,_BLANK | SHFTPULSE);
		defineRow(Y6,_BLANK | SHFTPULSE);
		defineRow(YRST,_BLANK | SHFTPULSE);
		defineAlias(YRST,RESET ALL);

   	definePage(SENSORS,HOME);
		defineRow(V1, _FLOAT1 );
		defineRow(V2, _FLOAT1 );
		defineRow(V3, _FLOAT1 );
		//defineRow(CZ, _FLOAT1 );
		//defineRow(CC, _FLOAT1 );
		//defineRow(CL, _FLOAT1 );
*/
	definePage(VGLOBALS,HOME);
		defineJump(LVDLIMITS);
		defineRow(VEXS, _FLOAT1 | _INCR | _LOADEE);
		defineRow(VALT, _FLOAT1 | _INCR | _LOADEE);
		defineRow(CHLO, _FLOAT1 | _INCR | _LOADEE);
		defineRow(CHHI, _FLOAT1 | _INCR | _LOADEE);
		defineRow(CPSEC,_FLOAT1 | _INCR | _LOADEE);
		defineRow(BKSEC,_FLOAT1 | _INCR | _LOADEE);
		defineRow(LGMIN,_FLOAT2 | _INCR | _LOADEE);
		defineRow(ESTOR,_BLANK);

	definePage(LVDLIMITS,VGLOBALS);
		defineRow(DLO1, _FLOAT1 | _INCR | _LOADEE);
		defineRow(DLO2, _FLOAT1 | _INCR | _LOADEE);
		defineRow(DLO3, _FLOAT1 | _INCR | _LOADEE);
		defineRow(DHI1, _FLOAT1 | _INCR | _LOADEE);
		defineRow(DHI2, _FLOAT1 | _INCR | _LOADEE);
		defineRow(DHI3, _FLOAT1 | _INCR | _LOADEE);
		defineRow(ESTOR,_BLANK);


 	//////////// DEFINE RECORDS

	defineLog(CPSEC,VGLOG);
	defineLog(BKSEC,VGLOG);
	defineLog(LGMIN,VGLOG);
	defineLog(VEXS,VGLOG);
	defineLog(VALT,VGLOG);
	defineLog(CHLO,VGLOG);
	defineLog(CHHI,VGLOG);
	defineLog(DLO1,VGLOG);
	defineLog(DLO2,VGLOG);
	defineLog(DLO3,VGLOG);
	defineLog(DHI1,VGLOG);
	defineLog(DHI2,VGLOG);
	defineLog(DHI3,VGLOG);

	defineTag(LBOOT, _BLAMP);
	defineLog(LBOOT,VDATA);
	defineLog(Y1S,VDATA);
	defineLog(Y2S,VDATA);
	defineLog(Y3S,VDATA);
	defineLog(Y4S,VDATA);
	defineLog(Y5S,VDATA);
	defineLog(Y6S,VDATA);
	defineLog(ALT,VDATA); 
	defineLog(V1,VDATA);
	defineLog(V2,VDATA);
	defineLog(V3,VDATA);
	defineLog(CL,VDATA);
	defineLog(CX,VDATA);
	//defineLog(CZ,VDATA);

	defineAlias(VSTATUS,VAN STATUS);
	defineAlias(VGLOBALS,VAN GLOBALS);
	defineAlias(ALT,ALTERNATOR);
	//defineAlias(RPULSE,RELAY SWITCH);
	defineAlias(V1,VOLTS B1);
	defineAlias(V2,VOLTS B2);
	defineAlias(V3,VOLTS B3);
	defineAlias(CX,AMPS EXT);
	defineAlias(CL,AMPS LOAD);
	defineAlias(LGMIN,LOG MINS);
	defineAlias(BKSEC,BKGR SECS);
	defineAlias(CPSEC,DISP SECS);

	//defineAlias(Y1S,RELAY 1);
	//defineAlias(Y2S,RELAY 2);
	//defineAlias(Y3S,RELAY 3);
	//defineAlias(Y4S,INVERTER);
	//defineAlias(Y5S,LOAD);
	//defineAlias(Y6S,FAN);
	defineCalibrate(V0,posOnly,0.01515,-.1818);
	defineCalibrate(V1,posOnly,0.0487,-22.99);
	defineCalibrate(V2,posOnly,0.0487,-22.99);
	defineCalibrate(V3,posOnly,0.0487,-22.99);

	defineCalibrate(CZ,sysAmps,.02744,-.31);
	defineCalibrate(CX,posOnly,0.03789,-4.020);
	defineCalibrate(CL,loadAmps,0.35,-179.20);

	defineValue(VEXS,9.0,0.1);
	defineValue(VALT,13.8,0.1);
	defineValue(CHLO,12.5,0.1);
	defineValue(CHHI,13.6,0.1);
	defineValue(DLO1,12.5,0.1);
	defineValue(DLO2,11.0,0.1);
	defineValue(DLO3,10.0,0.1);
	defineValue(DHI1,13.0,0.1);
	defineValue(DHI2,12.0,0.1);
	defineValue(DHI3,11.0,0.1);
	defineValue(CPSEC,4.0,0.5);
	defineValue(BKSEC,10.0,0.5);
	defineValue(LGMIN,5,0.25);

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
	_pTiK1coro = Tag(_WARN_OVR); // interrupt K1 coroborate

 }


	double posOnly(fB_Tag* pT,uint16_t intVal) { return max(intVal * pT->dVal->factor + pT->dVal->offset,0); }

	double sysAmps(fB_Tag* pT,uint16_t intVal) { return max((intVal - 512) * pT->dVal->factor + pT->dVal->offset,0); }

	double loadAmps(fB_Tag* pT,uint16_t intVal) {
		double load;
		load = (double)intVal * pT->dVal->factor + pT->dVal->offset;
		if(fabs(load) < 1) return 0;
		return load;
		//return  sign * ( fabs((intVal - 534 )* pT->dVal->factor) + pT->dVal->offset);
	}

