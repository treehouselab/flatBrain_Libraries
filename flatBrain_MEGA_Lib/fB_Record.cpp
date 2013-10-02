

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
	//dbug(F("R bfr entry  ext:%s, fc:%d"),ext,fileCount);
	sortRay = (uint16_t *) calloc(fileCount,2);
	fat.restartDir();
	//while ((fat.findNextFile()== NO_ERROR) && i<totalFiles){
	for( i = 0; fat.findNextExt(ext,offset) == NO_ERROR  && i < fileCount;i++) 	sortRay[i] = offset+i;
	///////////////////////// q sort indexes ////////////////////
	qsort(sortRay, fileCount,2, &compareFilename);
	
}

/*
void fB_Record::createTagDefLog() {
	fB_Tag* pT;
	uint8_t  res;
	char buffer[100];
	char Pbuffer[40];
	char title[MAXCHARSLINE+1];
	buffer[0] = '\0';
	if(!(_bootStatus & SD) || !rec.logCreate(P("TAGDEF"))) return;	
	if(fat.openFile(filename,FILEMODE_TEXT_WRITE)==NO_ERROR) { 
		//sprintf(buffer,P("GLOBAL,GTAG,LOG, VALUE,FACTOR, GPIN, GSYS, GINIT,GINP, GBIAS"));
		fat.writeLn(P("NAME,TAG,LOG, VALUE,FACTOR, OFFSET, TPIN, _LOADEE"));
		for(int i=0;i<tagCount;i++){
			pT = &tagRay[i];
			if(!pT || !(pT->flag16 & (_LOADEE | _PIN))) continue;
			getPtext(pT->Ptitle,title);
			char datastr[16];
			if(pT->flag16 & _UNDEF) 	sprintf(datastr,P("----"));
			else switch(pT->getFormat()){
				case _BLAMP:
				case _INT5:		sprintf(datastr,"%d",pT->iVal);break;
				case _FLOAT1:	
				case _FLOAT2:	menu.doubleToStr(pT->dVal->value,3,datastr); break;	
				case _TEXT:		sprintf(datastr,"%s",pT->ptext); break;
				case _PTEXT:		getPtext(pT->Ptext, datastr);break;
				case _BLANK:		datastr[0] = '\0' ;break;
			}

			sprintf(buffer,P("%s,%d,%s, %s,%s, %d,%d,%d,%d"),title,pT->tag ,base,datastr	,menu.doubleToStr(pT->dVal->factor,3,datastr),menu.doubleToStr(pT->dVal->offset,3,datastr),
				1 && pT->flag16 & _LOADEE
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
*/


char* fB_Record::fileFind(uint16_t index) {
	//dbug(F("R FFI entry  index:%d"),index);
	if(fat.findIndex(index) == NO_ERROR) {
		strcpy(filename, fat.DE.filename);
		strcpy(base, fat.DE.basename);
		setFtag(base);
		logGetAttributes();
		return filename;
	}
	return NULL;
}

bool fB_Record::fileFind(char *fname){
	if(fat.getFile(fname) == NO_ERROR) {
		strcpy(filename, fat.DE.filename);
		strcpy(base, fat.DE.basename);
		setFtag(base);
		logGetAttributes();
		return true;
	}
	return false;
}

void fB_Record::setFtag(char *base){
	char	buffer[MAXCHARSTEXT];
	logFile *pLog;
	fTag = NULL;
	for(int i=0;i < logFileCount;i++) {
		pLog = &logFileRay[i];
		getPtext(pLog->Pbase,buffer);
		if(!strcmp(buffer,base)) {
			fTag = pLog->fTag;
			break;
		}
	}
}

bool fB_Record::fileCreate(char *fname) {
	uint8_t  res;
	if(!fname) return false; 
	res = fat.createFile(fname);
	if(res == NO_ERROR) {
		//dbug(F("R FC created  fn:%s"),fat.DE.filename);
		strcpy(filename, fat.DE.filename);
		strcpy(base, fat.DE.basename);
		logSetDate();
		return true;
	}
	return false;
}

char* fB_Record::logGetFilename(uint16_t _fTag) {
	//dbug(F("R LGF entry  ftag:%d"),_fTag);
	if(!logTagCount) return NULL;
	logFile *pLog = LogFile(_fTag);
	if(!pLog) return NULL;
	fTag = _fTag;
	getPtext(pLog->Pbase,filename);
	strcat(filename,".LOG");
	return filename;
}

