#ifndef FLATBRAIN_H
#define FLATBRAIN_H

/* This master file includes all the modules needed 
for the I/O expander backplane functionality. 
Treehouselab.com - Mike Price */
//#include "Arduino.h"

//void * operator new(size_t size);
//void operator delete(void * ptr);

//void * operator new(size_t size) {  return malloc(size); }
//void operator delete(void * ptr) {  free(ptr); }


#include "fB_Header.h"

uint8_t 	_i2cspeed = I2CSLOW;			
fB_Brain	brain;
fB_I2C		i2c;
fB_RTC      rtc; 
fB_EEPROM   ee; 
fB_TFT      tft; 
fB_Menu     menu; 
fB_tFAT     fat; 
fB_Alarm    alarm; 
//fB_Seg		seg;

//fB_WTV   audio;
//fB_VLVD   vlvd;
void dbug(char *fmt, ... );

///////////////////// GLOBAL to main.c FUNCTIONS ////////////////////////////////////////////////////////////////

fB_Global * Global(uint16_t gTag) {
	for(int i=0;i < brain.globalCount;i++) if(brain.pGlobal[i]->gTag == gTag) return brain.pGlobal[i];
	return NULL;
}

// pin(TAG) function uses arrays pinTagMap and pPin to return pointer to fB_Pin object associated by TAG (eg. pin(LED1)->row)
fB_Pin * Pin(uint16_t pTag) {	
	for(int i=0;i < brain.pinCount;i++) if(brain.pPin[i]->pTag == pTag) return brain.pPin[i];
	return NULL;
}
fB_Card * Card(uint16_t cTag) {	
	for(int i=0;i < brain.cardCount;i++) if(brain.pCard[i]->cTag == cTag) return brain.pCard[i];
dbug("CARD NULL");
	return NULL;
}
/*
uint8_t  getTagState(uint16_t tTag) {
	fB_Pin *pP;
	fB_Global *pG;
	pP = Pin(tTag);
	if(pP) return pP->dRead();
	pG = Global(tTag);
	if(pG)  if (pG->value >0.5) return HIGH;
	return LOW;
}
void putTagState(uint16_t tTag,uint8_t  state) {
	fB_Pin *pP;
	fB_Global *pG;
	pP = Pin(tTag);
	if(pP) 	pP->dWrite(state);
	else {
		pG = Global(tTag);
		if(pG) pG->value = state;
	}
}
*/
void dbug(char *fmt, ... ){
  char prefix[41]; 
  char sbuffer[41] = { '\0' };
  int wid = 40;
  float f;
  char * s;
  int i,j,n,k;
  
  va_list args;
  va_start(args,fmt);
  for( i=0,j=0;i<strlen(fmt);i++) {		
	  if(fmt[i] == '%') {
		prefix[j] = '\0'; 
		Serial.print(prefix);
		prefix[0]='\0';
		j = 0;
		i++;
		if(fmt[i] == 'f') { 
			f=va_arg(args,double);
			Serial.print(f,DEC);
		}
		else if(fmt[i] == 's') { 
			s=va_arg(args,char *);
			for( k=0;k<wid && s[k] != '\0';k++) sbuffer[k] = s[k];
			sbuffer[k] = '\0';
			s = &sbuffer[0];
			Serial.print(s);
		}
		else if(fmt[i] == 'd') { 
			n=va_arg(args,int);
			Serial.print(n,DEC);
		}
		else if(fmt[i] == 'b') { 
			n=va_arg(args,int);
			Serial.print(n,BIN);
		}
		else if(fmt[i] == 'h' || fmt[i] == 'x') { 
			n=va_arg(args,int);
			Serial.print(n,HEX);
		}
	  }
	  else {
		  if(j<20) {
			  prefix[j] = fmt[i];
			  j++;
		  }
	  }
  }
  prefix[j] = '\0';
  va_end(args);
  Serial.println(prefix);
}
char* floatToStr(float value, int places,char *buffer) {
   // this is used to cast digits 
   int digit,dhit = 0;
   float tens = 0.1;
   int tenscount = 0;
   int i,j=0,k=0,abase= 48;
   float tempfloat = value;
   char dBuffer[places+1];

     // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
   // if this rounding step isn't here, the value  54.321 prints as 54.3209

   // calculate rounding term d:   0.5/pow(10,places)  
   float d = 0.5;
   if (value < 0)
     d *= -1.0;
   // divide by ten for each decimal place
   for (i = 0; i < places; i++)
     d/= 10.0;    
   // this small addition, combined with truncation will round our values properly 
   tempfloat +=  d;

   // first get value tens to be the large power of ten less than value
   // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

   if (value < 0)   tempfloat *= -1.0;
   while ((tens * 10.0) <= tempfloat) {
     tens *= 10.0;
     tenscount += 1;
   }
   if (value < 0) buffer[j++] = '-';// write out the negative if needed
   if (tenscount == 0) buffer[j++] = abase +0;
   for (i=0; i< tenscount; i++) {
     digit = (int) (tempfloat/tens);
     buffer[j++] = abase +digit;
     tempfloat = tempfloat - ((float)digit * tens);
     tens /= 10.0;
   }
   if (places <= 0){ // if no places after decimal, stop now and return
		buffer[j] = '\0';
		return buffer;
   }
   k = j;
   buffer[j++] = '.';// otherwise, write the point and continue on
   for (i = 0; i < places; i++) {   // write out each decimal place shifting digits into the ones place and writing the truncated value
     tempfloat *= 10.0; 
     digit = (int) tempfloat;
     buffer[j++] = abase + digit;
     tempfloat = tempfloat - (float) digit;      // once written, subtract off that digit
	 if(digit) dhit = 1;
   }
   if(!dhit) j=k;  // truncate if all decimals are zero
   buffer[j] = '\0';
   return buffer;
}

/*
 void softReset() // Restarts program from beginning but does not reset the peripherals and registers
  {
	asm volatile ("  jmp 0");  
  }
*/
// free RAM check for debugging. SRAM for ATmega328p = 2048Kb.
//  1024 with ATmega168
 
int freeRAM () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

#endif

