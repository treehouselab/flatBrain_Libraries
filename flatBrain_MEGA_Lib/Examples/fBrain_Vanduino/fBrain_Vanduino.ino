#include <Timer.h>
#include <fB_FlatBrain.h>
#include <fB_Vanduino.h>


fB_Vanduino vduino;

void setup(){

   Serial.begin(9600);
   //pinMode(13,OUTPUT);
   //digitalWrite(13,LOW);


	//alarm.bootBeepDisable();

	flatBrainInit();

	vduino.init(VSTATUS);
        vduinoUpdate(NULL);

	menu.showPage(VSTATUS);

	alarm.play(_ALRMIN);

   	//i2c.scan();
	//Card(YCRD)->LED(HIGH);
	timer.perpetual(_TIMER_UPDATE,4000,vduinoUpdate);
	//initState();

}



void loop() {
    menu.checkButtonCode();
    timer.update(_TIMER_UPDATE);
    if(warn.action != _WD_OFF) timer.updateWarn();
    vduino.logTimer();
}


void vduinoUpdate(uint16_t arg16) {
	vduino.getState();
	vduino.nextState();
	vduino.showState();
}