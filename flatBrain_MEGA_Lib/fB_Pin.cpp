#include "fB_Include.h"

// This class manages the ioperations of the backplane bus pins. 
// To read or write to a given bus pin, all that is needed to access the pin is its row and side.

fB_Pin::fB_Pin(uint16_t ptag,char* tStr,uint16_t ctag,uint8_t   rw,uint8_t   sid,uint8_t   pin,  uint8_t  mod, uint8_t  dir, uint8_t  onval) {
	pTag = ptag;
	tagStr = tStr;
	mode = mod; 
	row = rw; 
	side = sid; 
	cpin =  pin; 
	iodir = dir;
	onVal = onval;
	vdr = 0;  // undefined resistor value
	gate = OFF;  

	bcard = brain.Card(ctag);  // pointer into card array;
	switch(bcard->cType) {
		case X50:
		case X76:
			if(mode == A) 	pinMode(bcard->aChan ,iodir); // unecessary, analog line mode set at each read/write
			if(mode == D)   bcard->MCP_pinMode(cpin,iodir);
			break;
		case BRAIN: 
			pinMode(cpin,iodir);
			break;
	}
	pull(~onVal); // pulls to OFF for both input and output modes
}

void fB_Pin::pull(unsigned int value) {
	switch(bcard->cType) {
		case X50:
		case X76:
			//if(mode == A ) 	bcard->CD_analogWrite(cpin,value); 
			if(mode== D)    bcard->MCP_pull(cpin,value);
			break;
		case BRAIN: 
			if(mode== D)    digitalWrite(cpin,value);
			break;
	}
}
void fB_Pin::write(unsigned int value) {

	switch(bcard->cType) {
		case X50:
		case X76:
		//if(mode == A ) 	bcard->CD_analogWrite(this->CDchan(),value); 
		//if(mode== D)    bcard->MCP_digitalWrite(this->MCPchan(),value);
			if(mode == A ) 	bcard->CD_analogWrite(cpin,value); 
			if(mode== D)   bcard->MCP_digitalWrite(cpin,value);
			break;
		case BRAIN:
			if(mode== D) digitalWrite(cpin,value);
			if(mode== A)  analogWrite(cpin,value);
			break;

	}
}
void fB_Pin::dWrite(unsigned int value) {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(mode == A ) 	bcard->CD_digitalWrite(cpin,value);
			if(mode== D)    bcard->MCP_digitalWrite(cpin,value);
			break;
		case BRAIN:
			if(mode== D)  digitalWrite(cpin,value);
			if(mode== A)  analogWrite(cpin,value);
			break;

	}
}
void fB_Pin::aWrite(unsigned int value) {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(mode == A ) 	bcard->CD_analogWrite(cpin,value);
			if(mode== D)    bcard->MCP_analogWrite(cpin,value);
			break;
		case BRAIN:
			//if(mode== D)  digitalWrite(cpin,value);
			if(mode== A)  analogWrite(cpin,value);
			break;

	}
}

unsigned int fB_Pin::read() {

	switch(bcard->cType) {
		case X50:
		case X76:
			if(mode== A) {
					return(avgAnalogIn(cpin));
			}
			if(mode== D) {
				if(iodir==OUTPUT) {
					if (isLatched()) return HIGH;
					else return LOW;
				}
				else return (bcard->MCP_digitalRead(cpin));
			}
			break;
		case BRAIN:
			if(mode== D)  return digitalRead(cpin);
			if(mode== A)  return analogRead(cpin);
			break;

	}
}
bool fB_Pin::isLatched() {
	uint16_t latches,bit;

	bit = 1 << cpin;
	latches = bcard->MCP_getLatches();
	if(latches & bit) return true;
	else return false;
}

unsigned int fB_Pin::dRead() {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(mode== A) return (bcard->CD_digitalRead(cpin)); 
			if(mode== D)bcard->MCP_pinMode(cpin,INPUT);
			break;
		case BRAIN:
			return digitalRead(cpin);
			break;

	}
}
unsigned int fB_Pin::aRead() {
	switch(bcard->cType) {
		case X50:
		case X76:
			if(mode== A) return (bcard->CD_analogRead(cpin)); 
			if(mode== D) return (bcard->MCP_analogRead(cpin)); 
			break;
		case BRAIN:
			if(mode== A) return analogRead(cpin);
			break;

	}
}



uint16_t fB_Pin::avgAnalogIn(uint8_t  cpin) {
	uint16_t  sum = 0;
    for(int i = 0; i < ANALOGSAMPLESIZE; i++) {
        sum += bcard->CD_analogRead(cpin);
		delay(ANALOGSAMPLEDELAY);
    }
	return( (uint16_t) (sum/ANALOGSAMPLESIZE));
}

