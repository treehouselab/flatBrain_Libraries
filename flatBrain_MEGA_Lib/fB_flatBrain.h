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

uint8_t		alarmEnabled;
uint8_t 	SDstatus;
uint16_t	pinCount;
uint16_t	logCount;
uint16_t	tagCount;
uint16_t	cardCount;
uint16_t	recordCount;
uint16_t	pageCount;
uint16_t	rowCount;
uint8_t		secondPass;

fB_Tag		**pTag;				// sparse array of pointers 
fB_Card		**pCard;			// sparse array of pointers 
logStruc    *Logs;

void dbug(const __FlashStringHelper* Ptitle, ... );

// these functions are found in fB_Define.cpp
void defineTags();
void definePins();
void defineRecords();
void defineLogs();
void defineMenu();


///////////////////// GLOBAL to main.c FUNCTIONS ////////////////////////////////////////////////////////////////
void getPstr(uint16_t tag, char *buffer){
   int cursor = 0;
   prog_char *ptr = ( prog_char * ) PstrRay[tag];
   while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' && cursor < MAXCHARSTEXT ) ++cursor;
   buffer[cursor] = '\0';
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
	if(tag != NULL) for(int i=0;i < tagCount;i++) if(pTag[i]->tag == tag) return pTag[i];
	return NULL;
}
fB_Pin* Pin(uint16_t tag) {	
	if(tag != NULL) for(int i=0;i <tagCount;i++) if(pTag[i]->tag == tag && pTag[i]->pPin) return pTag[i]->pPin;
	return NULL;
}
fB_Card* Card(uint16_t cTag) {	
	if(cTag != NULL)for(int i=0;i < cardCount;i++) if(pCard[i]->cTag == cTag) return pCard[i];
	return NULL;
}
void initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint8_t format=NULL,uint16_t fTag=NULL,uint8_t flags=NULL) {
	if(!secondPass) tagCount++;
	else pTag[tagCount++] = new fB_Tag(tag,Ptitle,format,fTag,flags);
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
	if(!pT) defineTag(tag,NULL,NULL,NULL);
	if(secondPass && pT) pT->pPin = new fB_Pin(ctag,Ptitle,row,side,dir,onval) ;
}
void initCard(uint16_t ctag,const __FlashStringHelper* Ptitle, uint8_t  cType,uint8_t  i2cAddr, uint8_t  aChan )  {
	if(!secondPass) cardCount++;
	else pCard[cardCount++] = new fB_Card(ctag,Ptitle,cardCount, cType,i2cAddr,aChan );
}

void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle ) {	
	if(!secondPass) logCount++;
	else{
		char buffer[13] = { '\0'};
		char title[MAXCHARSTEXT];
		getPtext(Ptitle,title);
		Logs[logCount].tag = fTag;
		sprintf(Logs[logCount].name,"%.8s.TXT",title);
		fB_Log *pL;
		pL = new fB_Log(fTag,Logs[logCount].name);
		if((SDstatus & SD) && pL->create()) {  // using fB_Log object only long enough to create file, storing filename in lotags array.
			pL->writeHeader();
		}
		delete pL;    // file system directory will be created later
		logCount++;
	}
}

void navigate() {   menu.buttonCode = tft.readButtons(); }  // tft button interrupt handler

void defineMenu() { 	menu.defineMenu(); } // for visual consistecy in .ino 

void defineSystemTags() {
	// Probably also need to define row in fB_Menu.cpp
	initTag(GBOO,F("BOOTG"), BINARY,SYSTEM,GSYS);

}


