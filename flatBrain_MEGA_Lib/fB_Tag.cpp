#include "fB_Include.h"



fB_Tag::fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle,uint8_t _format, uint16_t _fTag, uint8_t _flags) {	
	
	tag = _tag; 
	Ptitle = _Ptitle;

	if(!_format) format = getFormat(value);
	else format = _format;
	fTag = _fTag; 
	flags = _flags;
	//if(flags & GBIAS) factor = 0;
	if(flags & TSYS) {
		flags |= TINIT;
		pSysTagRay[sysTagCount++] = this;
	}

}

float fB_Tag::getValue() {
	if(pPin) value = (float)pPin->read();
	if(!factor) factor = 1;
	return (value * factor + offset);
}

uint8_t  fB_Tag::getFormat(float value) {
	float mod = 0;
	mod = value-int(value);
	if(mod==0) return INT5;
	if(mod*10-int(mod*10) == 0) return FLOAT1;
	return FLOAT2;
}



