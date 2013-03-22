

#include "fB_Include.h"

////////////////// Functions global to all modules ///////////////////////////////////////////


///////////////////////// fB_Global Methods ////////////////////////////////////////////////////////////////

float fB_Global::getValue() {
	if(flags & GPIN)value = (float)Pin(gTag)->read();
	if(!(flags & GINPUT)) return value;
	if(flags & GBIAS) return ( value + factor) ;
	if(!factor) factor = 1;
	return (value * factor);
}
uint8_t  fB_Global::getFormat(float value) {
	float mod = 0;
	mod = value-int(value);
	if(mod==0) return INT5;
	if(mod*10-int(mod*10) == 0) return FLOAT1;
	return FLOAT2;
}
fB_Global::fB_Global(uint16_t _gTag,float _value, char *_tagStr, uint8_t  _format,uint16_t _fTag,uint8_t  _flags) {	
	
	gTag = _gTag; 
	value = _value;
	tagStr = _tagStr;
	if(!_format) format = getFormat(value);
	else format = _format;
	fTag = _fTag; 
	flags = _flags;
	if(flags & GBIAS) factor = 0;
	if(flags & GSYS) flags |= GINIT;
	else factor = 1;
	///if(fTag) flags |= LOG;
	}
///////////////////////// fB_Brain Methods ////////////////////////////////////////////////////////////////

void fB_Brain::init(uint8_t  i2cspeed ){
	
	uint8_t  res=0;
	fB_Global* pG;

	alarmEnabled = 1; // this should actually be toggled in fB_menu

	// turn off legacy SPI SS pin ( Mega D53) so it does not conflict w/ SD card or other SPI	
	// enable explicitly when you want to use (eg. ATTINY SPI uses legacy SS)
	pinMode(AT_SPISS,OUTPUT);  
    digitalWrite(AT_SPISS,LOW);

	status = 0;
	_i2cspeed = i2cspeed;
	i2c.begin();
	i2c.setSpeed(_i2cspeed);
	i2c.timeOut(I2CTIMEOUT);

	alarm.play(ALARM_1);


	res = rtc.init();
	if(res) {
		alarm.play(ALARM_2);
		dbug("RTC FAILED");
	}
	else {
		status |= RTC;
		dbug("BRAIN INIT RTC");
	}


	res = fat.initFAT(SD_SPISS ,SPISPEED);
	if(res) {
		dbug("SD ERROR 0X%h",res);
		alarm.play(ALARM_2);
	}
	else {
		status |= SD;
		dbug("BRAIN INIT SD");
	}


	initXmaps();
	totalGlobals = 0;
	totalGsys = 0;
	totalGusr = 0;
	totalLogs = 0;
	totalPins = 0;
	totalCards = 0;
	totalStacks = MAXLISTROWS * MAXSTACKPAGES;

dbug("BRAIN INIT PASS 1");

	passToggle =0;
	createCard(BRAIN,BRAIN,0,0);
	defineSystemGlobals();
	defineElements(); // 1st pass with passtog = 0, determine array sizes
dbug("BRAIN INIT MALLOC");
	pGlobal = (fB_Global **) malloc(sizeof(fB_Global*) * totalGlobals);
	gSys = (uint8_t  *) malloc(totalGsys);
	gUsr = (uint8_t  *) malloc(totalGusr);
	stack = (fB_Stack *) calloc(totalStacks,sizeof(fB_Stack));
	Logs =  (logStruc *) calloc(totalLogs,sizeof(logStruc));
	pPin = (fB_Pin **) malloc(sizeof(fB_Pin*) * totalPins);
	pCard = (fB_Card **) malloc(sizeof(fB_Card*) * totalCards);
	logCount = 0;
	globalCount = 0;
	gSysCount = 0;
	gUsrCount = 0;
	stackCount = 0;
	cardCount = 0;
	pinCount = 0;

	passToggle = 1;
dbug("BRAIN INIT PASS 2");

	createCard(BRAIN,BRAIN,0,0);
	defineSystemGlobals();
	
	defineElements(); // 2nd pass with passtog = 1, execute

dbug("BRAIN INIT EEPROM");

	pG = EEgetGlobal(Global(GBOO));
	res = (uint8_t ) pG->value;
	if(pG && res == HIGH) EEinitGlobals();     //initialize from eeprom globals defined with GINIT flag
	pG->value = (float) res;

	//seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;
	//seg.test();  // set segmented display address if necessary;
	//seg.displayDec(1434,2);

dbug("BRAIN INIT COMPLETE");


	// VDIV RESISTOR MAP TO CD4051 pin

	
	VDRmap[0] =		 5; /// NCON
	VDRmap[1] =		 0; /// no resistor
	VDRmap[2] = 	 2; 
	VDRmap[3] = 	 1; 
	VDRmap[4] = 	 4; 
	VDRmap[5] = 	 3; 
	VDRmap[6] = 	 6; 

	//set interrupt pins to high
	pinMode(NAV_INTPIN,INPUT_PULLUP);
	pinMode(K0_INTPIN,INPUT_PULLUP);
	pinMode(K1_INTPIN,INPUT_PULLUP);

}

