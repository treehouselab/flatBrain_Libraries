#include "fB_Include.h"


void defineUser() {

	//////////// DEFINE CARDS AND PINS

	defineCard(YCRD,X76,C0,B1);	

		definePin(Y1,YCRD,26,L,INPUT,LOW); 
		definePin(Y2,YCRD,26,R,INPUT,LOW); 
		definePin(Y3,YCRD,27,R,INPUT,LOW); 
		definePin(Y4,YCRD,27,L,INPUT,LOW); 
		definePin(Y5,YCRD,28,R,INPUT,LOW);
		definePin(Y6,YCRD,28,L,INPUT,LOW);
		definePin(YRST,YCRD,25,L,INPUT,LOW);
		definePin(YSHFT,YCRD,25,R,INPUT,LOW);

		definePin(Y1S,YCRD,18,L,INPUT,LOW); 
		definePin(Y2S,YCRD,19,R,INPUT,LOW); 
		definePin(Y3S,YCRD,19,L,INPUT,LOW); 
		definePin(Y4S,YCRD,20,R,INPUT,LOW); 
		definePin(Y5S,YCRD,20,L,INPUT,LOW);
		definePin(Y6S,YCRD,18,R,INPUT,LOW);

		definePin(V1,YCRD,22,L,INPUT,LOW); 
		definePin(V2,YCRD,24,R,INPUT,LOW); 
		definePin(V3,YCRD,22,R,INPUT,LOW); 
		definePin(CZ,YCRD,21,L,INPUT,LOW); 
		definePin(CC,YCRD,23,L,INPUT,LOW);
		definePin(CL,YCRD,23,R,INPUT,LOW); 

	//////////// DEFINE MENU

	//  SYNTAX: Tags can be user-defined, place tag in fB_USER_Define.h
	//  defineHome(tag);					
	//  definePage(tag,parentTag)	
	//  defineRow(tag,targetTag,flags)	
	//////////////////////////////////////////

	//////////////////////////////////////////
   	defineHome(FLATBRAIN);			// MUST HAVE A HOME
		defineJump(SYSTEM);			// SYSTEM Page created in fB_SYS_Defines.cpp
		defineJump(RELAYPULS);		// OPTIONAL User-Defined Rows start here
		defineJump(RELAYSTAT);
		defineJump(SENSORS);
	//////////////////////////////////////////
/*
   	definePage(DIAGNOSTICS,HOME);
		//defineRow(TSTAGS, NOACT,INT5,TSTAGS,200);
		//defineRow(TUTAGS, NOACT,INT5,TUTAGS,200);
		//defineRow(TPINS, NOACT,INT5,TPINS,200);
		//defineRow(TLOGS, NOACT,INT5,TLOGS,200);
		//defineRow(TPAGES, NOACT,INT5,TPAGES,200);
		//defineRow(TROWS, NOACT,INT5,TROWS,200);
		defineRow(FRAM, INT5,NOACT,NULL);
*/
   	definePage(RELAYPULS,HOME);
		defineRow(Y1,NULL,BLANK | SHFTPULSE);
		defineRow(Y2,NULL,BLANK | SHFTPULSE);
		defineRow(Y3,NULL,BLANK | SHFTPULSE);
		defineRow(Y4,NULL,BLANK | SHFTPULSE);
		defineRow(Y5,NULL,BLANK | SHFTPULSE);
		defineRow(Y6,NULL,BLANK | SHFTPULSE);
		defineRow(YRST,NULL,BLANK | SHFTPULSE);

   	definePage(RELAYSTAT,HOME);
		defineRow(Y1S, NULL, BINARY| REFRESH);
		defineRow(Y2S, NULL, BINARY| REFRESH);
		defineRow(Y3S, NULL, BINARY| REFRESH);
		defineRow(Y4S, NULL, BINARY| REFRESH);
		defineRow(Y5S, NULL, BINARY| REFRESH);
		defineRow(Y6S, NULL, BINARY| REFRESH);

   	definePage(SENSORS,HOME);
		defineRow(V1, NULL, FLOAT1);
		defineRow(V2, NULL, FLOAT1);
		defineRow(V3, NULL, FLOAT1);
		defineRow(CZ, NULL, FLOAT2);
		defineRow(CC, NULL, FLOAT2);
		defineRow(CL, NULL, FLOAT2);
 
 
 	//////////// DEFINE LOGS AND RECORDS
	//// Tags flagged with LOG which do not have a Record Log specified
	//// will be logged to LOGDEF.TXT

	 ////defineLog(LOG2);
     ////defineLog(LOG3);

	////defineRecord(tag,fTag,flags)
 
 }

