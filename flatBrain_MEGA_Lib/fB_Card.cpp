
#include "fB_Include.h"


fB_Card::fB_Card(uint16_t _tag,const __FlashStringHelper* _Ptitle, uint8_t _type,uint8_t  _i2cAddr, uint8_t  _aChan ) {
	type = _type;
	i2cAddr = _i2cAddr;
	aChan  = aChan;
	tag = _tag;
	Ptitle = _Ptitle;
	//initCard(tag,Ptitle,this);
	init();
}

void fB_Card::init() {

	switch(type) {
		case X50:
			MCP = new MCP23017;
			MCP->begin(i2cAddr);  
			MCP->allOff();
			//delay(500);
			MCP->pinMode(X50AA, OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(X50AB, OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(X50AC, OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(X50BA, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X50BB, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X50BC, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X50LD, OUTPUT); // pin maps to board LED
			MCP->pinMode(X50GT, OUTPUT);  // pin maps to N-Chan fet gate
			MCP->digitalWrite(X50GT, LOW);  //  gate off
			MCPd = MCP;
			//MCP->digitalWrite(X50LED, HIGH);
			break;
		case X76:
			MCP = new MCP23017;  
			MCP->begin(i2cAddr);  // 1st MCP chip as controller 
			MCP->allOff();

			MCPd = new MCP23017;  
			MCPd->begin(i2cAddr+1);  //2nd MCP chip which handles data pins
			MCPd->allOff();
		//delay(500);
			MCP->pinMode(X76AA, OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(X76AB, OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(X76AC, OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(X76AD, OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(X76IN, OUTPUT);  // pin maps to INHIBIT pin of CD4067, active HIGH

			//MCP->pinMode(X76BA, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			//MCP->pinMode(X76BB, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			//MCP->pinMode(X76BC, OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(X76RS, OUTPUT);  // pin maps to 2nd MCP _RST pin

			MCP->digitalWrite(X76IN, LOW);  
			MCP->digitalWrite(X76RS, HIGH);  
			MCP->pinMode(X76LD, OUTPUT); // pin maps to board LED
			MCP->pinMode(X76GT, OUTPUT);  // pin maps to N-Chan fet gate
			MCP->digitalWrite(X76GT, LOW);  //  gate off

			//VDselectR(pgm_read_byte(&VDRmap[0]));  // default no connect

			break;
	}
} 

void fB_Card::LED(uint8_t val) {
	
	switch(type) {
		case X50:
			MCP->digitalWrite(X50LD,val);
			break;
		case X76:
			MCP->digitalWrite(X76LD,val);
			break;
	}


}

void fB_Card::AnalogGate(bool logic) {
	uint8_t gatePin;
	if(type == X50) gatePin = X50GT;
	else if(type == X76) gatePin = X76GT;
	else return;
	if(logic) {
		MCP->digitalWrite(gatePin,HIGH);
	}
	else MCP->digitalWrite(gatePin,LOW);
}


void fB_Card::openCDchan(uint8_t  chan) {
	// Opens CD4051 channel by writing 3 or 4-bit address to CD address pins.
	if(!i2cAddr) return;
	switch(type) {
		case X50:
			if( chan & 1) MCP->digitalWrite(X50AA,HIGH);
			else MCP->digitalWrite(X50AA,LOW);
			if( chan & 2) MCP->digitalWrite(X50AB,HIGH);
			else MCP->digitalWrite(X50AB,LOW);
			if( chan & 4) MCP->digitalWrite(X50AC,HIGH);
			else MCP->digitalWrite(X50AC,LOW);
			//MCP->digitalWrite(X50CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
		case X76:
			if( chan & 1) MCP->digitalWrite(X76AA,HIGH);
			else MCP->digitalWrite(X76AA,LOW);
			if( chan & 2) MCP->digitalWrite(X76AB,HIGH);
			else MCP->digitalWrite(X76AB,LOW);
			if( chan & 4) MCP->digitalWrite(X76AC,HIGH);
			else MCP->digitalWrite(X76AC,LOW);
			if( chan & 8) MCP->digitalWrite(X76AD,HIGH);
			else MCP->digitalWrite(X76AD,LOW);
			//MCP->digitalWrite(X50CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
	}
}
/*
void fB_Card::VDselectR(uint8_t  chan) {
	// Opens CD4051 channel by writing 3 or 4-bit address to CD address pins.
	//dbug(F("Card::selectVDR  chan %d"),chan);
	if(!i2cAddr || chan > 7) return;
	switch(type) {
		case X50:
			if( chan & 1) MCP->digitalWrite(X50BA,HIGH);
			else MCP->digitalWrite(X50BA,LOW);
			if( chan & 2) MCP->digitalWrite(X50BB,HIGH);
			else MCP->digitalWrite(X50BB,LOW);
			if( chan & 4) MCP->digitalWrite(X50BC,HIGH);
			else MCP->digitalWrite(X50BC,LOW);
			//MCP->digitalWrite(X50CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
		case X76:
			if( chan & 1) MCP->digitalWrite(X76BA,HIGH);
			else MCP->digitalWrite(X76BA,LOW);
			if( chan & 2) MCP->digitalWrite(X76BB,HIGH);
			else MCP->digitalWrite(X76BB,LOW);
			if( chan & 4) MCP->digitalWrite(X76BC,HIGH);
			else MCP->digitalWrite(X76BC,LOW);
			//MCP->digitalWrite(X76CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
	}
}

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
	AnalogGate(ON);		  //  gate open
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
	AnalogGate(ON);		  //  gate open
	analogWrite(aChan , value);   
}
void fB_Card::CD_digitalWrite(uint8_t  chan, unsigned int value) {
	if(!i2cAddr) return;
	this->openCDchan(chan);
	pinMode(aChan , OUTPUT);		// set the ANALOG LINE
	digitalWrite(aChan , value);   
}


// MCP and MCPd versions of these methods because for X76 cards there are 2 MCP chips with different i2c addresses that use these methods
void fB_Card::MCP_pinMode(uint8_t  pin, unsigned int value) {
	//if(!i2cAddr) MCP->pinMode((uint8_t )pin,(uint8_t )value);//?
	if(i2cAddr) MCP->pinMode((uint8_t )pin,(uint8_t )value);
}
void fB_Card::MCPd_pinMode(uint8_t  pin, unsigned int value) {
	//if(!i2cAddr) MCP->pinMode((uint8_t )pin,(uint8_t )value);//?
	if(i2cAddr) MCPd->pinMode((uint8_t )pin,(uint8_t )value);
}
void fB_Card::MCP_digitalWrite(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) return;
	MCP->pinMode((uint8_t )pin,(uint8_t )OUTPUT);
	MCP->digitalWrite(pin,value);
}
void fB_Card::MCPd_digitalWrite(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) return;
	MCPd->pinMode((uint8_t )pin,(uint8_t )OUTPUT);
	MCPd->digitalWrite(pin,value);
}


void fB_Card::MCPd_pull(uint8_t  pin, unsigned int value) {
	//if(!i2cAddr) MCP->pull((uint8_t )pin,(uint8_t )value); //?
	if(i2cAddr) MCPd->pull((uint8_t )pin,(uint8_t )value);
}
unsigned int fB_Card::MCPd_analogRead(uint8_t  pin) {
	if(!i2cAddr) return fERR;
	MCPd->pinMode((uint8_t )pin,(uint8_t )INPUT);
	if(MCPd->digitalRead(pin) == HIGH) return AD_HIGH;
	else return AD_LOW;
}
unsigned int fB_Card::MCPd_digitalRead(uint8_t  pin) {
	if(!i2cAddr) return fERR;
	MCPd->pinMode((uint8_t )pin,(uint8_t )INPUT);	
	return (MCPd->digitalRead(pin));
}
unsigned int fB_Card::MCPd_getLatches() {
	if(!i2cAddr) return fERR;
	return (MCPd->readGPIOAB());
}
void fB_Card::MCPd_analogWrite(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) return;
	MCPd->pinMode((uint8_t )pin,(uint8_t )OUTPUT);
    if(value > AD_TRANS) MCPd->digitalWrite(pin,HIGH);
	else MCPd->digitalWrite(pin,LOW);
}
//unsigned int fB_Card::PCF_digitalRead(uint8_t  pin) {
//	return (PCF->digitalRead(pin));
//}