bool fB_Record::logCreate(uint16_t fTag) {
	//dbug(F("R LC entry  ftag:%d"),fTag);
	logGetFilename(fTag);
	if(fileCreate(filename)) {
		//dbug(F("R LC created  f:%s"),filename);
		logWriteHeader(); 
		logWriteData(); 
		logGetAttributes();
		return true;
	}
	return false;
}

void fB_Record::logSetDate() {
	rtc.now();
	fat.stampFile(filename,rtc.yOff+2000,rtc.m,rtc.d,rtc.hh,rtc.mm);
}

void fB_Record::logWriteHeader(uint16_t fTag) {
	if(!logGetFilename(fTag))return;
	logWriteHeader();
}


void fB_Record::logWriteHeader() {
	//dbug(F("R LWh entry  "));
	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);
	if(res!=NO_ERROR) return;
	logTag	*pL;
	fB_Tag * pT;
	char Pbuffer[15];
	char title[MAXCHARSLINE+1];
	int i,j = 0;
	for(i=0;i<logTagCount; i++) {
		if(logTagRay[i].fTag != fTag) continue;
		j++;
	}
	if(!j) {
		fat.closeFile();
		return;
	}
	char buffer[j * MAXCHARSLINE];
	buffer[0] = '\0';
	strcpy(buffer,P("DATE,TIME"));
	for(i=0;i<logTagCount; i++) {
		pL = &logTagRay[i];
		if(pL->fTag != fTag) continue;
		pT = Tag(pL->tag);
		if(!pT) continue;
		//if(strcmp(base,getPtext(LogFile(pT->fTag)->Pbase,Pbuffer)))continue;
		getPtext(pT->Ptitle,title);
		strcat(buffer,",");
		strcat(buffer,title);
	}
	fat.writeLn(buffer);
	fat.closeFile();
	logSetDate();
}
void fB_Record::logWriteData(uint16_t fTag) {
	if(!logGetFilename(fTag))return;
	logWriteData();
}
void fB_Record::logWriteData() {
	//dbug(F("REC WRITE LOG %s"),filename);

	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);
	if(res!=NO_ERROR) {
		//dbug(F("REC WRITE FAIL1 %s"),filename);
		if(fileCreate(filename)) {
			logWriteHeader();  
			logGetAttributes();
		}
		else {
			fat.closeFile();
			//dbug(F("REC WRITE FAIL2 %s"),filename);
			return;
		}
	}
	logTag *pL;
	fB_Tag *pT;
	int i,j = 0;
	char Pbuffer[15];
	char title[MAXCHARSLINE+1];
	char datastr[MAXCHARSLINE+1];
	for(i=0;i<logTagCount; i++) {
		if(logTagRay[i].fTag != fTag) continue;
		j++;
	}
	if(!j) {
			//dbug(F("REC WRITE FAIL3 ft:%d"),fTag);
			fat.closeFile();
			return;
	}
	char buffer[j* MAXCHARSLINE];
	buffer[0] = '\0';
	rtc.stamp(buffer);
	//dbug(F("R LWD j:%d  ltc:%d  ft:%d"),j,logTagCount,fTag);
	for( i=0;i<logTagCount; i++) {
		pL = &logTagRay[i];
		if(pL->fTag != fTag) continue;
		pT = Tag(pL->tag);
		if(!pT) continue;
		if(pT->flag16 & _UNDEF) 	strcpy(datastr,P("----"));
		else switch(pT->getFormat()){
			case _BLAMP:
			case _INT5:		sprintf(datastr,"%d",pT->iVal);	break;
			case _D2STR:		
			case _FLOAT1:	
			case _FLOAT2:	menu.doubleToStr(pT->dVal->value,_LOGDECIMALS,datastr); break;	
			case _TEXT:		sprintf(datastr,"%s",pT->ptext);break;
			case _PTEXT:	getPtext(pT->Ptext, datastr);break;
			case _BLANK:	datastr[0] = '\0' ;break;
		}
		strcat(buffer,",");
		strcat(buffer,datastr);
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
			if(fat.getFile(buf[j])==NO_ERROR) {
				fat.renameFile(buf[j],buf[k]);
				//dbug(F("REC ARCH RENAME %s to %s"),buf[j],buf[k]); 
			}
		}
		//dbug(F("REC ARCHx RENAME %s to %s"),filename,buf[++j]); 
		if(fat.renameFile(filename,buf[j])==NO_ERROR) {
			logRemove();
			return true;
		}
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
	if(fat.DE.fileSize >= 1000000) sprintf(sizeStr,P("%d.%d MB"),fat.DE.fileSize/1000000, (fat.DE.fileSize%1000)/10000);
	else if(fat.DE.fileSize >= 100000) sprintf(sizeStr,P("%d KB"),fat.DE.fileSize/1000);
	else if(fat.DE.fileSize >= 1000) sprintf(sizeStr,P("%d.%d KB"),fat.DE.fileSize/1000, (fat.DE.fileSize%1000)/100);
	else sprintf(sizeStr,P("%d BYTES"),fat.DE.fileSize);
}

