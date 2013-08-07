#ifndef FB_TAG_H
#define FB_TAG_H

#include "fB_Header.h"

class fB_Col {
	public:
		uint16_t fTag;
		uint8_t  flags;
		uint8_t  format;
		fB_Col( uint16_t _fTag,uint8_t  _flags);	


};

class fB_Tag {
	public:
		fB_Pin *pPin;
		uint16_t tag;
		double value;
		double factor;
		double offset;
		const __FlashStringHelper* Ptitle;
		uint8_t format;
		uint8_t flags;
		uint16_t fTag;

		float		getValue();
		uint8_t		getFormat(float value);
		fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle,uint8_t format, uint16_t _fTag, uint8_t _flags);

};

#endif




