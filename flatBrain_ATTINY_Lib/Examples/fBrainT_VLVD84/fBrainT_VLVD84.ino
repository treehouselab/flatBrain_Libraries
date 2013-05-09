#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_Seg.h"

fBT_Seg seg;

#define RELAY	3  //   pin 6 relay
#define DISPLAY	1  //   pin 3 seg display switch
#define BUZZER	2  //	pin 5 pwm buzzer
#define BUTTON	A5  //  pin 8 muxed switches
#define INTRPT	0  //	pin 2 interrupt
#define VZSENS	A3  //  pin 10 V0 sense
#define CALVZ	A2  //  pin 11 calib pot
#define CALLO	A1  //	pin 12  calib pot
#define CALHI	A0 //	pin 13 calib pot

#define MAXSAMPLES	10


float  vButton;
float vFact  = .00408;
float bFact  = .00489;
float cFact = .001;
float zFact  = .01444;
float vLoLimit = 10; //  default
float vHiLimit = 13; //  default
float v0sense =0; 
float v0raw =0; 
float vCalib = 0;

uint8_t maxSamples = 200;

void setup() {  
	TinyWireM.begin();
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;

    pinMode(CALVZ,INPUT);
    pinMode(CALLO,INPUT);
    pinMode(CALHI,INPUT);
    pinMode(VZSENS,INPUT);
    pinMode(BUTTON,INPUT);
    pinMode(BUZZER,OUTPUT);
    pinMode(RELAY,OUTPUT);
    pinMode(DISPLAY,OUTPUT);

	digitalWrite(RELAY,LOW);
	digitalWrite(DISPLAY,HIGH);

}

// the loop routine runs over and over again forever:
void loop() {
        
       
   
    vButton =	avgAnalog(BUTTON) * vFact;
	vLoLimit = (avgAnalog(CALLO) * bFact) + 10;
	vHiLimit = (avgAnalog(CALHI) * bFact) + 10;
    vCalib =	avgAnalog(CALVZ) * cFact - 0.5;
    v0raw = avgAnalog(VZSENS);
    v0sense = v0raw * zFact + vCalib;

	if(vButton < 3 && vButton > 1.5) seg.displayFloat(vLoLimit,2);
	else if(vButton <= 1.5 && vButton > 1 ) seg.displayFloat(vHiLimit,2);
	else seg.displayFloat(v0sense,2);

	if(v0sense > vHiLimit) digitalWrite(RELAY,HIGH);
	if(v0sense < vLoLimit) digitalWrite(RELAY,LOW);
         
	delay(1000);

}

uint16_t avgAnalog(uint8_t pin) {
	uint16_t sum = 0;
	for(uint8_t i=0;i<MAXSAMPLES;i++) sum += analogRead(pin);
	return ( (uint16_t) sum/MAXSAMPLES );
}