#include "fB_Include.h"

fB_Val::fB_Val() {
	offset = 0;
	factor = 1;
}

//fB_Tag::fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle, uint32_t flags = NULL, uint8_t _fTag=NULL, uint16_t _tTag=NULL) {	
	
	//tag = _tag; 
	//Ptitle = _Ptitle;
	//text = NULL;
	//pPin = NULL;
	//fTag= _fTag;
	//tTag= _tTag;
	//flag8 = NULL;
	//flag16 = NULL;
	//putFlags( flags);
	//putFormat( flags);
	//putAction( flags);

//}

void		fB_Tag::putFlags(uint32_t flags32) 	{ flag16 |= (uint16_t)flags32; }

uint16_t	fB_Tag::getFlags() { return flag16 & ~MASKP; }
void		fB_Tag::clearFlags() { flag16 &= ~0x0FFF; }


uint8_t	fB_Tag::getFormat8() {
	uint8_t j;
	return (flag8 & MASK8F) >> 4;
}
void	fB_Tag::putFormat(uint32_t flags32) {
	int i;
	uint16_t format16 = 0;
	format16 =(uint16_t) ((flags32 & MASK32F) >> 22);
	for( i=0;i< 10;i++) if((format16 >> i) & 1) break;
	if(i<10) flag8 |= (i+1)<<4;		
	//dbug(F("pF %P, i: %d, f8: %x"),Ptitle,i,flag8);
	//Serial.println(flags32,HEX);
	//Serial.println(getFormat(),HEX);
}
uint32_t	fB_Tag::getFormat() {
	uint8_t j;
	uint32_t format32 = 0;
	j = (flag8 & MASK8F) >> 4;
	if(j) {
		format32 = (uint32_t) (1 << (j-1)); 
		format32 = format32 << 22;
	}
	//dbug(F("***gF %P, i: %d, f8: %x"),Ptitle,i,flag8);
	return format32 ;
}


void	fB_Tag::putAction(uint32_t flags32) {
	int i;
	uint16_t action16 = 0;
	action16 = (uint16_t) ((flags32 & MASK32A) >> 12);
	for( i=0;i< 10;i++) if(action16 >> i & 1) break;
	if(i<10) flag8 |= (i+1);	
	//dbug(F("pa %P, i: %d, f8: %x"),Ptitle,i,flag8);

}
uint32_t	fB_Tag::getAction() {
	uint8_t j;
	uint32_t action32 = 0;
	j = flag8 & MASK8A;
	if(j) action32 = (1 << (uint32_t) (j-1)) << 12;
	return action32 ;
}


uint8_t	fB_Tag::getAction8() {
	return  (flag8 & MASK8A);
}
uint8_t fB_Tag::isDouble() {
	if(getFormat() == FLOAT1  || getFormat() == FLOAT2) return (1);
	else return 0;
}
uint32_t  fB_Tag::assignFormat(double value) {
	double mod = 0;
	mod = value-int(value);
	if(mod==0) return INT5;
	if(mod*10-int(mod*10) == 0) return FLOAT1;
	return FLOAT2;
}
double fB_Tag::getValue() {  // if pin, get value from ADC 
	uint16_t ival;
	double dval;
	if(pin) {
		if(isDouble()){
			if(!dVal->factor) dVal->factor = 1;
			return ((double) read() * dVal->factor) + dVal->offset;
		}
		else return read();
	}
	else{
		if(isDouble()) 	{
			if(!dVal->factor) dVal->factor = 1;
			return (dVal->dVal * dVal->factor) + dVal->offset;
		}
		else return (double) iVal;
	}
}

