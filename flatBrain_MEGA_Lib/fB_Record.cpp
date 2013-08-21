

#include "fB_Include.h"


void fB_Record::createTagDefLog() {
	fB_Log* tLog;
	fB_Tag* pT;
	uint8_t  res;
	char buffer[100];
	char Pbuffer[40];
	char title[MAXCHARSTEXT];
	buffer[0] = '\0';
	tLog = new fB_Log(NULL,P("TAGDEF.TXT"));  // using fB_Log object only long enough to create file
	if(bootStatus & SD) { 
		res = tLog->create(); // returns zero if created
		if(res) {
			tLog->remove(); 
			res = tLog->create();
		}
		if(!res) { 
			if(fat.openFile(tLog->filename,FILEMODE_TEXT_WRITE)==NO_ERROR) { 
				//sprintf(buffer,P("GLOBAL,GTAG,LOG, VALUE,FACTOR, GPIN, GSYS, GINIT,GINP, GBIAS"));
				fat.writeLn(P("NAME,TAG,LOG, VALUE,FACTOR, OFFSET, TPIN, TSYS, STOREE"));
				for(int i=0;i<tagCount;i++){
					pT = &tagRay[i];
					if(!pT) continue;
					getPtext(pT->Ptitle,title);
					char datastr[16];
					switch(pT->getFormat()){
						case BLAMP:
						case INT5:		sprintf(datastr,"%d",pT->iVal);break;
						case FLOAT1:	
						case FLOAT2:	doubleToStr(pT->dVal->value,3,datastr); break;	
						case TEXT:		sprintf(datastr,"%s",pT->ptext); break;
						case PTEXT:		getPtext(pT->Ptext, datastr);break;
						case STRIKE:	sprintf(datastr,P("----"));break;
						case BLANK:		datastr[0] = '\0' ;break;
					}

					sprintf(buffer,P("%s,%d,%s, %s,%s, %d,%d,%d,%d,%d"),title,pT->tag ,getLogName(pT->fTag),datastr	,doubleToStr(pT->dVal->factor,3,datastr),doubleToStr(pT->dVal->offset,3,datastr),
						1 && pT->flag16 & TSYS
						,1 && pT->flag16 & STOREE
	//					,1 && pT->flag16 & 0x04
	//					,1 && pT->flag16 & 0x08
	//					,1 && pT->flag16 & 0x10
						);
					fat.writeLn(buffer);
				}
				fat.closeFile();
				tLog->setDate();
			}
		}
	}
	delete tLog;
}
///////////////////////////////////////// fB_Log Methods //////////////////////////////////////////////////////

fB_Log::fB_Log(uint8_t	_fTag, char * _filename) {
	fTag = _fTag;
	sprintf(filename,"%s",_filename);
}


bool fB_Log::create(char *fname) {
	char *pF;
	uint8_t  res;
	dbug(F("LOG creat1 %s"),fname);

	if(fname) pF = fname; 
	else pF = filename; // if not given, use object filename
	dbug(F("LOG creat2 %s"),fname);
	res = fat.createFile(pF);
	dbug(F("LOG creat3 %s"),fname);
	if(!res) {
		setDate();
	dbug(F("LOG creat4 %s"),fname);
		return true;
	}
	return false;
}

void fB_Log::setDate() {
	rtc.now();
	fat.stampFile(filename,rtc.yOff+2000,rtc.m,rtc.d,rtc.hh,rtc.mm);
}

void fB_Log::writeHeader() {
	if(!logTagCount) return;

	fB_Tag * pT;
	char title[MAXCHARSTEXT+1];
	char Pbuffer[15];
	dbug(F("LOG WDH create %s"),filename);

	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE==NO_ERROR);
	dbug(F("LOG WDH2 create %s  tc:%d, ltc: %d,"),filename, tagCount,logTagCount);
	char buffer[(logTagCount+2) * MAXCHARSTEXT];
	buffer[0] = '\0';
	sprintf(buffer,P("DATE,TIME,%s"));
	dbug(F("LOG WDH3 create %s"),filename);
	if(res!=NO_ERROR) {
		//dbug("LOG WD create %s",filename);
	dbug(F("LOG WDH4 create %s"),filename);
		if(!create()) return;
		writeHeader();
	}
	dbug(F("LOG WDH5 create %s"),filename);
	for(int k = 0;k<tagCount;k++) {	
		pT = &tagRay[k];
		if(!pT) continue;
		getPtext(pT->Ptitle,title);
		if( pT->flag16 & LOG && pT->fTag == fTag) sprintf(buffer,"%s,%s",buffer,title);
	}
	fat.writeLn(buffer);
	fat.closeFile();
	dbug(F("LOG WDH6 create %s"),filename);
	setDate();
}
void fB_Log::writeData() {
	if(!logTagCount) return;

	fB_Tag * pT;
	char Pbuffer[6];
	char buffer[100];
	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);
	buffer[0] = '\0';

dbug(F("LOG WD create %s"),filename);

	rtc.stamp(buffer);
	if(res!=NO_ERROR) {
dbug(F("LOG2 WD create %s"),filename);
		if(!create()) return;
		writeHeader();
	}
