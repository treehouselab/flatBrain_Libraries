#ifndef FB_FLATBRAIN_H
#define FB_FLATBRAIN_H

#include "fB_Header.h"


fB_RTC          rtc; 
fB_EEPROM       ee; 
fB_TFT          tft; 
fB_Menu         menu; 
fB_tFAT         fat; 
//fB_Alarm        alarm; 
fB_Record       rec; 
fB_Curr		    curr; 
//fB_WarnDelay	warn;
fB_Timer		timer;
extern TwoWire Wire1;

uint8_t 	_fBiSelectK1 = 0;	// interrupt fork selector, used in fBinterruptHandlerK1
uint8_t 	_fBiK1 = 0;			// global interrupt flag
uint8_t 	_fBiTFT = 0;		// global interrupt flag from TFT switches
fB_Tag		*_pTiK1shft = NULL; // ptr to pin that shifts _fBiK1, defined in fB_USER_Define.cpp
fB_Tag		*_pTiK1coro = NULL; // ptr to pin that coroborates _fBiK1, "
uint8_t		logInitFlag = 0;	// set  at first log entry after reboot, helps synchro logs

//double VccRef;  // adjusted Vcc
//fB_WTV   audio;
//fB_VLVD   vlvd;
char* gRay[MAXGRAYCOUNT];

typedef double (*pFunc)(fB_Tag* pT,uint16_t ival); 

typedef union  PandT {			// array of tags, preserves menu structure
	fB_Tag*		p;				// allows me to re-use array,first to store
	uint16_t	t;              // the tags on one pass, then to convert to pointers
};	


typedef struct logFile {
	uint8_t		fTag;
	char*		base;
};

typedef struct logTag {
	uint8_t						fTag;
	uint16_t					tag;
};

uint8_t 	_sysStatus = 0;
uint8_t 	_bootMsgIndex = 0;
uint8_t 	_sdMsgIndex = 0;
uint8_t		secondPass;
fB_Tag		*Tag(uint16_t tag);
//uint16_t	farY =	0;

uint16_t	tagCount	= 0;
uint16_t	rowCount		= 0;
uint16_t	logTagCount		= 0;

uint8_t		pinCount		= 0;
uint8_t		cardCount		= 0;
uint8_t		pageCount		= 0;
uint8_t		logFileCount		= 0;


PandT*			rTP;	
uint8_t*		logTempRay;		
uint16_t*		tagTempRay;		// temp array for packed tag list
logTag*			logTagRay;		// array of structs containing log tag and basename pointer
logFile*		logFileRay;		// array of structs containing log tag and basename pointer
fB_Tag*			tagRay;			// array of Tag objects
fB_Tag*			rowTagRay;		// array of tags, preserves menu structure
fB_Card**		pCardRay;			// sparse array of pointers to Card objects


void dbug(char* title, ... );

// these functions are found in fB_USER_Define.cpp, fB_SYS_Define.cpp 
void defineUser();
void defineSystem();

void playWarning(uint16_t arg16);
void startWarnDelay(uint16_t arg16);
void startWarning(uint16_t arg16);
void endWarning(uint16_t arg16);



///////////////////// GLOBAL to main.c FUNCTIONS ////////////////////////////////////////////////////////////////
void buildGray() {  // constants in fB_SYS_Define.h
	gRay[G_LEFT] = 	"L";
	gRay[G_RIGHT ] = "R";
	gRay[G_STAMP ] = "STAMP";
	gRay[G_DELETE ] = "DELETE";
	gRay[G_NOLOG ] = "NO LOG";
	gRay[G_INPUT ] = "INPUT";
	gRay[G_AMP ] = 	"AMP";
	gRay[G_STRIKE] = "----";
	gRay[G_TOGGLE ] = "TOGGLE";
	gRay[G_GATE ] = 	"GATE";
	gRay[G_LOGS ] = 	"LOGS";
	gRay[G_SHUTDOWN]= "SHUTDOWN";
	gRay[G_DELAYSHUT]= "DELAY SHUTDN";
	gRay[G_DELAYSW2]= "DELAY SWITCH";
	gRay[G_CHGALT ] = "ALT CHARGE";
	gRay[G_CHGEXT] = "EXT CHARGE";
	gRay[G_SWITCHTO] = "SWITCHING TO";
	gRay[G_MANUAL] = "MANUAL OVERRIDE";
	gRay[G_FAIL] =     "FAILURE";
	gRay[G_FAIL_RTC] = "FAIL BOOT RTC";
	gRay[G_FAIL_SD] = "FAIL BOOT SD";
	gRay[G_ALARM] =     "ALARM";
	gRay[G_INVERTER] = "INVERTER ON";
	gRay[G_BLANK ] = "";
}


