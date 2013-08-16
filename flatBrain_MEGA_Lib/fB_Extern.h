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
extern uint8_t 		_i2cspeed;
extern		char		dateStr[MAXCHARSTEXT];
extern		char		sizeStr[MAXCHARSTEXT];

extern uint8_t  XmapB[];
extern uint8_t  Xmap50[];
extern uint8_t  Xmap76[];
extern const __FlashStringHelper* PstrRay[];

typedef union  PandT {		// array of tags, preserves menu structure
	fB_Tag*		p;
	uint16_t	t;
};
extern PandT*			rTP;	

extern	fB_Tag*			tagRay;			// array of Tag objects
extern	fB_Card**		pCardRay;		// sparse array of pointers 
extern	logStruc*		logRay;

extern uint8_t		tagCount;
extern uint8_t		logCount;
extern uint8_t		pinCount;
extern uint8_t		cardCount;
extern uint8_t		pageCount;
extern uint8_t		rowCount;
extern uint8_t		tListZeroIndex;

extern uint8_t		alarmEnabled;
extern uint8_t 		bootStatus;
extern uint8_t		secondPass;

char*	doubleToStr(double value, int places,char *buffer);
void dbug(const __FlashStringHelper* Pdata,... );


char* getPstr(uint16_t tag, char *buffer);
void getPtext(const __FlashStringHelper* Ptext,char *buffer);
void getPtextU(const __FlashStringHelper* Ptext,char *buffer);
fB_Tag* Tag(uint16_t tag) ;
fB_Pin* Pin(uint16_t tag) ;
fB_Card* Card(uint16_t tag) ;	
fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint32_t flags,uint8_t fTag=NULL,uint16_t tTag=NULL);
fB_Tag* initPage( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t parentTag); 
void initJump(uint16_t tag);
void initSpace();;
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) ;
void initCard(uint16_t tag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan );
void initRow(uint16_t tag, const __FlashStringHelper* Ptitle,uint32_t  flags,uint16_t tTag=NULL);
void initRowList(uint16_t tag, const __FlashStringHelper* Ptitle,uint8_t count,uint32_t  flags,uint16_t tTag=NULL);
void Calibrate( uint16_t tag, double factor=NULL,double offset=NULL) ;
void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle ) ;	
void navigate();
#endif