dbug(F("LOG3 WD create %s"),filename);
/*
	for(int k = 0;k<tagCount;k++) {	
		pT = &tagRay[k];
		if(!pT) continue;
		if(pT->fTag == fTag){
			char datastr[16];
			switch(pT->getFormat()){
				case BLAMP:
				case INT5:		sprintf(datastr,"%d",pT->iVal);break;
				case FLOAT1:	
				case FLOAT2:	doubleToStr(pT->dVal->value,3,datastr); break;	
				case TEXT:		sprintf(datastr,"%s",pT->ptext);break;
				case PTEXT:		getPtext(pT->Ptext, datastr);break;
				case STRIKE:	sprintf(datastr,P("----"));break;
				case BLANK:		datastr[0] = '\0' ;break;
			}
			sprintf(buffer,P("%s,%s"),buffer,datastr);
		}
	}
	fat.writeLn(buffer);
	*/
	fat.closeFile();
	setDate();
}
bool fB_Log::archive() {
dbug(F("fbLog1 arch fTag: %s "),fTag);
	if(!fTag) return false; // can't archive unless active .txt file
	char buf[2][MAXCHARSTEXT+1];
	char base[9];
	int i,j=1,k=0;
dbug(F("fbLog2 arch fn: %s "),filename);
	if(fat.getFile(filename)==NO_ERROR){
dbug(F("fbLog3 arch fn: %s "),filename);
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
	char Pbuffer[25];
	dateStr[0] = '\0';
	sizeStr[0] = '\0';
	char year[5];
	if(fat.getFile(filename)==NO_ERROR){
		sprintf(year,P("%4.4d"),(fat.DE.date>>9 & B01111111) + 0x7BC);
		for(int i=0;i<4;i++) year[i] = year[i+1];  // shift year to left
		//year[0] = 'M';
		sprintf(dateStr,P("%3s.%d.%d% 2.2d:%2.2d"),year
			,(fat.DE.date>>5 & B00001111), (fat.DE.date & B00011111),
			fat.DE.time>>11, (fat.DE.time>>5) & B00111111);
		long size = fat.DE.fileSize;
		if(size >= 1000) sprintf(sizeStr,P("%d KB"),fat.DE.fileSize/1000);
		else sprintf(sizeStr,P("%d BYTES"),fat.DE.fileSize);
	}
}

void fB_Log::remove() {
	fat.deleteFile(filename);
}

void fB_Log::dump() {
   char buffer[MAXCHARSDUMP+1] = { NULL };
   if(fat.openFile(filename,FILEMODE_TEXT_READ)==NO_ERROR) {
	 //Serial.begin(SERIALSPEED);
     Serial.print(F("FILENAME: "));
     Serial.println(filename);
     while(fat.readLn(buffer,MAXCHARSDUMP))  Serial.println(buffer);
     fat.closeFile();	

   }
}
/////////////////////////////// EEPROM METHODS /////////////////////////////////////////

void fB_Record::EEwriteTags() {
	if(!tagCount) return;
	fB_Tag * pT;
	uint16_t tAddr,vAddr;
	uint8_t  * data;
	char title[MAXCHARSTEXT];
	int j=0;
	for(int i=0;i<tagCount;i++){
		pT = &tagRay[i];
		if(!pT) continue;
		if(!pT->flag16 & STOREE) continue;
		getPtext(pT->Ptitle,title);
		tAddr = j*32+BASEGLOBAL;
		ee.setBlock(tAddr,'\0',48); // leaves a zeroed 8 bits at end of glist to mark end
		ee.writeBlock(tAddr,(uint8_t *)title,strlen(title));
		data = (uint8_t *)&(pT->dVal->value);
		vAddr = tAddr+8;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pT->dVal->factor);
		vAddr = tAddr+12;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pT->flag16);
		vAddr  = tAddr+16;
		ee.writeBlock(vAddr,data,1);
		j++;
	}	

	ee.dump(0,128);

}
void fB_Record::EEinitTags() {
	fB_Tag * pT;
	for(int k = 0;k<tagCount;k++) {	
		pT = &tagRay[k];
		if(!pT) continue;
		if(!(pT->flag16 & STOREE)) continue;
		EEgetTag(pT->tag);
	}
}
fB_Tag* fB_Record::EEgetTag( uint16_t tag) {
	uint8_t  tBuffer[9];
	uint8_t  vBuffer[4];
	uint8_t  fBuffer[4];
	uint8_t  gBuffer;
	char title[MAXCHARSTEXT];
	uint16_t tAddr,vAddr,fAddr,gAddr;
	double *data;
	char *tStr;
	fB_Tag * pT;
	pT = Tag(tag);
	if(!pT) return NULL;

	getPtext(pT->Ptitle,title);
	int j=0;
	for(int i=0;i<MAXEETAGS;i++){
		tAddr = j++ *32 + BASEGLOBAL;
		vAddr = tAddr+8;
		fAddr = tAddr+12;
		gAddr = tAddr+16;
		ee.readBlock(tAddr,tBuffer,8);
		if(tBuffer[0]=='\0') break;
		tBuffer[8] = '\0';
		tStr = (char *) tBuffer;

		if(!strcmp(title,tStr)) {
			ee.readBlock(vAddr,vBuffer,4);
			data = (double*)vBuffer ;
			pT->dVal->value = *data + ROUNDOFF;
			ee.readBlock(fAddr,fBuffer,4);
			data = (double*)fBuffer;
			pT->dVal->factor = *data;
			pT->flag16 = ee.readByte(gAddr);
			dbug(F("EEREAD tag: %s  value: %f  fact: %f  flag: 0x%h"),tBuffer,pT->dVal->value,pT->dVal->factor,pT->flag16);
			return pT;
		}
		
	}
	return NULL;
}
char* getLogName(uint8_t fTag) {	
	for(int i=0;i < logFileCount;i++) if(logRay[i].tag == fTag) return logRay[i].name;
	return NULL;
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
