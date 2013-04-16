
#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_Seg.h"

fBT_Seg seg;


#define	DISPLAY 1
#define	BUZZER	2
#define	BUTTON	3
#define	INTR	5
#define	V0		7
#define	V0CAL	8
#define	L0CAL	9
#define	HICAL	1


#define PIN_RELAY	0
#define PIN_DISPLAY	1  // seg display switch
#define PIN_BUZZER	2  // mcu int & pwm buzzer
#define PIN_BUTTON	7   // muxed switches
#define PIN_INTRPT	5   // muxed switches
#define PIN_VZSENS	7   // V0 sense
#define PIN_CALVZ	8  // calib pot
#define PIN_CALLO	9 // calib pot
#define PIN_CALHI	10 // calib pot

#define MAXSAMPLES	10


int readings[MAXSAMPLES];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
unsigned long average = 0;      // the average
int readPin = 2;

uint8_t maxSamples = 100;

void setup() {  
	TinyWireM.begin();
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;

    pinMode(PIN_CALVZ,INPUT);
    pinMode(PIN_CALLO,INPUT);
    pinMode(PIN_CALHI,INPUT);
    pinMode(PIN_VZSENS,INPUT);
    pinMode(PIN_BUTTON,INPUT);
    pinMode(PIN_BUZZER,OUTPUT);
    pinMode(PIN_RELAY,OUTPUT);
    pinMode(PIN_DISPLAY,OUTPUT);

	digitalWrite(PIN_RELAY,LOW);
	digitalWrite(PIN_DISPLAY,HIGH);

}

// the loop routine runs over and over again forever:
void loop() {
	digitalWrite(PIN_RELAY,HIGH);
        delay(2000);
	digitalWrite(PIN_RELAY,LOW);
        delay(2000);
  //total= total - readings[index];         
  //readings[index] = analogRead(readPin); 
  //total= total + readings[index];       
  //index = index + 1;                    
  //if (index >= MAXSAMPLES)   index = 0;                           
  //average = total /MAXSAMPLES;         

	//seg.test();  // set segmented display address if necessary;
	seg.displayDec(avgAnalog(readPin));
delay(1000);

}

uint16_t avgAnalog(uint8_t pin) {
	uint16_t sum = 0;
	for(uint8_t i=0;i<MAXSAMPLES;i++) sum += analogRead(pin);
	return ( (uint16_t) sum/MAXSAMPLES );
}