void fB_Tag::putValue(double value) { 
	if(isDouble()) 	dVal->dVal = value;
	iVal = (int) value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
fB_Curr::fB_Curr() {
	pageTag = NULL;
	parentTag = NULL;
	rowDex = 0;
	rowCount = 1;
	rSelected = 0;
}

void fB_Curr::setCurrPage(uint16_t _tag) {
	rowDex = 0;  // update when row selected
	if(_tag) {
		pageTag = _tag;
		pP = Tag(_tag);
		parentTag = pP->fTag;
		rowCount = getRowCount();
	}
	farY = STARTY + (ROWHT * (1+rowCount));
	//dbug(F("scp %P, rc:%d, f: %d"),pP->Ptitle,rowCount,farY);

}	

fB_Tag* fB_Curr::tag() { 
	if(!rowDex) return Tag(HEADER);
	else return rTP[pP->iVal + rowDex].p; 
	}
fB_Tag* fB_Curr::tag(uint8_t index) { 
	if(!index) return Tag(HEADER);
	else return rTP[pP->iVal + index].p; 
	}
	
uint8_t fB_Curr::getRowCount() { return ((pP->flag16 & MASKP)>>RCOFFSET); }  

void fB_Curr::putRowCount(uint8_t count){							
	pP->flag16 &= ~MASKP;
	pP->flag16 |= (count << RCOFFSET);		
	farY = STARTY + (ROWHT * (1+count ));
	rowCount = count;
}

void fB_Curr::incrRowCount() {
	uint8_t count = getRowCount();
	putRowCount(++count);
}

void fB_Curr::selectRow() {
	tft.setColor(FCOLOR,GCOLOR);
	tft.drawHLine(STARTX,tag()->getY()+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	rSelected = 1;
	
}

void fB_Curr::deselectRow() {
	if(!rSelected) return;
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,tag()->getY()+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();
	rSelected = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////


void fB_Tag::createPin(uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {

	uint8_t offset,cPin,mode;
	offset = (row - 17 + side)*8;
	cPin = getCpinMap(0);
	cPin = getCpinMap(0);
	//cPin = pgm_read_byte(&Xmap76[offset+3]);
	//mode = pgm_read_byte(&Xmap76[offset+2]);

	//mode = getModeMap(offset);

	pin = 0;
	pin |= (onval << 15);
	pin |= (dir   << 14);
	pin |= (mode  << 13);
	//unused flag bits 8-11
	pin |= (ctag  << 4);
	pin |= cPin;

dbug(F("pp ct:%d,t%d"),ctag,Card(ctag)->tag);
dbug(F("pp R%d,r%d,s%d,d%d,m%d,v%d, C%d, c:%d, p%d"),row,getRowBus(),getSideBus(),getDir(),getMode(),getOnVal(),ctag,getCtag(),cPin);
dbug(F(""));

	switch(Card(ctag)->type) {	 // pointer into card array;
		case X50:
		case X76:
			//if(getMode() == IO_A ) 	pinMode(getCard()->aChan ,iodir); // unecessary, analog line getMode() set at each read/write
			if(mode == IO_D)   Card(ctag)->MCPd_pinMode(cPin,dir);
			break;
		case BRAIN: 
			//pinMode(getCpin(),iodir);
			break;
	}
	pull(~onval); // pulls to OFF for both input and output getMode()s
}

uint8_t fB_Tag::getOnVal()	{return ( pin >> 15 ) &  0x01 ;}
uint8_t fB_Tag::getDir()	{return ( pin >> 14 ) &  0X01 ;}
uint8_t fB_Tag::getMode()   {return ( pin >> 13 ) &  0X01 ;}
uint8_t fB_Tag::getCtag()   {return ( pin >>  4 ) &  0X0F ;}
uint8_t fB_Tag::getCpin()   {return ( pin & 0X0F );}


fB_Card* fB_Tag::getCard() {  return Card(getCtag()); }

uint8_t fB_Tag::getCpinMap(uint8_t offset) {  // get chip pin
	switch(getCard()->type) {
		case X50:
			return pgm_read_byte(&Xmap50[offset+3]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[offset+3]);
			break;
		case BRAIN: 
			//if(getMode()== IO_D)    pinMode(getCpin(),iodir);
			break;
	}
}

uint8_t fB_Tag::getModeMap(uint8_t offset) {  
	//dbug(F("gM p %d, m %x"), poff, pgm_read_byte(&Xmap76[poff-1]));
	switch(getCard()->type) {
		case X50:
			return pgm_read_byte(&Xmap50[offset+2]);
			break;
		case X76:
			return pgm_read_byte(&Xmap76[offset+2]);
			break;
		case BRAIN: 
			break;
	}
}

uint8_t fB_Tag::getRowBus() {
	int i;
	switch(getCard()->type) {
		case X50:
			for( i=3; i < 64; i=i+4) if(pgm_read_byte(&Xmap76[i]) == getCpin()) break;
			if(i==64) return 0;
			return pgm_read_byte(&Xmap76[i-3]);
			break;
		case X76:
			for( i=3; i < 128; i=i+4) if(pgm_read_byte(&Xmap76[i]) == getCpin()) break;
			if(i==128) return 0;
			return pgm_read_byte(&Xmap76[i-3]);
			break;
		case BRAIN: 
			//if(getMode()== IO_D)    pinMode(getCpin(),iodir);
			break;
	}
}
uint8_t fB_Tag::getSideBus() {
	int i;
	switch(getCard()->type) {
		case X50:
			for( i=3; i < 64; i=i+4) if(pgm_read_byte(&Xmap76[i]) == getCpin()) break;
			if(i==64) return 0;
			return pgm_read_byte(&Xmap76[i-2]);
			break;
		case X76:
			for( i=3; i < 128; i=i+4) if(pgm_read_byte(&Xmap76[i]) == getCpin()) break;
			if(i==128) return 0;
			return pgm_read_byte(&Xmap76[i-2]);
			break;
		case BRAIN: 
			//if(getMode()== IO_D)    pinMode(getCpin(),iodir);
			break;
	}
}
void fB_Tag::pull(unsigned int value) {
	switch(getCard()->type) {
		case X50:
		case X76:
			//if(getMode() == IO_A ) 	getCard()->CD_analogWrite(getCpin(),value); 
			if(getMode()== IO_D)    getCard()->MCPd_pull(getCpin(),value);
			break;
		case BRAIN: 
			if(getMode()== IO_D)    digitalWrite(getCpin(),value);
			break;
	}
}
void fB_Tag::YshiftPulse(unsigned int msecs) {
		Tag(YSHFT)->pinMode(OUTPUT);
		Tag(YSHFT)->write(Tag(YSHFT)->getOnVal());
		pulse(msecs);
		Tag(YSHFT)->pinMode(INPUT);
}
void fB_Tag::pulse(unsigned int msecs) {
		pinMode(OUTPUT);
		write(getOnVal());
		delay(msecs);
		pinMode(INPUT);
}
void fB_Tag::write(unsigned int value) {

	switch(getCard()->type) {
		case X50:
		case X76:
		//if(getMode() == IO_A ) 	getCard()->CD_analogWrite(this->CDchan(),value); 
		//if(getMode()== IO_D)    getCard()->MCP_digitalWrite(this->MCPchan(),value);
			if(getMode() == IO_A && getOnVal() == GATE ) 	getCard()->CD_analogWrite(getCpin(),value); 
			if(getMode()== IO_D)   getCard()->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== IO_D)  digitalWrite(getCpin(),value);
			if(getMode()== IO_A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Tag::dWrite(unsigned int value) {
	switch(getCard()->type) {
		case X50:
		case X76:
			if(getMode() == IO_A )  getCard()->CD_digitalWrite(getCpin(),value);
			if(getMode()== IO_D)    getCard()->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== IO_D)  digitalWrite(getCpin(),value);
			if(getMode()== IO_A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Tag::aWrite(unsigned int value) {
	switch(getCard()->type) {
		case X50:
		case X76:
			if(getMode() == IO_A )  getCard()->CD_analogWrite(getCpin(),value);
			if(getMode()== IO_D)    getCard()->MCPd_analogWrite(getCpin(),value);
			break;
		case BRAIN:
			//if(getMode()== IO_D)  digitalWrite(getCpin(),value);
			if(getMode()== IO_A)  analogWrite(getCpin(),value);
			break;

	}
}

unsigned int fB_Tag::read() {

	switch(getCard()->type) {
		case X50:
		case X76:
			if(getMode()== IO_A  && getOnVal() == GATE) {
					return(avgAnalogIn());
			}
			if(getMode()== IO_D) {
				if(getDir()==OUTPUT) {
					if (isLatched()) return HIGH;
					else return LOW;
				}
				else return (getCard()->MCPd_digitalRead(getCpin()));
			}
			break;
		case BRAIN:
			if(getMode()== IO_D)  return digitalRead(getCpin());
			if(getMode()== IO_A)  return analogRead(getCpin());
			break;

	}
}
bool fB_Tag::isLatched() {
	uint16_t latches,bit;

	bit = 1 << getCpin();
	latches = getCard()->MCPd_getLatches();
	if(latches & bit) return true;
	else return false;
}

unsigned int fB_Tag::dRead() {
	switch(getCard()->type) {
		case X50:
		case X76:
			if(getMode()== IO_A  && getOnVal() == GATE) return (getCard()->CD_digitalRead(getCpin())); 
			if(getMode()== IO_D)getCard()->MCPd_pinMode(getCpin(),INPUT);
			break;
		case BRAIN:
			return digitalRead(getCpin());
			break;

	}
}
unsigned int fB_Tag::aRead() {
	switch(getCard()->type) {
		case X50:
		case X76:
			if(getMode()== IO_A && getOnVal() == GATE) return (getCard()->CD_analogRead(getCpin())); 
			if(getMode()== IO_D) return (getCard()->MCPd_analogRead(getCpin())); 
			break;
		case BRAIN:
			if(getMode()== IO_A) return analogRead(getCpin());
			break;

	}
}
uint16_t fB_Tag::avgAnalogIn() {
	uint16_t  sum = 0;
	if(getOnVal() != GATE) return 0;
    for(int i = 0; i < ANALOGSAMPLESIZE; i++) {
        sum += getCard()->CD_analogRead(getCpin());
		delay(ANALOGSAMPLEDELAY);
    }
	return( (uint16_t) (sum/ANALOGSAMPLESIZE));
}

