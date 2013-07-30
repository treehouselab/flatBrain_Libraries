#include <flatBrain.h>
#include <Wire.h>

unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

void setup(){

    Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);

    //_i2cspeed = i2cspeed;
	i2c.begin();
	i2c.setSpeed(I2CFAST);
	//i2c.timeOut(I2CTIMEOUT);
    //    i2c.begin();
        //i2c.scan();

         alarm.enable();
         brain.init(I2CFAST);

    Serial.print("Free RAM(b):");
    Serial.println(freeRAM());

         
 
        tft.init(PORTRAIT);
        tft.clear();
  
        menu.init();
  
        menu.show(HOME);
   
        createStack("Cards", brain.cardCount);   
        createStack("Pins", brain.pinCount);    
        createStack("Globals", brain.globalCount);    
        createStack("Stacks", brain.totalsStacks);    
        createStack("Logs", brain.logCount);    
        createStack("Pages", menu.totalPages);    
        createStack("Rows", menu.totalRows); 
        createStack("Free RAM",freeRAM()); 

               
       //char buffer[30];       
       //Serial.print("DateTime: ");
       //Serial.println(rtc.stamp(buffer));
 


    
      /*  
   	Serial.print("totalPins ");
    	Serial.println(brain.pinCount,DEC);
	Serial.print("totalPages ");
    	Serial.println(menu.totalPages,DEC);
	Serial.print("totalRows ");
    	Serial.println(menu.totalRows,DEC);
    	Serial.print("*totalStacks ");
        Serial.println(brain.stackCount,DEC);
 	Serial.print("*totalGlobals ");
    	Serial.println(brain.globalCount,DEC);
  	Serial.print("*totalLogs ");
    	Serial.println(brain.logCount,DEC);
*/
         alarm.disableBootBeep();
         //alarm.play(ALARM_INIT);
	attachInterrupt(NAV_INT, navigate,FALLING);

	//Card(YCRD)->LED(HIGH);
	unsigned int msec;
	msec = 200;
/*
	 Pin(Y1)->yButtonON();
	 delay(1000);
	 Pin(Y1)->yButtonOFF();
	 delay(1000);
	 Pin(Y2)->yButtonON();
	 delay(1000);
	 Pin(Y2)->yButtonOFF();
	 delay(1000);
	 Pin(Y3)->yButtonON();
	 delay(1000);
	 Pin(Y3)->yButtonOFF();
	 delay(1000);
	 Pin(Y4)->yButtonON();
	 delay(1000);
	 Pin(Y4)->yButtonOFF();
	 delay(1000);
	 Pin(Y5)->yButtonON();
	 delay(1000);
	 Pin(Y5)->yButtonOFF();
	 delay(1000);
	 Pin(Y6)->yButtonON();
	 delay(1000);
	 Pin(Y6)->yButtonOFF();
	 */

 
}

