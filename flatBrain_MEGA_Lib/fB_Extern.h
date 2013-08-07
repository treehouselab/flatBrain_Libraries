#ifndef FP_EXTERN_H
#define FP_EXTERN_H

	
extern fB_I2C		i2c;
extern fB_RTC		rtc;
extern fB_Alarm		alarm;
extern fB_EEPROM	ee;
extern fB_TFT		tft;
extern fB_Menu		menu; 
extern fB_RTC		rtc;
//extern fB_Seg		seg;
extern uint8_t 		_i2cspeed;

extern uint8_t  XmapB[];
extern uint8_t  Xmap50[];
extern uint8_t  Xmap76[];
extern const __FlashStringHelper* PstrRay[];

void getPstr(uint16_t tag, char *buffer);
void getPtext(const __FlashStringHelper* Ptext,char *buffer );
void dbug(const __FlashStringHelper* Pdata,... );
char*	floatToStr(float value, int places,char *buffer);
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) ;
void initCard(uint16_t ctag,const __FlashStringHelper* Ptitle, uint8_t  cType,uint8_t  i2cAddr, uint8_t  aChan );

//void dbug(char *fmt, ... );

fB_Tag*		Tag(uint16_t tag);
fB_Pin*		Pin(uint16_t pTag); 
fB_Card*	Card(uint16_t cTag); 
fB_Log *	Log(uint16_t tag);
extern fB_Tag		**pTag;				// sparse array of pointers 
extern fB_Card		**pCard;			// sparse array of pointers 
extern logStruc    *Logs;


extern uint8_t		alarmEnabled;
extern uint8_t 		SDstatus;
extern uint16_t		pinCount;
extern uint16_t		logCount;
extern uint16_t		tagCount;
extern uint16_t		cardCount;
extern uint16_t		recordCount;
extern uint16_t		pageCount;
extern uint16_t		rowCount;
extern uint8_t		secondPass;

#endif
