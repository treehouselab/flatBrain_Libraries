#ifndef FB_MENU_H
#define FB_MENU_H



class fB_Menu { 
	public:	
		//fB_Curr		curr;

		int			fListStart ;	// can go negative !	
		int			tListStart ;		

		uint8_t 	buttonCode;  // set by tft.readButtons in interrupt
		uint16_t 	PstrCount;	

		uint8_t 	totalFiles ;		
		fB_Log		**mFile;
		uint8_t 	*fSort; // pointer to array of sorted indexes of mFile

		void		showPage(uint16_t tag, uint8_t option = NULL);
		void		clearPage(uint8_t fullFlag=NULL);
		void		erase();
		void		context(uint8_t  hand); // RIGHT or LEFT
		void		checkButtonCode();
		void		jumpPage(uint16_t tag);
		void		jumpPage(fB_Tag* pT);
		void		selectHeader();
		void		listFiles(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
		void		getFileList(); 
		void		pinPageConstruct(uint8_t startDex, uint8_t hand = RIGHT);
		void		refreshRow(uint16_t tag = NULL);


		fB_Menu();

	private:


};


#endif
