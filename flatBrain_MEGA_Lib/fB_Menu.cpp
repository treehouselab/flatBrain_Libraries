#include "fB_Include.h"

int compareFilename(const void *x1, const void *x2) { 
	fB_Log *f1,*f2;
	f1 = menu.mFile[*(uint8_t *)x1];
	f2 = menu.mFile[*(uint8_t *)x2];
	if(f1->fTag !=NULL && f2->fTag == NULL) return -1;
	if(f1->fTag == NULL && f2->fTag !=NULL) return  1;
	return strcmp(f1->filename,f2->filename);
}

void fB_Menu::init(){

	totalFiles   = 0;
	fListStart = 0;
	tListStart = 0;
	buttonCode = 0;

	PstrCount = 0;
	#define P_LEFT 0
	createPstr("L");
	#define P_RIGHT 1
	createPstr("R");
	#define P_STAMP 2
	createPstr("STAMP");
	#define P_DELETE 3
	createPstr("DELETE");
	#define P_NOLOG 4
	createPstr("NO LOG");
	#define P_BIAS 5
	createPstr("BIAS");
	#define P_AMP 6
	createPstr("AMP");
	#define P_STRIKE 7
	createPstr("----");
	#define P_TOGGLE 8
	createPstr("TOGGLE");
	#define P_GATE 9
	createPstr("GATE");

}


void fB_Menu::defineSystem()  {

	definePage(SYSTEM,HOME);
		defineJump(CLOCK);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR, NULL,INCR | INT5);
		defineRow(CLKMO, NULL,INCR | INT5);
		defineRow(CLKDY, NULL,INCR | INT5);
		defineRow(CLKHH, NULL,INCR | INT5);
		defineRow(CLKMM, NULL,INCR | INT5);
		defineRow(CLKGET, NULL,BLANK | INCR);
		defineRow(CLKSET, NULL,BLANK | INCR);

/*
definePage(PINS,SYSTEM);
		defineRow(PNPIN,TEXT,NULL);
		defineRow(PNCRD,NOACT,NULL);
		defineRow(PNROW,INT5,NULL);
		defineRow(PNCOL,TEXT,NULL);
		//defineRow(PNRES,F("RESISTOR"),PNRES,INT5,NULL,NULL);
		defineRow(PNTOG,LAMP,NULL);
		defineRow(PNADC,INT5,NULL);
		//defineRow(PNSET,F("SET"),PNSET,TITLE,NULL,NULL);
		if(secondPass)Row(PNROW)->status |= MARK;
		if(secondPass)Row(PNCOL)->status |= MARK;


	
	definePage(GLOBALS,F("GLOBALS"),SYSTEM );
		defineRow(NULL,F("VAR LIST"),JPAGE,TITLE,TLIST,PUSR);
		defineRow(TLAU,F("VAR LOG"),TLAU,TITLE,NULL,NULL);
		defineRow(NULL,F("SYS LIST"),JPAGE,TITLE,TLIST,PSYS);
		defineRow(TLAS,F("SYS LOG"),TLAS,TITLE,NULL,NULL); 
		defineSpace();
		defineRow(TSAT,F("EEPROM STORE"),TSAT,TITLE,NULL,NULL);
		defineRow(TIAT,F("EEPROM READ"),TIAT,TITLE,NULL,NULL);
		defineJump(FILES);

	
		definePage(TLIST,F("TLIST"),GLOBALS);
			//for(int i=0;i<min(globalCount,MAXLISTROWS);i++) defineRow((uint8_t )TROW+i,NULL,(uint8_t )TROW,NULL,(uint16_t) TPANEL, (float) i);

			definePage(TPANEL,F("TPANEL"),TLIST);
				defineRow(TLOG,F("LOG"),NULL,NOACT,NULL,NULL);
				defineRow(TINP,F("INPUT"),NOACT,VALUE,NULL,NULL);
				defineRow(TVAL,F("VALUE"),TADJ,VALUE,NULL,NULL);
				defineRow(TOPR,NULL,TARB,TEXT,NULL,NULL); 
				defineRow(TFAC,F("FACTOR"),TADJ,VALUE,NULL,NULL);
				defineRow(TINC,F("INCR"),TINC,VALUE,NULL,NULL);
				defineRow(TSET,F("SET"),TSET,TITLE,NULL,NULL);
				if(secondPass)	Row(TLOG)->status |= MARK;
*/
	definePage(FILES,SYSTEM);
			for(int i=0;i<MAXLISTROWS;i++) defineRow(FROW+i,NULL,TEXT);
 //		defineRow(tag,format,flags,tTag)	

			definePage(FPANEL,FILES);
				defineRow(FDATE,NULL,TEXT | NOACT);
				defineRow(FSIZE,NULL,INT5 | NOACT);
				defineRow(FDUMP,NULL,BLANK);
				defineRow(FSTD,NULL,NULL);
				defineRow(FARCH,NULL,BLANK);
				if(secondPass)Tag(FDATE)->flag16 |= MARK;
				if(secondPass)Tag(FSIZE)->flag16 |= MARK;
					

}