/*
 void softReset() // Restarts program from beginning but does not reset the peripherals and registers
  {
	asm volatile ("  jmp 0");  
  }
*/
// free RAM check for debugging. SRAM for ATmega328p = 2048Kb.
//  1024 with ATmega168
void truncRound(double &x, double prec) {

	   // calculate rounding term d:   0.5/pow(10,places)  
   double d = 0.5;
   //int m,n;
   if (x < 0)   d *= -1.0;
   // divide by ten for each decimal place
   for (int i = 0; i < _PRECISION; i++)  d/= 10.0;    
   // this small addition, combined with truncation will round our values properly 
   //dbug("tRnd1 x: %f, xt: %f"),x,x+d); 
   x +=  d;
   x = (double) floor(x * pow(10,prec))/pow(10,prec);
   //dbug("tRnd2 x: %f"),x); 

}

void logData(uint16_t arg16) { 	rec.logData(arg16); }

bool existSD() {
	uint8_t res;		
	res = fat.initFAT(SPISPEED);
	if(res){
		dbug("SD ERROR 0x%x",res);
		Tag(_MOUNT)->iVal = 0;
		return false;
	}
	return true;
}

bool activeSD() {
	if(existSD() && Tag(_MOUNT)->iVal != 0) return true;
	return false;
}


void fBinterruptHandlerK1() {  // sets global flag, check and reset flag in user modules
	if(_fBiK1) return;
	//dbug("pTiK1shit ov:%d, val:%d"), _pTiK1shft->getOnVal(), _pTiK1shft->readInt()  );
	if(_pTiK1shft && (_pTiK1shft->readInt()== _pTiK1shft->getOnVal())) _fBiK1 = INTK1SHFT;
	else {
			//dbug("pTiK1coro1 ov:%d, val:%d"), _pTiK1coro->getOnVal(), _pTiK1coro->readInt()  );
		if(_pTiK1coro) {
			//dbug("pTiK1coro2 ov:%d, val:%d"), _pTiK1coro->getOnVal(), _pTiK1coro->readInt()  );
			if(_pTiK1coro->readInt() == _pTiK1coro->getOnVal()) _fBiK1 =  INTK1;
		}
		else _fBiK1 =  INTK1;
	}
	delayMicroseconds(100000);
	//dbug("FB_INTERRUPT msecs:%d   manOvr:%d"),msecs, _manOver);
}

void navigate() {   
	menu.buttonCode = tft.readButtons(); 
	_fBiTFT = 1;
}  

int freeRAM () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


void packtagTempRay(uint16_t tag) {
	int i;
	for( i=0; i < tagCount; i++ ) {
		if(tag == tagTempRay[i]) return;
		if(!tagTempRay[i]) break;
	}
	if(i<MAXTEMPTAG) {
		tagTempRay[i] = tag;
		tagCount++;
	}
}

logFile* LogFile(uint8_t fTag) {
	if(fTag != NULL) for(int i=0;i < logFileCount;i++) if(logFileRay[i].fTag == fTag) return &logFileRay[i];
	return NULL;
}