void flatBrainInit(){
	i2c.begin();
	i2c.setSpeed(I2CSPEED);
	i2c.timeOut(I2CTIMEOUT);

	uint8_t  res=0;
	fB_Tag* pT;

	alarmEnabled = 1; // this should actually be toggled in fB_menu

	// turn off legacy SPI SS pin ( Mega D53) so it does not conflict w/ SD card or other SPI	
	// enable explicitly when you want to use (eg. ATTINY SPI uses legacy SS)
	pinMode(AT_SPISS,OUTPUT);  
    digitalWrite(AT_SPISS,LOW);

	SDstatus = 0;
	alarm.play(ALARM_1);


	res = rtc.init();
	if(res) {
		alarm.play(ALARM_2);
		dbug(F("RTC FAILED"));
	}
	else {
		SDstatus |= RTC;
		dbug(F("flatBRAIN INIT RTC"));
	}


	res = fat.initFAT(SPISPEED);
	if(res) {
		dbug(F("SD ERROR 0X%h"),res);
		alarm.play(ALARM_2);
	}
	else {
		SDstatus |= SD;
		dbug(F("INIT SD"));
	}

	dbug(F("INIT PASS 1"));
	secondPass = 0;  // 1st pass , determine array sizes
	defineCard(BRAIN,BRAIN,0,0);
	defineSystemTags();
	defineTags(); 
	definePins(); 

	menu.defineSystem();
	menu.defineClock(); 
	menu.defineMenu(); // 1st pass with passtog = 0, determine array sizes
	
dbug(F("INIT MALLOC"));
	Logs =  (logStruc *) calloc(logCount,sizeof(logStruc));
	pTag = (fB_Tag **) malloc(sizeof(fB_Tag*) * tagCount);
	pCard = (fB_Card **) malloc(sizeof(fB_Card*) * cardCount);
	menu.mPage = (fB_Page *) malloc(sizeof(fB_Page) * pageCount);
	menu.mRow =  (fB_Row *)   calloc(rowCount,sizeof(fB_Row));

// reset counters
	logCount = 0;
	tagCount = 0;
	//gSysCount = 0;
	//gUsrCount = 0;
	cardCount = 0;
	pinCount = 0;
	pageCount = 0;
	rowCount = 0;

dbug(F("INIT PASS 2"));
	secondPass = 1;
	defineCard(BRAIN,BRAIN,0,0);
	defineSystemTags();
	defineTags(); // 2nd pass, execute
	definePins();
	menu.defineSystem();
	menu.defineClock();
	menu.defineMenu(); // 2nd pass with passtog = 1, execute

dbug(F("INIT EEPROM"));

	pT = record.EEgetTag(GBOO);
	res = (uint8_t ) pT->value;
	if(pT && res == HIGH) record.EEinitTags();     //initialize from eeprom globals defined with GINIT flag
	pT->value = (float) res;

	//seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;
	//seg.test();  // set segmented display address if necessary;
	//seg.displayDec(1434,2);
	menu.init();

dbug(F("INIT COMPLETE"));

	//set interrupt pins to high
	pinMode(NAV_INTPIN,INPUT_PULLUP);
	pinMode(K0_INTPIN,INPUT_PULLUP);
	pinMode(K1_INTPIN,INPUT_PULLUP);

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
 
int freeRAM () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

 void dumpEEPROM(unsigned int addr, unsigned int length)
{
   // block to 10
   addr = addr / 10 * 10;
   length = (length + 9)/10 * 10;

   byte b = ee.readByte(addr); 
   for (int i = 0; i < length; i++) 
   {
     if (addr % 10 == 0)
     {
       Serial.println();
       Serial.print(addr);
       Serial.print(":\t");
     }
     Serial.print(b);
     b = ee.readByte(++addr); 
     Serial.print("  ");
   }
   Serial.println();
}

#endif


/*
void fB_Brain::defineGlobal(uint16_t tag,float value, const __FlashStringHelper* Ptitle, uint8_t  format,uint16_t fTag,uint8_t  flags) {	
	if(!passToggle)	{
		totalGlobals++;
		if(flags & GSYS) totalGsys++;
		else  totalGusr++;
	}

	else{
			pGlobal[globalCount] = new fB_Global(tag,value, Ptitle, format,fTag,flags);
			if(flags & GSYS) gSys[gSysCount++]=globalCount;
			else gUsr[gUsrCount++]=globalCount;
			globalCount++;
		}
}



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

void fBdbug(const __FlashStringHelper* pData ){
  char buffer[ 20 ]; //Size array as needed.
  int cursor = 0;
  prog_char *ptr = ( prog_char * ) pData;

  while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' ) ++cursor;
  Serial.println(buffer);

}
*/

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
