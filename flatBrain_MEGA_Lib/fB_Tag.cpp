#include "fB_Include.h"

fB_Val::fB_Val() {
	offset = 0;
	factor = 1;
}

fB_Tag::fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle, uint16_t _fTag, uint8_t _flags=NULL) {	
	
	tag = _tag; 
	Ptitle = _Ptitle;

	fTag = _fTag; 
	flags = _flags;
	dp = NULL;
	pRow = NULL;
	pPin = NULL;

}

// Tag->flags is 8 bits, LAST 2bits IS MASKED FOR FORMAT
#define	PAGE			0x01	// ROW PAGE JUMP

#define	MASKP			0x1E	// next 4 bits overwritte in case of PAGE to hold pagerowcount

#define	TSYS			0x02	// System Tag
#define	STOREE			0x04	// store in eeprom
#define	LOG				0x08	
//#define	AVAIL		0x10	

#define	MASKF			0xE0	// last 3 bits reserved for format, EXCLUSIVE
#define	BLANK			0x00	
#define	TEXT			0x20	// default
#define	INT5			0x40	
#define	FLOAT1			0x60
#define	FLOAT2			0x80	
#define	BINARY			0xB0	
#define	STRIKE			0xC0	
//#define	AVAIL		0xD0	


uint8_t fB_Tag::putFormat(uint8_t format) { 
	if(format & MASKF)	{
		flags &= ~MASKF; 
		flags |= (format & MASKF);
	}
}
uint8_t fB_Tag::putFlags(uint8_t _flags) {
	flags |= _flags;
	if(_flags & PAGE)	flags &= ~MASKP; 
}
uint8_t fB_Tag::isDouble() {
	if(format() & FLOAT1  || format() & FLOAT2) return (1);
	else return 0;
}
uint8_t  fB_Tag::assignFormat(double value) {
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
			if(!data.pVal->factor) data.pVal->factor = 1;
			return ((double) pPin->read() * data.pVal->factor) + data.pVal->offset;
		}
		else return pPin->read();
	}
	else{
		if(isDouble()) 	{
			if(!data.pVal->factor) data.pVal->factor = 1;
			return (data.pVal->value * data.pVal->factor) + data.pVal->offset;
		}
		else return (double) data.iVal;
	}
}

void fB_Tag::putValue(double value) { 
	if(isDouble()) 	data.pVal->value = value;
	data.iVal = (int) value;
}


/*
uint8_t getProwCount() { return ((flags & MASKP)>>1);
void incProwCount() {
	uint8_t count = getProwCount();
	count++;
	putProwCount(count);
}
void putProwCount(uint8_t count);
	flags & ~MASKP;
	flags &= (count << 1);
}
//uint8_t fB_Tag::getFormat() { 
//	if(!(flags & INT5)) return TEXT; }
//	if((flags & INT5) == INT5) return INT5; 
//	return(flags & INT5);
	
//}
*/


