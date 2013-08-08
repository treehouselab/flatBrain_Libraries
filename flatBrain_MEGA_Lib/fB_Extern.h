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
extern uint8_t 		_i2cspeed;

extern uint8_t  XmapB[];
extern uint8_t  Xmap50[];
extern uint8_t  Xmap76[];
extern const __FlashStringHelper* PstrRay[];

char*	floatToStr(float value, int places,char *buffer);
char*   getPstr(uint16_t tag, char *buffer);
void getPtext(const __FlashStringHelper* Ptext,char *buffer );
void dbug(const __FlashStringHelper* Pdata,... );
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) ;
void initCard(uint16_t ctag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan );
void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle );	
void initValue( uint16_t tag,float value, float factor=NULL,float offset=NULL);
fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint8_t _format=NULL,uint16_t _fTag=NULL,uint8_t _flags=NULL);



//void dbug(char *fmt, ... );

fB_Tag*		Tag(uint16_t tag);
fB_Pin*		Pin(uint16_t pTag); 
fB_Card*	Card(uint16_t tag); 
fB_Log *	Log(uint16_t tag);

extern fB_Tag		**pUsrTagRay;				// sparse array of pointers 
extern fB_Tag		**pSysTagRay;			// sparse array of pointers 
extern fB_Card		**pCardRay;			// sparse array of pointers 
extern logStruc    *logRay;


extern uint8_t		logCount;
extern uint8_t		pinCount;
extern uint8_t		cardCount;
extern uint8_t		pageCount;
extern uint8_t		sysTagCount;
extern uint16_t		usrTagCount;
extern uint16_t		rowCount;

extern uint8_t		alarmEnabled;
extern uint8_t 		bootStatus;
extern uint8_t		secondPass;

#endif
