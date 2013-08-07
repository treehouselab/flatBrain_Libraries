#include <fB_flatBrain.h>
#include <Wire.h>

void setup(){

   Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);


	alarm.enable();

	flatBrain_init();
	defineTags();
	//i2c.scan();
	tft.init(PORTRAIT);
	tft.clear();


	menu.init();

	menu.show(HOME);

	alarm.disableBootBeep();
	alarm.play(ALARM_INIT);
	attachInterrupt(NAV_INT, navigate,FALLING);

	//Card(YCRD)->LED(HIGH);
	unsigned int msec;
	msec = 200;

  //Pin(Y6)->write(HIGH);
 
}

void loop() {
		menu.checkButtonCode();

 }
 
 void navigate() {
 
      menu.buttonCode = tft.readButtons();
}

 
void defineTags() {
//		TWO PASSES!
//		defineTag(uint16_t tag,float value=NULL,float factor=NULL,float offset=NULL)


	defineTag(TCARDS);   
	defineTag(TPINS);    
	defineTag(TTAGS);    
	defineTag(TLOGS);    
	defineTag(TPAGES);    
	defineTag(TROWS); 
	defineTag(FRAM); 
}

void definePins() {
//		TWO PASSES!
//		defineTag(uint16_t tag,float value=NULL,float factor=NULL,float offset=NULL)



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

//		defineGlobalPin(pinTag,format,fileTag, flags)

		defineGlobalPin(Y1S,BINARY,LOG1,NULL); 
		defineGlobalPin(Y2S,BINARY,LOG1,NULL); 
		defineGlobalPin(Y3S,BINARY,LOG1,NULL); 
		defineGlobalPin(Y4S,BINARY,LOG1,NULL); 
		defineGlobalPin(Y5S,BINARY,LOG1,NULL); 
		defineGlobalPin(Y6S,BINARY,LOG1,NULL); 
/*
		defineGlobalPin(Y1,BINARY,LOG1,NULL); 
		defineGlobalPin(Y2,BINARY,LOG1,NULL); 
		defineGlobalPin(Y3,BINARY,LOG1,NULL); 
		defineGlobalPin(Y4,BINARY,LOG1,NULL); 
		defineGlobalPin(Y5,BINARY,LOG1,NULL); 
		defineGlobalPin(Y6,BINARY,LOG1,NULL); 
*/

		defineGlobalPin(V1,FLOAT1,LOG1,NULL); 
		defineGlobalPin(V2,FLOAT1,LOG1,NULL); 
		defineGlobalPin(V3,FLOAT1,LOG1,NULL); 
		defineGlobalPin(CZ,FLOAT2,LOG1,NULL); 
		defineGlobalPin(CC,FLOAT2,LOG1,NULL); 
		defineGlobalPin(CL,FLOAT2,LOG1,NULL); 
       //defineGlobal(G2QWE,1,INT5,LOG1,GINIT | GINPUT );

        defineLog(LOG1);
        //defineLog(LOG2);
        //defineLog(LOG3);
 
      
 }
  void fB_Menu::defineMenu() {
 //												defineRow(uint16_t mTag, const __FlashStringHelper* Ptitle,uint8_t  type,uint8_t  format,uint16_t tTag, float value = NULL)
 // TWO PASSES!

   	defineHome();
		defineRowJumpPage(SYSTEM);
		defineRowJumpPage(GLOBALS);
		defineRowJumpPage(STACK);
		defineRowJumpPage(RELAYPULS);
		defineRowJumpPage(RELAYSTAT);
		defineRowJumpPage(SENSORS);

   	definePage(RELAYPULS,HOME);
		defineRow(Y1, YPULSE,Y1,200);
		defineRow(Y2, YPULSE,Y2, 200);
		defineRow(Y3, YPULSE,Y3, 200);
		defineRow(Y4, YPULSE,Y4, 200);
		defineRow(Y5, YPULSE,Y5, 200);
		defineRow(Y6, YPULSE,Y6, 200);
		defineRow(YRST,PULSE,YRST, 200);

   	definePage(RELAYSTAT,HOME);
		defineRow(Y1S, LAMP,Y1S,NULL);
		defineRow(Y2S, LAMP,Y2S,NULL);
		defineRow(Y3S, LAMP,Y3S,NULL);
		defineRow(Y4S, LAMP,Y4S,NULL);
		defineRow(Y5S, LAMP,Y5S,NULL);
		defineRow(Y6S, LAMP,Y6S,NULL);

   	definePage(SENSORS,HOME);
		defineRow(V1, FLOAT1,V1,NULL);
		defineRow(V2, FLOAT1,V2,NULL);
		defineRow(V3, FLOAT1,V3,NULL);
		defineRow(CZ, FLOAT2,CZ,NULL);
		defineRow(CC, FLOAT2,CC,NULL);
		defineRow(CL, FLOAT2,CL,NULL);
 
 }


 void dumpEEPROM(unsigned int addr, unsigned int length)
{
   // block to 10
   addr = addr / 10 * 10;
   length = (length + 9)/10 * 10;

   byte b = ee.readByte(addr); 
   for (int i = 0; i < length; i++) 
   {
     if (addr % 10 == 0)
     {
       Serial.println();
       Serial.print(addr);
       Serial.print(":\t");
     }
     Serial.print(b);
     b = ee.readByte(++addr); 
     Serial.print("  ");
   }
   Serial.println();
}