void loop() {
		menu.checkButtonCode();
		/*
		Pin(PLED1)->write(HIGH);
			 Card(YCRD)->LED(LOW);

		delay(1000);
		Pin(PLED1)->write(LOW);
			 Card(YCRD)->LED(HIGH);

		delay(1000);
		*/

         //Pin(UTEST1)->write(HIGH);
         //pcf->digitalWrite(CD16INH,HIGH);
 
 //delay(2000);
           //Pin(UTEST1)->write(LOW);
           //Pin(UTEST2)->write(LOW);
          //pcf->digitalWrite(CD161NH,LOW);


  //for(int i=0;i<8;i++) mcp->digitalWrite(i,LOW);
  //delay(2000);
  
  

 }
 
 void navigate() {
 
      menu.buttonCode = tft.readButtons();
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
 

   
void fB_Brain::defineElements() {
    // TWO PASSES!
    
  /*
	createCard(YCRD,X76,C0,B1);		
		createPin(Y1,YCRD,26,L,INPUT,LOW); 
		createPin(Y2,YCRD,26,R,INPUT,LOW); 
		createPin(Y3,YCRD,27,R,INPUT,LOW); 
		createPin(Y4,YCRD,27,L,INPUT,LOW); 
		createPin(Y5,YCRD,28,R,INPUT,LOW);
		createPin(Y6,YCRD,28,L,INPUT,LOW);
		createPin(YRST,YCRD,25,L,INPUT,LOW);
		createPin(YSHFT,YCRD,25,R,INPUT,LOW);

		createGlobalPin(Y1,PULSE,LOG2,NULL); 
		createGlobalPin(Y2,PULSE,LOG2,NULL); 
		createGlobalPin(Y3,PULSE,LOG2,NULL); 
		createGlobalPin(Y4,PULSE,LOG2,NULL); 
		createGlobalPin(Y5,PULSE,LOG2,NULL); 
		createGlobalPin(Y6,PULSE,LOG2,NULL); 
		createGlobalPin(YRST,PULSE,LOG2,NULL); 
		createGlobalPin(YSHFT,PULSE,LOG2,NULL); 


		createPin(Y1S,YCRD,18,L,INPUT,LOW); 
		createPin(Y2S,YCRD,19,R,INPUT,LOW); 
		createPin(Y3S,YCRD,19,L,INPUT,LOW); 
		createPin(Y4S,YCRD,20,R,INPUT,LOW); 
		createPin(Y5S,YCRD,20,L,INPUT,LOW);
		createPin(Y6S,YCRD,18,R,INPUT,LOW);



		//createPin(LRON,VLVD,18,R,OUTPUT,LOW);
		//createPin(LOVR,VLVD,19,L,INPUT,LOW);
		//createPin(LADR,VLVD,20,R,OUTPUT,LOW);
		//createPin(LV0,VLVD,22,L,INPUT,LOW);

  	   //createPin(UTEST2,UTIL,32,R,OUTPUT,HIGH);

       //createGlobal(G2QWE,1,INT5,LOG1,GINIT | GINPUT );
       //createGlobal(G4,25.99,FLOAT1,LOG1,GINIT | GINPUT );
       //createGlobal(G5,3076,INT5,LOG1,GINIT);

        //createLog(LOG1);
        createLog(LOG2);
        //createLog(LOG3);
 
*/
       
      //void createPin(uint16_t pTag, uint8_t  cTag, uint8_t row, uint8_t side, uint8_t iodir, uint8_t ival)
      //void createPin(uint16_t pTag, uint8_t pnum, uint8_t mode(D,A), uint8_t iodir(INPUT<OUTPUT), uint8_t ival)
      
 }
  void fB_Menu::defineMenu() {
   // TWO PASSES!
 
   	createHome(FLATBRAIN);
            createRow(SYSTEM);
            createRow(GLOBALS);
            createRow(STACK);
            
            /*
            createRow(VANDUINO);

   	createPage(VANDUINO);
		createArow(BY1, YPULSE,Y1,200);
		createArow(BY2, YPULSE,Y2, 200);
		createArow(BY3, YPULSE,Y3, 200);
		createArow(BY4, YPULSE,Y4, 200);
		createArow(BY5, YPULSE,Y5, 200);
		createArow(BY6, YPULSE,Y6, 200);
		createArow(BYRST, PULSE,YRST, 200);

    */    
 
    //createDiagnostics(STKPAGE,"STACK");
           //createClock(DATE);
           //createRow(ALARM);
                //menu.createRow(MON2,FLOAT2,"V2",P22R);
		//menu.createGauge(G1,INT5,"VOLT^P22R",P22R);
		//menu.calibrate(G1,AMPLIFY,0.00504);
		//menu.createWindow(SDW, 280,WINDOW,SDDIR,NULL, NULL);

     //createClock(DATEPAGE,"SET DATE");
            //  menu.createRow(DIAG);
             // menu.createRow(DIAG);
              //menu.createRow(DIAG);
              		//
		//
		//menu.createRow(MON2,FLOAT2,"V2",P22R);
		//menu.createGauge(G1,INT5,"VOLT^P22R",P22R);
		//menu.calibrate(G1,AMPLIFY,0.00504);
		//menu.calibrate(MON2,AMPLIFY,0.00504);
	//writeRow(M1,MONF1C,"V4 Volts",15.3);
		//updateRow(M1);
		//showRow(M1);         i2c.write((uint8_t)PCFaddress, (uint8_t)PCFPORTA);

   
 }



