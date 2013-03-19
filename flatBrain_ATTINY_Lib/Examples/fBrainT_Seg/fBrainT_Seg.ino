

#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_Seg.h"

fBT_Seg seg;

const int numReadings = 100;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
unsigned long average = 0;      // the average
int vPin = A1;

void setup() {  
	TinyWireM.begin();
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;

	  for (int thisReading = 0; thisReading < numReadings; thisReading++)   readings[thisReading] = 0;          

        pinMode(vPin,INPUT);

}

// the loop routine runs over and over again forever:
void loop() {

  total= total - readings[index];         
  readings[index] = analogRead(vPin); 
  total= total + readings[index];       
  index = index + 1;                    
  if (index >= numReadings)   index = 0;                           
  average = total / numReadings;         

	//seg.test();  // set segmented display address if necessary;
	seg.displayDec(average,0);
delay(1000);