uint8_t fB_Brain::getXmapIndex(uint8_t mapCode,uint8_t  row, uint8_t  side) {
	uint8_t index = 0;
	switch(mapCode) {
		case 0: 
			index = (row-12)*2 + side;
			break;
		case 2:
			index = (row-17)*2 + side;
			break;
		case 3:
			index = (row-17)*2 + side;
			break;
	}
	return index;
}


void fB_Brain::insertXmap(uint8_t mapCode,uint8_t  row, uint8_t  side, uint8_t  type, uint8_t  pn) {
	//uint8_t  index = (row-17)*2 + side;
	uint8_t  index;
	index = getXmapIndex(mapCode,row,side); 
	switch(mapCode) {
		case 0: 
			x0Map[index] = pn;  // should set bits only in the lowest 6 bits, pin 0-63
			bitWrite(x0Map[index],6,type); // overlay  bit for type
			break;
		case 2: 
			x2Map[index] = pn;  // should set bits only in the lowest 6 bits, pin 0-63
			bitWrite(x2Map[index],6,type); // overlay  bit for type
			break;
		case 3: 
			x3Map[index] = pn;  // should set bits only in the lowest 6 bits, pin 0-63
			bitWrite(x3Map[index],6,type); // overlay  bit for type
			break;
	}
}

void fB_Brain::defineSystemGlobals() {
	defineGlobal(GBOO,HIGH,"BOOT GLOBALS", TOGL,SYSTEM,GSYS);


}


void fB_Brain::createStack(char *text,float value) {	
	if(!passToggle) return;
	for(int i = stackCount;i>0;i--) {
		stack[i].text = stack[i-1].text;
		stack[i].value = stack[i-1].value;
	}
	stack[0].text = text;
	stack[0].value = value;
	stackCount = min(stackCount++,MAXLISTROWS * MAXSTACKPAGES);
}
void fB_Brain::defineGlobalPin(uint16_t gTag, uint16_t fTag,uint8_t  flags) {
	float value;
	fB_Pin* pPin;
	pPin = Pin(gTag);
	if(pPin) {
		value = (float) pPin->onVal;
		defineGlobal(gTag,value, pPin->tagStr, TOGL,fTag, flags |= GPIN);	
	}

}	

void fB_Brain::defineGlobal(uint16_t gTag,float value, char *tagStr, uint8_t  format,uint16_t fTag,uint8_t  flags) {	
	if(!passToggle)	{
		totalGlobals++;
		if(flags & GSYS) totalGsys++;
		else  totalGusr++;
	}

	else{
			pGlobal[globalCount] = new fB_Global(gTag,value, tagStr, format,fTag,flags);
			if(flags & GSYS) gSys[gSysCount++]=globalCount;
			else gUsr[gUsrCount++]=globalCount;
			globalCount++;
		}
}


// pin(TAG) function uses arrays pinTagMap and pPin to return pointer to fB_Pin object associated by TAG (eg. pin(LED1)->row)
char * fB_Brain::getLogName(uint16_t fTag) {	
	for(int i=0;i < logCount;i++) if(Logs[i].tag == fTag) return Logs[i].name;
	return NULL;
}

// initcard() creates instance of fB_card in pcard array and puts in tag in cardTagMap array for lookup by tag by fcard()
void fB_Brain::defineCard(uint16_t ctag,char *tagStr, uint8_t  cType,uint8_t  i2cAddr, uint8_t  aChan )  {
		if(!passToggle) {
		totalCards++;
		return;
	}
	pCard[cardCount++] = new fB_Card(ctag,tagStr,cardCount, cType,i2cAddr,aChan );
}

