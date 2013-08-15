#include "fB_Include.h"

// This class manages the ioperations of the backplane bus pins. 
// To read or write to a given bus pin, all that is needed to access the pin is its row and side.

fB_Pin::fB_Pin(uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {
	flags = 0;
	flags |= (dir << 6);
	flags |= (onval << 7);
	uint8_t offset = row-1+side;  // should be 6 bits (<=32);
	flags |= offset;


	pCard = Card(ctag);  // pointer into card array;
	/*
	uint8_t start,poff;
	if(row<17) start = 12;
	else start = 17;
	poff = (row-start)*8 +3;
	if(side == R) poff +=4;
*/
	switch(pCard->type) {
		case X50:
		case X76:
			//if(getMode() == A ) 	pinMode(pCard->aChan ,iodir); // unecessary, analog line getMode() set at each read/write
			if(getMode() == D)   pCard->MCPd_pinMode(getCpin(),getDirection());
			break;
		case BRAIN: 
			//pinMode(getCpin(),iodir);
			break;
	}
	pull(~getOnVal()); // pulls to OFF for both input and output getMode()s
}

uint8_t fB_Pin::getOnVal() {return ( flags & 0X80 );}
uint8_t fB_Pin::getDirection() {return ( flags & 0X40 );}
uint8_t fB_Pin::getOffset() {return ( flags & 0X3F );}

uint8_t fB_Pin::getCpin() {  // get chip pin
	switch(pCard->type) {
		case X50:
			return pgm_read_byte(&Xmap50[getOffset()*4]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[getOffset()*4]);
			break;
		case BRAIN: 
			//if(getMode()== D)    pinMode(getCpin(),iodir);
			break;
	}
}
uint8_t fB_Pin::getMode() {  // get chip pin
	//dbug(F("gM p %d, m %x"), poff, pgm_read_byte(&Xmap76[poff-1]));
	switch(pCard->type) {
		case X50:
			return pgm_read_byte(&Xmap50[getOffset()*4-1]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[getOffset()*4-1]);
			break;
		case BRAIN: 
			break;
	}
}
uint8_t fB_Pin::getSide() {  // get chip pin
	switch(pCard->type) {
		case X50:
			return pgm_read_byte(&Xmap50[getOffset()*4-2]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[getOffset()*4-2]);
			break;
		case BRAIN: 
			break;
	}
}
uint8_t fB_Pin::getRowBus() {  // get chip pin
	switch(pCard->type) {
		case X50:
			return pgm_read_byte(&Xmap50[getOffset()*4-3]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[getOffset()*4-3]);
			break;
		case BRAIN: 
			break;
	}
}

void fB_Pin::pinMode(unsigned int iodir) {
	switch(pCard->type) {
		case X50:
		case X76:
			if(getMode() == D)    pCard->MCPd_pinMode(getCpin(),getDirection());
			break;
		case BRAIN: 
			//if(getMode()== D)    pinMode(getCpin(),iodir);
			break;
	}
}
void fB_Pin::pull(unsigned int value) {
	switch(pCard->type) {
		case X50:
		case X76:
			//if(getMode() == A ) 	pCard->CD_analogWrite(getCpin(),value); 
			if(getMode()== D)    pCard->MCPd_pull(getCpin(),value);
			break;
		case BRAIN: 
			if(getMode()== D)    digitalWrite(getCpin(),value);
			break;
	}
}
void fB_Pin::YshiftPulse(unsigned int msecs) {
		Pin(YSHFT)->pinMode(OUTPUT);
		Pin(YSHFT)->write(Pin(YSHFT)->getOnVal());
		pulse(msecs);
		Pin(YSHFT)->pinMode(INPUT);
}
void fB_Pin::pulse(unsigned int msecs) {
		pinMode(OUTPUT);
		write(getOnVal());
		delay(msecs);
		pinMode(INPUT);
}
void fB_Pin::write(unsigned int value) {

	switch(pCard->type) {
		case X50:
		case X76:
		//if(getMode() == A ) 	pCard->CD_analogWrite(this->CDchan(),value); 
		//if(getMode()== D)    pCard->MCP_digitalWrite(this->MCPchan(),value);
			if(getMode() == A && getOnVal() == GATE ) 	pCard->CD_analogWrite(getCpin(),value); 
			if(getMode()== D)   pCard->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== D)  digitalWrite(getCpin(),value);
			if(getMode()== A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Pin::dWrite(unsigned int value) {
	switch(pCard->type) {
		case X50:
		case X76:
			if(getMode() == A )  pCard->CD_digitalWrite(getCpin(),value);
			if(getMode()== D)    pCard->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== D)  digitalWrite(getCpin(),value);
			if(getMode()== A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Pin::aWrite(unsigned int value) {
	switch(pCard->type) {
		case X50:
		case X76:
			if(getMode() == A )  pCard->CD_analogWrite(getCpin(),value);
			if(getMode()== D)    pCard->MCPd_analogWrite(getCpin(),value);
			break;
		case BRAIN:
			//if(getMode()== D)  digitalWrite(getCpin(),value);
			if(getMode()== A)  analogWrite(getCpin(),value);
			break;

	}
}

unsigned int fB_Pin::read() {

	switch(pCard->type) {
		case X50:
		case X76:
			if(getMode()== A  && getOnVal() == GATE) {
					return(avgAnalogIn());
			}
			if(getMode()== D) {
				if(getDirection()==OUTPUT) {
					if (isLatched()) return HIGH;
					else return LOW;
				}
				else return (pCard->MCPd_digitalRead(getCpin()));
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
	latches = pCard->MCPd_getLatches();
	if(latches & bit) return true;
	else return false;
}

unsigned int fB_Pin::dRead() {
	switch(pCard->type) {
		case X50:
		case X76:
			if(getMode()== A  && getOnVal() == GATE) return (pCard->CD_digitalRead(getCpin())); 
			if(getMode()== D)pCard->MCPd_pinMode(getCpin(),INPUT);
			break;
		case BRAIN:
			return digitalRead(getCpin());
			break;

	}
}
unsigned int fB_Pin::aRead() {
	switch(pCard->type) {
		case X50:
		case X76:
			if(getMode()== A && getOnVal() == GATE) return (pCard->CD_analogRead(getCpin())); 
			if(getMode()== D) return (pCard->MCPd_analogRead(getCpin())); 
			break;
		case BRAIN:
			if(getMode()== A) return analogRead(getCpin());
			break;

	}
}



uint16_t fB_Pin::avgAnalogIn() {
	uint16_t  sum = 0;
	if(getOnVal() != GATE) return 0;
    for(int i = 0; i < ANALOGSAMPLESIZE; i++) {
        sum += pCard->CD_analogRead(getCpin());
		delay(ANALOGSAMPLEDELAY);
    }
	return( (uint16_t) (sum/ANALOGSAMPLESIZE));
}

