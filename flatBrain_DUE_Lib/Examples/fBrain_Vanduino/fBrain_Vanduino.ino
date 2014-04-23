/#include <Timer.h>
#include <Wire.h>
#include <fB_FlatBrain.h>
//#include <fB_Vanduino.h>


//fB_Vanduino vduino;
#define LED_HB    13

uint8_t hbval=128;
int8_t hbdelta=16;


//void vduinoUpdate(uint16_t arg) {
//	vduino.getState();
//	vduino.nextState();
//	vduino.showState();
//}



void setup(){

   Serial.begin(57600);
  
    //pinMode(13,OUTPUT);
    //digitalWrite(13,LOW);
    pinMode(LED_HB, OUTPUT);


	//alarm.disable();
	//alarm.bootBeepDisable();

	flatBrainInit();
    Serial.println("fbinit");

	//vduino.init(VSTATUS);
   // Serial.println("vdinit");

	menu.showPage(HOME);
    //Serial.println("MSPinit");
     //   vduinoUpdate(NULL);
	//i2c.scan();
	//Card(YCRD)->LED(HIGH);
	//timer.perpetual(_TIMER_UPDATE,(unsigned long)(Tag(CPSEC)->dVal->value * 1000),vduinoUpdate,NULL);
	//initState();
  Serial.println("setup exit");

}



void loop() {
  
    menu.checkButtonCode();
    heartbeat();
    //timer.update(_TIMER_UPDATE);
    //if(warn.action == _WD_WARN) {
    //    timer.update(_TIMER_WARN);
     //   timer.update(_TIMER_ALARM);
  //  }
    //if(warn.action == _WD_DELAY) timer.update(_TIMER_WARNDELAY);
   

}

void heartbeat() {
  if (hbval > 192) hbdelta = -hbdelta;
  if (hbval < 32) hbdelta = -hbdelta;
  hbval += hbdelta;
  analogWrite(LED_HB, hbval);
  delay(20);
}