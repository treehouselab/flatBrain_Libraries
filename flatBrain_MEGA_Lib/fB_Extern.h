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

extern uint8_t  XmapB[];
extern uint8_t  Xmap50[];
extern uint8_t  Xmap76[];

extern const __FlashStringHelper* PstrRay[MAXPSTRCOUNT];

typedef union  PandT {			// array of tags, preserves menu structure
	fB_Tag*		p;				// allows me to re-use array,first to store
	uint16_t	t;              // the tags on one pass, then to convert to pointers
};

typedef struct logTag {
	uint8_t						tag;
	const __FlashStringHelper*	Pbase;
};

extern uint8_t 		bootStatus;
extern uint8_t		secondPass;
extern fB_Tag		*Tag(uint16_t tag);


extern uint16_t		tagCount;
extern uint8_t		pinCount;
extern uint8_t		cardCount;
extern uint8_t		pageCount;
extern uint16_t		rowCount;
extern uint16_t		logTagCount	;
extern uint8_t		logFileCount;
extern uint8_t		archiveCount;


extern	PandT*			rTP;	
extern	uint16_t*		tempTagRay;		// temp array for packed tag list
extern	fB_Tag*			tagRay;			// array of Tag objects
extern	fB_Tag*			rowTagRay;		// array of tags, preserves menu structure
extern	fB_Card**		pCardRay;			// sparse array of pointers to Card objects

void dbug(const __FlashStringHelper* Pdata,... );

int freeRAM();
char* getPstr(uint16_t tag, char *buffer);
char* getPtext(const __FlashStringHelper* Ptext,char *buffer);
void getPtextU(const __FlashStringHelper* Ptext,char *buffer);
logTag* Log(uint8_t tag) ;
fB_Tag* Tag(uint16_t tag) ;
fB_Card* Card(uint16_t tag) ;	
fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint32_t flags,uint8_t fTag=NULL,const __FlashStringHelper* Plog  = NULL);
fB_Tag* initPage( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t parentTag); 
void initJump(uint16_t tag);
void initSpace();
void initAlias(uint16_t tag, const __FlashStringHelper* Palias);
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) ;
void initCard(uint16_t tag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan );
void initRow(uint16_t tag, const __FlashStringHelper* Ptitle,uint32_t  flags);
void initRowList(uint16_t tag,const __FlashStringHelper* Ptitle,uint16_t parentTag,uint32_t flags);
void defineCalibrate( uint16_t tag, double factor=NULL,double offset=NULL) ;
void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle ) ;	
void navigate();
#endif

