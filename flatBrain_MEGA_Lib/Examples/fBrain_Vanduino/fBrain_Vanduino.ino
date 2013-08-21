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


}

void loop() {
		menu.checkButtonCode();

 }