void fB_Record::logRemove() {
		//dbug(F("R FILE remove %s  "),filename);
		fat.deleteFile(filename);
		EEclearLog(fTag); 
		logCreate(fTag);
	//fat.deleteFile(fat.DE.filename); //DO NOT DO THIS, THE CHAR POINTER CANNNOT POINT TO THE fat.DE. RECORD!
}
void fB_Record::logStamp() {
	//dbug(F("R LS df:%s, f:%s"),fat.DE.filename,filename);

	logWriteData();
	logGetAttributes();
}

void fB_Record::logDump() {
	int res;
    char buffer[MAXCHARSDUMP+2];

   res = fat.openFile(filename,FILEMODE_TEXT_READ);
	//dbug(F("R LDUMP df:%s, f:%s, s:%d r:0x%x"),fat.DE.filename,filename,fat.DE.fileSize,res);
   if(res ==NO_ERROR) {
		if(fat.DE.fileSize > 0) {
			 //Serial.begin(SERIALSPEED);
			 Serial.print(F("FILENAME: "));
			 Serial.println(filename);
			 while(fat.readLn(buffer,MAXCHARSDUMP))  Serial.println(buffer);
		 }
		 fat.closeFile();	
   }
  
}
/////////////////////////////// EEPROM METHODS /////////////////////////////////////////

void fB_Record::EEclearTags(uint16_t offTags, uint16_t base) {
	// offTags is the number of tags in the front of the section NOT to clear

		uint16_t count;
		for(int i=0;i<tagCount;i++)	if(tagRay[i].flag16 & _LOADEE) count++;
		ee.setBlock(base+(offTags*16),'\0',16 * (count +2));  // leave blank of 32 bytes for EOF
}

int fB_Record::EEgetEAUTO() {   // return value of EE autoload flag
	uint8_t  buffer[11];
	ee.readBlock(BASEETAG+10,buffer,2);
	return  *(int*)buffer;
}

void fB_Record::EEwriteEAUTO() {
	fB_Tag * pT;
	pT = Tag(EAUTO);
	uint8_t  * data;
	char title[MAXCHARSLINE+1];
	int j=0;
	getPtext(pT->Ptitle,title);
	title[10]= '\0';
	ee.writeBlock(BASEETAG ,(uint8_t *)title,strlen(title));
	data = (uint8_t *)&(pT->iVal);
	ee.writeBlock(BASEETAG +10,data,2);
}

void fB_Record::EEwriteTags(uint16_t base) {
	if(!tagCount) return;
	//dbug(F("******R EEwTags"));
	fB_Tag * pT;
	uint16_t addr;
	uint8_t  * data;
	char title[MAXCHARSLINE+1];
	int j=0;
	for(int i=0;i<tagCount;i++){
		pT = &tagRay[i];
		if(!pT || !(pT->flag16 & _LOADEE) || pT->tag == EAUTO) continue;
		getPtext(pT->Ptitle,title);
		title[10]= '\0';
		addr = j*16 + (base+16);
		ee.setBlock(addr,'\0',32); // leaves a zeroed 8 bits at end of glist to mark end
		ee.writeBlock(addr,(uint8_t *)title,strlen(title));
		if(!(pT->flag16 & _DUBL)){	
			data = (uint8_t *)&(pT->iVal);
			ee.writeBlock(addr+10,data,2);
		}
		else {
			data = (uint8_t *)&(pT->dVal->value);
			ee.writeBlock(addr+12,data,4);
		}
		j++;
	}	
	//ee.dump(0,128);
}



