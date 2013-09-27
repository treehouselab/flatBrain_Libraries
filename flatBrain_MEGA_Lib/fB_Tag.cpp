#include "fB_Include.h"

fB_Val::fB_Val() {
	value = 0;
	offset = 0;
	factor = 1;
	vFunc = NULL;
}

void	fB_Tag::putFormat(uint32_t flags32) {
	int i;
	uint16_t format16 = 0;
	format16 =(uint16_t) ((flags32 & MASK32F) >> 22);
	for( i=0;i< 10;i++) if((format16 >> i) & 1) break;
	//if(i<10) flag8 = (uint8_t)(i+1)<<4;		
	if(i<10) {
		flag8 &= 0x0F;		
		flag8 |= (i+1)<<4;	
	}
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
	if(i<10) {
		flag8 &= 0xF0;	
		flag8 |= (i+1);
	}
	//dbug(F("pa %P, i: %d, f8: %x"),Ptitle,i,flag8);

}
uint32_t	fB_Tag::getAction() {
	uint8_t j;
	uint32_t action32 = 0;
	j = flag8 & MASK8A;
	if(j) action32 = (uint32_t)(1 <<  (j-1)) << 12;
//dbug(F("ga %P, j: %d, f8: %x, %L"),Ptitle,j,flag8,action32);
//Serial.println(action32,DEC);
//Serial.println(action32,HEX);
return action32 ;
}

/*
uint8_t fB_Tag::isDouble() {
	if(getFormat() == _FLOAT1  || getFormat() == _FLOAT2 || getFormat() == _D2STR) return (1);
	if(pin && getMode()== _ANALOG) return(1);
	else return 0;
}
*/
uint32_t  fB_Tag::assignFormat(double value) {
	double mod = 0;
	mod = value-int(value);
	if(mod==0) return _INT5;
	if(mod*10-int(mod*10) == 0) return _FLOAT1;
	return _FLOAT2;
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
	//rowDex = 0;  // update when row selected
	if(_tag) {
		pageTag = _tag;
		pP = Tag(_tag);
		//parentTag = pP->fTag;
		parentTag =  pP->tTag;
		rowCount = getRowCount();
	}
	farY = STARTY + (ROWHT * (1+rowCount));
dbug(F("Tscp  tag:%d , rc:%d, fy:%d"),_tag,rowCount,farY);

}	

fB_Tag* fB_Curr::tag(uint8_t rowIndex) { 
	//int index;
	//if(!rowIndex) index = rowIndex;
	//else return rTP[pP->iVal + rowIndex].p; 
	return rTP[pP->iVal + rowIndex].p; 
	}

uint8_t fB_Curr::row(uint16_t tag) { // set current row to tag's row on page
	int i;
	for(i=1; i<= rowCount; i++) if(rTP[pP->iVal + i].p->tag == tag) return i;
	return 0;
}
	
uint8_t fB_Curr::getRowCount() { return ((pP->flag16 & _MASKP)>>_RCOFFSET); }  

void fB_Curr::putRowCount(uint8_t count){							
	pP->flag16 &= ~_MASKP;
	pP->flag16 |= (count << _RCOFFSET);		
	farY = STARTY + (ROWHT * (1+count ));
	rowCount = count;
}

void fB_Curr::incrRowCount() {
	uint8_t count = getRowCount();
	putRowCount(++count);
}

