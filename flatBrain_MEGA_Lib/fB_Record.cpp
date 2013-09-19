

#include "fB_Include.h"

//dbug(F("bfr fc: %d"), fCount);
static int compareFilename(const void *x1, const void *x2) { 
	char	fname1[MAXCHARSLINE+1],fname2[MAXCHARSLINE+1] ;
	fat.findIndex(*(uint16_t *)x1);
	strcpy(fname1,fat.DE.filename);
	fat.findIndex(*(uint16_t *)x2);
	strcpy(fname2,fat.DE.filename);
	return strcmp(fname1,fname2);
}


void fB_Record::buildFileRay(char *ext) {
	// will select all files if !ext
	uint16_t offset = 0,i;
	//if(fileCount) free(sortRay);
	fileCount = fat.fileCountExt(ext);
	sortRay = (uint16_t *) calloc(fileCount,2);
	fat.restartDir();
	//while ((fat.findNextFile()== NO_ERROR) && i<totalFiles){
	for( i = 0; fat.findNextExt(ext,offset) == NO_ERROR  && i < fileCount;i++) 	sortRay[i] = offset+i;
	///////////////////////// q sort indexes ////////////////////
	qsort(sortRay, fileCount,2, &compareFilename);
	
}


void fB_Record::createTagDefLog() {
	fB_Tag* pT;
	uint8_t  res;
	char buffer[100];
	char Pbuffer[40];
	char title[MAXCHARSLINE+1];
	buffer[0] = '\0';
	if(!(bootStatus & SD) || !rec.logCreate(P("TAGDEF"))) return;	
	if(fat.openFile(filename,FILEMODE_TEXT_WRITE)==NO_ERROR) { 
		//sprintf(buffer,P("GLOBAL,GTAG,LOG, VALUE,FACTOR, GPIN, GSYS, GINIT,GINP, GBIAS"));
		fat.writeLn(P("NAME,TAG,LOG, VALUE,FACTOR, OFFSET, TPIN, _TSYS, _STOREE"));
		for(int i=0;i<tagCount;i++){
			pT = &tagRay[i];
			if(!pT || !(pT->flag16 & (_TSYS | _LOG) && !pT->pin)) continue;
			getPtext(pT->Ptitle,title);
			char datastr[16];
			if(pT->flag16 & _UNDEF) 	sprintf(datastr,P("----"));
			else switch(pT->getFormat()){
				case BLAMP:
				case _INT5:		sprintf(datastr,"%d",pT->iVal);break;
				case _FLOAT1:	
				case _FLOAT2:	menu.doubleToStr(pT->dVal->value,3,datastr); break;	
				case _TEXT:		sprintf(datastr,"%s",pT->ptext); break;
				case _PTEXT:		getPtext(pT->Ptext, datastr);break;
				case _BLANK:		datastr[0] = '\0' ;break;
			}

			sprintf(buffer,P("%s,%d,%s, %s,%s, %d,%d,%d,%d,%d"),title,pT->tag ,base,datastr	,menu.doubleToStr(pT->dVal->factor,3,datastr),menu.doubleToStr(pT->dVal->offset,3,datastr),
				1 && pT->flag16 & _TSYS
				,1 && pT->flag16 & _STOREE
//					,1 && pT->flag16 & 0x04
//					,1 && pT->flag16 & 0x08
//					,1 && pT->flag16 & 0x10
				);
			fat.writeLn(buffer);
		}
		fat.closeFile();
		logSetDate();
	}
}


void fB_Record::logStamp() {
	logWriteData();
	logGetAttributes();
}

char* fB_Record::fileFind(uint16_t index) {
	if(fat.findIndex(index) == NO_ERROR) {
		strcpy(filename, fat.DE.filename);
		strcpy(base, fat.DE.basename);
		logGetAttributes();
		return filename;
	}
	return NULL;
}

bool fB_Record::fileFind(char *fname){
	if(fat.getFile(fname) == NO_ERROR) {
		strcpy(filename, fat.DE.filename);
		strcpy(base, fat.DE.basename);
		logGetAttributes();
		return true;
	}
	return false;
}

bool fB_Record::fileCreate(char *fname) {
	uint8_t  res;
	if(!fname) return false; 
	res = fat.createFile(fname);
	if(res == NO_ERROR) {
		strcpy(filename, fat.DE.filename);
		strcpy(base, fat.DE.basename);
		logSetDate();
		return true;
	}

	return false;
}
bool fB_Record::logCreate(char *base) {
	char buffer[MAXCHARSLINE+1];
	//dbug(F("R lc  entry  base:%s"),base);
	if(!base) return false; 
	sprintf(buffer,"%s.LOG",base);
	if(fileCreate(buffer)) {
		logWriteHeader();  
		logGetAttributes();
		return true;
	}
	return false;
}

void fB_Record::logSetDate() {
	rtc.now();
	fat.stampFile(filename,rtc.yOff+2000,rtc.m,rtc.d,rtc.hh,rtc.mm);
}

