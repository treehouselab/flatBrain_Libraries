#ifndef FB_FLATBRAIN_H
#define FB_FLATBRAIN_H

#include "fB_Header.h"

uint8_t 	_i2cspeed = I2CSLOW;			
fB_I2C		i2c;
fB_RTC      rtc; 
fB_EEPROM   ee; 
fB_TFT      tft; 
fB_Menu     menu; 
fB_tFAT     fat; 
fB_Alarm    alarm; 
fB_Record   record; 
//fB_Seg		seg;

//fB_WTV   audio;
//fB_VLVD   vlvd;

const __FlashStringHelper* PstrRay[MAXPSTRCOUNT];

uint8_t		logCount		= 0;
uint8_t		sysTagCount		= 0;
uint8_t		pinCount		= 0;
uint8_t		cardCount		= 0;
uint8_t		pageCount		= 0;
uint16_t	usrTagCount		= 0;
uint16_t	rowCount		= 0;

uint8_t 	bootStatus;
uint8_t		secondPass;

fB_Tag		**pUsrTagRay;				// sparse array of pointers 
fB_Tag		**pSysTagRay;			// sparse array of pointers 
fB_Card		**pCardRay;			// sparse array of pointers 
logStruc    *logRay;

void dbug(const __FlashStringHelper* Ptitle, ... );

// these functions are found in fB_Define.cpp
void defineTags();
void definePins();
void defineRecords();
void defineLogs();
void defineMenu();


///////////////////// GLOBAL to main.c FUNCTIONS ////////////////////////////////////////////////////////////////

int freeRAM () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

char* getPstr(uint16_t tag, char *buffer){
   int cursor = 0;
   prog_char *ptr = ( prog_char * ) PstrRay[tag];
   while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' && cursor < MAXCHARSTEXT ) ++cursor;
   buffer[cursor] = '\0';
   return buffer;
}

void getPtext(const __FlashStringHelper* Ptext,char *buffer){
  int cursor = 0;
   prog_char *ptr = ( prog_char * ) Ptext;
   if(Ptext) while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' && cursor < MAXCHARSTEXT ) ++cursor;
   buffer[cursor] = '\0';
}
void getPtextU(const __FlashStringHelper* Ptext,char *buffer){
  int cursor = 0;
   prog_char *ptr = ( prog_char * )Ptext;
   if(Ptext)while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' ) ++cursor;
   buffer[cursor] = '\0';
}



fB_Tag* Tag(uint16_t tag) {
	if(tag != NULL) {
		for(int i=0;i < usrTagCount;i++) if(pUsrTagRay[i]->tag == tag) return pUsrTagRay[i];
		for(int i=0;i < sysTagCount;i++) if(pSysTagRay[i]->tag == tag) return pSysTagRay[i];
	}
	return NULL;
}
fB_Pin* Pin(uint16_t tag) {	
	if(tag != NULL) for(int i=0;i < usrTagCount;i++) if(pUsrTagRay[i]->tag == tag && pUsrTagRay[i]->pPin) return pUsrTagRay[i]->pPin;
	return NULL;
}
fB_Card* Card(uint16_t tag) {	
	if(tag != NULL)for(int i=0;i < cardCount;i++) if(pCardRay[i]->tag == tag) return pCardRay[i];
	return NULL;
}
fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint8_t _format=NULL,uint16_t _fTag=NULL,uint8_t _flags=NULL) {
	if(!secondPass && tag) {
		if(_flags & TSYS) sysTagCount++;
		else usrTagCount++;
	}
	else if(tag) {
		fB_Tag * pT;
		pT = Tag(tag);
		if(!pT) 	pT = new fB_Tag(tag,Ptitle,_format,_fTag,_flags);
		else { 
			if(!_format) pT->format = pT->getFormat(pT->value);
			else pT->format = _format;
			pT->fTag = _fTag; 
			pT->flags = _flags;
			//if(_flags & GBIAS) pT->factor = 0;
			//if(_flags & TSYS) pT->flags |= TINIT;
		}
		if(_flags & TSYS) pSysTagRay[sysTagCount++] = pT;
		else pUsrTagRay[usrTagCount++] = pT;
		return pT;
	}
	return NULL;

}
void initValue( uint16_t tag,float value, float factor=NULL,float offset=NULL) {
	fB_Tag *pT;
	pT = Tag(tag);
	if(pT) {
		pT->value = value;
		if(factor != NULL) pT->factor = factor;
		if(offset != NULL) pT->offset = offset;
	}
}
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {
	pinCount++;
	fB_Tag * pT;
	pT = Tag(tag);
	if(!pT) initTag(tag,Ptitle,NULL,NULL,NULL);
	if(secondPass && pT) pT->pPin = new fB_Pin(ctag,row,side,dir,onval) ;
}

void initCard(uint16_t ctag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan )  {
	if(!secondPass) cardCount++;
	else pCardRay[cardCount++] = new fB_Card(ctag,Ptitle,cardCount, type,i2cAddr,aChan );
}

void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle ) {	
	if(!secondPass) logCount++;
	else{
		char buffer[13] = { '\0'};
		char title[MAXCHARSTEXT];
		getPtext(Ptitle,title);
		logRay[logCount].tag = fTag;
		sprintf(logRay[logCount].name,"%.8s.TXT",title);
		fB_Log *pL;
		pL = new fB_Log(fTag,logRay[logCount].name);
		if((bootStatus & SD) && pL->create()) {  // using fB_Log object only long enough to create file, storing filename in lotags array.
			pL->writeHeader();
		}
		delete pL;    // file system directory will be created later
		logCount++;
	}
}

