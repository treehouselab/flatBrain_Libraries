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
fB_Record   rec; 
fB_Curr		curr; 
//fB_Seg		seg;

//fB_WTV   audio;
//fB_VLVD   vlvd;
const __FlashStringHelper* PstrRay[MAXPSTRCOUNT];

typedef union  PandT {			// array of tags, preserves menu structure
	fB_Tag*		p;				// allows me to re-use array,first to store
	uint16_t	t;              // the tags on one pass, then to convert to pointers
};	

typedef struct logTag {
	uint8_t						tag;
	const __FlashStringHelper*	Pbase;
};

// on the next pass
uint8_t 	bootStatus;
uint8_t		secondPass;
fB_Tag		*Tag(uint16_t tag);
//uint16_t	farY =	0;

uint16_t	tagCount	= 0;
uint8_t		pinCount		= 0;
uint8_t		cardCount		= 0;
uint8_t		pageCount		= 0;
uint16_t	rowCount		= 0;
uint16_t	logTagCount		= 0;
uint8_t		logFileCount	= 0;
uint8_t		archiveCount	= 0;


PandT*			rTP;	
uint16_t*		tempTagRay;		// temp array for packed tag list
logTag*			logTagRay;		// array of structs containing log tag and basename pointer
fB_Tag*			tagRay;			// array of Tag objects
fB_Tag*			rowTagRay;		// array of tags, preserves menu structure
fB_Card**		pCard;			// sparse array of pointers to Card objects


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
   while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' && cursor < MAXCHARSLINE ) ++cursor;
   buffer[cursor] = '\0';
   return buffer;
}

char* getPtext(const __FlashStringHelper* Ptext,char *buffer){
  int cursor = 0;
   prog_char *ptr = ( prog_char * ) Ptext;
   if(Ptext) {
	   while( ( buffer[ cursor ] = pgm_read_byte_near( ptr + cursor ) ) != '\0' && cursor < MAXCHARSLINE ) ++cursor;
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

logTag* Log(uint8_t tag) {
	if(tag != NULL) for(int i=0;i < logTagCount;i++) if(logTagRay[i].tag == tag) return &logTagRay[i];
	return NULL;
}

fB_Tag* Tag(uint16_t tag) {
	if(tag != NULL) for(int i=0;i < tagCount;i++) if(tagRay[i].tag == tag) return &tagRay[i];
	return NULL;
}
fB_Card* Card(uint16_t tag) {	
	if(tag != NULL)for(int i=0;i < cardCount;i++) if(pCard[i]->tag == tag) return pCard[i];
	return NULL;
}

fB_Tag* initTag(uint16_t tag,const __FlashStringHelper* Ptitle,uint32_t flags,uint8_t fTag=NULL,const __FlashStringHelper* Pbase  = NULL) {
	char Pbuffer[MAXCHARSLINE+1];	
	fB_Tag *pT = NULL;
	if(!secondPass) {
		packTempTagRay(tag); // add to tempTagRay if unique;
		if(fTag && !(flags & PAGE)) logTagCount++;
	}
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
		if(!pT->isDouble() && (flags & (FLOAT1 | FLOAT2 | D2STR))){
				pT->dVal = new fB_Val;
		}
		pT->putFlags(flags);
		pT->putFormat(flags);
		pT->putAction(flags);
		if(fTag && !(flags & PAGE)) {
			pT->flag16 |= LOG;
			if(bootStatus & SD) rec.logCreate(getPtext(Pbase,Pbuffer));
			if(!Log(tag)) {
				logTagRay[logTagCount].tag = fTag;
				logTagRay[logTagCount].Pbase = Pbase;
				logTagCount++;
			}
		}
	
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
	}
}

void defineCalibrate( uint16_t tag, double factor=NULL,double offset=NULL) {
	if(secondPass) {
		fB_Tag *pT;
		pT = Tag(tag);
		if(!pT->isDouble()) pT->dVal = new fB_Val;
		pT->putFormat(D2STR);
		if(!factor || factor < 0.00001 ) factor=1;
		pT->dVal->factor = factor;
		pT->dVal->offset = offset;
		//dbug(F("CALIB t:%d, %P %f"),tag,pT->Ptitle, factor);
		//dbug(F("CL0 P:%P fac:%f"), Tag(CL)->Ptitle, Tag(CL)->dVal->factor);

	}
}


void flatBrainInit(){


	dbug(F(" "));
	dbug(F("---------"));
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
	logTagRay = (logTag *) calloc(logTagCount,sizeof(logTag));
	pCard  =	(fB_Card **) calloc(cardCount,sizeof(fB_Card*));

	dbug(F("INIT PASS 1"));
	dbug(F("tagCount %d"),tagCount);
	dbug(F("logTagCount %d"),logTagCount);
	dbug(F("pageCount %d"),pageCount);
	dbug(F("rowCount %d"),rowCount);
	dbug(F("cardCount %d"),cardCount);
	dbug(F("pinCount %d"),pinCount);
	dbug(F("tag size %d"),sizeof(fB_Tag));

	dbug(F("INIT MALLOC"));
	dbug(F("free RAM %d"),freeRAM());

	// reset counters
	tagCount    = 0;	// unique tags
	pageCount = 0;	// unique pages ( incl in tagCount)
	rowCount  = 0;	// all rows tags, incl spaces, jumps, and pin duplicates
	pinCount  = 0;	// all pin tags, possible duplicates of row tags
	cardCount = 0;
	logTagCount  = 0;

	secondPass = 1;  // 2nd pass , execute
	defineSystem();
	defineUser();

	for(int i=0; i< rowCount; i++) rTP[i].p = Tag(rTP[i].t); // might be obscure. ( see note at top of file ).
															//  takes array of uint16_t tags and converts them to fB_Tag*

	dbug(F("INIT PASS 2"));

	pT = rec.EEgetTag(TBOOT);
	res = (uint8_t ) pT->iVal;
	if(pT && res == HIGH) rec.EEinitTags();     //initialize from eeprom globals defined with GINIT flag
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
			char pstr[MAXCHARSLINE+1];
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
