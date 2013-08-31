#include "fB_Include.h"


void defineUser() {



	//////////// DEFINE MENU

	//  SYNTAX: Tags can be user-defined, place tag in fB_USER_Define.h
	//  defineHome(tag);					
	//  definePage(tag,parentTag)	
	//  defineRow(tag,targetTag,flags)	
	//////////////////////////////////////////

 	//////////// DEFINE CARDS AND PINS
		defineCard(YCRD,X76,C0,B0);	

		definePin(Y1,YCRD,26,COL_L,INPUT,LOW); 
		definePin(Y2,YCRD,26,COL_R,INPUT,LOW); 
		definePin(Y3,YCRD,27,COL_R,INPUT,LOW); 
		definePin(Y4,YCRD,27,COL_L,INPUT,LOW); 
		definePin(Y5,YCRD,28,COL_R,INPUT,LOW);
		definePin(Y6,YCRD,28,COL_L,INPUT,LOW);
		definePin(YRST,YCRD,25,COL_L,INPUT,LOW);
		definePin(YSHFT,YCRD,25,COL_R,INPUT,LOW);

		definePin(Y1S,YCRD,18,COL_L,INPUT,HIGH); 
		definePin(Y2S,YCRD,19,COL_R,INPUT,HIGH); 
		definePin(Y3S,YCRD,19,COL_L,INPUT,HIGH); 
		definePin(Y4S,YCRD,20,COL_R,INPUT,HIGH); 
		definePin(Y5S,YCRD,20,COL_L,INPUT,HIGH);
		definePin(Y6S,YCRD,18,COL_R,INPUT,HIGH);

		definePin(V0,YCRD,21,COL_L,INPUT,PGATE); 
		definePin(V1,YCRD,22,COL_L,INPUT,PGATE); 
		definePin(V2,YCRD,24,COL_R,INPUT,PGATE); 
		definePin(V3,YCRD,22,COL_R,INPUT,LOW); 
		definePin(CZ,YCRD,21,COL_L,INPUT,LOW); 
		definePin(CC,YCRD,23,COL_L,INPUT,LOW);
		definePin(CL,YCRD,23,COL_R,INPUT,LOW); 

	//////////////////////////////////////////
   	defineHome(FLATBRAIN);			// MUST HAVE A HOME
		defineJump(SYSTEM);			// SYSTEM Page created in fB_SYS_Defines.cpp
		// OPTIONAL User-Defined Rows start here
		defineJump(RSTATUS);
		defineJump(RPULSE);		
		defineJump(SENSORS);
	//////////////////////////////////////////

   	definePage(RSTATUS,HOME);
		defineRow(V1, FLOAT1 | LOG);
		defineRow(V2, FLOAT1 | LOG);
		defineRow(V3, FLOAT1 | LOG);
		defineRow(Y1S, BLAMP| UPDATE);
		defineRow(Y2S, BLAMP| UPDATE);
		defineRow(Y3S, BLAMP| UPDATE);
		defineRow(Y4S, BLAMP| UPDATE);
		defineRow(Y5S, BLAMP| UPDATE);
		defineRow(Y6S, BLAMP| UPDATE);
		defineRow(V0, FLOAT2 | LOG);


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
		defineRow(V1, FLOAT1 | LOG);
		defineRow(V2, FLOAT1 | LOG);
		defineRow(V3, FLOAT1 | LOG);
		defineRow(CZ, FLOAT1 | LOG);
		defineRow(CC, FLOAT1 | LOG);
		defineRow(CL, FLOAT1 | LOG);
 

 	//////////// DEFINE RECORDS

	defineRecord(V1,LOG2,NULL); // flags: [format] | STOREE ( for eeprom )
	defineRecord(V2,LOG2,NULL);
	defineRecord(V3,LOG3,NULL);
	defineRecord(CZ,LOG1,NULL);
	defineRecord(CC,LOG1,NULL);
	defineRecord(CL,LOG1,NULL);

	defineCalibrate(V0,.032895,-13.26);
	defineCalibrate(V1,0.032468,-12.8);
	defineCalibrate(V2,0.032468,-12.8);
	defineCalibrate(V3,0.032468,-12.8);
	defineCalibrate(CZ,.004882,0.19);
	defineCalibrate(CC,.004882,0.19);
	defineCalibrate(CL,.004882,0.19);

	defineAlias(RSTATUS,RELAY STATUS);
	defineAlias(RPULSE,RELAY SWITCH);
	defineAlias(V1,VOLTS B1);
	defineAlias(V2,VOLTS B2);
	defineAlias(V3,VOLTS B3);
	defineAlias(Y1S,RELAY 1);
	defineAlias(Y2S,RELAY 2);
	defineAlias(Y3S,RELAY 3);
	defineAlias(Y4S,INVERTER);
	defineAlias(Y5S,LOAD);
	defineAlias(Y6S,FAN);




 }