//void fB_Menu::erase() {
//	pCurrPageTag->mp.pPage->hide();
//}

void fB_Menu::jumpPage(uint16_t tag) {
	fB_Tag *pT;
	pT = Tag(tag);
	if(!pT ||!( pT->flag16 & PAGE)) return;
	dbug(F("Jump from %P to tag:%d , title:%P"),curr.pP->Ptitle,pT->tag,pT->Ptitle);
	if(curr.pageTag) clearPage();
	menu.showPage(tag);

}
void fB_Menu::jumpPage(fB_Tag *pT) {
	if(!pT ||!( pT->flag16 & PAGE)) return;
	dbug(F("Jump from %P to tag:%d , title:%P"),curr.pP->Ptitle,pT->tag,pT->Ptitle);
	if(curr.pageTag) clearPage();
	menu.showPage(pT->tag);

}

void fB_Menu::checkButtonCode() {
	if(!buttonCode) return;

	switch(buttonCode) {
		case 1: context(RIGHT);break;
		case 2: context(LEFT);break;
		case 3: prevSwitch(); break;
		case 4: nextSwitch(); break;
		case 12: // dual buttons, see tft.readButtons()
		case 13:
		case 14:
		case 23:
		case 24:
		case 34:
			break;
	}
	buttonCode = 0;

}
void fB_Menu::context(uint8_t  hand) {
	int* pListStart;
	uint8_t  totalLines;

	if(!curr.rowDex) { // jump  page if row[0]
		switch(curr.pageTag) {
			case HOME:		return; break;
			case FILES: 	pListStart = &fListStart; totalLines = totalFiles; break;
			case TLIST: 	pListStart = &tListStart; totalLines = tagIndexCount;   break;
		}
		if(hand==RIGHT) {
			switch(curr.pageTag) {
				case FILES: 		
				case TLIST: 
					if(	totalLines >  *pListStart+MAXLISTROWS) {
						*pListStart += MAXLISTROWS; 
						jumpPage(curr.pageTag);
						return;
					}
			}
		}
		else {
			switch(curr.pageTag) {
				case FILES: 		
				case TLIST: 	
					if(	*pListStart >= MAXLISTROWS) {
						*pListStart -= MAXLISTROWS;	
						jumpPage(curr.pageTag); 
						return;
					}
			}				
		jumpPage(curr.parentTag);
		}
	}
	else curr.tag()->action(hand);
}

void fB_Menu:: nextSwitch() {
	int i;
	for(i=0;i < curr.rowCount;i++)	{
		if(curr.tag(i)->flag16 & DISABLE) continue;
		if((curr.tag(i)->getAction() != NOACT ) && i > curr.rowDex) {
			curr.tag()->deselectRow();
			curr.tag(i)->selectRow();
			curr.rowDex = i;
			break;
		}
	}
}
void fB_Menu:: prevSwitch() {
	int i;
	if(!curr.rowDex) return;
	for(i=curr.rowCount-1;i >=0 ;i--)	{
		if(curr.tag(i)->flag16 & DISABLE) continue;
		if((curr.tag(i)->getAction() != NOACT ) && i < curr.rowDex) {
			curr.tag()->deselectRow();
			curr.tag(i)->selectRow();
			curr.rowDex = i;
			break;
		}
	}
}

void fB_Menu:: selectHeader() {
	curr.tag()->deselectRow();
	curr.rowDex = 0;			
	curr.tag()->selectRow();
}

void fB_Menu:: clearPage(uint8_t  full) {
	curr.tag()->deselectRow();
	curr.rowDex = 0;
	if(!full) tft.clear(curr.farY);
	else tft.clear();
}



