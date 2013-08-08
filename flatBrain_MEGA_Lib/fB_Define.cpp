#include "fB_Include.h"

void definePins() {
//		defineCard(ctag,ctype,i2cAddr, aChan )
//		definePin( uint16_t tag, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval = NULL)

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
}

void defineLogs() {
//		defineRec(tag,format,fTag,flags)

        defineLog(LOG1);
        //defineLog(LOG2);
        //defineLog(LOG3);
}

void defineTags() {
//		defineTag(tag,format,fTag,flags)

		defineTag(Y1S,BINARY,LOG1,NULL); 
		defineTag(Y2S,BINARY,LOG1,NULL); 
		defineTag(Y3S,BINARY,LOG1,NULL); 
		defineTag(Y4S,BINARY,LOG1,NULL); 
		defineTag(Y5S,BINARY,LOG1,NULL); 
		defineTag(Y6S,BINARY,LOG1,NULL); 
/*
		defineTag(Y1,BINARY,LOG1,NULL); 
		defineTag(Y2,BINARY,LOG1,NULL); 
		defineTag(Y3,BINARY,LOG1,NULL); 
		defineTag(Y4,BINARY,LOG1,NULL); 
		defineTag(Y5,BINARY,LOG1,NULL); 
		defineTag(Y6,BINARY,LOG1,NULL); 
*/

		defineTag(V1,FLOAT1,LOG1,NULL); 
		defineTag(V2,FLOAT1,LOG1,NULL); 
		defineTag(V3,FLOAT1,LOG1,NULL); 
		defineTag(CZ,FLOAT2,LOG1,NULL); 
		defineTag(CC,FLOAT2,LOG1,NULL); 
		defineTag(CL,FLOAT2,LOG1,NULL); 
       //defineGlobal(G2QWE,1,INT5,LOG1,GINIT | GINPUT );
 }

  void defineMenu() {
 //		definePage(uint16_t tag, uint16_t  parentTag)
 //		defineRow(uint16_t tag, uint8_t  type,uint8_t  format,uint16_t tTag, float value = NULL)

   	menuHome();
		menuJump(SYSTEM);
		menuJump(RELAYPULS);
		menuJump(RELAYSTAT);
		menuJump(SENSORS);

   	menuPage(DIAGNOSTICS,HOME);
		//menuRow(TSTAGS, NOACT,INT5,TSTAGS,200);
		//menuRow(TUTAGS, NOACT,INT5,TUTAGS,200);
		//menuRow(TPINS, NOACT,INT5,TPINS,200);
		//menuRow(TLOGS, NOACT,INT5,TLOGS,200);
		//menuRow(TPAGES, NOACT,INT5,TPAGES,200);
		//menuRow(TROWS, NOACT,INT5,TROWS,200);
		menuRow(FRAM, NOACT,INT5,FRAM,200);

   	menuPage(RELAYPULS,HOME);
		menuRow(Y1, YPULSE,NULL,Y1,200);
		menuRow(Y2, YPULSE,NULL,Y2,200);
		menuRow(Y3, YPULSE,NULL,Y3,200);
		menuRow(Y4, YPULSE,NULL,Y4,200);
		menuRow(Y5, YPULSE,NULL,Y5,200);
		menuRow(Y6, YPULSE,NULL,Y6,200);
		menuRow(YRST, PULSE,NULL,YRST,200);

   	menuPage(RELAYSTAT,HOME);
		menuRow(Y1S,NOACT, LAMP,Y1S,NULL);
		menuRow(Y2S,NOACT, LAMP,Y2S,NULL);
		menuRow(Y3S,NOACT, LAMP,Y3S,NULL);
		menuRow(Y4S,NOACT, LAMP,Y4S,NULL);
		menuRow(Y5S,NOACT, LAMP,Y5S,NULL);
		menuRow(Y6S,NOACT, LAMP,Y6S,NULL);

   	menuPage(SENSORS,HOME);
		menuRow(V1,NOACT, FLOAT1,V1,NULL);
		menuRow(V2,NOACT, FLOAT1,V2,NULL);
		menuRow(V3,NOACT, FLOAT1,V3,NULL);
		menuRow(CZ,NOACT, FLOAT2,CZ,NULL);
		menuRow(CC,NOACT, FLOAT2,CC,NULL);
		menuRow(CL,NOACT, FLOAT2,CL,NULL);
 
 }


