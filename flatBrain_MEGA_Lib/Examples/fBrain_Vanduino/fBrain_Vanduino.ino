#include <fB_flatBrain.h>
#include <Wire.h>

void setup(){

   Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);

	alarm.enable();
	flatBrain_init();
	//i2c.scan();
	tft.init(PORTRAIT);
	tft.clear();

	menu.show(HOME);

	alarm.disableBootBeep();
	alarm.play(ALARM_INIT);
	attachInterrupt(NAV_INT, navigate,FALLING);

	//Card(YCRD)->LED(HIGH);

   	Pin(TTAGS)->value = tagCount;
   	Pin(TPINS)->value = pinCount;
   	Pin(TLOGS)->value = logCount;
   	Pin(TPAGES)->value = pageCount;
   	Pin(TROWS)->value = rowCount;
   	Pin(FRAM)->value = freeRam();;

}

void loop() {
		menu.checkButtonCode();

 }
 

 


