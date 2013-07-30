#ifndef FB_BRAIN_H
#define FB_BRAIN_H

#include <fB_Define.h>
//#include <math.h>


class fB_Pin; // pre-declare
class fB_Card; // pre-declare

class fB_Global {
	public:
		uint16_t gTag;
		uint16_t fTag;
		uint8_t  flags;
		uint8_t  format;
		float factor;
		float value;
		char *tagStr;
		float getValue();
		uint8_t  getFormat(float value);
		fB_Global(uint16_t gTag,float value, char *tagStr, uint8_t  _format,uint16_t fTag,uint8_t  flags=0);	

};
class fB_Stack {
	public:
		float value;
		const __FlashStringHelper* pTitle;
};
class fB_Log {
	public:
		uint16_t	fTag;
		char		filename[14];
		//uint8_t 		enabled;
		//char	fileext[4];
		char		dateStr[MAXCHARSTEXT];
		char		sizeStr[MAXCHARSTEXT];
		void		show();
		bool		create(char *fname=NULL);
		void		setDate();
		void		writeHeader();
		void		writeData();
		bool		archive();
		void		remove();
		void		getAttributes();
		void		dump();
		fB_Log(uint16_t	fTag, char * filename) ;
};

class fB_Brain {
	public:
		uint8_t			alarmEnabled;
		uint8_t 		status;
		uint8_t 		passToggle;			
		uint8_t 		totalGlobals;		// totals from 1st pass
		uint8_t 		totalGsys;		
		uint8_t 		totalGusr;		
		uint8_t 		totalStacks;		
		uint8_t 		totalLogs;	
		uint8_t 		totalPins;			 
		uint8_t 	    totalCards;				
		uint8_t 		globalCount;		// current count 
		uint8_t 		gSysCount;		// current count 
		uint8_t 		gUsrCount;		// current count 
		uint8_t 		stackCount;			
		uint8_t 		logCount;			
		uint8_t 		pinCount;			
		uint8_t 	    cardCount;	
		uint8_t 		*gSys,*gUsr;
		fB_Stack	*stack; 
		fB_Global	**pGlobal;				// sparse array of pointers 
		fB_Pin		**pPin;				// sparse array of pointers to inited pins, built by initPin() 
		fB_Card		**pCard;				// sparse array of pointers to inited cards, built by init_card() 
		uint8_t 		x0Map[10];		    // map of expansion card bus pins to Mega Pins
		uint8_t 		x2Map[16];		    // map of expansion card X2 chip pins to bus pins
		uint8_t 		x3Map[32];		    // map of expansion card X3 chip pins to bus pins
		uint8_t 		VDRmap[8];		    // map of expansion card CD B pins 1- VDIV resistors

		typedef struct		logStruc {	
			uint16_t	tag;	 
			char		name[13];
			fB_Log*		pLog;			
			};

		logStruc   *Logs;
		void		createGdefLog();		
		void		defineSystemGlobals();
		void		defineElements();
		void		EEwriteGlobals();
		void		EEinitGlobals();
		fB_Global *	EEgetGlobal(fB_Global *);
		char*		getLogName(uint16_t fTag);
		void		init(uint8_t  i2cspeed = I2CSLOW);
		bool		SDinit(uint8_t  SSpin, uint8_t  SPIspeed);
		void		defineStack(const __FlashStringHelper* pTitle,float value);
		void		defineGlobal(uint16_t gTag,float value, char *tagStr, uint8_t  format,uint16_t fTag,uint8_t  flags);	
		void		defineGlobalPin(uint16_t gTag, uint8_t format,uint16_t fTag,uint8_t  flags);	
		void		defineLog(uint16_t fTag, char *tagStr);
		void		defineCard(uint16_t ctag,char *text, uint8_t  cType,uint8_t  i2cAddr, uint8_t  aChan );
		void		definePin(uint16_t pTag, char *tagStr, uint16_t  cTag, uint8_t  row, uint8_t  side, uint8_t  iodir, uint8_t  ival=0);
		//void		createPin(uint16_t pTag, uint8_t  pnum, uint8_t  mode, uint8_t  iodir, uint8_t  ival=0);
		fB_Card*	Card(uint16_t cTag);

	private:
		void		initXmaps();
		uint8_t		getXmapIndex(uint8_t mapCode,uint8_t  row, uint8_t  side);
		void		insertXmap(uint8_t  mapCode,uint8_t  row, uint8_t  side, uint8_t  type, uint8_t  pn);

};



#endif
