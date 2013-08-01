#include "fB_Include.h"

// This class manages the ioperations of the backplane bus pins. 
// To read or write to a given bus pin, all that is needed to access the pin is its row and side.

fB_Pin::fB_Pin(uint16_t ptag,const __FlashStringHelper* Ptit,uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {
	pTag = ptag;
	Ptitle = Ptit;
	iodir = dir;
	onVal = onval;
	vdr = 0;  // undefined resistor value
	gate = OFF;  

	bcard = brain.Card(ctag);  // pointer into card array;
	uint8_t start;
	if(row<17) start = 12;
	else start = 17;
	poff = (row-start)*8 +3;
	if(side == R) poff +=4;

	switch(bcard->cType) {
		case X50:
		case X76:
			//if(getMode() == A) 	pinMode(bcard->aChan ,iodir); // unecessary, analog line getMode() set at each read/write
			if(getMode() == D)   bcard->MCPd_pinMode(getCpin(),iodir);
			break;
		case BRAIN: 
			//pinMode(getCpin(),iodir);
			break;
	}
	pull(~onVal); // pulls to OFF for both input and output getMode()s
}

uint8_t fB_Pin::getCpin() {  // get chip pin
	switch(bcard->cType) {
		case X50:
			return pgm_read_byte(&Xmap50[poff]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[poff]);
			break;
		case BRAIN: 
			//if(getMode()== D)    pinMode(getCpin(),iodir);
			break;
	}
}
uint8_t fB_Pin::getMode() {  // get chip pin
	switch(bcard->cType) {
		case X50:
			return pgm_read_byte(&Xmap50[poff]-1);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[poff]-1);
			break;
		case BRAIN: 
			break;
	}
}
uint8_t fB_Pin::getSide() {  // get chip pin
	switch(bcard->cType) {
		case X50:
			return pgm_read_byte(&Xmap50[poff]-2);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[poff]-2);
			break;
		case BRAIN: 
			break;
	}
}
uint8_t fB_Pin::getRow() {  // get chip pin
	switch(bcard->cType) {
		case X50:
			return pgm_read_byte(&Xmap50[poff]-3);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[poff]-3);
			break;
		case BRAIN: 
			break;
	}
}

void fB_Pin::pinMode(unsigned int iodir) {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(getMode() == D)    bcard->MCPd_pinMode(getCpin(),iodir);
			break;
		case BRAIN: 
			//if(getMode()== D)    pinMode(getCpin(),iodir);
			break;
	}
}
void fB_Pin::pull(unsigned int value) {
	switch(bcard->cType) {
		case X50:
		case X76:
			//if(getMode() == A ) 	bcard->CD_analogWrite(getCpin(),value); 
			if(getMode()== D)    bcard->MCPd_pull(getCpin(),value);
			break;
		case BRAIN: 
			if(getMode()== D)    digitalWrite(getCpin(),value);
			break;
	}
}
void fB_Pin::YshiftPulse(unsigned int msecs) {
		Pin(YSHFT)->pinMode(OUTPUT);
		Pin(YSHFT)->write(Pin(YSHFT)->onVal);
		pulse(msecs);
		Pin(YSHFT)->pinMode(INPUT);
}
void fB_Pin::pulse(unsigned int msecs) {
		pinMode(OUTPUT);
		write(onVal);
		delay(msecs);
		pinMode(INPUT);
}
void fB_Pin::write(unsigned int value) {

	switch(bcard->cType) {
		case X50:
		case X76:
		//if(getMode() == A ) 	bcard->CD_analogWrite(this->CDchan(),value); 
		//if(getMode()== D)    bcard->MCP_digitalWrite(this->MCPchan(),value);
			if(getMode() == A ) 	bcard->CD_analogWrite(getCpin(),value); 
			if(getMode()== D)   bcard->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== D) digitalWrite(getCpin(),value);
			if(getMode()== A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Pin::dWrite(unsigned int value) {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(getMode() == A ) 	bcard->CD_digitalWrite(getCpin(),value);
			if(getMode()== D)    bcard->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== D)  digitalWrite(getCpin(),value);
			if(getMode()== A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Pin::aWrite(unsigned int value) {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(getMode() == A ) 	bcard->CD_analogWrite(getCpin(),value);
			if(getMode()== D)    bcard->MCPd_analogWrite(getCpin(),value);
			break;
		case BRAIN:
			//if(getMode()== D)  digitalWrite(getCpin(),value);
			if(getMode()== A)  analogWrite(getCpin(),value);
			break;

	}
}

unsigned int fB_Pin::read() {

	switch(bcard->cType) {
		case X50:
		case X76:
			if(getMode()== A) {
					return(avgAnalogIn());
			}
			if(getMode()== D) {
				if(iodir==OUTPUT) {
					if (isLatched()) return HIGH;
					else return LOW;
				}
				else return (bcard->MCPd_digitalRead(getCpin()));
			}
			break;
		case BRAIN:
			if(getMode()== D)  return digitalRead(getCpin());
			if(getMode()== A)  return analogRead(getCpin());
			break;

	}
}
bool fB_Pin::isLatched() {
	uint16_t latches,bit;

	bit = 1 << getCpin();
	latches = bcard->MCPd_getLatches();
	if(latches & bit) return true;
	else return false;
}

unsigned int fB_Pin::dRead() {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(getMode()== A) return (bcard->CD_digitalRead(getCpin())); 
			if(getMode()== D)bcard->MCPd_pinMode(getCpin(),INPUT);
			break;
		case BRAIN:
			return digitalRead(getCpin());
			break;

	}
}
unsigned int fB_Pin::aRead() {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(getMode()== A) return (bcard->CD_analogRead(getCpin())); 
			if(getMode()== D) return (bcard->MCPd_analogRead(getCpin())); 
			break;
		case BRAIN:
			if(getMode()== A) return analogRead(getCpin());
			break;

	}
}



uint16_t fB_Pin::avgAnalogIn() {
	uint16_t  sum = 0;
    for(int i = 0; i < ANALOGSAMPLESIZE; i++) {
        sum += bcard->CD_analogRead(getCpin());
		delay(ANALOGSAMPLEDELAY);
    }
	return( (uint16_t) (sum/ANALOGSAMPLESIZE));
}