logTag* LogTag(uint8_t fTag) {
	if(fTag != NULL) for(int i=0;i < logTagCount;i++) if(logTagRay[i].fTag == fTag) return &logTagRay[i];
	return NULL;
}

fB_Tag* Tag(uint16_t tag) {
	if(tag != NULL) for(int i=1;i < tagCount;i++) if(tagRay[i].tag == tag) return &tagRay[i];
	return NULL;
}

fB_Card* Card(uint16_t tag) {	
	if(tag != NULL)for(int i=0;i < cardCount;i++) if(pCardRay[i]->tag == tag) return pCardRay[i];
	return NULL;
}

void initLog(uint16_t tag, uint8_t fTag, char* base) {
	fB_Tag *pT = NULL;
	if(!secondPass) {
		int i;
		logTagCount++;
		for(i=0;i < logFileCount;i++) if(logTempRay[i] == fTag) break;
		if(i == logFileCount) logTempRay[logFileCount++] = fTag;
	}
	else {
		pT = Tag(tag);
		if(pT) {
			logTagRay[logTagCount].fTag = fTag;
			logTagRay[logTagCount].tag = tag;
			logTagCount++;
			if(!LogFile(fTag)) {
				logFileRay[logFileCount].fTag = fTag;
				logFileRay[logFileCount].base = base;
				logFileCount++;
			}
		}
	}
}

fB_Tag* initTag(uint16_t tag,char* title,uint32_t flags,uint16_t tTag =NULL) {
	fB_Tag *pT = NULL;
	if(!secondPass) {
		packtagTempRay(tag); // add to tagTempRay if unique;
		//if(fTag && !(flags & _PAGE)) logTagCount++;
	}
	else {
		pT = Tag(tag);
		if(!pT)	{
			pT =  &tagRay[tagCount++]; 
			pT->pin = NULL;
			pT->alias = NULL;
			pT->tag = tag;
			pT->title = title;
			pT->tTag = tTag;  // 16bit 
		}
		if(!(pT->flag16 & _DUBL) && (flags & (_FLOAT1 | _FLOAT2 | _D2STR)))	{
			pT->dVal = new fB_Val;
			pT->flag16 |= _DUBL;
		}
		pT->putFlags(flags);
		pT->putFormat(flags);
		pT->putAction(flags);

//dbug("fBIT  %P ptag:%d, f:0X%x, flag:0x%x"),pT->title, pT->tTag,flags,pT->flag16);
	}
	return pT;
}

