#ifndef FB_MENU_H
#define FB_MENU_H


class fB_Row {
	public:
		const __FlashStringHelper* Ptext;
		char		*ptext;	// alternative non-PROGMEM text
		char		*ptitle;	// alternative non-PROGMEM title

		uint16_t		tTag;	  // pointer to target Tag 
		uint8_t 	flags;
		
		uint8_t		getAction();
		uint8_t		putAction(uint8_t action);
		uint16_t	getY();
		void		frame();
		void		unframe();
		void		clearRow1();
		void		clearRow();
		void		showRow(uint8_t  hide=0);
		fB_Row(); 
};


class fB_Curr {
	public:
		uint8_t		tag;			// tag of current page
		fB_Tag*		pTag;			// pointer to tag used to store Page info ( flags = PAGE )
		fB_Tag*		pRzero;			// first  tag index (row) of current page in pTagRay[]
		uint16_t	parentTag;      // parent Tag of current page
		uint8_t		rowDex;			// offset from pRzero of current selected row, updated by setCurrPage and menu.selectRow()
		uint8_t		rowCount;		// 
		uint16_t	farY;			// used to determine max Y coord of page for refresh
		void		updateRowCount(uint8_t count);
		void		setCurrPage(uint16_t _tag);

		fB_Curr();
};


class fB_Menu { 
	public:	
		fB_Curr		curr;

		int			fListStart ;	// can go negative !	
		int			tListStart ;		

		uint16_t 	PstrCount;	
		uint8_t 	buttonCode;  // set by tft.readButtons in interrupt

		uint8_t 	totalFiles ;		
		fB_Log		**mFile;
		uint8_t 	*fSort; // pointer to array of sorted indexes of mFile

		void        init();
		void		show(uint16_t pTag);
		void		showPage(uint16_t pTag);
		void		erase();
		void		context(uint8_t  hand); // RIGHT or LEFT
		void		checkButtonCode();
		void		jumpPage(uint16_t tag);
		void		jumpPage(fB_Tag* pT);
		void		selectHeader();
		void		defineSystem();
		void		listFiles(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
		void		getFileList(); 

	private:


};


#endif
