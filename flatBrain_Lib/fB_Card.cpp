
#include "fB_Include.h"


fB_Card::fB_Card(uint16_t ctag,char *tStr, uint8_t  dex, uint8_t  ctyp,uint8_t  i2c, uint8_t  al) {
  cTag = ctag;
  index = dex;
  cType = ctyp;
  i2cAddr = i2c;
  aChan  = al;
  tagStr =tStr;
  init();
   
}

void fB_Card::init() {

	switch(cType) {
		case X2:
			MCP = new MCP23017;
			MCP->begin(i2cAddr);  // MCP adress is offset one bit from PFC base address
			MCP->allOff();
			//delay(500);
			MCP->pinMode(X2AA, OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(X2AB, OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(X2AC, OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(X2BA, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X2BB, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X2BC, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X2LD, OUTPUT); // pin maps to board LED
			MCP->pinMode(X2GT, OUTPUT);  // pin maps to N-Chan fet gate
			MCP->digitalWrite(X2GT, LOW);  //  gate off
			VDselectR(brain.VDRmap[0]);  // default no connect

			//MCP->digitalWrite(X2LED, HIGH);
			break;
		case X3:
			/*
			PCF = new PCF8574;
			PCF->begin(i2cAddr);		// store i2c address for this IC
			PCF->clear();				// set pins to low
			PCF->pinMode(CDA,OUTPUT); // maps to CD address pins
			PCF->pinMode(CDB,OUTPUT); 
			PCF->pinMode(CDC,OUTPUT); 
			PCF->pinMode(CDD,OUTPUT); 
			PCF->pinMode(CD16INH,OUTPUT); // maps to CD inhibit pin ( active high)
			PCF->pinMode(MCR_,OUTPUT);  // maps to MCPinhibit pin
			PCF->pinMode(CD8INH,OUTPUT);  // maps to CD4051
			PCF->pinMode(CUTOFF,OUTPUT);  // maps to P-CHAN VD CUTOFF

			PCF->digitalWrite(CUTOFF,LOW);
			PCF->digitalWrite(CD16INH,LOW);
			PCF->digitalWrite(MCR_,LOW);  // maps to MCP reset pin ( active low)
			delay(200);
			PCF->digitalWrite(MCR_,HIGH);
			delay(200);

			MCP = new MCP23017;
			MCP->begin(i2cAddr | 0x01);  // MCP adress is offset one bit from PFC base address
			MCP->allOff();
			delay(500);
			*/
			break;
	}
} 

void fB_Card::LED(uint8_t val) {
		MCP->digitalWrite(X2LD,val);
}
void fB_Card::VDgate(bool logic) {
	uint8_t gatePin;
	if(cType == X2) gatePin = X2GT;
	else if(cType == X3) gatePin = X3GT;
	else return;
	if(logic) {
		MCP->digitalWrite(gatePin,HIGH);
	}
	else MCP->digitalWrite(gatePin,LOW);
}


void fB_Card::openCDchan(uint8_t  chan) {
	// Opens CD4051 channel by writing 3 or 4-bit address to CD address pins.
	if(!i2cAddr) return;
	switch(cType) {
		case X2:
			if( chan & 1) MCP->digitalWrite(X2AA,HIGH);
			else MCP->digitalWrite(X2AA,LOW);
			if( chan & 2) MCP->digitalWrite(X2AB,HIGH);
			else MCP->digitalWrite(X2AB,LOW);
			if( chan & 4) MCP->digitalWrite(X2AC,HIGH);
			else MCP->digitalWrite(X2AC,LOW);
			//MCP->digitalWrite(X2CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
	}
}
void fB_Card::VDselectR(uint8_t  chan) {
	// Opens CD4051 channel by writing 3 or 4-bit address to CD address pins.
	dbug("Card::selectVDR  chan %d",chan);
	if(!i2cAddr || chan > 7) return;
	switch(cType) {
		case X2:
		dbug("Card::selectVDR2  ");
			if( chan & 1) MCP->digitalWrite(X2BA,HIGH);
			else MCP->digitalWrite(X2BA,LOW);
			if( chan & 2) MCP->digitalWrite(X2BB,HIGH);
			else MCP->digitalWrite(X2BB,LOW);
			if( chan & 4) MCP->digitalWrite(X2BC,HIGH);
			else MCP->digitalWrite(X2BC,LOW);
			//MCP->digitalWrite(X2CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
	}
}
/*
void fB_Card::PCF_openCDchan(uint8_t  chan) {
	// Opens CD4067 channel by writing 4-bit address to CD address pins.
	if(!i2cAddr) return;
	if( chan & 1) PCF->digitalWrite(CDA,HIGH);
	else PCF->digitalWrite(CDA,LOW);
	if( chan & 2) PCF->digitalWrite(CDB,HIGH);
	else PCF->digitalWrite(CDB,LOW);
	if( chan & 4) PCF->digitalWrite(CDC,HIGH);
	else PCF->digitalWrite(CDC,LOW);
	if( chan & 8) PCF->digitalWrite(CDD,HIGH);
	else PCF->digitalWrite(CDD,LOW);

	PCF->digitalWrite(CD16INH,LOW); // SET INHIBIT LOW ( OFF)

}
*/
unsigned int fB_Card::CD_analogRead(uint8_t  chan) {
	if(!i2cAddr) return fERR;
	this->openCDchan(chan);
	pinMode(aChan , INPUT);		// set the ANALOG LINE
	return (analogRead(aChan )); 
}
unsigned int fB_Card::CD_digitalRead(uint8_t  chan) {
	if(!i2cAddr) return fERR;
	this->openCDchan(chan);
	pinMode(aChan , INPUT);		// set the ANALOG LINE
	return (digitalRead(aChan )); 
}
void fB_Card::CD_analogWrite(uint8_t  chan, unsigned int value) {
	if(!i2cAddr) return;
	this->openCDchan(chan);
	pinMode(aChan , OUTPUT);		// set the ANALOG LINE
	analogWrite(aChan , value);   
}
void fB_Card::CD_digitalWrite(uint8_t  chan, unsigned int value) {
	if(!i2cAddr) return;
	this->openCDchan(chan);
	pinMode(aChan , OUTPUT);		// set the ANALOG LINE
	digitalWrite(aChan , value);   
}

void fB_Card::MCP_pull(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) MCP->pull((uint8_t )pin,(uint8_t )value);
}
void fB_Card::MCP_pinMode(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) MCP->pinMode((uint8_t )pin,(uint8_t )value);
}
unsigned int fB_Card::MCP_analogRead(uint8_t  pin) {
	if(!i2cAddr) return fERR;
	MCP->pinMode((uint8_t )pin,(uint8_t )INPUT);
	if(MCP->digitalRead(pin) == HIGH) return AD_HIGH;
	else return AD_LOW;
}
unsigned int fB_Card::MCP_digitalRead(uint8_t  pin) {
	if(!i2cAddr) return fERR;
	MCP->pinMode((uint8_t )pin,(uint8_t )INPUT);	
	return (MCP->digitalRead(pin));
}
unsigned int fB_Card::MCP_getLatches() {
	if(!i2cAddr) return fERR;
	return (MCP->readGPIOAB());
}
void fB_Card::MCP_analogWrite(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) return;
	MCP->pinMode((uint8_t )pin,(uint8_t )OUTPUT);
    if(value > AD_TRANS) MCP->digitalWrite(pin,HIGH);
	else MCP->digitalWrite(pin,LOW);
}
void fB_Card::MCP_digitalWrite(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) return;
	MCP->pinMode((uint8_t )pin,(uint8_t )OUTPUT);
	MCP->digitalWrite(pin,value);
}
unsigned int fB_Card::PCF_digitalRead(uint8_t  pin) {
	return (PCF->digitalRead(pin));
}