void navigate() {   menu.buttonCode = tft.readButtons(); }  // tft button interrupt handler

void defineSystemTags() {
	// Probably also need to define row in fB_Menu.cpp
	initTag(TBOOT,F("BOOTG"), BINARY,SYSTEM,TSYS);

}

void flatBrainInit(){
	i2c.begin();
	i2c.setSpeed(I2CSPEED);
	i2c.timeOut(I2CTIMEOUT);

	uint8_t  res=0;
	fB_Tag* pT;

	// turn off legacy SPI SS pin ( Mega D53) so it does not conflict w/ SD card or other SPI	
	// enable explicitly when you want to use (eg. ATTINY SPI uses legacy SS)
	pinMode(AT_SPISS,OUTPUT);  
    digitalWrite(AT_SPISS,LOW);

	bootStatus = 0;

	alarm.enable();
	alarm.play(ALARM_1);


	res = rtc.init();
	if(res) {
		alarm.play(ALARM_2);
		dbug(F("RTC FAILED"));
	}
	else {
		bootStatus |= RTC;
		dbug(F("INIT RTC"));
	}


	res = fat.initFAT(SPISPEED);
	if(res) {
		dbug(F("SD ERROR 0X%h"),res);
		alarm.play(ALARM_2);
	}
	else {
		bootStatus |= SD;
		dbug(F("INIT SD"));
	}

	menu.init();
	dbug(F("INIT MENU"));

	secondPass = 0;  // 1st pass , determine array sizes
	defineCard(BRAIN,BRAIN,0,0);
	defineSystemTags();
	defineTags(); 
	definePins(); 
	menu.defineSystem();
	defineMenu(); 

	dbug(F("logCount %d"),logCount);
	dbug(F("INIT PASS 1"));
	dbug(F("pageCount %d"),pageCount);
	dbug(F("rowCount %d"),rowCount);
	dbug(F("row size %d"),sizeof(fB_Row));
	dbug(F("tag size %d"),sizeof(fB_Tag*));
	dbug(F("sysTagCount %d"),sysTagCount);
	dbug(F("usrtCount %d"),usrTagCount);
	dbug(F("cardCount %d"),cardCount);

	dbug(F("free RAM %d"),freeRAM());
	
	menu.mPage = (fB_Page *)  calloc(pageCount,sizeof(fB_Page));
	menu.mRow =  (fB_Row *)   calloc(rowCount,sizeof(fB_Row));
	logRay =  (logStruc *) calloc(logCount,sizeof(logStruc));
	pSysTagRay = (fB_Tag **) malloc(sizeof(fB_Tag*) * sysTagCount);
	pUsrTagRay = (fB_Tag **) malloc(sizeof(fB_Tag*) * usrTagCount);
	pCardRay = (fB_Card **) malloc(sizeof(fB_Card*) * cardCount);
	dbug(F("INIT MALLOC"));
	dbug(F("free RAM %d"),freeRAM());

// reset counters
	logCount = 0;
	usrTagCount = 0;
	sysTagCount = 0;
	cardCount = 0;
	pageCount = 0;
	rowCount = 0;

	secondPass = 1;

	defineCard(BRAIN,BRAIN,0,0);
	defineSystemTags();
	defineTags(); // 2nd pass, execute
	definePins();
	menu.defineSystem();
	defineMenu(); // 2nd pass with passtog = 1, execute
	dbug(F("INIT PASS 2"));


	pT = record.EEgetTag(TBOOT);
	res = (uint8_t ) pT->value;
	if(pT && res == HIGH) record.EEinitTags();     //initialize from eeprom globals defined with GINIT flag
	pT->value = (float) res;
	dbug(F("INIT EEPROM"));

	//seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;
	//seg.test();  // set segmented display address if necessary;
	//seg.displayDec(1434,2);

	tft.init(PORTRAIT);
	alarm.bootBeepEnable();

	tft.clear();
	dbug(F("INIT TFT"));

	menu.show(HOME);

	alarm.play(ALARM_INIT);

	attachInterrupt(NAV_INT, navigate,FALLING);

	//set interrupt pins to high
	pinMode(NAV_INTPIN,INPUT_PULLUP);
	pinMode(K0_INTPIN,INPUT_PULLUP);
	pinMode(K1_INTPIN,INPUT_PULLUP);

	dbug(F("INIT COMPLETE"));
	alarm.bootBeepDisable();
	dbug(F("free RAM %d"),freeRAM());

}


void dbug(const __FlashStringHelper* Ptitle, ... ){
  char fmt[ 40 ]; //Size array as needed.

  getPtextU(Ptitle,fmt);
  char prefix[ 41 ]; 
  char sbuffer[41] = { '\0' };
  int wid = 40;
  float f;
  char * s;
  const __FlashStringHelper* Ptext;
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
		if(fmt[i] == 'P') { 
			Ptext =va_arg(args,const __FlashStringHelper*);
			char pstr[MAXCHARSTEXT];
			getPtext(Ptext,pstr);
			Serial.print(pstr);
		}
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
 


#endif


/*

/*
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
*/