fB_Tag* fB_Record::EEgetTag(fB_Tag &bufTag, uint16_t tag,uint16_t base) {
	fB_Tag * pT;
	pT = Tag(tag);
	if(!pT || !(pT->flag16 & _LOADEE) ) return NULL;
	uint8_t  buffer[11];
	char title[MAXCHARSLINE+1];
	uint16_t addr;
	double  *data;
	getPtext(pT->Ptitle,title);
	for(int i=0;i<tagCount;i++){
		addr = i*16 + (base+16);
		ee.readBlock(addr,buffer,10);
		if(buffer[0]=='\0') break;
		buffer[10] = '\0';
		if(!strcmp(title,(char *) buffer)) {
			bufTag.Ptitle = pT->Ptitle;
			if(pT->flag16 & _DUBL) {
				ee.readBlock(addr+12,buffer,4);
				bufTag.dVal->value = *(double*)buffer;
					//dbug(F("R EEgT %P v:%f"),bufTag.Ptitle, bufTag.dVal->value);
			}
			else {
				ee.readBlock(addr+10,buffer,2);
					//dbug(F("#########R EEgT %P i:%f"),bufTag.Ptitle, bufTag.iVal);
				bufTag.iVal = *(int*)buffer;
			}
			return pT;
		}	

	}		
	return NULL;
}

void fB_Record::EEclearLog( uint16_t fTag) {
	if(!logTagCount) return;
	//dbug(F("******R EEclearLog"));
	fB_Tag * pT;
	logTag * pL;
	uint8_t  buffer[11];
	uint16_t addr;
	uint8_t  * data;
	char title[MAXCHARSLINE+1];
	for(int i=0;i<logTagCount; i++) {
		pL = &logTagRay[i];
		if(pL->fTag != fTag) continue;
		pT = Tag(pL->tag);
		if(!pT) continue;
		getPtext(pT->Ptitle,title);
		for(int j=0;j<tagCount;j++){
			addr = j*16 + (BASEELOG +16);
			ee.readBlock(addr,buffer,10);
			if(buffer[0]=='\0') break;
			buffer[10] = '\0';
			if(!strcmp(title,(char *) buffer)) {
				ee.writeBlock(addr+10,NULL,2);
				ee.writeBlock(addr+12,NULL,4);
			}
		}
	}
}


fB_Tag* fB_Record::EEloadTag(uint16_t tag,uint16_t base) {
	fB_Tag * pT;
	pT = Tag(tag);
	if(!pT || !(pT->flag16 & _LOADEE) ) return NULL;
	uint8_t  buffer[11];
	char title[MAXCHARSLINE+1];
	uint16_t addr;
	double  *data;
	getPtext(pT->Ptitle,title);
	for(int i=0;i<tagCount;i++){
		addr = i*16 + (base+16);
		ee.readBlock(addr,buffer,10);
		if(buffer[0]=='\0') break;
		buffer[10] = '\0';
		if(!strcmp(title,(char *) buffer)) {
			if(pT->flag16 & _DUBL) {
				ee.readBlock(addr+12,buffer,4);
				pT->dVal->value = *(double*)buffer;
			}
			else {
				ee.readBlock(addr+10,buffer,2);
				pT->iVal = *(int*)buffer;
			}
			return pT;
		}	

	}		
	return NULL;
}

void fB_Record::EEloadTags(uint16_t base) {
	fB_Tag * pT;
	for(int k = 0;k<tagCount;k++) {	
		pT = &tagRay[k];
		if(!pT) continue;
		if(pT->flag16 & _LOADEE) EEloadTag(pT->tag,base);
	}
}

void fB_Record::EEdumpTags(uint16_t base){
   uint16_t addr,count;
   count = 11;
   char *title;
   uint16_t *pFlag;
   int *pInt;
   double *pDub;
   uint8_t buffer[11];
   Serial.println();		

   for(int i=0;i< count;i++) {
		addr = base + i*16;
		if (addr % 16 == 0)	Serial.print(i);
		Serial.print(":\t");
	    ee.readBlock(addr, buffer, 10) ;  
		buffer[10] = '\0';
		title = (char *) buffer;
		Serial.print(title);
		Serial.print("\t");
		ee.readBlock(addr+10, buffer, 2) ;  
		pInt = (int *) buffer;
		Serial.print(*pInt,DEC);
		Serial.print("\t");
		ee.readBlock(addr+12, buffer, 4) ;  
		pDub = (double *) buffer;
		Serial.print(*pDub);
		Serial.println();		
   }
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
