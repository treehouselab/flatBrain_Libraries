

#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_Seg.h"

fBT_Seg seg;

#define PIN_CALVZ	7
#define PIN_CALLO	8
#define PIN_CALHI	9
#define PIN_VZSEN	10
#define PIN_BUTLO	0
#define PIN_BUTHI	1
#define PIN_SWVZ	3
#define PIN_SWSEG	2
//#define PIN_INT		7
#define MAXSAMPLES	10


int readings[nSamples];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
unsigned long average = 0;      // the average
int vPin = A1;

uint8_t maxSamples = 100;

void setup() {  
	TinyWireM.begin();
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;

    pinMode(PIN_CALVZ,INPUT);
    pinMode(PIN_CALLO,INPUT);
    pinMode(PIN_CALHI,INPUT);
    pinMode(PIN_VZSEN,INPUT);
    pinMode(PIN_BUTHI,INPUT);
    pinMode(PIN_BUTLO,INPUT);
    pinMode(PIN_SWVZ,OUTPUT);
    pinMode(PIN_SWSEG,OUTPUT);

	digitalWrite(PIN_SWVZ,LOW);
	digitalWrite(PIN_SWSEG,LOW);

}

// the loop routine runs over and over again forever:
void loop() {

  total= total - readings[index];         
  readings[index] = analogRead(vPin); 
  total= total + readings[index];       
  index = index + 1;                    
  if (index >= nSamples)   index = 0;                           
  average = total / nSamples;         

	//seg.test();  // set segmented display address if necessary;
	seg.displayDec(average,0);
delay(1000);

}

uint16_t avgAnalog(uint8_t pin) {
	uint16_t sum = 0;
	for(uint8_t i=0;i<MAXSAMPLES;i++) sum += analogRead(pin);
	return ( (uint16_t) sum/MAXSAMPLES );
}

	