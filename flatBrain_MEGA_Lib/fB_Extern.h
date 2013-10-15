#ifndef FP_EXTERN_H
#define FP_EXTERN_H

	
extern fB_I2C		i2c;
extern fB_RTC		rtc;
extern fB_Alarm		alarm;
extern fB_EEPROM	ee;
extern fB_TFT		tft;
extern fB_Menu		menu; 
extern fB_RTC		rtc;
extern fB_Record	record;
extern fB_Curr		curr;
extern fB_Record	rec;
extern uint8_t 		_i2cspeed;
extern fB_WarnDelay	warn;
extern fB_Timer		timer;

extern	uint8_t 	_fBiSelectK1;	// interrupt fork selector, used in fBinterruptHandlerK1
extern	uint8_t 	_fBiK1;			// global interrupt flag

extern	uint8_t		_fBiTFT;
extern	fB_Tag		*_pTiK1shft;
extern  fB_Tag		*_pTiK1coro; // ptr to pin that coroborates _fBiK1, "


extern uint8_t  Xmap50[];
extern uint8_t  Xmap76[];

extern const __FlashStringHelper* PstrRay[MAXPSTRCOUNT];

typedef double (*pFunc)(fB_Tag* pT, uint16_t ival); 

typedef union  PandT {			// array of tags, preserves menu structure
	fB_Tag*		p;				// allows me to re-use array,first to store
	uint16_t	t;              // the tags on one pass, then to convert to pointers
};

typedef struct logFile {
	uint8_t						fTag;
	const __FlashStringHelper*	Pbase;
};

typedef struct logTag {
	uint8_t						fTag;
	uint16_t					tag;
};

extern uint8_t 		_sysStatus;
extern uint8_t 		_bootMsgIndex;
extern uint8_t 		_sdMsgIndex;

extern uint8_t		secondPass;
extern fB_Tag		*Tag(uint16_t tag);

extern uint16_t		tagCount;
extern uint16_t		rowCount;
extern uint16_t		logTagCount	;

extern uint8_t		pinCount;
extern uint8_t		cardCount;
extern uint8_t		pageCount;
extern uint8_t		logFileCount	;
//extern uint8_t		logFileCount;
//extern uint8_t		archiveCount;


extern	PandT*			rTP;	
extern	uint16_t*		tempTagRay;		// temp array for packed tag list
extern	fB_Tag*			tagRay;			// array of Tag objects
extern	fB_Tag*			rowTagRay;		// array of tags, preserves menu structure
extern	fB_Card**		pCardRay;			// sparse array of pointers to Card objects
extern	logTag*			logTagRay;			// sparse array of pointers to Card objects
extern	logFile*		logFileRay;			// sparse array of pointers to Card objects

extern  double VccRef;  // adjusted Vcc
void dbug(const __FlashStringHelper* Pdata,... );

bool activeSD();
bool existSD();
void logData(uint16_t arg16);
int freeRAM();
double readVcc();
char* getPstr(uint16_t tag, char *buffer);
char* getPtext(const __FlashStringHelper* Ptext,char *buffer);
void getPtextU(const __FlashStringHelper* Ptext,char *buffer);
//logTag* LogTag(uint16_t tag) ;
logFile* LogFile(uint8_t fTag) ;
logTag*  LogTag(uint8_t fTag) ;
fB_Tag*  Tag(uint16_t tag) ;
fB_Card* Card(uint16_t tag) ;	
fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint32_t flags,uint16_t tTag=NULL);
fB_Tag* initPage( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t parentTag); 
void initJump(uint16_t tag);
void initAlias(uint16_t tag, const __FlashStringHelper* Palias);
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) ;
void initCard(uint16_t tag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan );
void initRow(uint16_t tag, const __FlashStringHelper* Ptitle,uint32_t  flags);
void initRowList(uint16_t tag,const __FlashStringHelper* Ptitle,uint16_t parentTag,uint32_t flags);
void initLog(uint16_t tag, uint8_t fTag, const __FlashStringHelper* Pbase);
void defineCalibrate( uint16_t tag, pFunc _vFunc, double factor=1,double offset=NULL);	
void defineTarget(uint16_t tag,uint16_t tTag);
void defineSpace();
void defineValue(uint16_t tag,double value,double offset = NULL);
void navigate();
void logData(uint16_t arg16);
void playWarning(uint16_t arg16);
void startWarning(uint16_t arg16);
void endWarning(uint16_t arg16);
void endWarnDelay(uint16_t arg16);
uint16_t	getY(int rowIndex);



double posOnly(fB_Tag* pT,uint16_t intVal);
double sysAmps(fB_Tag* pT,uint16_t intVal);
double loadAmps(fB_Tag* pT,uint16_t intVal);

#endif

