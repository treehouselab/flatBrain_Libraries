#ifndef FB_TAG_H
#define FB_TAG_H

#include "fB_Header.h"

class fB_Row;// pre-declare

class fB_Val {
	public:
		double dVal;
		double factor;
		double offset;
		fB_Val();	
};

class fB_Tag {
	public:
		uint16_t tag;
		const __FlashStringHelper* Ptitle;
		uint8_t  flags;
		uint8_t  fTag;
		union {	
			int iVal; 	
			fB_Val*	pVal;	
			char *text; 
			const __FlashStringHelper* _Ptext; 
		} dp;

		fB_Pin* pPin; 
		fB_Row*	pRow; 

		uint8_t		putFlags(uint8_t _flags); 
		uint8_t		putFormat(uint8_t format); 
		uint8_t		getFormat();					// return format bits masked from flags
		uint8_t		assignFormat(double value);	// assign format flags based on decimal places in value
		uint8_t		isDouble();					// is format a FLOAT1 or FLOAT2?
		double		getValue();					// get value from data, based on format. Cast as double. If Pin, use value from ADC.
		void		putValue(double value);		// put value in data union according to format
		fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle, uint16_t _fTag, uint8_t _flags);

};



#endif




