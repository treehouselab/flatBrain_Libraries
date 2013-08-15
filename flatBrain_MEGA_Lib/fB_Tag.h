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
		fB_Tag*		tag();					    //  current row tag 
		fB_Tag*		tag(uint8_t index);		// set current row
		uint16_t	farY;			// used to determine max Y coord of page for refresh

		uint8_t		getRowCount(); 
		void		putRowCount(uint8_t count); 
		void		incrRowCount(); 
		void		setCurrPage(uint16_t _tag);
		fB_Curr();
};


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
		//char* ptitle;
		union {	
			int		iVal; 	
			fB_Val*	dVal;	
			char	*text; 
			const __FlashStringHelper* Ptext;
		} ;

		uint16_t	flag16;
		uint8_t 	flag8;

		uint8_t		fTag;
		uint16_t	tTag;	  // pointer to target Tag 

		fB_Pin* pPin; 

		void		action(uint8_t hand);
		void		showRow(uint8_t  hide=NULL);
		uint16_t	getY();
		void		frame();
		void		unframe();
		void		clearRow1();
		void		clearRow();
		void		hideRow();
		void		selectRow();
		void		deselectRow();

		
		void		clearFlags();
		uint16_t	getFlags();
		void		putFlags(uint32_t flags32);
		uint32_t	getFormat();
		void		putFormat(uint32_t flags32);
		uint32_t	getAction();
		void		putAction(uint32_t flags32);

		uint32_t		assignFormat(double value);
		uint8_t		isDouble();					// is format a FLOAT1 or FLOAT2?
		double		getValue();					// get value from data, based on format. Cast as double. If Pin, use value from ADC.
		void		putValue(double value);		// put value in data union according to format
		fB_Tag(uint16_t _tag,const __FlashStringHelper* _Ptitle, uint32_t _flags,  uint8_t _fTag, uint16_t _tTag);

};



#endif




