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

extern uint8_t  XmapB[];
extern uint8_t  Xmap50[];
extern uint8_t  Xmap76[];
extern const __FlashStringHelper* PstrRay[];

extern fB_Tag		**pTagRay;				// sparse array of pointers 
extern fB_Card		**pCardRay;			// sparse array of pointers 
extern logStruc    *logRay;

extern uint8_t		tagCount;
extern uint8_t		logCount;
extern uint8_t		pinCount;
extern uint8_t		cardCount;
extern uint8_t		pageCount;
extern uint16_t		rowCount;
extern uint8_t		tListZeroIndex;

extern uint8_t		alarmEnabled;
extern uint8_t 		bootStatus;
extern uint8_t		secondPass;

uint8_t		logCount		= 0;
uint8_t		tagCount		= 0;
uint8_t		pinCount		= 0;
uint8_t		cardCount		= 0;
uint8_t		pageCount		= 0;
uint16_t	rowCount		= 0;



char*	floatToStr(float value, int places,char *buffer);
void dbug(const __FlashStringHelper* Pdata,... );


char* getPstr(uint16_t tag, char *buffer);
void getPtext(const __FlashStringHelper* Ptext,char *buffer);
void getPtextU(const __FlashStringHelper* Ptext,char *buffer);
fB_Tag* Tag(uint16_t tag) ;
fB_Row* Row(uint16_t tag) ;
fB_Pin* Pin(uint16_t tag) ;
fB_Card* Card(uint16_t tag) ;	
fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint8_t fTag=NULL,uint8_t flags=NULL) ;
fB_Tag* initPage( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t parentTag); 
void initRow(uint16_t tag, const __FlashStringHelper* Ptitle,uint8_t  format,uint8_t  flags,uint16_t tTag=NULL);
void initJump(uint16_t tag);
void initSpace();;
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) ;
void initCard( uint16_t tag,const __FlashStringHelper* Ptitle, ctype,i2cAddr, aChan );
void Calibrate( uint16_t tag, double factor=NULL,double offset=NULL) ;
void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle ) ;	
void navigate();
void defineSystemTags();

#endif

