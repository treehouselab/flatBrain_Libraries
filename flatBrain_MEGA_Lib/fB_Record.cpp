

#include "fB_Include.h"




void fB_Record::createTagDefLog() {
	fB_Log* tLog;
	fB_Tag* pT;
	uint8_t  res;
	char vBuf[13];
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
				fat.writeLn(P("NAME,TAG,LOG, VALUE,FACTOR, TPIN, TSYS, TINIT"));
				for(int i=0;i<sysTagCount;i++){
					pT = pSysTagRay[i];
					if(!pT) continue;
					getPtext(pT->Ptitle,title);
					sprintf(buffer,P("%s,%d,%s, %s,%s, %d,%d,%d,%d,%d"),title,pT->tag ,getLogName(pT->fTag),
						floatToStr(pT->value,3,vBuf),floatToStr(pT->factor,3,vBuf),
						1 && pT->flags & 0x01
						,1 && pT->flags & 0x02
	//					,1 && pT->flags & 0x04
	//					,1 && pT->flags & 0x08
	//					,1 && pT->flags & 0x10
						);
					fat.writeLn(buffer);
				}
				for(int i=0;i<usrTagCount;i++){
					pT = pUsrTagRay[i];
					if(!pT) continue;
					getPtext(pT->Ptitle,title);
					sprintf(buffer,P("%s,%d,%s, %s,%s, %d,%d,%d,%d,%d"),title,pT->tag ,getLogName(pT->fTag),
						floatToStr(pT->value,3,vBuf),floatToStr(pT->factor,3,vBuf),
						1 && pT->flags & 0x01
						,1 && pT->flags & 0x02
	//					,1 && pT->flags & 0x04
	//					,1 && pT->flags & 0x08
	//					,1 && pT->flags & 0x10
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
	fB_Tag * pT;
	char title[MAXCHARSTEXT];
	char Pbuffer[15];
	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE==NO_ERROR);
	uint16_t tagCount;
	tagCount = max(usrTagCount,sysTagCount);
	char buffer[ tagCount*10];
	buffer[0] = '\0';
	sprintf(buffer,P("DATE,TIME,%s"));
	if(res!=NO_ERROR) {
		//dbug("LOG WD create %s",filename);
		if(!create()) return;
		writeHeader();
	}
	for(int k = 0;k<sysTagCount;k++) {	
		pT = pSysTagRay[k];
		if(!pT) continue;
		getPtext(pT->Ptitle,title);
		if( pT->fTag == fTag) sprintf(buffer,P("%s,%s"),buffer,title);
	}
	for(int k = 0;k<usrTagCount;k++) {	
		pT = pUsrTagRay[k];
		if(!pT) continue;
		getPtext(pT->Ptitle,title);
		if( pT->fTag == fTag) sprintf(buffer,P("%s,%s"),buffer,title);
	}
	fat.writeLn(buffer);
	fat.closeFile();
	setDate();
}
void fB_Log::writeData() {
	fB_Tag * pT;
	char fbuffer[10];
	char Pbuffer[6];

	uint8_t res =fat.openFile(filename,FILEMODE_TEXT_WRITE);
	uint16_t tagCount;
	tagCount = max(usrTagCount,sysTagCount);
	char buffer[20 + tagCount*12];
	buffer[0] = '\0';
	rtc.stamp(buffer);
	if(res!=NO_ERROR) {
		//dbug("LOG WD create %s",filename);
		if(!create()) return;
		writeHeader();
	}
	for(int k = 0;k<sysTagCount;k++) {	
		pT = pSysTagRay[k];
		if(!pT) continue;
		if(pT->fTag == fTag) 	sprintf(buffer,P("%s,%s"),buffer,floatToStr(pT->getValue(), 3, fbuffer));
	}
	for(int k = 0;k<usrTagCount;k++) {	
		pT = pUsrTagRay[k];
		if(!pT) continue;
		if(pT->fTag == fTag) 	sprintf(buffer,P("%s,%s"),buffer,floatToStr(pT->getValue(), 3, fbuffer));
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
	if(!(usrTagCount + sysTagCount)) return;
	fB_Tag * pT;
	uint16_t tAddr,vAddr;
	uint8_t  * data;
	char title[MAXCHARSTEXT];
	int j=0;
	for(int i=0;i<sysTagCount;i++){
		pT = pSysTagRay[i];
		if(!pT) continue;
		if(!pT->flags & TINIT) continue;
		getPtext(pT->Ptitle,title);
		tAddr = j*32+BASEGLOBAL;
		ee.setBlock(tAddr,'\0',48); // leaves a zeroed 8 bits at end of glist to mark end
		ee.writeBlock(tAddr,(uint8_t *)title,strlen(title));
		data = (uint8_t *)&(pT->value);
		vAddr = tAddr+8;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pT->factor);
		vAddr = tAddr+12;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pT->flags);
		vAddr  = tAddr+16;
		ee.writeBlock(vAddr,data,1);
		j++;
	}	
	for(int i=0;i<usrTagCount;i++){
		pT = pUsrTagRay[i];
		if(!pT) continue;
		if(!pT->flags & TINIT) continue;
		getPtext(pT->Ptitle,title);
		tAddr = j*32+BASEGLOBAL;
		ee.setBlock(tAddr,'\0',48); // leaves a zeroed 8 bits at end of glist to mark end
		ee.writeBlock(tAddr,(uint8_t *)title,strlen(title));
		data = (uint8_t *)&(pT->value);
		vAddr = tAddr+8;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pT->factor);
		vAddr = tAddr+12;
		ee.writeBlock(vAddr,data,4);
		data = (uint8_t *)&(pT->flags);
		vAddr  = tAddr+16;
		ee.writeBlock(vAddr,data,1);
		j++;
	}

	ee.dump(0,128);

}
void fB_Record::EEinitTags() {
	fB_Tag * pT;
	for(int k = 0;k<sysTagCount;k++) {	
		pT = pSysTagRay[k];
		if(!pT) continue;
		if(!(pT->flags & TINIT)) continue;
		EEgetTag(pT->tag);
	}
	for(int k = 0;k<usrTagCount;k++) {	
		pT = pUsrTagRay[k];
		if(!pT) continue;
		if(!(pT->flags & TINIT)) continue;
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
	float *data;
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
			data = (float*)vBuffer ;
			pT->value = *data + ROUNDOFF;
			ee.readBlock(fAddr,fBuffer,4);
			data = (float*)fBuffer;
			pT->factor = *data;
			pT->flags = ee.readByte(gAddr);
			dbug(F("EEREAD tag: %s  value: %f  fact: %f  flag: 0x%h"),tBuffer,pT->value,pT->factor,pT->flags);
			return pT;
		}
		
	}
	return NULL;
}
char* getLogName(uint16_t fTag) {	
	for(int i=0;i < logCount;i++) if(logRay[i].tag == fTag) return logRay[i].name;
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