void fB_Record::logWriteHeader() {
	dbug(F("R LWh entry  "));
	if(!logTagCount) return;
	
	fB_Tag * pT;
	char title[MAXCHARSLINE+1];
	char Pbuffer[15];
	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);
	if(res!=NO_ERROR) return;
	char buffer[(logTagCount+2) * MAXCHARSLINE];
	buffer[0] = '\0';
	strcpy(buffer,P("DATE,TIME"));
	for(int k = 0;k<tagCount;k++) {	
		pT = &tagRay[k];
		if(!pT) continue;
		if(!(pT->flag16 & _LOG)) continue;
		if(strcmp(base,getPtext(Log(pT->fTag)->Pbase,Pbuffer)))continue;
		getPtext(pT->Ptitle,title);
		strcat(buffer,",");
		strcat(buffer,title);

	}

	fat.writeLn(buffer);
	fat.closeFile();
	logSetDate();
}
void fB_Record::logWriteData() {
	if(!logTagCount) return;
	fB_Tag * pT;
	char Pbuffer[6];
	char buffer[100];
	char datastr[MAXCHARSLINE+1];
	buffer[0] = '\0';
	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);

	if(res!=NO_ERROR) return;
	rtc.stamp(buffer);

	for(int k = 0;k<tagCount;k++) {	
		pT = &tagRay[k];
		if(!pT) continue;
		if(!(pT->flag16 & _LOG)) continue;
		if(strcmp( base, getPtext(Log(pT->fTag)->Pbase,Pbuffer) ) )continue;
dbug(F("R LWD  %P f:%d"),pT->Ptitle,pT->flag16);

		if(pT->flag16 & _UNDEF) 	strcpy(datastr,P("----"));
		else switch(pT->getFormat()){
			case BLAMP:
			case _INT5:		sprintf(datastr,"%d",pT->iVal);break;
			case _D2STR:		
			case _FLOAT1:	
			case _FLOAT2:	menu.doubleToStr(pT->dVal->value,3,datastr); break;	
			case _TEXT:		sprintf(datastr,"%s",pT->ptext);break;
			case _PTEXT:		getPtext(pT->Ptext, datastr);break;
			case _BLANK:		datastr[0] = '\0' ;break;
		}
		sprintf(buffer,P("%s,%s"),buffer,datastr);
	}
	fat.writeLn(buffer);
	fat.closeFile();
	logSetDate();
}
bool fB_Record::logArchive() {

	//if(strcmp(fat.DE.fileext,"LOG")) return false;
	char buf[2][MAXCHARSLINE+1];
	int i,j=1,k=0;
	if(fat.getFile(filename)==NO_ERROR){
		sprintf(buf[k],"%s.A%d",base,MAXAFILES);
		if(fat.getFile(buf[k])==NO_ERROR) fat.deleteFile(buf[k]);
		for(i=MAXAFILES-1 ;i>0;i--,j--,k--) {
			if(j <0) j=1;
			if(k <0) k=1;
			sprintf(buf[j],"%s.A%d",base,i);
			if(fat.getFile(buf[j])==NO_ERROR) fat.renameFile(buf[j],buf[k]);
		}
		//dbug(F("REC LARCH RENAME DEF:%s to %s"),filename,buf[++j]); 
		//if(fat.renameFile(filename,buf[j])==NO_ERROR) logRemove();
		if(fat.renameFile(fat.DE.filename,buf[++j])==NO_ERROR) logRemove();
		return true;
	}
	return false;
}
void fB_Record::logGetAttributes() {
	char Pbuffer[25];
	dateStr[0] = '\0';
	sizeStr[0] = '\0';
	char year[5];
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

void fB_Record::logRemove() {
		dbug(F("R FILE remove %s  "),filename);
		fat.deleteFile(filename);
	//fat.deleteFile(fat.DE.filename); //DO NOT DO THIS, THE CHAR POINTER CANNNOT POINT TO THE fat.DE. RECORD!
}

void fB_Record::logDump() {
   char buffer[MAXCHARSDUMP+1] = { NULL };
   if(fat.openFile(filename,FILEMODE_TEXT_READ)==NO_ERROR) {
	 if(fat.DE.fileSize > 0) {
		 Serial.begin(SERIALSPEED);
		 Serial.print(F("FILENAME: "));
		 Serial.println(filename);
		 while(fat.readLn(buffer,MAXCHARSDUMP))  Serial.println(buffer);
	 }
     fat.closeFile();	

   }
}
/////////////////////////////// EEPROM METHODS /////////////////////////////////////////

void fB_Record::EEwriteTags() {
	if(!tagCount) return;
	fB_Tag * pT;
	uint16_t tAddr,vAddr;
	uint8_t  * data;
	char title[MAXCHARSLINE+1];
	int j=0;
	for(int i=0;i<tagCount;i++){
		pT = &tagRay[i];
		if(!pT) continue;
		if(!pT->flag16 & _STOREE) continue;
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
		if(!(pT->flag16 & _STOREE)) continue;
		EEgetTag(pT->tag);
	}
}
fB_Tag* fB_Record::EEgetTag( uint16_t tag) {
	uint8_t  tBuffer[9];
	uint8_t  vBuffer[4];
	uint8_t  fBuffer[4];
	uint8_t  gBuffer;
	char title[MAXCHARSLINE+1];
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
/*
void fB_Record::listFiles(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
	uint8_t  len,maxlines, linecount=0;
	uint8_t res;
	char strBuffer[81];
	char Pbuffer[13];

	tft.currY = y1 +1;
	len = (int) ((x2-x1+1)/tft.cfont.x_size);
	maxlines = (int) ((y2-y1+1)/LINEHT);

	fat.restartDir();
	while(linecount < maxlines) {
		res = fat.findNextFile();
		if (res==NO_ERROR){
			sprintf(strBuffer,P("%4.4s.%3s %d"),fat.DE.basename,fat.DE.fileext,fat.DE.fileSize/1000);
			if(!linecount) tft.printLine(strBuffer,len);
			else tft.printNewLine(strBuffer,len);
			linecount++;
		}
		else {
			if(!linecount) tft.printNewLine(P("* NO FILE *"),len);
			return;
		}
	}
}
*/
