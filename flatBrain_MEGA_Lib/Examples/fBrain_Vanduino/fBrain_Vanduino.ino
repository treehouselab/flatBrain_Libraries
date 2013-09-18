#include <Timer.h>
#include <fB_flatBrain.h>
#include <fB_Vanduino.h>


fB_Vanduino vduino;

void setup(){

   Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);


	//alarm.disable();
	alarm.bootBeepDisable();

	flatBrainInit();

	vduino.init(RSTATUS);

	menu.showPage(RSTATUS);

        vduinoUpdate();
	//i2c.scan();
	//Card(YCRD)->LED(HIGH);
	timer.every(_TIMER_UPDATE,10000,vduinoUpdate);
	//initState();

}



void loop() {
	menu.checkButtonCode();
	
	if(curr.pageTag == RSTATUS) timer.update();
}


void vduinoUpdate() {
	vduino.getState();
	vduino.nextState();
	vduino.setState();
	vduino.showState();
}