void fB_Curr::selectRow(int rowIndex) {
	if(!rowIndex) rowIndex = rowDex;
	tft.setColor(FCOLOR,GCOLOR);
	tft.drawHLine(STARTX,getY(rowIndex)+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	rSelected = 1;
	
}

void fB_Curr::deselectRow(int rowIndex) {
	if(!rSelected) return;
	if(!rowIndex) rowIndex = rowDex;
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,getY(rowIndex)+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();
	rSelected = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////


void fB_Tag::createPin(uint8_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {

	uint8_t offset,cPin,mode;


	offset = (row - 12) * 8 + (side*4);
	switch(Card(ctag)->type) {
		case _X50:
			cPin =  pgm_read_byte(&Xmap50[offset+3]);
			mode =  pgm_read_byte(&Xmap50[offset+2]);
			break;
		case _X76:
			cPin =  pgm_read_byte(&Xmap76[offset+3]);
			mode =  pgm_read_byte(&Xmap76[offset+2]);
			if(mode == _DIGITAL)   Card(ctag)->MCPd_pinMode(cPin,dir);
			break;
		case BRAIN: 
			//if(getMode()== _DIGITAL)    pinMode(getCpin(),iodir);
			break;
	}

	pin = 0;
	pin |= ((dir   << 15)& 0x8000);
	pin |= ((mode  << 14) & 0x4000);
	//unused flag bits 9-13
	pin |= ((onval  << 7)& 0x0180);  //2bit
	pin |= ((ctag  << 4)&  0x0070);  //3bits
	pin |= (cPin & 0x0F);			 //4bits

	if(onval != LOW) pull(LOW);
	else pull(HIGH);


//dbug(F("CP %P  pin:0x%x , ctag:%d,cpin:%d,m:%d,onv:%d "),Ptitle,pin,getCtag(),getCpin(),getMode(),getOnVal());
//dbug(F("CP %P  pin:0x%x , cpin:%d/%d,m:%d/%d,onv:%d/%d "),Ptitle,pin,cPin,getCpin(),mode,getMode(),onval,getOnVal());
//dbug(F(""));

}

uint8_t fB_Tag::getDir()	{return ( pin >> 15 ) &  0X01 ;}
uint8_t fB_Tag::getMode()   {return ( pin >> 14 ) &  0X01 ;}
uint8_t fB_Tag::getOnVal()	{return ( pin >>  7 ) &  0x03 ;}
uint8_t fB_Tag::getCtag()   {return ( pin >>  4 ) &  0X07 ;}
uint8_t fB_Tag::getCpin()   {return ( pin & 0X0F );}

void fB_Tag::putOnVal(uint8_t onval) {
	pin &= ~0x0180;
	pin |= ((onval  << 7)& 0x0180);  //2bit
}


void fB_Tag::getRowSide(uint8_t& rowSide) {
	int i;
	rowSide = 0;
	switch(Card(getCtag())->type) {
		case _X50:
			for( i=3; i < 104; i=i+4) {
				if ( pgm_read_byte(&Xmap50[i]) == getCpin()
				&& ( pgm_read_byte(&Xmap50[i-1]) == getMode()) ) break;
			}
			rowSide |=  pgm_read_byte(&Xmap50[i-3]); ///row
			rowSide |=  pgm_read_byte(&Xmap50[i-2]) << 7 ;
			break;
		case _X76:
			for( i=3; i < 168; i=i+4)	{
				if ( pgm_read_byte(&Xmap76[i]) == getCpin()
				&& ( pgm_read_byte(&Xmap76[i-1]) == getMode()) ) break;
			}
			rowSide |=  pgm_read_byte(&Xmap76[i-3]); ///row
			rowSide |=  pgm_read_byte(&Xmap76[i-2]) << 7 ; // side
			break;
		case BRAIN: 
			//if(getMode()== _DIGITAL)    pinMode(getCpin(),iodir);
			break;
	}
}
void fB_Tag::pinMode(unsigned int iodir) {
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			if(getMode() == _DIGITAL)    Card(getCtag())->MCPd_pinMode(getCpin(),getDir());
			break;
		case BRAIN: 
			//if(getMode()== D)    pinMode(getCpin(),iodir);
			break;
	}
}

void fB_Tag::pull(unsigned int value) {
			//dbug(F("** %P  pin:%d ,  ctag:%d"),Ptitle,getCpin(),getCtag());
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			//dbug(F("** %P  pin:%d , pull:%d "),Ptitle,getCpin(),value);

			if(getMode()== _ANALOG ) 	Card(getCtag())->CD_analogWrite(getCpin(),value); 
			if(getMode()== _DIGITAL)    Card(getCtag())->MCPd_pull(getCpin(),value);
			break;
		case BRAIN: 
			if(getMode()== _DIGITAL)    digitalWrite(getCpin(),value);
			break;
	}
}
void fB_Tag::YshiftPulse(unsigned int msecs) {
		if(!pin) return;
		Tag(YSHFT)->pinMode(_OUTPUT);
		Tag(YSHFT)->write(Tag(YSHFT)->getOnVal());
		pulse(msecs);
		Tag(YSHFT)->pinMode(_INPUT);
}
void fB_Tag::pulse(unsigned int msecs) {
		if(!pin) return;
		pinMode(_OUTPUT);
		write(getOnVal());
		delay(msecs);
		pinMode(_INPUT);
}
void fB_Tag::flash(unsigned int msecs) {
		if(!pin) return;
		write(getOnVal());
		delay(msecs);
		write(~getOnVal());
}
void fB_Tag::write(unsigned int value) {
	//dbug(F("tw %P ,  onval:%d, vAL:%d"),Ptitle,getOnVal(),value);

	if(!pin) return;

	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
		//if(getMode() == _ANALOG ) 	Card(getCtag())->CD_analogWrite(this->CDchan(),value); 
		//if(getMode()== _DIGITAL)    Card(getCtag())->MCP_digitalWrite(this->MCPchan(),value);
			if(getMode() == _ANALOG && getOnVal() == _PGATE ) 	Card(getCtag())->CD_analogWrite(getCpin(),value); 
			if(getMode()== _DIGITAL)  {
				value &= 0x0001;
				Card(getCtag())->MCPd_digitalWrite(getCpin(),value);
				//dbug(F("tw2 %P ,  ctag:%d, cp:%d,type:%d, val:%d"),Ptitle,getCtag(),getCpin(),Card(getCtag())->type,value);
			}
			break;
		case BRAIN:
			if(getMode()== _DIGITAL)  digitalWrite(getCpin(),value);
			if(getMode()== _ANALOG)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Tag::dWrite(unsigned int value) {
	if(!pin) return;
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			if(getMode() == _ANALOG )  Card(getCtag())->CD_digitalWrite(getCpin(),value);
			if(getMode()== _DIGITAL)    Card(getCtag())->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== _DIGITAL)  digitalWrite(getCpin(),value);
			if(getMode()== _ANALOG)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Tag::aWrite(unsigned int value) {
	if(!pin) return;
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			if(getMode() == _ANALOG )  Card(getCtag())->CD_analogWrite(getCpin(),value);
			if(getMode()== _DIGITAL)    Card(getCtag())->MCPd_analogWrite(getCpin(),value);
			break;
		case BRAIN:
			//if(getMode()== _DIGITAL)  digitalWrite(getCpin(),value);
			if(getMode()== _ANALOG)  analogWrite(getCpin(),value);
			break;
	}
}

uint16_t fB_Tag::readInt() {
	uint16_t intval;
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			if(getMode()== _ANALOG  && getOnVal() == _PGATE) 	intval =  avgAnalogIn();
			if(getMode()== _DIGITAL) {
				if(getDir()==_OUTPUT) {
					if (isLatched()) intval =    HIGH;
					else intval = LOW;
				}
				else intval =   Card(getCtag())->MCPd_digitalRead(getCpin());
			}
			break;
		case BRAIN:
			if(getMode()== _DIGITAL)  intval =   digitalRead(getCpin()); 
			if(getMode()== _ANALOG)  intval =    analogRead(getCpin());  
			break;
	}
	flag16 &= ~_UNDEF;
//dbug(F("READ %P ,  onval:%d, mode:%d ival:%d"),Ptitle,getOnVal(),getMode(),intval);
	return intval;
}
void fB_Tag::read() {
	if(flag16 & _TTAG) {
		fB_Tag* pT = Tag(tTag);
		if(pT && pT->pin) {
				if(flag16 & _DUBL  ) calibrate(pT->readInt()); 
				else iVal = pT->readInt();
		//dbug(F("tr %P val:%d "),Ptitle,iVal);
				if(getFormat() == _BLAMP && iVal <= MAXZEROADC) iVal = 0;
		//dbug(F("tr2 %P val:%d \n"),Ptitle,iVal);
		}
		return;
	}
	if(flag16 & _DUBL) calibrate(readInt()); 
	else iVal = readInt();
}