void fB_Brain::definePin(uint16_t pTag,char* tStr, uint16_t  cTag, uint8_t  row, uint8_t  side, uint8_t  iodir, uint8_t  ival) {
	if(!passToggle) {
		totalPins++;
		return;
	}
	uint8_t  mode;
	uint8_t  cpin;
	uint8_t  index;
	fB_Card *pCard = Card(cTag);


	switch(pCard->cType) {
		case BRAIN:
			if(row>69) return;
			cpin = row;
			mode = side;
			break;
		case X50:
			if(row>24 || row < 17) return;
			index = getXmapIndex(2,row,side);
			cpin = x2Map[index] & 0x3F; // read   chip pin address 0..63
			mode = bitRead(x2Map[index],6); // read   bit for type (D/A)
			break;
		case X76:
			if(row>32 || row < 17) return;
			index = getXmapIndex(3,row,side);
			cpin = x3Map[index] & 0x3F; // read   chip pin address 0..63
			mode = bitRead(x3Map[index],6); // read   bit for type (D/A)
			break;
	}
	pPin[pinCount++] = new fB_Pin(pTag,tStr,cTag,row,side,cpin,mode,iodir,ival);
}
fB_Card * fB_Brain::Card(uint16_t cTag) {
	for(int i=0;i < cardCount;i++) if(pCard[i]->cTag ==cTag) return pCard[i];
	return NULL;
	}


void fB_Brain::defineLog(uint16_t fTag, char *tagStr) {	
	if(!passToggle) {
		totalLogs++;
		return;
	}
	char buffer[13] = { '\0'};
	Logs[logCount].tag = fTag;
	sprintf(Logs[logCount].name,"%.8s.TXT",tagStr);
	fB_Log *pL;
	pL = new fB_Log(fTag,Logs[logCount].name);
	if((status & SD) && pL->create()) {  // using fB_Log object only long enough to create file, storing filename in logTags array.
		pL->writeHeader();
	}
	delete pL;    // file system directory will be created later
	logCount++;
}

void fB_Brain::createGdefLog() {
	fB_Log* gLog;
	fB_Global* pG;
	uint8_t  res;
	char vBuf[13];
	char buffer[100];
	buffer[0] = '\0';
	gLog = new fB_Log(GDEF,"GDEF.TXT");  // using fB_Log object only long enough to create file
	if(status & SD) { 
		res = gLog->create(); // returns zero if created
		if(res) {
			gLog->remove(); 
			res = gLog->create();
		}
		if(!res) { 
			if(fat.openFile(gLog->filename,FILEMODE_TEXT_WRITE)==NO_ERROR) { 
				sprintf(buffer,"GLOBAL,GTAG,LOG, VALUE,FACTOR, GPIN, GSYS, GINIT,GINP, GBIAS");
				fat.writeLn(buffer);
				for(int i=0;i < brain.totalGlobals;i++) {
					pG = brain.pGlobal[i];
					sprintf(buffer,"%s,%d,%s, %s,%s, %d,%d,%d,%d,%d",pG->tagStr,pG->gTag ,getLogName(pG->fTag),
						floatToStr(pG->value,3,vBuf),floatToStr(pG->factor,3,vBuf),
						1 && pG->flags & 0x01,
						1 && pG->flags & 0x02,
						1 && pG->flags & 0x04,
						1 && pG->flags & 0x08,
						1 && pG->flags & 0x10);
					fat.writeLn(buffer);
				}
				fat.closeFile();
				gLog->setDate();
			}
		}
	}
	delete gLog;
}
///////////////////////////////////////// fB_Log Methods //////////////////////////////////////////////////////

fB_Log::fB_Log(uint16_t	_fTag, char * _filename) {
	fTag = _fTag;
	sprintf(filename,"%s",_filename);
}


bool fB_Log::create(char *fname) {
	char *pF;
	uint8_t  res;
	if(fname) pF = fname; 
	else pF = filename; // if not given, use object filename
	res = fat.createFile(pF);
	if(!res) {
		setDate();
		return true;
	}
	return false;
}

void fB_Log::setDate() {
	rtc.now();
	fat.stampFile(filename,rtc.yOff+2000,rtc.m,rtc.d,rtc.hh,rtc.mm);
}

