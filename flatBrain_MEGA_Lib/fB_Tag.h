#ifndef FB_TAG_H
#define FB_TAG_H

#include "fB_Header.h"

class fB_Tag;// pre-declare

class fB_Curr {
	public:
		uint16_t	pageTag;		// tag of current page
		fB_Tag*		pP;				// pointer to tag used to store Page info ( flags = PAGE )
		uint16_t	parentTag;      // parent Tag of current page
		uint8_t		rowDex;			// offset from rZero of current selected row, updated by  and menu.selectRow()
		uint8_t		rowCount;		// 
		uint16_t	farY;			// used to determine max Y coord of page for refresh
		uint8_t		rSelected;		// row is selected (highlighted)

		fB_Tag*		tag(uint8_t index = NULL);		// set current row
		uint8_t     row(uint16_t tag);  // return index of tag's row on page
		uint8_t		getRowCount(); 
		void		putRowCount(uint8_t count); 
		void		incrRowCount(); 
		void		setCurrPage(uint16_t _tag);
		void        nextSwitch();
		void        prevSwitch();
		void		selectRow(uint8_t rowIndex = NULL);
		void		deselectRow(uint8_t rowIndex = NULL);
		fB_Curr();
};


class fB_Val {
	public:
		double value;
		double factor;
		double offset;
		fB_Val();	
};

class fB_Tag {
	public:
		uint16_t tag;
		union {	
			const __FlashStringHelper	*Ptitle;
			char						*ptitle;
		};
		union {	
			int							iVal; 	
			fB_Val						*dVal;	
			const __FlashStringHelper	*Ptext;
			char						*ptext; 
		} ;

		union {
			uint8_t		fTag;
			uint8_t		buf8;
		};
		uint16_t	flag16;
		uint8_t 	flag8;
		uint16_t	pin; 

		//uint16_t	tTag;	  // pointer to target Tag 

		void		action(uint8_t hand);
		void		showRow(uint8_t rowIndex, uint8_t  pageOption=NULL);

		uint16_t	getY(uint8_t rowIndex);
		//void		frame();
		//void		unframe();
		void		clearRow1(uint8_t rowIndex);
		void		clearRow(uint8_t rowIndex);
		void		hideRow(uint8_t rowIndex);

		void		clearFlags() { flag16 &= ~0x0FFF; }
		uint16_t	getFlags() { return flag16 & ~MASKP; }
		void		putFlags(uint32_t flags32) 	{ flag16 |= (uint16_t)flags32; }
		uint8_t		getFormat8() {	return (flag8 & MASK8F) >> 4;  }
		uint32_t	getFormat();
		void		putFormat(uint32_t flags32);
		uint32_t	getAction();
		uint8_t		getAction8() { 	return  (flag8 & MASK8A); }
		void		putAction(uint32_t flags32);
		uint32_t	assignFormat(double value);
		uint8_t		isDouble();					// is format a FLOAT1 or FLOAT2?
		double		getDval() { return dVal->value = read() * dVal->factor + dVal->offset; };
		void		putDval(double value) { if(isDouble()) 	dVal->value = value; }

		void createPin(uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval);
		void pull(unsigned int value);  // HIGH or LOW
		void pinMode(unsigned int iodir); //INPUT or OUTPUT
		void pulse(unsigned int msecs); 
		void YshiftPulse(unsigned int msecs); 
		uint8_t getOnVal();
		uint8_t getDir();
		uint8_t getMode();
		uint8_t getCtag() ;
		uint8_t getCpin() ;
		void putOnVal(uint8_t onval); // 3 options: HIGH, LOW, PGATE
		void getRowSide(uint8_t &rowSide);
		void write(unsigned int value);
		void aWrite(unsigned int value);
 		void dWrite(unsigned int value);
		unsigned int read();
		unsigned int aRead();
		unsigned int dRead();
		bool isLatched();
		uint16_t avgAnalogIn();
		double calibrate() { return (dVal->value = iVal * dVal->factor + dVal->offset); }

};



#endif




