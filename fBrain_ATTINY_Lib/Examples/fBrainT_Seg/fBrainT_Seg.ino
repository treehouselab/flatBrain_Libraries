

#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_Seg.h"
fBT_Seg seg;

void setup() {  
	TinyWireM.begin();
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;
}

// the loop routine runs over and over again forever:
void loop() {
	//seg.test();  // set segmented display address if necessary;
	seg.displayDec(9999,1);
			digitalWrite(0,HIGH);
	        //seg.test();  // set segmented display address if necessary;
			delay(1000);
			digitalWrite(0,LOW);
	        //seg.test();  // set segmented display address if necessary;
			delay(1000);


}
