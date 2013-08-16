#include "fB_Include.h"

fB_Val::fB_Val() {
	offset = 0;
	factor = 1;
}

fB_Tag::fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle, uint32_t flags = NULL, uint8_t _fTag=NULL, uint16_t _tTag=NULL) {	
	
	tag = _tag; 
	Ptitle = _Ptitle;
	text = NULL;
	pPin = NULL;
	fTag= _fTag;
	tTag= _fTag;
	flag8 = NULL;
	flag16 = NULL;
	putFlags( flags);
	putFormat( flags);
	putAction( flags);

}

void		fB_Tag::putFlags(uint32_t flags32) { flag16 |= flags32; }
uint16_t	fB_Tag::getFlags() { return flag16 & ~MASKP; }
void		fB_Tag::clearFlags() { flag16 &= ~0x0FFF; }

void	fB_Tag::putFormat(uint32_t flags32) {
	uint8_t format8 = 0;
	uint16_t format16 = 0;
	format16 = (flags32 & MASK32F) >> 21;
	for(int i=0;i< 10;i++) if(format16 >> i & 1) {
			format8 = i << 4;
			break;
	}
	flag8 |= format8;
}

uint32_t	fB_Tag::getFormat() {
	uint8_t j;
	uint32_t format32 = 0;
	j = (flag8 & MASK8F) >> 4;
	return (format32 & (1 << j)) << 22 ;
}

void	fB_Tag::putAction(uint32_t flags32) {
	uint8_t format8 = 0;
	uint16_t format16 = 0;
	format16 = (flags32 & MASK32A) >> 17;
	for(int i=0;i< 10;i++) if(format16 >> i & 1) {
			format8 = i << 4;
			break;
	}
	flag8 |= format8;
}

uint32_t	fB_Tag::getAction() {
	uint8_t j;
	uint32_t format32 = 0;
	j = (flag8 & MASK8A) >> 4;
	return (format32 & (1 << j)) << 18 ;
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
	if(pPin) {
		if(isDouble()){
			if(!dVal->factor) dVal->factor = 1;
			return ((double) pPin->read() * dVal->factor) + dVal->offset;
		}
		else return pPin->read();
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
}

void fB_Curr::setCurrPage(uint16_t _tag) {
	rowDex = 0;  // update when row selected
	farY = STARTY + (ROWHT * rowCount);
	if(_tag) {
		pageTag = _tag;
		pP = Tag(_tag);
		parentTag = pP->fTag;
		rowCount = getRowCount();
	}
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
	farY = STARTY + (ROWHT * count);
	rowCount = count;
	//dbug(F("PRC tag:%d, %P, rc:%d, f: %x"),pageTag, Tag(pageTag)->Ptitle,rowCount,pP->flag16);
}

void fB_Curr::incrRowCount() {
	uint8_t count = getRowCount();
	putRowCount(++count);
}



