#include "fB_Include.h"


void defineUser() {



	//////////// DEFINE MENU

	//  SYNTAX: Tags can be user-defined, place tag in fB_USER_Define.h
	//  defineHome(tag);					
	//  definePage(tag,parentTag)	
	//  defineRow(tag,targetTag,flags)	
	//////////////////////////////////////////

 	//////////// DEFINE CARDS AND PINS
		defineCard(YCRD,X76,C0,B1);	

		definePin(Y1,YCRD,26,COL_L,INPUT,LOW); 
		definePin(Y2,YCRD,26,COL_R,INPUT,LOW); 
		definePin(Y3,YCRD,27,COL_R,INPUT,LOW); 
		definePin(Y4,YCRD,27,COL_L,INPUT,LOW); 
		definePin(Y5,YCRD,28,COL_R,INPUT,LOW);
		definePin(Y6,YCRD,28,COL_L,INPUT,LOW);
		definePin(YRST,YCRD,25,COL_L,INPUT,LOW);
		definePin(YSHFT,YCRD,25,COL_R,INPUT,LOW);

		definePin(Y1S,YCRD,18,COL_L,INPUT,LOW); 
		definePin(Y2S,YCRD,19,COL_R,INPUT,LOW); 
		definePin(Y3S,YCRD,19,COL_L,INPUT,LOW); 
		definePin(Y4S,YCRD,20,COL_R,INPUT,LOW); 
		definePin(Y5S,YCRD,20,COL_L,INPUT,LOW);
		definePin(Y6S,YCRD,18,COL_R,INPUT,LOW);

		definePin(V1,YCRD,22,COL_L,INPUT ,LOW); 
		definePin(V2,YCRD,24,COL_R,INPUT,LOW); 
		definePin(V3,YCRD,22,COL_R,INPUT,LOW); 
		definePin(CZ,YCRD,21,COL_L,INPUT,LOW); 
		definePin(CC,YCRD,23,COL_L,INPUT,LOW);
		definePin(CL,YCRD,23,COL_R,INPUT,LOW); 

	//////////////////////////////////////////
   	defineHome(FLATBRAIN);			// MUST HAVE A HOME
		defineJump(SYSTEM);			// SYSTEM Page created in fB_SYS_Defines.cpp
		defineJump(RPULSE);		// OPTIONAL User-Defined Rows start here
		defineJump(RSTATUS);
		defineJump(SENSORS);
	//////////////////////////////////////////

   	definePage(RPULSE,HOME);
		defineRow(Y1,BLANK | SHFTPULSE);
		defineRow(Y2,BLANK | SHFTPULSE);
		defineRow(Y3,BLANK | SHFTPULSE);
		defineRow(Y4,BLANK | SHFTPULSE);
		defineRow(Y5,BLANK | SHFTPULSE);
		defineRow(Y6,BLANK | SHFTPULSE);
		defineRow(YRST,BLANK | SHFTPULSE);
		defineAlias(YRST,RESET ALL);

   	definePage(RSTATUS,HOME);
		defineRow(Y1S, BLAMP| UPDATE);
		defineRow(Y2S, BLAMP| UPDATE);
		defineRow(Y3S, BLAMP| UPDATE);
		defineRow(Y4S, BLAMP| UPDATE);
		defineRow(Y5S, BLAMP| UPDATE);
		defineRow(Y6S, BLAMP| UPDATE);

   	definePage(SENSORS,HOME);
		defineRow(V1, FLOAT1 | LOG);
		defineRow(V2, FLOAT1 | LOG);
		defineRow(V3, FLOAT1 | LOG);
		defineRow(CZ, FLOAT2 | LOG);
		defineRow(CC, FLOAT2 | LOG);
		defineRow(CL, FLOAT2 | LOG);
 

 	//////////// DEFINE RECORDS

	defineRecord(V1,LOG2,NULL); // flags: [format] | STOREE ( for eeprom )
	defineRecord(V2,LOG2,NULL);
	defineRecord(V3,LOG3,NULL);
	defineRecord(CZ,LOG1,NULL);
	defineRecord(CC,LOG1,NULL);
	defineRecord(CL,LOG1,NULL);

	defineCalibrate(V1,.004882,0.19);
	defineCalibrate(V2,.004882,0.19);
	defineCalibrate(V3,.004882,0.19);
	defineCalibrate(CZ,.004882,0.19);
	defineCalibrate(CC,.004882,0.19);
	defineCalibrate(CL,.004882,0.19);

	defineAlias(RSTATUS,RELAY STATUS);
	defineAlias(RPULSE,RELAY SWITCH);




 }

