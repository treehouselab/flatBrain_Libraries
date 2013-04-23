//#include <USI_TWI_Master.h>
//#include <TinyWireM.h>
#include "fBT_yCard.h"

fBT_yCard yC;

void setup() {  

	yC.initCard();

}

// the loop routine runs over and over again forever:
void loop() {
	yC.pollButtons();       

   
	

}