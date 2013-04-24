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
	altBut.bDg = 8;// Button  maps to D8,  ATTINY pin 11, to switch D6, ATTINY pin 7 switch
	altBut.sDg = 0;
	altBut.mode = 0;
}
void fBT_yCard::insertYmap(uint8_t buttonDg, uint8_t switchDg) {
	yMap[mapCount].bDg = buttonDg;
	yMap[mapCount].sDg = switchDg;
	yMap[mapCount].mode = REG; // default
	mapCount++;
}
void fBT_yCard::setButtonMode(uint8_t bnum, uint8_t mod) {
	if(bnum >0 && bnum <= mapCount) 	yMap[bnum-1].mode = mod;
}

void fBT_yCard::buildCardMap() {
	insertYmap(12,6);	// Button 1 maps to D12, ATTINY pin 15, to switch D6, ATTINY pin 8 switch
	insertYmap(13,3);	// Button 1 maps to D13, ATTINY pin 16, to switch D3, ATTINY pin 2 switch
	insertYmap(11,2);	// Button 1 maps to D11, ATTINY pin 14, to switch D2, ATTINY pin 3 switch
	insertYmap(10,4);	// Button 1 maps to D10, ATTINY pin 13, to switch D4, ATTINY pin 4 switch
	insertYmap(9,7);	// Button 1 maps to D9,  ATTINY pin 12, to switch D7, ATTINY pin 5 switch
	insertYmap(1,5);	// Button 1 maps to D1,  ATTINY pin 13, to switch D5, ATTINY pin 6 switch
}

void fBT_yCard::initCard() {
	int i;

	for(i=0;i<mapCount;i++ ) {
		pinMode(yMap[i].bDg,INPUT);			// init button pin
		digitalWrite(yMap[i].bDg,HIGH);		// set pullup
		pinMode(yMap[i].sDg,OUTPUT);		// init switch pin
		digitalWrite(yMap[i].sDg,LOW);		// init switch pin
	}
	pinMode(altBut.bDg,INPUT);			// init alt button pin
	digitalWrite(altBut.bDg,HIGH);		// set pullup
}

void fBT_yCard::getSstate() {
	int i,j;
	enableDelay = 0;
	if(digitalRead(altBut.bDg) == LOW ) sState = 0;
	else {
		for(i= 0; i<yC.mapCount; i++) {
			if(digitalRead(yMap[i].bDg) == LOW ) {
				if(!(bState & (0x01 << i))) { // if button not already down
					if(digitalRead(yMap[i].sDg) == HIGH) sState &= ~(0x01 << i);
					else {
						if(yMap[i].mode & RAD) 	for(j= 0; j<yC.mapCount; j++) if(yMap[j].mode & RAD  && i!=j && !(bState & 0x01 << j)) sState &= ~(0x01 << j);
						sState |= 0x01 << i;
					}
					bState |= 0x01 << i;
					enableDelay = 1;
				}
			}
			else 	{
				bState  &= ~(0x01 << i);
				if(yMap[i].mode & MOM) 	sState &= ~(0x01 << i);
			}
		}
	}
}

void fBT_yCard::applySstate() {
	int i;
	for(i= 0; i<yC.mapCount; i++) {

		if(sState >> i & 0x01)  	digitalWrite(yMap[i].sDg,HIGH); 
		else  digitalWrite(yMap[i].sDg,LOW); 
	}
}

void fBT_yCard::pollButtons() {
	getSstate();
	applySstate();
	if(enableDelay) delay(200);
}


