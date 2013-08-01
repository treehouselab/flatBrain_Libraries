#ifndef FB_MENU_H
#define FB_MENU_H


class fB_Row; // pre-declare
class fB_Page; // pre-declare
class fB_Log; // pre-declare

class fB_Row {

	public:
		const __FlashStringHelper* Ptitle;
		const __FlashStringHelper* Ptext;
		char		*ptitle;				// alternative non-PROGMEM title
		char		*ptext;				// alternative non-PROGMEM text
		uint16_t	tag;	// this tag
		uint8_t 	pDex;
		fB_Page		*pPage;
		uint16_t	Y; // upper left of button
		uint8_t 	type;

		uint8_t 	status;
		uint8_t 	format;
		float		value;
		uint16_t	tTag;	// target Tag ( Pins or Globals )
		//uint8_t 	state;
		//float		factor; // calibration factor
		//uint8_t 		ftype;  // factor type ( AMPLIFY{*}, BIAS{+} )
		void	frame();
		void	unframe();
		void	clearRow1();
		void	clearRow();
		void	show(uint8_t  flag=0);
		void	hide();
		void    action(uint8_t  flag);
		void	select();
		void	deselect();

};


class fB_Page {
	public:
		
		const __FlashStringHelper* Ptitle;
		uint16_t	pTag; 
		uint16_t	parentTag; 
		uint16_t	Y; // upper left 
		uint8_t 	type;
		uint8_t 	pDex;
		byte		currRowDex;
		uint8_t 	status;
		fB_Row	    * pRow;
		//fB_Window	win;
		int			farY; 
		uint8_t 	pageStackCount;				
		uint8_t 	pageRowCount;				

		void		show();
		void		hide(uint8_t  flag=0);
		void		selectHeader();
		void		nextSwitch();
		void		prevSwitch();
		//void		update();

};


class fB_Menu { 
	public:	
		char		nullText[2];
		void        init();
		void		show(uint16_t pTag);
		void		erase();
		void		context(uint8_t  hand); // RIGHT or LEFT
		//void		nextSwitch();
		//void		prevSwitch();
		void		checkButtonCode();
		//void		updateButtons();
		void		jumpPage(uint16_t pTag);
		void		defineMenu(); 
		//void		definePage( uint16_t pTag,char *tagStr,uint8_t  type,uint16_t parentTag = NULL);
		void		definePage( uint16_t pTag,const __FlashStringHelper* pTitle,uint16_t parentTag = NULL,uint8_t  = NULL);
		void		defineStacks();
		void		defineGlobals();
		void		defineClock();
		void		defineSystem();
		//void		definePageArchive( );

		
		void        calibrate(uint16_t mTag,uint8_t  ftype,float value);

		void        defineJrow(uint16_t tTag,const __FlashStringHelper* pText);
		void        defineArow(uint8_t  type,const __FlashStringHelper* pText);
		void        defineDrow(const __FlashStringHelper* pText);
		void	    defineRow(uint16_t mTag,const __FlashStringHelper* pTitle,uint8_t  type,uint8_t  format=NULL);
		fB_Row*     defineRow(uint16_t mTag,const __FlashStringHelper* pTitle,uint8_t  type, uint8_t  format,uint16_t tTag, float value=0);
		void        writeRow(uint16_t mTag,char* text,float value=NULL);
		void		updateGauge(uint16_t mTag);
		void		updateRow(uint16_t mTag);
		void		showGauge(uint16_t mTag);
		void		showRow(uint16_t mTag);
		//void        defineGauge(uint16_t mTag,uint8_t  type, char *text, uint16_t tTag);
		//void		resetGauge(uint16_t mTag,uint8_t  type, char* text, uint16_t tTag);
		//void		writeGauge(uint16_t mTag,uint8_t  type, char* text, float value);
		//void        defineWindow(uint16_t mTag,uint16_t ht);
		//void        defineWindow(uint16_t  mTag, uint16_t ht,uint8_t   type,uint8_t  type,char *text, uint16_t pin);
		//void        writeWindow(uint16_t mTag,uint8_t  type, char* text, uint16_t tTag,float value);// general case
		void		listFiles(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
		void		getFileList(); 
//		int			compareFilename(const void *_x1, const void *_x2);

		uint8_t 	    pageCount;				
		uint8_t 	    rowCount;				
		uint8_t 	    mTagCount;	
		uint16_t 	    PstrCount;	
		int	    fListStart ;	// can go negative !	
		int	    sListStart ;		
		int	    gyListStart ;		
		int	    guListStart ;		
		//uint8_t 		currPageDex;
		fB_Page		*pCurrPage;
		char stampStr[6];
		char deleteStr[7];
		char noLogStr[7];
		char biasStr[5];
		char ampStr[4];
		char strikeStr[5];


		uint8_t 		buttonCode;  // set by tft.readButtons in interrupt
		uint8_t 		passTog;
		uint8_t 		totalPages;
		uint8_t 		totalRows;
		//uint8_t 		totalMtags;
		uint8_t 	    totalFiles ;		

		fB_Page		*mPage;
		fB_Row  	*mRow;
		fB_Log		**mFile;
		uint8_t 		*fSort; // pointer to array of sorted indexes of mFile
		//fB_Page		*Page(uint8_t  index);
		//fB_Window   *Gauge(uint16_t mTag);
		//fB_Window   *Window(uint16_t mTag);
		fB_Row		*Row(uint16_t mTag);
		fB_Page		*Page(uint16_t tTag);



	private:

		//void		createMenu();

};


#endif
/*
class fB_Window: public fB_Block{

	public:
		
		//char	*gTitle1;
		//char	*gTitle2;
		uint8_t  type;
		void	clear();

		void	show(uint8_t  flag=0);
		void	hide();

		//void	gaugeParseTitle(char *text);
		//void	gaugeShowMinus();
		//void	gaugeShowPeriod(uint8_t  gtype);
		//void    gaugeShowTitle();
	
};
*/