#include <Arduino.h>
#include <avr/pgmspace.h>
#include <pins_arduino.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "fBT_yCard.h"
extern fBT_yCard yC;



fBT_yCard::fBT_yCard() {
	bState = 0;
	mapCount = 0;
	buildCardMap();
	shiftBut.Bd = 1;// Button  maps to D1,  ATTINY pin 3
	shiftBut.Yd = 0;
	shiftBut.mode = 0;
}
void fBT_yCard::insertYmap(uint8_t dButton, uint8_t dRelay) {
	yMap[mapCount].Bd = dButton;
	yMap[mapCount].Yd = dRelay;
	yMap[mapCount].mode = REG; // default
	mapCount++;
}
void fBT_yCard::setButtonMode(uint8_t bnum, uint8_t mod) {
	if(bnum >0 && bnum <= mapCount) 	yMap[bnum-1].mode = mod;
}

void fBT_yCard::buildCardMap() {
	//YCARD V3.0
	insertYmap(8,13);	// Button 1 maps to D8(ATTINY pin 11), RELAY/LED on D13, (ATTINY pin 16)
	insertYmap(6,12);	// Button 2 maps to D6(ATTINY pin 8), RELAY/LED on D12, (ATTINY pin 15)
	insertYmap(5,11);	// Button 3 maps to D5(ATTINY pin 7), RELAY/LED on D11, (ATTINY pin 14)
	insertYmap(4,10);	// Button 4 maps to D4(ATTINY pin 6), RELAY/LED on D10, (ATTINY pin 13)
	insertYmap(3,9);	// Button 5 maps to D3(ATTINY pin 5), RELAY/LED on D9, (ATTINY pin 12)
	insertYmap(2,7);	// Button 6 maps to D2(ATTINY pin 4), RELAY/LED on D7, (ATTINY pin 9)
}

void fBT_yCard::initCard() {
	int i;

	for(i=0;i<mapCount;i++ ) {
		pinMode(yMap[i].Bd,_INPUT);			// init button pin
		digitalWrite(yMap[i].Bd,HIGH);		// set pullup
		pinMode(yMap[i].Yd,_OUTPUT);		    // init relay pin
		digitalWrite(yMap[i].Yd,LOW);		// set relay pin
	}
	pinMode(shiftBut.Bd,_INPUT);			// init shift button pin
	digitalWrite(shiftBut.Bd,HIGH);		// set pullup
}

void fBT_yCard::getAstate() {
	int i,j;
	enableDelay = 0;
	if(digitalRead(shiftBut.Bd) == LOW ) sState =1;
	else sState = 0;
	for(i= 0; i<yC.mapCount; i++) {
		if(digitalRead(yMap[i].Bd) == LOW ) {
			if(!(bState & (0x01 << i))) { // if button not already down
				if(digitalRead(yMap[i].Yd) == HIGH) aState &= ~(0x01 << i);
				else {
					//if(!sState && yMap[i].mode & RAD) 	for(j= 0; j<yC.mapCount; j++) if(yMap[j].mode & RAD  && i!=j && !(bState & 0x01 << j)) aState &= ~(0x01 << j);
					if(!sState && yMap[i].mode & RAD) 	for(j= 0; j<yC.mapCount; j++) if(yMap[j].mode & RAD  && i!=j ) aState &= ~(0x01 << j);
					aState |= 0x01 << i;
				}
				bState |= 0x01 << i;
				enableDelay = 1;
			}
		}
		else 	{
			bState  &= ~(0x01 << i);
			if(yMap[i].mode & MOM) 	aState &= ~(0x01 << i);
		}
	}
}

void fBT_yCard::applyAstate() {
	int i;
	for(i= 0; i<yC.mapCount; i++) {

		if(aState >> i & 0x01)  	digitalWrite(yMap[i].Yd,HIGH); 
		else  digitalWrite(yMap[i].Yd,LOW); 
	}
}

void fBT_yCard::pollButtons() {
	getAstate();
	applyAstate();
	if(enableDelay) delay(200);
	/*
	int i;
		digitalWrite(yMap[4].Yd,HIGH); 
		delay(500);
		digitalWrite(yMap[4].Yd,LOW); 
		delay(500);
	for(i=0; i < yC.mapCount-2; i++) {
		digitalWrite(yMap[i].Yd,HIGH); 
		//delay(1000);
		//digitalWrite(yMap[i].Yd,LOW); 
		delay(500);
	}
	for(i=0; i < yC.mapCount-2; i++) {
		digitalWrite(yMap[i].Yd,LOW); 
		//delay(1000);
		//digitalWrite(yMap[i].Yd,LOW); 
		delay(1000);
	}

	delay(1000);

*/
	
}


