#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_yCard.h"

//#define	 REG	0x01  // button modes definwd in fBT_yCard.h
//#define  MOM	0x02
//#define  RAD	0x04

fBT_yCard yC;

void setup() {  

	yC.initCard();
	yC.setButtonMode(1,RAD);// Button 1 , from top of card
	yC.setButtonMode(2,RAD);
	yC.setButtonMode(3,RAD);
	yC.setButtonMode(4,REG);
	yC.setButtonMode(5,REG);
	yC.setButtonMode(6,REG);
}

// the loop routine runs over and over again forever:
void loop() {
	yC.pollButtons();       

}