void initPage( uint16_t tag,char* title, uint16_t parentTag){  
	fB_Tag * pT;
	pT = initTag(tag,title,_PAGE,parentTag);  // for _PAGE, fTag holds parentTag 
	if(secondPass) { 
		rTP[rowCount].t = tag;
		pT->iVal = rowCount;		// index in tagRay of first row of Page
		curr.setCurrPage(tag);      // fTag = parentTag
		curr.rowCount = 1;
//dbug("fBIP  %P ptag:%d, flag:0x%x"),pT->title, pT->tTag,pT->flag16);
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

void initRow(uint16_t tag, char* title,uint32_t  flags){
	initTag(tag,title,flags,NULL);
	if(secondPass) 	{
		rTP[rowCount].t = tag;
		curr.incrRowCount(); // increment rowCount for this page, store in page flags
		curr.rowDex++;
	}	
	rowCount++ ;
}

void initRowList(uint16_t tag,char* title,uint16_t parentTag,uint32_t flags){
	initPage(tag,title,parentTag);
	for(int i=0; i<MAXLISTROWS; i++)  initRow(tag+i+1,NULL,flags);
}

void defineSpace() { 	// does not work!!!
	if(secondPass) 	{
		rTP[rowCount].t = NULL;
		curr.incrRowCount(); // increment rowCount for this page, store in page flags
	}
	rowCount++;
}

void initPin( uint16_t tag,char* title, uint16_t ctag,uint8_t   row,uint8_t   side,   uint8_t  dir, uint8_t  onval) {
	fB_Tag *pT;
	pT = initTag(tag,title,NULL,NULL);
	if(secondPass)	{
		pT->createPin(ctag,row,side,dir,onval) ;
		pT->flag16 |= ( _PIN | _UNDEF);
		if( pT->getMode() == _ANALOG) {
			if(!(pT->flag16 & _DUBL)) {
				pT->dVal = new fB_Val;
			}
			pT->flag16 |= _DUBL;
			pT->putFormat(_FLOAT2);
		}
		else pT->putFormat(_BLAMP);
	}
	pinCount++;
}

void  initCard(uint16_t tag,char* title, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan ) {
	if(secondPass)	pCardRay[cardCount] = new fB_Card(tag,title,type,i2cAddr,aChan ); // Card array is separate from Tag array
	cardCount++;
}

void defineCalibrate( uint16_t tag, pFunc _vFunc, double bias=1,double offset=NULL) {
	if(secondPass) {
		fB_Tag *pT;
		pT = Tag(tag);
		if(pT && !(pT->flag16 & _DUBL)) {
			pT->dVal = new fB_Val;
			pT->putFormat(_FLOAT2);
		}
		pT->dVal->vFunc = _vFunc;
		if(!bias || bias < 0.00001 ) bias=1;
		pT->dVal->bias = bias;
		pT->dVal->offset = offset;
	}
}
void defineValue( uint16_t tag, double value,double offset = NULL) {
	if(secondPass) {
		fB_Tag *pT;
		pT = Tag(tag);
		if(pT->flag16 & _DUBL) {
			pT->dVal->value = value;
			pT->dVal->offset = offset;
		}
		else {
			if(value > 0.99 && value < 1.0) value = 1.0;			
			pT->iVal = (int) value;
			if(pT->flag16 & _INCR) 	pT->buf16 = (uint16_t) offset;
		}
	}
}

void defineTarget(uint16_t tag,uint16_t tTag){
	if(secondPass) {
		fB_Tag *pT = Tag(tag);
		if(pT) pT->tTag = tTag;
	}
}

void initAlias(uint16_t tag, char* alias) {
	if(secondPass) {
		fB_Tag *pT;
		pT = Tag(tag);
		if(pT) pT->alias = alias;
	}
}


void flatBrainInit(){
	char Pbuffer[MAXCHARSLINE+1];	
	fB_Tag	bufTag;
	uint8_t res;

	//buildgRay();

	//alarm.enable();
	//alarm.play(_ALRMBT);

	dbug(" ");
	dbug("---------");
	dbug("FB INIT ENTRY");
	dbug("FREE RAM %d",freeRAM());

	Wire1.begin();
	//Wire1.setSpeed(I2CSPEED);
	//Wire1.timeOut(I2CTIMEOUT);

	tft.init(PORTRAIT);
	tft.clear();
	dbug("INIT TFT");

	menu.init();
	dbug("INIT MENU");

	//alarm.play(_ALRMWN);

	// turn off legacy SPI SS pin ( Mega D53) so it does not conflict w/ SD card or other SPI	
	// enable explicitly when you want to use (eg. ATTINY SPI uses legacy SS)
	//pinMode(AT_SPISS,_OUTPUT);  
    //digitalWrite(AT_SPISS,LOW);

	secondPass = 0;  // 1st pass , determine array sizes

	logTempRay = (uint8_t*) calloc( MAXTEMPLOG,1); // temp array for log list
	tagTempRay = (uint16_t*) calloc( MAXTEMPTAG,2); // temp array for packed tag list
	defineSystem();
	defineUser();
	free(tagTempRay);
	free(logTempRay);

	dbug("free RAM3 %d",freeRAM());
	tagRay =	(fB_Tag *) calloc(tagCount,sizeof(fB_Tag));			// array of Tag objects
	rTP    =	(PandT *) calloc(rowCount,sizeof(PandT));			// array of tags or pointers, for menu operations
	logTagRay =	(logTag *) calloc(logTagCount,sizeof(logTag));
	logFileRay =(logFile *) calloc(logFileCount,sizeof(logFile));
	pCardRay  =	(fB_Card **) calloc(cardCount,sizeof(fB_Card*));

	dbug("INIT PASS 1");
	dbug("tagCount %d",tagCount);
	dbug("logFileCount %d",logFileCount);
	dbug("logTagCount %d",logTagCount);
	dbug("pageCount %d",pageCount);
	dbug("rowCount %d",rowCount);
	dbug("cardCount %d",cardCount);
	dbug("pinCount %d",pinCount);
	dbug("tag size %d",sizeof(fB_Tag));

	dbug("INIT MALLOC");
	dbug("free RAM %d",freeRAM());


	// reset counters
	tagCount    = 0;	// unique tags
	pageCount = 0;	// unique pages ( incl in tagCount)
	rowCount  = 0;	// all rows tags, incl spaces, jumps, and pin duplicates
	pinCount  = 0;	// all pin tags, possible duplicates of row tags
	cardCount = 0;
	logFileCount  = 0;
	logTagCount  = 0;

	secondPass = 1;  // 2nd pass , execute
	defineSystem();
	defineUser();
	dbug("INIT PASS 2");

	for(int i=0; i< rowCount; i++) 	rTP[i].p = Tag(rTP[i].t); // might be obscure. ( see note at top of file ).

	if(rec.EEgetEAUTO()) {
		Tag(_EAUTO)->iVal = 1;
		rec.EEloadTags(BASEETAG);   
		dbug("INIT LOAD EEPROM");
	}
	else Tag(_EAUTO)->iVal = 0;

	if(Tag(_TALON)->iVal == LOW) {
		//alarm.disable();
		dbug("ALARM DISABLE");
	}
	
	if(res = rtc.init()) {
		dbug("RTC FAILED");
		_bootMsgIndex =G_FAIL_RTC;	
		//alarm.play(_ALRMFL);
	}
	else {
		//_sysStatus |= _RTC;
		dbug("INIT RTC");
	}

	if(!existSD()) {
		dbug("SD ERROR 0X%h",res);
		_bootMsgIndex =G_FAIL_SD;
		//alarm.play(_ALRMFL);
		Tag(_MOUNT)->iVal = 0;
	}
	else {
		//_sysStatus |= _SD;
		Tag(_MOUNT)->iVal = 1;
		dbug("INIT SD");
		for(int i=0;i<logFileCount;i++) rec.logCreate(logFileRay[i].fTag);
		dbug("INIT LOGS");
	}

	
	//set interrupt pins to high
	pinMode(NAV_INTPIN,INPUT_PULLUP);
	pinMode(WARN_INTPIN,INPUT_PULLUP);
	pinMode(K0_INTPIN,INPUT_PULLUP);
	pinMode(K1_INTPIN,INPUT_PULLUP);

	attachInterrupt(NAV_INT, navigate,FALLING);
	attachInterrupt(WARN_INT, fBinterruptHandlerK1 ,FALLING);
	_fBiK1 = 0;			// global interrupt flag
 	_fBiTFT = 0;		// global interrupt flag from TFT switches

	//VccRef = readVcc();

	dbug("free RAM %d",freeRAM());
	Tag(_FRAM)->iVal = freeRAM();
	//Tag(VCC)->dVal->value = VccRef;
	//warn.init();
	//alarm.playTag(_TALWN);
	dbug("INIT COMPLETE");	
}


void dbug(char* title, ... ){
  char fmt[ 60 ]; //Size array as needed.

  char prefix[ 61 ]; 
  char sbuffer[61] = { '\0' };
  int wid = 60;
  double f;
  char *s, *text;
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
			text =va_arg(args,char*);
			char pstr[MAXCHARSLINE+1];
			//getPtext(Ptext,pstr);
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
		else if(fmt[i] == 'u') { 
			n=va_arg(args,uint16_t);
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