double fB_Tag::calibrate(uint16_t intval) { 
	if(dVal->vFunc) return dVal->value = dVal->vFunc(this,intval);
	else return (dVal->value = (double)intval * dVal->factor + dVal->offset);
}

bool fB_Tag::isLatched() {
	uint16_t latches,bit;
	if(!pin) return false;
	bit = 1 << getCpin();
	latches = Card(getCtag())->MCPd_getLatches();
	if(latches & bit) return true;
	else return false;
}

unsigned int fB_Tag::dRead() {
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			if(getMode()== _ANALOG  && getOnVal() == _PGATE) return (Card(getCtag())->CD_digitalRead(getCpin())); 
			if(getMode()== _DIGITAL)Card(getCtag())->MCPd_pinMode(getCpin(),_INPUT);
			break;
		case BRAIN:
			return digitalRead(getCpin());
			break;
	}
}
unsigned int fB_Tag::aRead() {
	switch(Card(getCtag())->type) {
		case _X50:
		case _X76:
			if(getMode()== _ANALOG && getOnVal() == _PGATE) return (Card(getCtag())->CD_analogRead(getCpin())); 
			if(getMode()== _DIGITAL) return (Card(getCtag())->MCPd_analogRead(getCpin())); 
			break;
		case BRAIN:
			if(getMode()== _ANALOG) return analogRead(getCpin());
			break;

	}
}
uint16_t fB_Tag::avgAnalogIn() {
	uint16_t  ival,sum = 0;
	if(getOnVal() != _PGATE) return 0;
	return Card(getCtag())->CD_avgAnalogRead(getCpin(),_ANALOGSAMPLESIZE);
}

