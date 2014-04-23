
#include "fB_Include.h"


fB_Card::fB_Card(uint16_t _tag,char* _title, uint8_t _type,uint8_t  _i2cAddr, uint8_t  _aChan ) {
	type = _type;
	i2cAddr = _i2cAddr;
	aChan  = _aChan;
	tag = _tag;
	title = _title;
	//initCard(tag,Ptitle,this);
	init();
}

void fB_Card::init() {

	switch(type) {
		case _X50:
			MCP = new MCP23017;
			MCP->begin(i2cAddr);  
			MCP->allOff();
			//delay(500);
			MCP->pinMode(_X50AA, _OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(_X50AB, _OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(_X50AC, _OUTPUT);  // pin maps to address pin of CD4051, for analog channel
			MCP->pinMode(_X50BA, _OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(_X50BB, _OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(_X50BC, _OUTPUT);  // pin maps to address pin of CD4051, for VD channel
			MCP->pinMode(_X50LD, _OUTPUT); // pin maps to board LED
			MCP->pinMode(_X50GT, _OUTPUT);  // pin maps to N-Chan fet gate
			MCP->digitalWrite(_X50GT, LOW);  //  gate off
			MCPd = MCP;
			//MCP->digitalWrite(_X50LED, HIGH);
			break;
		case _X76:
			MCP = new MCP23017;  
			MCP->begin(i2cAddr);  // 1st MCP chip as controller 
			MCP->allOff();

			MCPd = new MCP23017;  
			MCPd->begin(i2cAddr+1);  //2nd MCP chip which handles data pins
			MCPd->allOff();
		//delay(500);
			MCP->pinMode(_X76AA, _OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(_X76AB, _OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(_X76AC, _OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(_X76AD, _OUTPUT);  // pin maps to address pin of CD4067, for analog channel
			MCP->pinMode(_X76IN, _OUTPUT);  // pin maps to INHIBIT pin of CD4067, active HIGH
			MCP->pinMode(_X76RS, _OUTPUT);  // pin maps to 2nd MCP _RST pin

			MCP->digitalWrite(_X76IN, LOW);  
			MCP->digitalWrite(_X76RS, HIGH);  
			MCP->pinMode(_X76LD, _OUTPUT); // pin maps to board LED
			MCP->pinMode(_X76GT, _OUTPUT);  // pin maps to N-Chan fet gate
			MCP->digitalWrite(_X76GT, LOW);  //  gate off
			break;
	}
} 

void fB_Card::LED(uint8_t val) {
	
	switch(type) {
		case _X50:
			MCP->digitalWrite(_X50LD,val);
			break;
		case _X76:
			MCP->digitalWrite(_X76LD,val);
			break;
	}


}

void fB_Card::digitalReset() {
	if(type == _X76) {
			MCP->digitalWrite(_X76RS, LOW);  
			delay(50);
			MCP->digitalWrite(_X76RS, HIGH); 
	}
}

void fB_Card::AnalogGate(bool logic) {
	uint8_t gatePin;
	if(type == _X50) gatePin = _X50GT;
	else if(type == _X76) gatePin = _X76GT;
	else return;
	if(logic) MCP->digitalWrite(gatePin,HIGH);
	else MCP->digitalWrite(gatePin,LOW);
}


void fB_Card::openCDchan(uint8_t  chan) {
	// Opens CD4051 channel by writing 3 or 4-bit address to CD address pins.
	if(!i2cAddr) return;
	switch(type) {
		case _X50:
			if( chan & 1) MCP->digitalWrite(_X50AA,HIGH);
			else MCP->digitalWrite(_X50AA,LOW);
			if( chan & 2) MCP->digitalWrite(_X50AB,HIGH);
			else MCP->digitalWrite(_X50AB,LOW);
			if( chan & 4) MCP->digitalWrite(_X50AC,HIGH);
			else MCP->digitalWrite(_X50AC,LOW);
			//MCP->digitalWrite(_X50CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
		case _X76:
			if( chan & 1) MCP->digitalWrite(_X76AA,HIGH);
			else MCP->digitalWrite(_X76AA,LOW);
			if( chan & 2) MCP->digitalWrite(_X76AB,HIGH);
			else MCP->digitalWrite(_X76AB,LOW);
			if( chan & 4) MCP->digitalWrite(_X76AC,HIGH);
			else MCP->digitalWrite(_X76AC,LOW);
			if( chan & 8) MCP->digitalWrite(_X76AD,HIGH);
			else MCP->digitalWrite(_X76AD,LOW);
			//MCP->digitalWrite(_X50CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
	}
}
unsigned int fB_Card::CD_analogRead(uint8_t  chan) {
	if(!i2cAddr) return fERR;
	uint16_t data;
	openCDchan(chan);
	pinMode(aChan , _INPUT);		// set the ANALOG LINE
	AnalogGate(ON);		  //  gate open
	data = analogRead(aChan); 
	AnalogGate(OFF);		  
	return analogRead(aChan); 
}
unsigned int fB_Card::CD_avgAnalogRead(uint8_t  chan,uint16_t size) {
//dbug(F("AVGAI %P ,  CHAN:%d"),Ptitle,chan);
	uint16_t  ival,sum = 0;
	AnalogGate(ON);
	openCDchan(chan);
	pinMode(aChan ,_INPUT);		// set the ANALOG LINE
	analogRead(aChan);
	delay(20);    
	for(int i = 0; i < size; i++) {
		ival = analogRead(aChan);
        sum += ival;
		delay(_ANALOGSAMPLEDELAY);
    }
	//AnalogGate(OFF); //************************* COMMENT OUT FOR TESTING 
	return( (uint16_t) (sum/size));
}


unsigned int fB_Card::CD_digitalRead(uint8_t  chan) {
	if(!i2cAddr) return fERR;
	this->openCDchan(chan);
	pinMode(aChan , _INPUT);		// set the ANALOG LINE
	return (digitalRead(aChan )); 
}
void fB_Card::CD_analogWrite(uint8_t  chan, unsigned int value) {
	if(!i2cAddr) return;
	this->openCDchan(chan);
	pinMode(aChan , _OUTPUT);		// set the ANALOG LINE
	AnalogGate(ON);		  //  gate open
	analogWrite(aChan , value);   
}
void fB_Card::CD_digitalWrite(uint8_t  chan, unsigned int value) {
	if(!i2cAddr) return;
	this->openCDchan(chan);
	pinMode(aChan , _OUTPUT);		// set the ANALOG LINE
	digitalWrite(aChan , value);   
}


// MCP and MCPd versions of these methods because for _X76 cards there are 2 MCP chips with different i2c addresses that use these methods
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
	MCP->pinMode((uint8_t )pin,(uint8_t )_OUTPUT);
	MCP->digitalWrite(pin,value);
}
void fB_Card::MCPd_digitalWrite(uint8_t  pin, unsigned int value) {
	//dbug(F("pin on ********* %d"),pin);
	if(!i2cAddr) return;
	MCPd->pinMode((uint8_t )pin,(uint8_t )_OUTPUT);
	MCPd->digitalWrite(pin,value);
}


void fB_Card::MCPd_pull(uint8_t  pin, unsigned int value) {
	//if(!i2cAddr) MCP->pull((uint8_t )pin,(uint8_t )value); //?
	if(i2cAddr) MCPd->pull((uint8_t )pin,(uint8_t )value);
}
unsigned int fB_Card::MCPd_analogRead(uint8_t  pin) {
	if(!i2cAddr) return fERR;
	MCPd->pinMode((uint8_t )pin,(uint8_t )_INPUT);
	if(MCPd->digitalRead(pin) == HIGH) return AD_HIGH;
	else return AD_LOW;
}

unsigned int fB_Card::MCPd_digitalRead(uint8_t  pin) {
	if(!i2cAddr) return fERR;
	MCPd->pinMode((uint8_t )pin,(uint8_t )_INPUT);	
	return (MCPd->digitalRead(pin));
}
unsigned int fB_Card::MCPd_getLatches() {
	if(!i2cAddr) return fERR;
	return (MCPd->readGPIOAB());
}
void fB_Card::MCPd_analogWrite(uint8_t  pin, unsigned int value) {
	if(!i2cAddr) return;
	MCPd->pinMode((uint8_t )pin,(uint8_t )_OUTPUT);
    if(value > AD_TRANS) MCPd->digitalWrite(pin,HIGH);
	else MCPd->digitalWrite(pin,LOW);
}
//unsigned int fB_Card::PCF_digitalRead(uint8_t  pin) {
//	return (PCF->digitalRead(pin));
//}
/*
void fB_Card::VDselectR(uint8_t  chan) {
	// Opens CD4051 channel by writing 3 or 4-bit address to CD address pins.
	//dbug(F("Card::selectVDR  chan %d"),chan);
	if(!i2cAddr || chan > 7) return;
	switch(type) {
		case _X50:
			if( chan & 1) MCP->digitalWrite(_X50BA,HIGH);
			else MCP->digitalWrite(_X50BA,LOW);
			if( chan & 2) MCP->digitalWrite(_X50BB,HIGH);
			else MCP->digitalWrite(_X50BB,LOW);
			if( chan & 4) MCP->digitalWrite(_X50BC,HIGH);
			else MCP->digitalWrite(_X50BC,LOW);
			//MCP->digitalWrite(_X50CUT,LOW); // SET INHIBIT LOW ( OFF)
			break;
		case _X76:
			if( chan & 1) MCP->digitalWrite(_X76BA,HIGH);
			else MCP->digitalWrite(_X76BA,LOW);
			if( chan & 2) MCP->digitalWrite(_X76BB,HIGH);
			else MCP->digitalWrite(_X76BB,LOW);
			if( chan & 4) MCP->digitalWrite(_X76BC,HIGH);
			else MCP->digitalWrite(_X76BC,LOW);
			//MCP->digitalWrite(_X76CUT,LOW); // SET INHIBIT LOW ( OFF)
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



