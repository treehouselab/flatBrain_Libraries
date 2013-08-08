#include <fB_flatBrain.h>
#include <Wire.h>

void setup(){

   Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);


	//alarm.disable();
	alarm.bootBeepDisable();

	flatBrainInit();
	//i2c.scan();

	//Card(YCRD)->LED(HIGH);

   	Tag(TSTAGS)->value = sysTagCount;
   	Tag(TUTAGS)->value = usrTagCount;
   	Tag(TPINS)->value = pinCount;
   	Tag(TLOGS)->value = logCount;
   	Tag(TPAGES)->value = pageCount;
   	Tag(TROWS)->value = rowCount;
   	Tag(FRAM)->value = freeRAM();

}

void loop() {
		menu.checkButtonCode();

 }