void fB_Menu::listFiles(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
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
void fB_Menu::getFileList() {
	int i,j,k;

	if(mFile) {
		for( i = 0; i < totalFiles; i++)  delete mFile[i];
		delete mFile;
		delete fSort;
	}
	totalFiles = fat.fileCount();
	mFile = (fB_Log **) malloc(sizeof(fB_Log*) * totalFiles);
	fat.restartDir();
	i=0;
	k=0;
	//while ((fat.findNextFile()== NO_ERROR) && i<totalFiles){
		/*
	while (!fat.findNextFile() && i<totalFiles) {
		mFile[i]= new fB_Log(NULL,fat.DE.filename); // non-active ( non-.txt) tags are NULL
		for(j=0;j<logCount && k<logCount;j++ ) {
			if(!strcmp(logRay[j].name,mFile[i]->filename)) {
				mFile[i]->fTag = logRay[j].tag;
				logRay[j].pLog = mFile[i];
				k++;
				break;
			}
		}
		i++;
	}
			*/

	///////////////////////// q sort indexes ////////////////////
	fSort = (uint8_t *) malloc (totalFiles);
	for(i=0;i<totalFiles;i++) fSort[i] = i;
	qsort(fSort, totalFiles, 1, compareFilename);
}


void fB_Menu:: showPage(uint16_t tag) {
	int dr;
	uint8_t  i,x;
	uint8_t  rows = 0;  //  calculated rows on this page (may be less than default pageRowCount)
	uint8_t  count,listStart;
	float dv;
	//fB_Tag *pT;
	curr.setCurrPage(tag);


	switch(tag) {
		case FILES:
			menu.getFileList();
			count = menu.totalFiles;
			listStart = menu.fListStart;
			break;
		case TLIST:
			count = tagIndexCount;
			listStart = menu.tListStart;
			break;
		case CLOCK:
			if(!(bootStatus & RTC)) return;
			rtc.now();
			Tag(CLKYR)->iVal = rtc.yOff+2000 ;
			Tag(CLKMO)->iVal = rtc.m ;
			Tag(CLKDY)->iVal = rtc.d ;
			Tag(CLKHH)->iVal = rtc.hh ;
			Tag(CLKMM)->iVal = rtc.mm ;
			break;
		case PINS:{
			if(cardCount<2) jumpPage(SYSTEM);//jumpPage(curr.parentTag);
			uint8_t  index;
			fB_Tag* pT;
			fB_Pin* pP;
			fB_Card* pC;
			for(index=0;index<tagIndexCount;index++) if(pTagRay[index]->pPin) break; // find first pin
			if(i==tagIndexCount) break;
			pT = pTagRay[index];
			pP = pT->pPin;
			pC = pP->pCard;
			pC = pTagRay[index]->pPin->pCard;
			Tag(PNPIN)->Ptext= pT->Ptitle;
			Tag(PNPIN)->iVal= index;
			Tag(PNCRD)->Ptext= pC->Ptitle;
			Tag(PNADC)->iVal=0;
			Tag(PNROW)->iVal = pP->getRowBus();
			if(pP->getSide() == L) Tag(PNCOL)->Ptext = PstrRay[P_LEFT];
			else Tag(PNCOL)->Ptext = PstrRay[P_RIGHT];

			if( pP->getMode() == D) {
				Tag(PNADC)->flag16 |= DISABLE;
				Tag(PNTOG)->putAction(TOGGLE);
				Tag(PNTOG)->Ptitle = PstrRay[P_TOGGLE];
				//Tag(PNTOG)->Ptitle=PstrRay[P_TOGGLE];
			}
			else {
				Tag(PNADC)->flag16  &= ~DISABLE;
				//Tag(PNTOG)->Ptitle = PstrRay[P_GATE];
				Tag(PNTOG)->Ptitle = PstrRay[P_GATE];
				Tag(PNTOG)->putAction(CGATE);
				pC->openCDchan(pP->getCpin());
				if(pP->getOnVal() == GATE) {
					Tag(GATE)->iVal= HIGH;
					Tag(PNADC)->iVal= pP->read();
				}
				else {
					Tag(PNADC)->iVal= 0;
					Tag(GATE)->iVal= LOW;
				}
			}		
			break;

	}

	switch(tag) {
		case FILES:
		case TLIST:
			if(count <= listStart)listStart = max(count - MAXLISTROWS,0);
			if(MAXLISTROWS < count - listStart ) rows = MAXLISTROWS;
			else rows = count - listStart;
			for( i=0;i<rows;i++) {
				switch(tag) {
					case FILES:
						x = menu.fSort[i+listStart];
						Tag(FROW+i)->text = menu.mFile[x]->filename;
						Tag(FROW+i)->fTag = x; // use fTag to store array index
						if(menu.mFile[x]->fTag != NULL) Tag(FROW+i)->flag16 |= MARK; // |=log
						else Tag(FROW+i)->flag16 &= ~MARK; 
						break;
					case TLIST: // temporary test case for TLIST
						pTagRay[tListZeroIndex + i] = pTagRay[listStart + i];
						break;
				}
			}
			curr.putRowCount(rows+1);
			/*
			dr = curr.rowCount - (rows+1); // for page tags, pagerow count is stored in iVal
			iVal = rows+1;
			if(dr > 0) farY -= dr * ROWHT;
			else farY = iVal * ROWHT;
			*/
			break;
		}

	}

	menu.selectHeader();
	Tag(HEADER)->Ptitle = curr.pP->Ptitle; 
	
	for(curr.rowDex=0; curr.rowDex < curr.rowCount; curr.rowDex++) 	curr.tag()->showRow();

}


 ///////////////////////////////////////// ROW METHODS //////////////////////////////////////////////



uint16_t fB_Tag::getY() {  return ( STARTY + (ROWHT) * curr.rowDex); }

void fB_Tag::showRow(uint8_t  hide) {  //when hide ==1, page is being updated only

	int i;
	char bufferTitle[MAXCHARSTEXT+1];
	char Pbuffer[6];
		
	if(Ptitle != NULL) getPtext(Ptitle,bufferTitle);
	else bufferTitle[0] = '\0';	

	if(!curr.rowDex) {
		char header[MAXCHARSTEXT+1];
		sprintf(header,"< %s >",bufferTitle);
		tft.print(CENTER,getY(),header,MAXCHARSTEXT);
		return;
	}

	char bufferText[MAXCHARSTEXT+1];
	if(text != NULL) strcpy(bufferText,text);  // use non-PROGMEM title (left side string) if avail
	else {
		if(Ptext != NULL) getPtext(Ptext,bufferText);
		else bufferText[0] = '\0';
	}
	if( hide != HIDE  ) 	tft.resetDefColors();
	if( (hide == HIDE)  || (flag16 & DISABLE)) 	tft.setAll2Bcolor();
	else if(flag16 & MARK)	tft.setColor(FCOLOR,HCOLOR);
	if( getAction() != REFRESH ) tft.print(STARTX +ROWTEXTX,getY(),bufferTitle,MAXCHARSTEXT);

//dbug(F("Show Row tag %d, ttag %d, tit: %s"),tag,tTag,bufferTitle);

	if(getFormat() & BINARY) {
		if(pPin) {
			iVal =  pPin->read();
			int x = STARTX +ROWSTATEX; int y = getY()+ROWSTATEY+2;
			tft.drawCircle(x,y,8);
			if(iVal == LOW) {
				tft.setAll2Bcolor();
				tft.fillCircle(x,y,6);
				tft.resetDefColors();
			}
			else  {
				tft.setColor(FCOLOR,GCOLOR);
				tft.fillCircle(x,y,6);
			}
		}
	}
	else {
		switch(getFormat()) {
			case INT5:
				if(flag16 & UNDEF) tft.print( RIGHT, getY(),getPstr(P_STRIKE,Pbuffer));
				else tft.printInt(STARTX +MONX,getY(),iVal,6,RIGHT);
				break;
			case FLOAT1:
				if(flag16 & UNDEF) tft.print( RIGHT, getY(),getPstr(P_STRIKE,Pbuffer));
				else tft.printFloat(STARTX +MONX,getY(),dVal->dVal,1,RIGHT);
				break;
			case FLOAT2:
				if(flag16 & UNDEF) tft.print( RIGHT, getY(), getPstr(P_STRIKE,Pbuffer));
				else tft.printFloat(STARTX +MONX,getY(),dVal->dVal,AR4_2,RIGHT);
				break;
				break;
			case TEXT:
				tft.print( RIGHT, getY(), bufferText);
				break;
			case PTEXT:
				( RIGHT, getY(), getPtext(Ptext,Pbuffer));
				break;
		}
	}
	tft.resetDefColors();
	flag16 |= VISIBLE;
}

void fB_Tag::action(uint8_t  hand) {
	if(flag16 & DISABLE ) return;
	char Pbuffer[7];
	fB_Pin *pP;
	fB_Tag *pT;
	pT = curr.tag();
	pP = pT->pPin;
	fB_Log *pF = NULL;   //  ptr to  logfile
	uint16_t  index;
	if(curr.pageTag == FPANEL)	pF = menu.mFile[pT->fTag];//   value = index of the selected logfile
	//dbug(F("ACTION flag %d , tTag %d,  type %d"),flag,tTag,type);
	switch (hand)	{
		case RIGHT:
			//check page jump first 
			if(pT->flag16 & PAGE) {	menu.jumpPage(pT->tag); return; }
			// then check tag
			switch(pT->tag) {
				case PNPIN: {
					index = Tag(PNPIN)->iVal;
					if(index++ >= tagIndexCount) index=0;
					pT = pTagRay[index];
					pP = pT->pPin;
					//pP->pCard->AnalogGate(OFF);
					Tag(PNTOG)->iVal= LOW;
					Tag(PNCRD)->Ptext= pP->pCard->Ptitle;
					Tag(PNPIN)->Ptext=pT->Ptitle;
					Tag(PNPIN)->iVal= index;
					Tag(PNROW)->iVal = pP->getRowBus();
					if(pP->getSide() == L) Tag(PNCOL)->Ptext = PstrRay[P_LEFT];
					else Tag(PNCOL)->Ptext = PstrRay[P_RIGHT];

					if( pP->getMode() == D) {
						Tag(PNADC)->flag16 |= DISABLE;
						Tag(PNTOG)->putAction(TOGGLE);
						pT->Ptitle=PstrRay[P_TOGGLE];
					}
					else {
						Tag(PNADC)->flag16  &= ~DISABLE;
						pT->Ptitle = PstrRay[P_GATE];
						Tag(PNTOG)->putAction(CGATE);
						pP->pCard->openCDchan(pP->getCpin());
						if(pP->getOnVal() == GATE) {
							Tag(PNTOG)->iVal= HIGH;
							Tag(PNADC)->iVal= pP->read();
						}
						else {
							Tag(PNADC)->iVal= 0;
							Tag(PNTOG)->iVal= LOW;
						}
					}
					showRow();
					Tag(PNROW)->showRow();
					Tag(PNCOL)->showRow();
					Tag(PNTOG)->showRow();
					Tag(PNADC)->showRow();
					return;
				case PNADC:
					pP = pTagRay[ Tag(PNPIN)->iVal]->pPin;
					Tag(PNADC)->iVal = pP->read();
					showRow();
					return;
				case FROW: {
					Tag(HEADER)->text = pF->filename;
					pF->getAttributes();
					//Tag(FDATE)->text = pF->dateStr;
					//Tag(FSIZE)->text = pF->sizeStr;
					Tag(FDATE)->text = dateStr;
					Tag(FSIZE)->text = sizeStr;
					Tag(FDUMP)->iVal = pT->iVal;
					Tag(FSTD)->iVal = pT->iVal;
					Tag(FARCH)->iVal = pT->iVal;
					//if(flag16 & LOG) {
						Tag(FARCH)->flag16 |= VISIBLE;  // file is logfile
						Tag(FSTD)->Ptitle = PstrRay[P_STAMP];  
						Tag(FSTD)->tag = FSTAMP;  
					//}
					//else {
					//	Tag(FARCH)->status &= ~VISIBLE;  // file is other file
					//	Tag(FSTD)->Ptitle = PstrRay[P_DELETE];  
					//	Tag(FSTD)->type = FDEL;  
					//}
					menu.jumpPage(FPANEL);
					return;
				case FSTAMP:
					pF->writeData();
					pF->getAttributes();
					menu.selectHeader();
					//Tag(FDATE)->text = pF->dateStr; // new atts
					//Tag(FSIZE)->text = pF->sizeStr;
					Tag(FDATE)->text = dateStr; // new atts
					Tag(FSIZE)->text = sizeStr;
					Tag(FDATE)->showRow();
					Tag(FSIZE)->showRow();
					return;
				case FARCH:
					if(pF->archive()){
						pF->getAttributes();
						menu.selectHeader();
						//Tag(FDATE)->text = pF->dateStr; // new atts
						//Tag(FSIZE)->text = pF->sizeStr;
						Tag(FDATE)->text = dateStr; // new atts
						Tag(FSIZE)->text = sizeStr;
						Tag(FDATE)->showRow();
						Tag(FSIZE)->showRow();
					}
					return;
				case FDEL:
					pF->remove();
					menu.jumpPage(curr.parentTag);					
					return;
				case FDUMP:
					pF->dump();
					menu.selectHeader();
					return;
				case CLKSET:
					//dbug(F("clkset"));
					rtc.yOff = (uint8_t ) (Tag(CLKYR)->iVal - 2000);
					rtc.m = (uint8_t ) Tag(CLKMO)->iVal ;
					rtc.d = (uint8_t ) Tag(CLKDY)->iVal ;
					rtc.hh = (uint8_t ) Tag(CLKHH)->iVal ;
					rtc.mm = (uint8_t ) Tag(CLKMM)->iVal ;
					rtc.adjust();
					menu.showPage();
					break;
				case CLKGET:
					rtc.set(__DATE__,__TIME__);
					menu.showPage();
					break;
				case CLKYR: 
					pT->iVal += 1;
					showRow();
					return;
				case CLKMO: 
					if(pT->iVal < 12) pT->iVal += 1;
					else pT->iVal = 1;
					showRow();
					return;
				case CLKDY: 
					if(pT->iVal < 31) pT->iVal += 1;
					else pT->iVal = 1;
					showRow();
					return;
				case CLKHH: 
					if(pT->iVal < 23) pT->iVal += 1;
					else pT->iVal = 1;
					showRow();
					return;
				case CLKMM: 
					if(pT->iVal < 59) pT->iVal += 1;
					else pT->iVal = 1;
					showRow();
					return;

				//case TLAS:	createGdefLog() ; pPage->selectHeader();break;
				case TIAT:	record.EEinitTags(); menu.selectHeader();return;
				case TSAT:	record.EEwriteTags(); menu.selectHeader();return;
				case TLAU:	
							for(int i = 0;i<logCount;i++) logRay[i].pLog->writeData();
							menu.selectHeader();
							return;

			} // END SWITCH on Tag

			switch(getAction()) {
				case CGATE:
					pP = pTagRay[Tag(PNPIN)->iVal]->pPin;
					if(pT->iVal == LOW) pT->	iVal = HIGH;
					else pT->iVal = LOW;
					pP->pCard->AnalogGate(pT->iVal);
					if(pT->iVal = HIGH) Tag(PNADC)->iVal = pP->read();
					else Tag(PNADC)->iVal = 0;
					showRow();
					Tag(PNADC)->showRow();
					return;
				case TOGGLE:
					if(pT->iVal==LOW) { 
						pT->iVal = HIGH;
						if(pP) pP->write(pP->getOnVal());
					}
					else {
						pT->iVal=LOW; 
						pT->iVal = LOW;
						if(pP) pP->write(~(pP->getOnVal()));
					}
					showRow();
					return;
				case PULSE:
					if(pP)  pP->pulse(PULSEMSECS);
					showRow();
					return;
				case SHFTPULSE:
					if(pP) pP->YshiftPulse(PULSEMSECS);
					showRow();
					return;
				case INCR:
					pT->iVal++;
					showRow();
					return;
				case REFRESH:
					if(pP) pP->read();
					showRow();
					return;
			} // END SWITCH on Action
			break;
		}// END SWITCH on Right
		case LEFT:
			break;
	}// END SWITCH ON HAND

	}}

/*				case TROW:
					if(!Tag(TLOG)->ptext)  Tag(TLOG)->ptext = getPstr(P_NOLOG,Pbuffer) ;
					Tag(TSET)->tTag = pT->tag;
					Tag(TFAC)->tTag = pT->tag;
					Tag(TFAC)->iVal = pT->factor;
					Tag(TFAC)->iVal = pT->factor;
					Tag(TVAL)->tTag = pT->tag;
					Tag(TVAL)->iVal = pT->getValue();
					Tag(TVAL)->format = pT->format;
					Tag(TINC)->format = FLOAT2;
					Tag(TINC)->iVal = 0.01;
					Tag(TINP)->iVal = pT->iVal;
					Tag(TOPR)->tTag = pT->tag;
					Tag(TINP)->status |= MARK;

					switch(pT->format) {
						case INT5:   Tag(TINC)->iVal = 1; break;
						case FLOAT1: Tag(TINC)->iVal = .1; break;
						case FLOAT2: 
						default:     Tag(TINC)->iVal = .01; break;
					}

						if(!(pT->rFlags & TINPUT))  {
							Tag(TINP)->format = STRIKE;
							Tag(TFAC)->format = STRIKE;
							Tag(TOPR)->format = STRIKE;
							Tag(TFAC)->type = NOACT;
							Tag(TOPR)->type = NOACT;
							Tag(TVAL)->type =  TADJ;
							Tag(TFAC)->status |= MARK;
							Tag(TOPR)->status |= MARK;
							Tag(TVAL)->status &= ~MARK;
						}
						else {
							Tag(TFAC)->status &= ~MARK;
							Tag(TOPR)->status &= ~MARK;
							Tag(TVAL)->status |= MARK;

							Tag(TINP)->format = pT->format;
							Tag(TOPR)->format = TEXT;
							Tag(TFAC)->format = pT->format;
							Tag(TFAC)->type = TADJ;
							Tag(TOPR)->type = TARB;
							Tag(TVAL)->type = NOACT;
								Tag(TOPR)->ptext =getPstr(P_AMP,Pbuffer);
								if(!pT->factor) pT->factor = 1;
								Tag(TINC)->format = FLOAT2;
 					    //}
						menu.jumpPage(TPANEL);

					}
					break;
				case TARB:	if (!strcmp(ptext,menu.biasStr)) {
								ptext = menu.ampStr;
								Tag(TFAC)->iVal = 1;
							}
							else {
								ptext = menu.biasStr;
								Tag(TFAC)->iVal = 0;
							}
							show();
							Tag(TFAC)->show();
							break;
						
				case TINC:	value*=10;
							if(value> 1000) value=1000;
							show();
							break;
				case TADJ:
							value += Tag(TINC)->iVal ;
							show();
							break;
				case TSET:	
												if(pT->rFlags & TINPUT)  {
								if(!strcmp(Tag(TOPR)->ptext,menu.biasStr))  {
									Tag(TVAL)->iVal = Tag(TINP)->iVal + Tag(TFAC)->iVal + ROUNDOFF;
									pT->rFlags |= TBIAS;
								}
								else  {
									Tag(TVAL)->iVal = Tag(TINP)->iVal * Tag(TFAC)->iVal  + ROUNDOFF;
									pT->flags &= ~TBIAS;
								}
								pT->factor = Tag(TFAC)->iVal;
							}
							else
				
							
							pT->iVal = Tag(TVAL)->iVal;
							pPage->selectHeader();
							Tag(TVAL)->show();
							break;
				//case SET:
				//			pT->iVal = Tag((uint16_t)value)->iVal;
				//			pPage->selectHeader();
				//			break;

		

			}


		case LEFT: 
			switch(type) {

				case TOGGLE:
					pT->iVal=LOW; 
					value = LOW;
					if(pP) pP->write(~(pP->getOnVal()));
					show();
					break;
				case TADJ:
					value -= Tag(TINC)->iVal ;
					show();
					break;
				case CLK:
					if(value > 0 || tag == CLKYR) value -= 1;
					else {
						switch(tag) {
							case CLKMO:  value = 12; break;
							case CLKDY:  value = 31; break;
							case CLKHH:  value = 23; break;
							case CLKMM:  value = 59; break;
						}
					}
					show();
					break;
				case TINC:
						value/=10;
						switch (format) {
							case INT5:
								if(value<1) value=1;
								break;
							case FLOAT1:
								if(value < 0.1) value=0.1;
								break;
							case FLOAT2:
								if(value < 0.01) value=0.01;
								break;
						}
						show();
						break;

			}
			break;
	}
}

*/
void fB_Tag::hideRow() {
	flag16 &= ~VISIBLE;
	//tft.setAll2Bcolor();
	showRow(HIDE);
	//tft.resetDefColors();
	flag16 &= ~VISIBLE;
}

void fB_Tag::selectRow() {
	tft.setColor(FCOLOR,GCOLOR);
	tft.drawHLine(STARTX,getY()+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	//status |= SELECTED;
}

void fB_Tag::deselectRow() {
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,getY()+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	//status &= ~SELECTED;

}

void fB_Tag::frame() {

	tft.drawRect(STARTX,getY() ,STARTX + MAXPIXELWID,getY() + ROWHT);
}

void fB_Tag::unframe() {
	tft.setAll2Bcolor();
	tft.drawRect(STARTX,getY() ,STARTX + MAXPIXELWID,getY() + ROWHT);
	tft.resetDefColors();
}
void fB_Tag::clearRow() {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,getY()+1,STARTX +MAXPIXELWID-1,getY()+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	flag16 &= ~VISIBLE;


}
void fB_Tag::clearRow1() {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,getY()+1,STARTX +GTITLEX-10,getY()+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	flag16 &= ~VISIBLE;


}

/*
////////////////////////////// WINDOW METHODS /////////////////////////////////////////////////////////

void fB_Window::gaugeParseTitle(char *ptext) {
	int i;
	text = text;
	int len = strlen(text);
	if(type == GAUGE) {
		gTitle1 =  text;
		for(i=0; i< len && text[i] != '^'; i++);
		if(++i < len) {
			gTitle2 =  &text[i];
			if(strlen(gTitle2) > GMAXCHARSTITLE) gTitle2[GMAXCHARSTITLE] = '\0';
		}
		if(strlen(gTitle1) > GMAXCHARSTITLE) gTitle1[GMAXCHARSTITLE] = '\0';
	}
}

void fB_Window::gaugeShowTitle() {

	if(gTitle1 != NULL) tft.print(STARTX +GTITLEX,Y+GTITLEY1,gTitle1,GMAXCHARSTITLE);
	if(gTitle2 != NULL) tft.print(STARTX +GTITLEX,Y+GTITLEY2,gTitle2,GMAXCHARSTITLE);
}
void fB_Window:: show(uint8_t  flag) {
//stack("wshow");

	if(!(status & VISIBLE)) frame();
	switch (type) {
		case GAUGE:
			if(flag)clearBlock1();
			else clearBlock();

			tft.setFont(SegFont);
			if(status == NOSTATUS) for(int i=0, dx=30;i<4;i++,dx+=30) tft.printSpecChar( STARTX + dx, Y+ tft.cfont.y_size/2 +5,DASH);
			else {
				switch(type) {
					case FLOAT1: tft.printFloat(STARTX +GSTARTX,Y+GSTARTY,value,1); break;
					case FLOAT2: tft.printFloat(STARTX +GSTARTX,Y+GSTARTY,value,AR4_2); break;
					case INT5  : tft.printInt(STARTX +GSTARTX,Y+GSTARTY,(long)value,6); break;
				}
			}
			tft.setFont(BigFont);
			if(!flag) gaugeShowTitle();
			break;
		case WINDOW:
			switch (type){
				case SDDIR:
					menu.listFiles(STARTX +1,Y+1,MAXPIXELWID-1,MAXPIXELHT-1);
					break;
			}
			break;
	
	}
	status |= VISIBLE;

}


void fB_Window:: hide() {
	clearBlock();
	unframe();
	status &= ~VISIBLE;
}
*/
/*
void fB_Menu::defineGauge(uint16_t mTag,uint8_t  type, char *text, uint16_t tTag) {
	defineWindow(mTag,GAUGEHT,GAUGE, type,text,tTag) ;
}

void fB_Menu::defineWindow(uint16_t  mTag, uint16_t ROWHT,uint8_t   type,uint8_t  type,char *text, uint16_t tTag) { // general case
	
	if(!secondPass) {
		//totalMtags++;
		return;
	}
	//Serial.print(" wsize ");
	//Serial.println(sizeof(fB_Window),DEC);
	fB_Page *p;
	p = &mPage[pageCount-1]; // get last initialized page in array
	
	fB_Window *w;

	w = &(p->win);
	//w->X = p->X;
	w->Y = MAXPIXELHT - ROWHT ;
	w->ROWHT = ROWHT ;

	//p->pWindow = w;
	p->farY = 0;
	w->pDex = pageCount-1;
	//w->type = type;	
	w->type = type;	
	w->tTag = tTag;
	w->text = text;
	w->gTitle1 = NULL;
	w->gTitle2 = NULL;
	if(type == GAUGE) w->gaugeParseTitle(text);
	w->status |= CREATED;
	//w->factor = 0;

	//mMap[mTagCount].mTag = mTag;
	//mMap[mTagCount++].pmWindow = w; 
}
void fB_Menu::writeRow(uint16_t mTag, char* text, float value) {// general case
	int i;
	fB_Tag* r = Row(mTag);
	r->iVal = value;
	r->text = text;
	if(r->pPage == pCurrPage && r->pPage->status & VISIBLE) r->show();
}

void fB_Menu::resetGauge(uint16_t mTag,uint8_t  type, char* text, uint16_t tTag) {  // set gauge to a new pin
	writeWindow( mTag, type,  text, tTag ,NULL);
}
void fB_Menu::writeGauge(uint16_t mTag,uint8_t  type, char* text, float value) { // set gauge value ( same pin)
	writeWindow( mTag, type,  text, NULL,value);
}
void fB_Menu::writeWindow(uint16_t mTag,uint8_t  type, char* text, uint16_t tTag,float value) { // general case
	int i;
	fB_Window *w = Window(mTag);
	w->type = type;
	w->iVal = value;
	w->gaugeParseTitle(text);
	w->status |= INITED;
}
*/	
/*
fB_Window*  fB_Menu::Gauge(uint16_t mTag){ // used to access Gauge at menu level
	int i;
}
fB_Window*  fB_Menu::Window(uint16_t mTag){ // used to access Window at menu level
	int i;
}
*/
// The update functions update the object value member, they do not refresh the display,
// therefore they do not affect program flow by introducing tft delays. 
// To display the current value of the object, use the show() funtions.

//void fB_Menu::updateRow(uint16_t mTag) {
//	fB_Tag* r = Row(mTag);
//	r->update();
//}	
/*
//void fB_Menu::updateGauge(uint16_t mTag) {
//	fB_Window* w = Gauge(mTag);
//	w->update();
//}	
void fB_Menu::showGauge(uint16_t mTag) {
	fB_Window* w = Gauge(mTag);
	w->show(FORCE);
}	

*/
/*
void fB_Menu::createRow(uint16_t tTag,char * text){  // target 
	defineRow(  NULL, JUMPAGE,text,tTag, (float) NULL);
}
void fB_Menu::createRow(uint16_t tTag,uint8_t  type,char * text){  // target 
	defineRow(  NULL, type,text,tTag, (float) NULL);
}
void fB_Menu::defineRow(uint8_t  type,char*  text){// 
	defineRow(NULL, type,text,(uint16_t) NULL, (float) NULL);
}
void fB_Menu::defineRow( uint8_t  type,char*  text,uint16_t tTag, float value){// 
	defineRow( NULL, type,text,(uint16_t) tTag, (float)value);
}
*/
