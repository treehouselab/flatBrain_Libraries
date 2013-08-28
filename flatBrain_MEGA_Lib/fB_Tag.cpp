#include "fB_Include.h"

fB_Val::fB_Val() {
	value = 0;
	offset = 0;
	factor = 1;
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


uint8_t fB_Tag::isDouble() {
	if(getFormat() == FLOAT1  || getFormat() == FLOAT2 || getFormat() == D2STR) return (1);
	else return 0;
}
uint32_t  fB_Tag::assignFormat(double value) {
	double mod = 0;
	mod = value-int(value);
	if(mod==0) return INT5;
	if(mod*10-int(mod*10) == 0) return FLOAT1;
	return FLOAT2;
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
		parentTag = pP->fTag;
		rowCount = getRowCount();
	}
	farY = STARTY + (ROWHT * (1+rowCount));
	//dbug(F("scp %P, rc:%d, f: %d"),pP->Ptitle,rowCount,farY);

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

void fB_Curr::selectRow(uint8_t rowIndex) {
	if(!rowIndex) rowIndex = rowDex;
	tft.setColor(FCOLOR,GCOLOR);
	tft.drawHLine(STARTX,tag()->getY(rowIndex)+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	rSelected = 1;
	
}

void fB_Curr::deselectRow(uint8_t rowIndex) {
	if(!rSelected) return;
	if(!rowIndex) rowIndex = rowDex;
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,tag()->getY(rowIndex)+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();
	rSelected = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////


void fB_Tag::createPin(uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {

	uint8_t offset,cPin,mode;


	offset = (row - 17) * 8 + (side*4);
	switch(Card(ctag)->type) {
		case X50:
			cPin =  pgm_read_byte(&Xmap50[offset+3]);
			mode =  pgm_read_byte(&Xmap50[offset+2]);
			break;
		case X76:
			cPin =  pgm_read_byte(&Xmap76[offset+3]);
			mode =  pgm_read_byte(&Xmap76[offset+2]);
			if(mode == IO_D)   Card(ctag)->MCPd_pinMode(cPin,dir);
			break;
		case BRAIN: 
			//if(getMode()== IO_D)    pinMode(getCpin(),iodir);
			break;
	}
	//if(mode == INPUT) {
		if(onval == HIGH) pull(LOW);
		else pull(HIGH);
	//}

	//pull(~onval); // pulls to OFF for both input and output getMode()s

	pin = 0;
	pin |= ((dir   << 15)& 0x8000);
	pin |= ((mode  << 14) & 0x4000);
	//unused flag bits 9-13
	pin |= ((onval  << 7)& 0x0180);  //2bit
	pin |= ((ctag  << 4)&  0x0070);  //3bits
	pin |= (cPin & 0x0F);			 //4bits
dbug(F("CP %P  pin:0x%x , cpin:%d/%d,m:%d/%d,onv:%d/%d "),Ptitle,pin,cPin,getCpin(),mode,getMode(),onval,getOnVal());
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
		case X50:
			for( i=3; i < 64; i=i+4) {
				if ( pgm_read_byte(&Xmap50[i]) == getCpin()
				&& ( pgm_read_byte(&Xmap50[i-1]) == getMode()) ) break;
			}
			rowSide |=  pgm_read_byte(&Xmap50[i-3]); ///row
			rowSide |=  pgm_read_byte(&Xmap50[i-2]) << 7 ;
			break;
		case X76:
			for( i=3; i < 128; i=i+4)	{
				if ( pgm_read_byte(&Xmap76[i]) == getCpin()
				&& ( pgm_read_byte(&Xmap76[i-1]) == getMode()) ) break;
			}
			rowSide |=  pgm_read_byte(&Xmap76[i-3]); ///row
			rowSide |=  pgm_read_byte(&Xmap76[i-2]) << 7 ; // side
			break;
		case BRAIN: 
			//if(getMode()== IO_D)    pinMode(getCpin(),iodir);
			break;
	}
}
void fB_Tag::pinMode(unsigned int iodir) {
	switch(Card(getCtag())->type) {
		case X50:
		case X76:
			if(getMode() == IO_D)    Card(getCtag())->MCPd_pinMode(getCpin(),getDir());
			break;
		case BRAIN: 
			//if(getMode()== D)    pinMode(getCpin(),iodir);
			break;
	}
}

void fB_Tag::pull(unsigned int value) {
	switch(Card(getCtag())->type) {
		case X50:
		case X76:
			//if(getMode() == IO_A ) 	Card(getCtag())->CD_analogWrite(getCpin(),value); 
			if(getMode()== IO_D)    Card(getCtag())->MCPd_pull(getCpin(),value);
			break;
		case BRAIN: 
			if(getMode()== IO_D)    digitalWrite(getCpin(),value);
			break;
	}
}
void fB_Tag::YshiftPulse(unsigned int msecs) {
		if(!pin) return;
		Tag(YSHFT)->pinMode(OUTPUT);
		Tag(YSHFT)->write(Tag(YSHFT)->getOnVal());
		pulse(msecs);
		Tag(YSHFT)->pinMode(INPUT);
}
void fB_Tag::pulse(unsigned int msecs) {
		if(!pin) return;
		pinMode(OUTPUT);
		write(getOnVal());
		delay(msecs);
		pinMode(INPUT);
}
void fB_Tag::write(unsigned int value) {
	//dbug(F("tw %P ,  onval:%d, vAL:%d"),Ptitle,getOnVal(),value);

	if(!pin) return;

	switch(Card(getCtag())->type) {
		case X50:
		case X76:
		//if(getMode() == IO_A ) 	Card(getCtag())->CD_analogWrite(this->CDchan(),value); 
		//if(getMode()== IO_D)    Card(getCtag())->MCP_digitalWrite(this->MCPchan(),value);
			if(getMode() == IO_A && getOnVal() == PGATE ) 	Card(getCtag())->CD_analogWrite(getCpin(),value); 
			if(getMode()== IO_D)  {
				value &= 0x0001;
				Card(getCtag())->MCPd_digitalWrite(getCpin(),value);
				//dbug(F("tw2 %P ,  ctag:%d, cp:%d,type:%d, val:%d"),Ptitle,getCtag(),getCpin(),Card(getCtag())->type,value);
			}
			break;
		case BRAIN:
			if(getMode()== IO_D)  digitalWrite(getCpin(),value);
			if(getMode()== IO_A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Tag::dWrite(unsigned int value) {
	if(!pin) return;
	switch(Card(getCtag())->type) {
		case X50:
		case X76:
			if(getMode() == IO_A )  Card(getCtag())->CD_digitalWrite(getCpin(),value);
			if(getMode()== IO_D)    Card(getCtag())->MCPd_digitalWrite(getCpin(),value);
			break;
		case BRAIN:
			if(getMode()== IO_D)  digitalWrite(getCpin(),value);
			if(getMode()== IO_A)  analogWrite(getCpin(),value);
			break;

	}
}
void fB_Tag::aWrite(unsigned int value) {
	if(!pin) return;
	switch(Card(getCtag())->type) {
		case X50:
		case X76:
			if(getMode() == IO_A )  Card(getCtag())->CD_analogWrite(getCpin(),value);
			if(getMode()== IO_D)    Card(getCtag())->MCPd_analogWrite(getCpin(),value);
			break;
		case BRAIN:
			//if(getMode()== IO_D)  digitalWrite(getCpin(),value);
			if(getMode()== IO_A)  analogWrite(getCpin(),value);
			break;
	}
}

uint16_t fB_Tag::readInt() {
	uint16_t intval;
	switch(Card(getCtag())->type) {
		case X50:
		case X76:
			if(getMode()== IO_A  && getOnVal() == PGATE) 	intval =  avgAnalogIn();
			if(getMode()== IO_D) {
				if(getDir()==OUTPUT) {
					if (isLatched()) intval =    HIGH;
					else intval = LOW;
				}
				else intval =   Card(getCtag())->MCPd_digitalRead(getCpin());
			}
			break;
		case BRAIN:
			if(getMode()== IO_D)  intval =   digitalRead(getCpin()); 
			if(getMode()== IO_A)  intval =    analogRead(getCpin());  
			break;
	}
	flag16 &= ~UNDEF;
//dbug(F("READ %P ,  onval:%d, mode:%d ival:%d"),Ptitle,getOnVal(),getMode(),intval);
	return intval;
}
uint16_t fB_Tag::read() {
	if(isDouble()) return calibrate(readInt());
	else return (iVal = readInt());
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
		case X50:
		case X76:
			if(getMode()== IO_A  && getOnVal() == PGATE) return (Card(getCtag())->CD_digitalRead(getCpin())); 
			if(getMode()== IO_D)Card(getCtag())->MCPd_pinMode(getCpin(),INPUT);
			break;
		case BRAIN:
			return digitalRead(getCpin());
			break;
	}
}
unsigned int fB_Tag::aRead() {
	switch(Card(getCtag())->type) {
		case X50:
		case X76:
			if(getMode()== IO_A && getOnVal() == PGATE) return (Card(getCtag())->CD_analogRead(getCpin())); 
			if(getMode()== IO_D) return (Card(getCtag())->MCPd_analogRead(getCpin())); 
			break;
		case BRAIN:
			if(getMode()== IO_A) return analogRead(getCpin());
			break;

	}
}
uint16_t fB_Tag::avgAnalogIn() {
	uint16_t  sum = 0;
	//dbug(F("AVGAI %P ,  onval:%d"),Ptitle,getOnVal());
	//if(getOnVal() != PGATE) return 0;
	Card(getCtag())->CD_analogRead(getCpin());
	Card(getCtag())->CD_analogRead(getCpin());
	Card(getCtag())->CD_analogRead(getCpin());
	Card(getCtag())->CD_analogRead(getCpin());
	Card(getCtag())->CD_analogRead(getCpin());
    for(int i = 0; i < ANALOGSAMPLESIZE; i++) {
        sum += Card(getCtag())->CD_analogRead(getCpin());
		delay(ANALOGSAMPLEDELAY);
    }
	return( (uint16_t) (sum/ANALOGSAMPLESIZE));
}

