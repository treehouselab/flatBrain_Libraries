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
fB_Curr		curr; 
//fB_Seg		seg;

//fB_WTV   audio;
//fB_VLVD   vlvd;
		char		dateStr[MAXCHARSTEXT];
		char		sizeStr[MAXCHARSTEXT];

const __FlashStringHelper* PstrRay[MAXPSTRCOUNT];

uint16_t	tagCount	= 0;
uint16_t	logTagCount		= 0;
uint8_t		logFileCount	= 0;
uint8_t		pinCount		= 0;
uint8_t		cardCount		= 0;
uint8_t		pageCount		= 0;
uint16_t	rowCount		= 0;
uint16_t	sListCount		= 0;
uint16_t	farY =	0;

uint8_t 	bootStatus;
uint8_t		secondPass;
fB_Tag		*Tag(uint16_t tag);

typedef union  PandT {			// array of tags, preserves menu structure
	fB_Tag*		p;				// allows me to re-use array,first to store
	uint16_t	t;              // the tags on one pass, then to convert to pointers
};								// on the next pass
PandT*			rTP;	
uint16_t*		tempTagRay;		// temp array for packed tag list
fB_Tag*			tagRay;			// array of Tag objects
fB_Tag**		sListRay;		// array of Tag pointers
fB_Tag*			rowTagRay;		// array of tags, preserves menu structure
fB_Card**		pCard;			// sparse array of pointers to Card objects
logStruc*		logRay;


void dbug(const __FlashStringHelper* Ptitle, ... );

// these functions are found in fB_USER_Define.cpp, fB_SYS_Define.cpp 
void defineUser();
void defineSystem();


///////////////////// GLOBAL to main.c FUNCTIONS ////////////////////////////////////////////////////////////////
void navigate() {   menu.buttonCode = tft.readButtons(); }  // tft button interrupt handler

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

char* getPtext(const __FlashStringHelper* Ptext,char *buffer){
  int cursor = 0;
   prog_char *ptr = ( prog_char * ) Ptext;
   if(Ptext) {
	   while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' && cursor < MAXCHARSTEXT ) ++cursor;
	   buffer[cursor] = '\0';
	   return buffer;
   }
   return NULL;
}
void getPtextU(const __FlashStringHelper* Ptext,char *buffer){
  int cursor = 0;
   prog_char *ptr = ( prog_char * )Ptext;
   if(Ptext)while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' ) ++cursor;
   buffer[cursor] = '\0';
}

void packTempTagRay(uint16_t tag) {
	int i;
	for( i=0; i < tagCount; i++ ) {
		if(tag == tempTagRay[i]) return;
		if(!tempTagRay[i]) break;
	}
	if(i<MAXTEMPTAG) {
		tempTagRay[i] = tag;
		tagCount++;
	}
}

logStruc*	Log(uint8_t tag) {
	if(tag != NULL) for(int i=0;i < logFileCount;i++) if(logRay[i].tag == tag) return &logRay[i];
	return NULL;
}
fB_Tag* Tag(uint16_t tag) {
	if(tag != NULL) for(int i=0;i < tagCount;i++) if(tagRay[i].tag == tag) return &tagRay[i];
	return NULL;
}
//fB_Pin* Pin(uint16_t tag) {	
//	if(tag != NULL) for(int i=0;i < tagCount;i++) if(tagRay[i].tag == tag && tagRay[i].pPin) return tagRay[i].pPin;
//	return NULL;
//}
fB_Card* Card(uint16_t tag) {	
	if(tag != NULL)for(int i=0;i < cardCount;i++) if(pCard[i]->tag == tag) return pCard[i];
	return NULL;
}

void initLog(uint16_t fTag,const __FlashStringHelper* Ptitle ) {
	if(!fTag || !Ptitle) return;
	if(!secondPass) logFileCount++;
	else{
		char buffer[13] = { '\0'};
		char title[MAXCHARSTEXT];
		getPtext(Ptitle,title);
		logRay[logFileCount].tag = fTag;
		sprintf(logRay[logFileCount].name,"%.8s.TXT",title);
		fB_Log *pL;
		pL = new fB_Log(fTag,logRay[logFileCount].name);
		if((bootStatus & SD) && pL->create()) {  // using fB_Log object only long enough to create file, storing filename in lotags array.
			pL->writeHeader();
		}
		delete pL;    // file system directory will be created later
		logFileCount++;
	}
}



fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint32_t flags,uint8_t fTag=NULL,const __FlashStringHelper* Plog  = NULL) {
		
	fB_Tag *pT = NULL;
	if(!secondPass) packTempTagRay(tag); // add to tempTagRay if unique;
	else {
		pT = Tag(tag);
		if(!pT)	{
			pT =  &tagRay[tagCount++]; 
			pT->tag = tag;
			pT->Ptitle = Ptitle;
			pT->fTag = fTag;
			//pT->tTag = tTag;
			pT->pin = NULL;
		}
		else if(fTag) pT->fTag = fTag;
		pT->putFlags(flags);
		pT->putFormat(flags);
		pT->putAction(flags);
		if(fTag && !Log(fTag)) {
			initLog(fTag,Plog );
			pT->flag16 |= LOG;
			logTagCount++;
		}
		dbug(F("IT  %P, t:%d "),Ptitle,tag);
	
	}
	return pT;
}

void initPage( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t parentTag){  
	fB_Tag * pT;
	pT = initTag(tag,Ptitle,PAGE,parentTag,NULL);  // for PAGE, fTag holds parentTag (8bits)
	if(secondPass) { 
		rTP[rowCount].t = tag;
		pT->iVal = rowCount;		// index in tagRay of first row of Page
		curr.setCurrPage(tag);      // fTag = parentTag
		curr.rowCount = 1;
		dbug(F("***IP %P, t:%d ***"),Ptitle,tag);
	}
	rowCount++;
	pageCount++;
}

void initJump(uint16_t tag) {	
	if(secondPass ) {
		fB_Tag * pT;
		pT = Tag(tag);
		rTP[rowCount].t = tag;
		curr.incrRowCount();
	}
	rowCount++;
}
void initRow(uint16_t tag, const __FlashStringHelper* Ptitle,uint32_t  flags){
	initTag(tag,Ptitle,flags,NULL,NULL);
	if(secondPass) 	{
		rTP[rowCount].t = tag;
		curr.incrRowCount(); // increment rowCount for this page, store in page flags
		curr.rowDex++;
	}	
	rowCount++ ;
}

