
 
#include <Timer.h>
#include <fB_FlatBrain.h>
#include <fB_Vanduino.h>


fB_Vanduino vduino;

void vduinoUpdate(uint16_t arg) {
	vduino.getState();
	vduino.nextState();
	vduino.showState();
}



void setup(){

   Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);


	//alarm.disable();
	//alarm.bootBeepDisable();

	flatBrainInit();

	vduino.init(VSTATUS);

	menu.showPage(VSTATUS);

    vduinoUpdate(NULL);
	//i2c.scan();
	//Card(YCRD)->LED(HIGH);
	timer.perpetual(_TIMER_UPDATE,(unsigned long)(Tag(CPSEC)->dVal->value * 1000),vduinoUpdate,NULL);
	//initState();


}



void loop() {
    menu.checkButtonCode();
    timer.update();
}