void fB_Log::writeHeader() {
	char buffer[brain.globalCount*10];
	buffer[0] = '\0';
	if(fat.openFile(filename,FILEMODE_TEXT_WRITE)==NO_ERROR) { 
		sprintf(buffer,"DATE,TIME");
		for(int i=0;i < brain.totalGlobals;i++) {
			//if(brain.pGlobal[i]->flags |= LOG && brain.pGlobal[i]->fTag == fTag) sprintf(buffer,"%s,%s",buffer,brain.pGlobal[i]->tagStr);
			if( brain.pGlobal[i]->fTag == fTag) sprintf(buffer,"%s,%s",buffer,brain.pGlobal[i]->tagStr);
		}
		fat.writeLn(buffer);
		fat.closeFile();
		setDate();
	}
}
void fB_Log::writeData() {
	if(!brain.globalCount  || !fTag) return;
	char buffer[20 + brain.totalGlobals*12];
	buffer[0] = '\0';
	rtc.stamp(buffer);
	char fbuffer[10];
	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);
	if(res!=NO_ERROR) {
		//dbug("LOG WD create %s",filename);
		if(!create()) return;
		writeHeader();
	}
		
	for(int i=0;i < brain.totalGlobals;i++) {
		if(brain.pGlobal[i]->fTag == fTag) 	sprintf(buffer,"%s,%s",buffer,floatToStr(brain.pGlobal[i]->getValue(), 3, fbuffer));
	}
	fat.writeLn(buffer);
	fat.closeFile();
	setDate();
}
bool fB_Log::archive() {
	if(!fTag) return false; // can't archive unless active .txt file
	char buf[2][14];
	char base[9];
	int i,j=1,k=0;
	if(fat.getFile(filename)==NO_ERROR){
		sprintf(base,"%s",fat.DE.basename);
		sprintf(buf[k],"%s.A%d",base,MAXAFILES);
		if(fat.getFile(buf[k])==NO_ERROR) fat.deleteFile(buf[k]);
		for(i=MAXAFILES-1 ;i>0;i--,j--,k--) {
			if(j <0) j=1;
			if(k <0) k=1;
			sprintf(buf[j],"%s.A%d",base,i);
			if(fat.getFile(buf[j])==NO_ERROR) fat.renameFile(buf[j],buf[k]);
		}
		if(fat.renameFile(filename,buf[++j])==NO_ERROR) remove();
		return true;
	}
	return false;
}
void fB_Log::getAttributes() {
	dateStr[0] = '\0';
	sizeStr[0] = '\0';
	if(fat.getFile(filename)==NO_ERROR){
		sprintf(dateStr,"%4.4d-%d-%d% 2.2d:%2.2d",
			(fat.DE.date>>9 & B01111111) + 0x7BC,(fat.DE.date>>5 & B00001111), (fat.DE.date & B00011111),
			fat.DE.time>>11, (fat.DE.time>>5) & B00111111);
		long size = fat.DE.fileSize;
		if(size >= 1000) sprintf(sizeStr,"%d KB",fat.DE.fileSize/1000);
		else sprintf(sizeStr,"%d BYTES",fat.DE.fileSize);
	}
}

void fB_Log::remove() {
	fat.deleteFile(filename);
}

void fB_Log::dump() {
   char buffer[MAXCHARSDUMP+1] = { NULL };
   if(fat.openFile(filename,FILEMODE_TEXT_READ)==NO_ERROR) {
	 //Serial.begin(SERIALSPEED);
     Serial.print("FILENAME: ");
     Serial.println(filename);
     while(fat.readLn(buffer,MAXCHARSDUMP))  Serial.println(buffer);
     fat.closeFile();	

   }
}
/////////////////////////////// EEPROM METHODS /////////////////////////////////////////