void initRowList(uint16_t tag,const __FlashStringHelper* Ptitle,uint16_t parentTag,uint32_t flags){
	initPage(tag,Ptitle,parentTag);
	for(int i=0; i<MAXLISTROWS; i++)  initRow(tag+i+1,NULL,flags);
}
void initSpace() { 	
	if(secondPass) 	{
		rTP[rowCount].t = NULL;
		curr.incrRowCount(); // increment rowCount for this page, store in page flags
	}
	rowCount++;
}
void initPin( uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {
	fB_Tag *pT;
	pT = initTag(tag,Ptitle,NULL,NULL,NULL);
	if(secondPass)	{
		pT->createPin(ctag,row,side,dir,onval) ;
	}
	pinCount++;
}

void  initCard(uint16_t tag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan ) {

	if(secondPass)	{
		pCard[cardCount++] = new fB_Card(tag,Ptitle,type,i2cAddr,aChan ); // Card array is separate from Tag array
		dbug(F("IC cc:%d,t;%d, pt:%d"),cardCount-1,pCard[cardCount-1]->tag,Card(tag)->tag);
	}
}

void Calibrate( uint16_t tag, double factor=NULL,double offset=NULL) {
	fB_Tag *pT;
	pT = Tag(tag);
	if(pT->isDouble()) {
		if(!factor) factor=1;
		pT->dVal->factor = factor;
		pT->dVal->offset = offset;
	}
}


void flatBrainInit(){


	dbug(F("FB INIT ENTRY"));

	dbug(F("free RAM %d"),freeRAM());

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

	secondPass = 0;  // 1st pass , determine array sizes
	tempTagRay = (uint16_t*) calloc( MAXTEMPTAG,2); // temp array for packed tag list

	defineSystem();
	defineUser();

	free(tempTagRay);
	dbug(F("free RAM3 %d"),freeRAM());
	tagRay =	(fB_Tag *) calloc(tagCount,sizeof(fB_Tag));			// array of Tag objects
	rTP    =	(PandT *) calloc(rowCount,sizeof(PandT));			// array of tags or pointers, for menu operations
	logRay =	(logStruc *) calloc(logFileCount,sizeof(logStruc));
	pCard  =	(fB_Card **) calloc(cardCount,sizeof(fB_Card*));

	dbug(F("INIT PASS 1"));
	dbug(F("tagCount %d"),tagCount);
	//dbug(F("sListCount %d"),sListCount);
	dbug(F("pageCount %d"),pageCount);
	dbug(F("rowCount %d"),rowCount);
	dbug(F("cardCount %d"),cardCount);
	dbug(F("pinCount %d"),pinCount);
	dbug(F("logFileCount %d"),logFileCount);
	dbug(F("tag size %d"),sizeof(fB_Tag));
	dbug(F("log size %d"),sizeof(fB_Log));

	dbug(F("INIT MALLOC"));
	dbug(F("free RAM %d"),freeRAM());

	// reset counters
	tagCount    = 0;	// unique tags
//	sListCount  = 0;	// unique tags
	pageCount = 0;	// unique pages ( incl in tagCount)
	rowCount  = 0;	// all rows tags, incl spaces, jumps, and pin duplicates
	pinCount  = 0;	// all pin tags, possible duplicates of row tags
	cardCount = 0;
	logFileCount  = 0;
	logTagCount  = 0;

	secondPass = 1;  // 2nd pass , execute
	defineSystem();
	defineUser();
	for(int i=0; i< rowCount; i++) rTP[i].p = Tag(rTP[i].t); // might be obscure. ( see note at top of file ).
															//  takes array of uint16_t tags and converts them to fB_Tag*
	
	//sListRay  =	(fB_Tag **) calloc(sListCount,sizeof(fB_Tag*));

	dbug(F("INIT PASS 2"));

	pT = record.EEgetTag(TBOOT);
	res = (uint8_t ) pT->iVal;
	if(pT && res == HIGH) record.EEinitTags();     //initialize from eeprom globals defined with GINIT flag
	pT->iVal = res;
	dbug(F("INIT EEPROM"));

	//seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;
	//seg.test();  // set segmented display address if necessary;
	//seg.displayDec(1434,2);
	menu.init();
	tft.init(PORTRAIT);
	//alarm.bootBeepEnable();

	tft.clear();
	dbug(F("INIT TFT"));
	//dbug(F("FBf  %P , flags:%x"),Tag(HOME)->Ptitle,Tag(HOME)->flags);

	menu.showPage(HOME);

	alarm.play(ALARM_INIT);

	attachInterrupt(NAV_INT, navigate,FALLING);

	//set interrupt pins to high
	pinMode(NAV_INTPIN,INPUT_PULLUP);
	pinMode(K0_INTPIN,INPUT_PULLUP);
	pinMode(K1_INTPIN,INPUT_PULLUP);

	dbug(F("INIT COMPLETE"));
	//alarm.bootBeepDisable();
	dbug(F("free RAM %d"),freeRAM());
	Tag(FRAM)->iVal = freeRAM();


}


void dbug(const __FlashStringHelper* Ptitle, ... ){
  char fmt[ 60 ]; //Size array as needed.

  getPtextU(Ptitle,fmt);
  char prefix[ 61 ]; 
  char sbuffer[61] = { '\0' };
  int wid = 60;
  double f;
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
		else if(fmt[i] == 'L' || fmt[i] == 'l') { 
			n=va_arg(args,unsigned long);
			Serial.print(n,DEC);
		}
		else if(fmt[i] == 'J' || fmt[i] == 'j') { 
			n=va_arg(args,unsigned long);
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


char* doubleToStr(double value, int places,char *buffer) {
   // this is used to cast digits 
   int digit,dhit = 0;
   double tens = 0.1;
   int tenscount = 0;
   int i,j=0,k=0,abase= 48;
   double tempdouble = value;
   char dBuffer[places+1];

     // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
   // if this rounding step isn't here, the value  54.321 prints as 54.3209

   // calculate rounding term d:   0.5/pow(10,places)  
   double d = 0.5;
   if (value < 0)
     d *= -1.0;
   // divide by ten for each decimal place
   for (i = 0; i < places; i++)
     d/= 10.0;    
   // this small addition, combined with truncation will round our values properly 
   tempdouble +=  d;

   // first get value tens to be the large power of ten less than value
   // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

   if (value < 0)   tempdouble *= -1.0;
   while ((tens * 10.0) <= tempdouble) {
     tens *= 10.0;
     tenscount += 1;
   }
   if (value < 0) buffer[j++] = '-';// write out the negative if needed
   if (tenscount == 0) buffer[j++] = abase +0;
   for (i=0; i< tenscount; i++) {
     digit = (int) (tempdouble/tens);
     buffer[j++] = abase +digit;
     tempdouble = tempdouble - ((double)digit * tens);
     tens /= 10.0;
   }
   if (places <= 0){ // if no places after decimal, stop now and return
		buffer[j] = '\0';
		return buffer;
   }
   k = j;
   buffer[j++] = '.';// otherwise, write the point and continue on
   for (i = 0; i < places; i++) {   // write out each decimal place shifting digits into the ones place and writing the truncated value
     tempdouble *= 10.0; 
     digit = (int) tempdouble;
     buffer[j++] = abase + digit;
     tempdouble = tempdouble - (double) digit;      // once written, subtract off that digit
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
  double f;
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
