#ifndef FP_EXTERN_H
#define FP_EXTERN_H

	
extern fB_I2C		i2c;
extern fB_RTC		rtc;
extern fB_Brain		brain;
extern fB_Alarm		alarm;
extern fB_EEPROM	ee;
extern fB_TFT		tft;
extern fB_Menu		menu; 
extern fB_RTC		rtc;
//extern fB_Seg		seg;
extern uint8_t 		_i2cspeed;

void getPtext(const __FlashStringHelper* pText,char *buffer );
void dbug(const __FlashStringHelper* pData,... );
//void dbug(char *fmt, ... );
fB_Global * Global(uint16_t gTag);
fB_Pin*	Pin(uint16_t pTag); 
fB_Card* Card(uint16_t cTag); 
char* floatToStr(float value, int places,char *buffer);
fB_Log * Log(uint16_t gTag);


#endif
