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

		definePin(Y1,YCRD,26,COL_L,OUTPUT,HIGH); 
		definePin(Y2,YCRD,26,COL_R,OUTPUT,HIGH); 
		definePin(Y3,YCRD,27,COL_R,OUTPUT,LOW); 
		definePin(Y4,YCRD,27,COL_L,INPUT,LOW); 
		definePin(Y5,YCRD,28,COL_R,INPUT,LOW);
		definePin(Y6,YCRD,28,COL_L,INPUT,LOW);
		definePin(YRST,YCRD,25,COL_L,INPUT,LOW);
		definePin(YSHFT,YCRD,25,COL_R,INPUT,LOW);

		definePin(Y1S,YCRD,18,COL_L,INPUT,HIGH); 
		definePin(Y2S,YCRD,19,COL_R,INPUT,LOW); 
		definePin(Y3S,YCRD,19,COL_L,OUTPUT,HIGH); 
		definePin(Y4S,YCRD,20,COL_R,INPUT,LOW); 
		definePin(Y5S,YCRD,20,COL_L,INPUT,LOW);
		definePin(Y6S,YCRD,18,COL_R,INPUT,LOW);

		definePin(V1,YCRD,22,COL_L,INPUT ,LOW); 
		definePin(V2,YCRD,24,COL_R,INPUT,PGATE); 
		definePin(V3,YCRD,22,COL_R,INPUT,LOW); 
		definePin(CZ,YCRD,21,COL_L,INPUT,PGATE); 
		definePin(CC,YCRD,23,COL_L,INPUT,LOW);
		definePin(CL,YCRD,23,COL_R,INPUT,LOW); 

	//////////////////////////////////////////
   	defineHome(FLATBRAIN);			// MUST HAVE A HOME
		defineJump(SYSTEM);			// SYSTEM Page created in fB_SYS_Defines.cpp
		defineJump(RELAYPULS);		// OPTIONAL User-Defined Rows start here
		defineJump(RELAYSTAT);
		defineJump(SENSORS);
	//////////////////////////////////////////

   	definePage(RELAYPULS,HOME);
		defineRow(Y1,NULL,BLANK | SHFTPULSE);
		defineRow(Y2,NULL,BLANK | SHFTPULSE);
		defineRow(Y3,NULL,BLANK | SHFTPULSE);
		defineRow(Y4,NULL,BLANK | SHFTPULSE);
		defineRow(Y5,NULL,BLANK | SHFTPULSE);
		defineRow(Y6,NULL,BLANK | SHFTPULSE);
		defineRow(YRST,NULL,BLANK | SHFTPULSE);

   	definePage(RELAYSTAT,HOME);
		defineRow(Y1S, NULL, BLAMP| UPDATE);
		defineRow(Y2S, NULL, BLAMP| UPDATE);
		defineRow(Y3S, NULL, BLAMP| UPDATE);
		defineRow(Y4S, NULL, BLAMP| UPDATE);
		defineRow(Y5S, NULL, BLAMP| UPDATE);
		defineRow(Y6S, NULL, BLAMP| UPDATE);

   	definePage(SENSORS,HOME);
		defineRow(V1, NULL, FLOAT1 | LOG);
		defineRow(V2, NULL, FLOAT1 | LOG);
		defineRow(V3, NULL, FLOAT1 | LOG);
		defineRow(CZ, NULL, FLOAT2 | LOG);
		defineRow(CC, NULL, FLOAT2 | LOG);
		defineRow(CL, NULL, FLOAT2 | LOG);
 

 	//////////// DEFINE RECORDS

	defineRecord(V1,LOG2,NULL); // flags: [format] | STOREE ( for eeprom )
	defineRecord(V1,LOG2,NULL);
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




 }