void fB_Brain::EEwriteGlobals() {
	if(!globalCount) return;
	uint16_t tAddr,vAddr;
	uint8_t  * data;
	int j=0;
	for(int i=0;i<globalCount;i++){
		if(!(pGlobal[i]->flags & GINIT)) continue;
		tAddr = j*32+BASEGLOBAL;
		ee.setBlock(tAddr,'\0',48); // leaves a zeroed 8 bits at end of glist to mark end
		ee.writeBlock(tAddr,(uint8_t *)pGlobal[i]->tagStr,strlen(pGlobal[i]->tagStr));
		data = (uint8_t *)&(pGlobal[i]->value);
		vAddr = tAddr+8;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pGlobal[i]->factor);
		vAddr = tAddr+12;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pGlobal[i]->flags);
		vAddr  = tAddr+16;
		ee.writeBlock(vAddr,data,1);
		j++;
	}
	ee.dump(0,128);

}
void fB_Brain::EEinitGlobals() {
	if(!globalCount) return;
	for(int k = 0;k<globalCount;k++) {		
		if(!(pGlobal[k]->flags & GINIT)) continue;
		EEgetGlobal(pGlobal[k]);
	}
}
fB_Global* fB_Brain::EEgetGlobal( fB_Global* pG) {
	uint8_t  tBuffer[9];
	uint8_t  vBuffer[4];
	uint8_t  fBuffer[4];
	uint8_t  gBuffer;
	uint16_t tAddr,vAddr,fAddr,gAddr;
	float *data;
	char *tStr;
	
	int j=0;
	for(int i=0;i<MAXGLOBALS;i++){
		tAddr = j++ *32 + BASEGLOBAL;
		vAddr = tAddr+8;
		fAddr = tAddr+12;
		gAddr = tAddr+16;
		ee.readBlock(tAddr,tBuffer,8);
		if(tBuffer[0]=='\0') break;
		tBuffer[8] = '\0';
		tStr = (char *) tBuffer;

		if(!strcmp(pG->tagStr,tStr)) {
			ee.readBlock(vAddr,vBuffer,4);
			data = (float*)vBuffer ;
			pG->value = *data + ROUNDOFF;
			ee.readBlock(fAddr,fBuffer,4);
			data = (float*)fBuffer;
			pG->factor = *data;
			pG->flags = ee.readByte(gAddr);
			dbug("EEREAD tag: %s  value: %f  fact: %f  flag: 0x%h",tBuffer,pG->value,pG->factor,pG->flags);
			return pG;
		}
		
	}
	return NULL;
}

//////////////////////////////// MISC BRAIN METHODS ////////////////////////////////////////
void fB_Brain::initXmaps() {
// in this block,  D coresponds to MCP pin, A to CD pin
	insertXmap(0,12,L,A,8);  // BO
	insertXmap(0,12,R,A,9);  // B1
	insertXmap(0,13,L,A,10);  // B2
	insertXmap(0,13,R,A,11);  // B3
	insertXmap(0,14,L,D,0);   // VD
	insertXmap(0,14,R,D,0);   // K0
	insertXmap(0,15,L,D,44);  // K1
	insertXmap(0,15,R,D,42);  // K2
	insertXmap(0,16,L,D,2);  // K3
	insertXmap(0,16,R,D,3);  // K4

	insertXmap(2,17,L,D,9);
	insertXmap(2,17,R,D,8);
	insertXmap(2,18,L,D,11);
	insertXmap(2,18,R,D,10);
	insertXmap(2,19,L,D,13);
	insertXmap(2,19,R,D,12);
	insertXmap(2,20,L,D,15);
	insertXmap(2,20,R,D,14);
	insertXmap(2,21,L,A,7);
	insertXmap(2,21,R,A,5);
	insertXmap(2,22,L,A,4);
	insertXmap(2,22,R,A,6);
	insertXmap(2,23,L,A,0);
	insertXmap(2,23,R,A,2);
	insertXmap(2,24,L,A,3);
	insertXmap(2,24,R,A,1);
	

	insertXmap(3,17,L,D,8);
	insertXmap(3,17,R,D,9);
	insertXmap(3,18,L,D,11);
	insertXmap(3,18,R,D,10);
	insertXmap(3,19,L,D,13);
	insertXmap(3,19,R,D,12);
	insertXmap(3,20,L,D,15);
	insertXmap(3,20,R,D,14);
	insertXmap(3,21,L,A,6);
	insertXmap(3,21,R,A,7);
	insertXmap(3,22,L,A,4);
	insertXmap(3,22,R,A,5);
	insertXmap(3,23,L,A,2);
	insertXmap(3,23,R,A,3);
	insertXmap(3,24,L,A,0);
	insertXmap(3,24,R,A,1);
	insertXmap(3,25,L,D,7);
	insertXmap(3,25,R,D,6);
	insertXmap(3,26,L,D,5);
	insertXmap(3,26,R,D,4);
	insertXmap(3,27,L,D,3);
	insertXmap(3,27,R,D,2);
	insertXmap(3,28,L,D,1);
	insertXmap(3,28,R,D,0);
	insertXmap(3,29,L,A,12);
	insertXmap(3,29,R,A,11);
	insertXmap(3,30,L,A,10);
	insertXmap(3,30,R,A,9);	
	insertXmap(3,31,L,A,13);  
	insertXmap(3,31,R,A,8);
	insertXmap(3,32,L,A,14);  
	insertXmap(3,32,R,A,15);

}

