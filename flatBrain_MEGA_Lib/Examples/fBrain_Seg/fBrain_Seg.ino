#include <Arduino.h>
#include <Wire.h>
#include "fB_Seg.h"


fB_Seg seg;

void setup() {   
    Wire.begin();  
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;
        seg.test();  // set segmented display address if necessary;
	seg.displayDec(1111,2);
}

// the loop routine runs over and over again forever:
void loop() {
	        seg.test();  // set segmented display address if necessary;

	
}
