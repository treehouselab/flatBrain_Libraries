#include "fB_Include.h"

int compareFilename(const void *x1, const void *x2) { 
	fB_Log *f1,*f2;
	f1 = menu.mFile[*(uint8_t *)x1];
	f2 = menu.mFile[*(uint8_t *)x2];
	if(f1->fTag !=NULL && f2->fTag == NULL) return -1;
	if(f1->fTag == NULL && f2->fTag !=NULL) return  1;
	return strcmp(f1->filename,f2->filename);
}

fB_Curr::fB_Curr() {
	tag = NULL;
	pTag = NULL;
	pRzero = NULL;
	parentTag = NULL;
	rowDex = NULL;
	rowCount = NULL;
}

void fB_Curr::setCurrPage(fB_Tag _tag) {
	tag = _tag
	pTag = Tag(tag);
	pRzero=  pTag->pRow;
	parentTag = pTag->fTag
	rowDex = 0;  // update when row selected
	rowCount =  pTag->dp.iVal;
	farY = STARTY + (ROWHT * rowCount);
}	

void fB_Curr::updateRowCount(uint8_t count) { 
	rowCount = count;
	 pTag->dp.iVal = count;
	farY = STARTY + (ROWHT * count);
}


/* 
As each row,gauge, or window is created at menu level, its pointer is added to the mMap table 
( fB_Row* for rows, fB_Window* for windows and gauges). Since the mTag values are user-defined and arbitrary, 
they would inefficient as lookup indexes, so we use the functions Row(mTag),Gauge(mTag), and Window(mTag) to scan through
the mMap table to find the pointer for a given mTag.

Note that Gauges and Windows are different types of the same fB_Window object.
Note that each entry mMap table contains either a pointer to an fB_Row or an fB_Window object.
*/


void fB_Menu::init(){

	totalFiles   = 0;
	fListStart = 0;
	tUsrListStart = 0;
	tSysListStart = 0;
	buttonCode = 0;
	pCurrPageTag = NULL;

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
//		defineRow(NULL,JPAGE,NULL,GLOBALS,NULL);
//		defineRow(NULL,JPAGE,NULL,PINS,NULL);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR,INT5,NULL);
		defineRow(CLKMO,INT5,NULL);
		defineRow(CLKDY,INT5,NULL);
		defineRow(CLKHH,INT5,NULL);
		defineRow(CLKMM,INT5,NULL);
		defineRow(CLKGET,TITLE,NULL);
		defineRow(CLKSET,TITLE,NULL);
		//Row(FDATE)->status |= MARK;

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
			//for(int i=0;i<min(globalCount,MAXLISTROWS);i++) defineRow((uint8_t )GROW+i,NULL,(uint8_t )GROW,NULL,(uint16_t) GPANEL, (float) i);

			definePage(TPANEL,F("TPANEL"),TLIST);
				defineRow(TLOG,F("LOG"),NULL,NOACT,NULL,NULL);
				defineRow(TINP,F("INPUT"),NOACT,VALUE,NULL,NULL);
				defineRow(TVAL,F("VALUE"),TADJ,VALUE,NULL,NULL);
				defineRow(TOPR,NULL,TARB,TEXT,NULL,NULL); 
				defineRow(TFAC,F("FACTOR"),TADJ,VALUE,NULL,NULL);
				defineRow(TINC,F("INCR"),TINC,VALUE,NULL,NULL);
				defineRow(TSET,F("SET"),TSET,TITLE,NULL,NULL);
				if(secondPass)	Row(TLOG)->status |= MARK;

		definePage(FILES,F("FILES"),GLOBALS);
			for(int i=0;i<MAXLISTROWS;i++) defineRow((uint16_t)FROW+i,NULL,(uint8_t )FROW,NULL,(uint16_t) FPANEL, (float) i);

			definePage(FPANEL,F("PANEL"),FILES);
				defineRow(FDATE,F("TIME"),NOACT,NULL,NULL,NULL);
				defineRow(FSIZE,F("SIZE"),NOACT,TEXT,NULL,NULL);
				defineRow(FDUMP,F("DUMP"),FDUMP,NULL,NULL,NULL);
				defineRow(FSTD,NULL,FSTD,NULL,NULL,NULL);
				defineRow(FARCH,F("ARCHIVE"),FARCH,NULL,NULL,NULL);
				if(secondPass)Row(FDATE)->status |= MARK;
				if(secondPass)Row(FSIZE)->status |= MARK;
*/
}

//void fB_Menu::erase() {
//	pCurrPageTag->mp.pPage->hide();
//}

void fB_Menu::jumpPage(uint16_t tag) {
	fB_Tag *pT;
	pT = Tag(tag);
	if(!pT ||!( pT->flags & PAGE)) return;
	dbug(F("Jump from %P to tag:%d , title:%P"),curr.pTag->Ptitle,pT->tag,pT->Ptitle);
	if(curr.tag) hidePage(curr.tag);
	menu.showPage(tag);

}
void fB_Menu::jumpPage(fB_Tag *pT) {
	if(!pT ||!( pT->flags & PAGE)) return;
	dbug(F("Jump from %P to tag:%d , title:%P"),curr.pTag->Ptitle,pT->tag,pT->Ptitle);
	if(curr.pTag) hidePage(curr.pTag->tag);
	menu.showPage(pT->tag);

}
//void fB_Menu::jumpPage(fB_Page *pP) {
	//if(!pP) return;
//	if(pCurrPage) pCurrPage->hide();
//	pP->showPage();
//	pCurrPage = pP;

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
		switch(curr.tag) {
			case HOME:		return; break;
			case FILES: 	pListStart = &fListStart; totalLines = totalFiles; break;
			case TLIST: 	pListStart = &tListStart; totalLines = tagCount;   break;
		}
		if(hand==RIGHT) {
			switch(curr.tag) {
				case FILES: 		
				case TLIST: 
					if(	totalLines >  *pListStart+MAXLISTROWS) {
						*pListStart += MAXLISTROWS; 
						jumpPage(curr.tag);
						return;
					}
			}
		}
		else {
			switch(curr.tag) {
				case FILES: 		
				case TLIST: 	
					if(	*pListStart >= MAXLISTROWS) {
						*pListStart -= MAXLISTROWS;	
						jumpPage(curr.tag); 
						return;
					}
			}				
		jumpPage(curr.parentTag);
		}
	}
	else curr.pRzero[curr.rowDex]->action(hand);
}

void fB_Menu:: nextSwitch() {
	int i;
	for(i=0;i < rowCount;i++)	{
		if(curr.pRzero[i].flags & DISABLE) continue;
		if((curr.pRzero[i].getAction() != NOACT ) && i > curr.rowDex) {
			curr.pRzero[curr.rowDex].deselect();
			curr.pRzero[i].select();
			curr.rowDex = i;
			break;
		}
	}
}
void fB_Menu:: prevSwitch() {
	int i;
	if(!curr.rowDex) return;
	for(i=rowCount;i >=0 ;i--)	{
		if(curr.pRzero[i].flags & DISABLE) continue;
		if((pRsero[i].getAction() != NOACT ) && i < curr.rowDex) {
			curr.pRzero[curr.rowDex].deselect();
			curr.pRzero[i].select();
			curr.rowDex = i;
			break;
		}
	}
}

void fB_Menu:: selectHeader() {
	curr.pRzero[curr.rowDex]->deselect();
	curr.rowDex = 0;			
	curr.pRzero[rowDex]->select();
}

void fB_Menu:: hidePage(uint8_t  flag) {
	curr.pRzero[curr.rowDex]->deselect();
	if(!flag) tft.clear(farY);
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
	
	///////////////////////// q sort indexes ////////////////////
	fSort = (uint8_t *) malloc (totalFiles);
	for(i=0;i<totalFiles;i++) fSort[i] = i;
	qsort(fSort, totalFiles, 1, compareFilename);
}


void fB_Menu:: showPage(uint8_t tag) {
	int dr;
	uint8_t  i,x;
	uint8_t  rows = 0;  //  calculated rows on this page (may be less than default pageRowCount)
	uint8_t  count,listStart
	float dv;
	fB_Tag *pT;

	curr.setCurrPage(tag);

	switch(tag) {
		case FILES:
			menu.getFileList();
			count = menu.totalFiles;
			listStart = menu.fListStart;
			break;
		case TAGS:
			count = tagCount;
			listStart = menu.tListStart;
			break;
		case CLOCK:
			if(!(bootStatus & RTC)) return;
			rtc.now();
			Tag(CLKYR)->pdiVal = rtc.yOff+2000 ;
			Tag(CLKMO)->pdiVal = rtc.m ;
			Tag(CLKDY)->pdiVal = rtc.d ;
			Tag(CLKHH)->pdiVal = rtc.hh ;
			Tag(CLKMM)->pdiVal = rtc.mm ;
			break;
		case PINS:{
			if(cardCount<2) jumpPage(parentTag);
			uint8_t  index;
			fB_Tag* pT,pCt;
			fB_Pin* pPin;
			fB_Card* pC;
			for(index=0;index<tagCount;index++) if(pTagRay[index]->pPin) break; // find first pin
			if(i==tagCount) break;
			pT = pTagRay[index];
			pP = pT->cp.pPin;
			pC = pP->pCard;
			pC = pTagRay[index]->pPin->pCard;
			Tag(PNPIN)->Ptext= pT->Ptitle;
			Tag(PNPIN)->dp.iVal= index;
			Tag(PNCRD)->Ptext= pC->Ptitle;
			Tag(PNADC)->dp.iVal=0;
			Tag(PNROW)->dp.iVal = pP->getRow();
			if(pP->getSide() == L) Tag(PNCOL)->Ptext = PstrRay[P_LEFT];
			else Tag(PNCOL)->Ptext = PstrRay[P_RIGHT];

			if( pP->getMode() == D) {
				Tag(PNADC)->status |= DISABLE;
				Tag(PNTOG)->type = TOGGLE;
				Tag(PNTOG)->Ptitle = PstrRay[P_TOGGLE];
				//Tag(PNTOG)->Ptitle=PstrRay[P_TOGGLE];
			}
			else {
				Tag(PNADC)->status  &= ~DISABLE;
				//Tag(PNTOG)->Ptitle = PstrRay[P_GATE];
				Tag(PNTOG)->Ptitle = PstrRay[P_GATE];
				Tag(PNTOG)->putAction(CGATE);
				pC->openCDchan(pP->getCpin());
				if(pP->onVal == GATE) {
					Tag(GATE)->dp.iVal= HIGH;
					Tag(PNADC)->dp.iVal= pP->read();
				}
				else {
					Tag(PNADC)->dp.iVal= 0;
					Tag(GATE)->dp.iVal= LOW;
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
						Row(FROW+i)->ptitle = menu.mFile[x]->filename;
						Row(FROW+i)->dp.iVal = x;
						if(menu.mFile[x]->fTag != NULL) Row(FROW+i)->flags |= MARK; // |=log
						else Row(FROW+i)->flags &= ~MARK; 
						break;
					case TLIST: // temporary test case for TLIST
						pTagRay[tListZeroIndex + i] = pTagRay[listStart + i];
						break;
				}
			}
			curr.updateRowCount(rows+1);
			/*
			dr = curr.rowCount - (rows+1); // for page tags, pagerow count is stored in dp.iVal
			dp.iVal = rows+1;
			if(dr > 0) farY -= dr * ROWHT;
			else farY = dp.iVal * ROWHT;
			*/
			break;
		}

	}

	menu.selectHeader();
	Tag(Header)->Ptitle = pT->Ptitle;  
	for(int i=0;i < curr.rowCount; i++) curr.pRzero[i]->showRow();
	//status |= VISIBLE;
	//dbug(F("ps r %d i %d"),pageRowCount,i);

}


 ///////////////////////////////////////// ROW METHODS //////////////////////////////////////////////

 void fB_Row::fB_Row()  {
	ptitle = NULL;
	pTtag = NULL;
	pPage = NULL;
	flags = VISIBLE;
}

uint8_t fB_Row::getAction() { return flags & MASKA; }

uint8_t fB_Row::putAction(uint8_t action) { 
	flags &= ~MASKA; 
	flags |= action;
}

uint16_t fB_Row::getY() {  return ( STARTY + (ROWHT) * curr.rowCount; }

void fB_Row:: showRow(uint8_t  hide) {  //when flag ==1, page is being updated only
	int i;
	char bufferTitle[MAXCHARSTEXT+1];
	char Pbuffer[6];
	fB_Tag *pT;
	pT = Tag(tTag);
	
	if(pT->ptitle != NULL) strcpy(bufferTitle,pT->ptitle);  // use non-PROGMEM title (left side string) if avail
	else {
		if(pT->Ptitle != NULL) getPtext(pT->Ptitle,bufferTitle);
		else bufferTitle[0] = '\0';	
	}	

	if(!curr.rowDex) {
		char header[MAXCHARSTEXT+1];
			sprintf(bufferTitle,"< %s >",bufferTitle);
			tft.print(CENTER,getY(),bufferTitle,MAXCHARSTEXT);
			return;
	}

	char bufferText[MAXCHARSTEXT+1];
	if(pT->dp.text != NULL) strcpy(bufferText,pT->dp.text);  // use non-PROGMEM title (left side string) if avail
	else {
		if(pT->dp.Ptext != NULL) getPtext(pT->dp.Ptext,bufferText);
		else bufferText[0] = '\0';
	}
	if( hide != HIDE  ) 	tft.resetDefColors();
	if( (hide == HIDE)  || (flags & DISABLE)) 	tft.setAll2Bcolor();
	else if(flags & MARK)	tft.setColor(FCOLOR,HCOLOR);
	if( getAction() != REFRESH ) tft.print(STARTX +ROWTEXTX,getY(),bufferTitle,MAXCHARSTEXT);

dbug(F("Show Row tag %d, ttag %d, tit: %s"),tag,tTag,bufferTitle);

	if(flags & LAMP) {
		if(cp.pPin) {
			pd.iVal =  cp.pPin->read();
			int x = STARTX +ROWSTATEX; int y = getY()+ROWSTATEY+2;
			tft.drawCircle(x,y,8);
			if(pT->dp.iVal == LOW) {
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
		switch(pT->pRow->getFormat()) {
			case INT5:
				if(flags & UNDEF) tft.print( RIGHT, getY(),getPstr(P_STRIKE,Pbuffer));
				else tft.printInt(STARTX +MONX,getY(),pT->dp.iVal,6,RIGHT);
				break;
			case FLOAT1:
				if(flags & UNDEF) tft.print( RIGHT, getY(),getPstr(P_STRIKE,Pbuffer));
				else tft.printFloat(STARTX +MONX,getY(),pT->dp.dVal,1,RIGHT);
				break;
			case FLOAT2:
				if(flags & UNDEF) tft.print( RIGHT, getY(), getPstr(P_STRIKE,Pbuffer));
				else tft.printFloat(STARTX +MONX,getY(),pT->dp.dVal,AR4_2,RIGHT);
				break;
				break;
			case TEXT:
				tft.print( RIGHT, getY(), bufferText);
				break;

		}
	}
	tft.resetDefColors();
	flags |= VISIBLE;
}

void fB_Row::action(uint8_t  hand) {
	if(flags & DISABLE ) return;
	char Pbuffer[7];
	fB_Pin *pP;
	fB_Tag *pT;
	pT = Tag(tTag);
	fB_Log *pF = NULL;   //  ptr to  logfile
	pF = menu.mFile[(int)value];//   value = index of the selected logfile
	uint16_t  index;
	//dbug(F("ACTION flag %d , tTag %d,  type %d"),flag,tTag,type);

	switch (hand)	{
		case RIGHT:
			//check page jump first 
			if(flags & PAGE) {	menu.jumpPage(tag); break; }
			// then check tag
			switch(tag) {
				case PNPIN: {
					index = Tag(PNPIN)->pd.iVal;
					if(index++ >= tagCount) index=0;
					pT = pTagRay[index];
					pP = pT->cp.pPin;
					//pP->pCard->AnalogGate(OFF);
					Tag(PNTOG)->pd.iVal= LOW;
					Tag(PNCRD)->Ptext= pP->pCard->Ptitle;
					Tag(PNPIN)->Ptext=Ptitle;
					Tag(PNPIN)->dp.iVal= index;
					Tag(PNROW)->dp.iVal = pP->getRow();
					if(pP->getSide() == L) Tag(PNCOL)->Ptext = PstrRay[P_LEFT];
					else Tag(PNCOL)->Ptext = PstrRay[P_RIGHT];

					if( pP->getMode() == D) {
						Tag(PNADC)->flags |= DISABLE;
						Tag(PNTOG)->pRow->putAction(TOGGLE);
						Ptitle=PstrRay[P_TOGGLE];
					}
					else {
						Tag(PNADC)->flags  &= ~DISABLE;
						Ptitle = PstrRay[P_GATE];
						Tag(PNTOG)->pRow->putAction(CGATE);
						pCard->openCDchan(pP->getCpin());
						if(pP->onVal == GATE) {
							Tag(PNTOG)->dp.iVal= HIGH;
							Tag(PNADC)->dp.iVal= pP->read();
						}
						else {
							Tag(PNADC)->dp.iVal= 0;
							Tag(PNTOG)->dp.iVal= LOW;
						}
					}
					show();
					Tag(PNROW)->showRow();
					Tag(PNCOL)->showRow();
					Tag(PNTOG)->showRow();
					Tag(PNADC)->showRow();
					return;
				case PNADC:
					pP = pTagRay[ Tag(PNPIN)->dp.iVal]->pPin;
					Tag(PNADC)->dp.iVal = pP->read();
					showRow();
					return;
				case FROW: {
					Tag(HEADER)->ptitle = pF->filename;
					pF->getAttributes();
					Tag(FDATE)->ptitle = pF->dateStr;
					Tag(FSIZE)->ptext = pF->sizeStr;
					Tag(FDUMP)->dp.iVal = value;
					Tag(FSTD)->dp.iVal = value;
					Tag(FARCH)->dp.iVal = value;
					//if(flags & LOG) {
						Tag(FARCH)->pRow |= VISIBLE;  // file is logfile
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
					Tag(FDATE)->ptext = pF->dateStr; // new atts
					Tag(FSIZE)->ptext = pF->sizeStr;
					Tag(FDATE)->showRow();
					Tag(FSIZE)->showRow();
					return;
				case FARCH:
					if(pF->archive()){
						pF->getAttributes();
						menu.selectHeader();
						Tag(FDATE)->pRow->ptext = pF->dateStr; // new atts
						Tag(FSIZE)->pRow->ptext = pF->sizeStr;
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
					rtc.yOff = (uint8_t ) (Tag(CLKYR)->dp.iVal - 2000);
					rtc.m = (uint8_t ) Tag(CLKMO)->dp.iVal ;
					rtc.d = (uint8_t ) Tag(CLKDY)->dp.iVal ;
					rtc.hh = (uint8_t ) Tag(CLKHH)->dp.iVal ;
					rtc.mm = (uint8_t ) Tag(CLKMM)->dp.iVal ;
					rtc.adjust();
					menu.showPage();
					break;
				case CLKGET:
					rtc.set(__DATE__,__TIME__);
					menu.showPage();
					break;
				case CLKYR: 
					pT->dp.iVal += 1;
					showRow();
					return;
				case CLKMO: 
					if(pT->dp.iVal < 12) pT->dp.iVal += 1;
					else pT->dp.iVal = 1;
					showRow();
					return;
				case CLKDY: 
					if(pT->dp.iVal < 31) pT->dp.iVal += 1;
					else pT->dp.iVal = 1;
					showRow();
					return;
				case CLKHH: 
					if(pT->dp.iVal < 23) pT->dp.iVal += 1;
					else pT->dp.iVal = 1;
					showRow();
					return;
				case CLKMM: 
					if(pT->dp.iVal < 59) pT->dp.iVal += 1;
					else pT->dp.iVal = 1;
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
			switch(pT->pRow->getAction()) {
				case CGATE:
					pP = pTagRay[Tag(PNPIN)->pd.iVal]->pPin;
					if(pP->onVal!=GATE) { 
						pP->onVal = GATE;
						dp.iVal = HIGH;
						pP->pCard->AnalogGate(ON);
						Tag(PNADC)->dp.iVal = pP->read();
					}
					else {
						pP->onVal = OFF;
						dp.iVal = LOW;
						pP->pCard->AnalogGate(OFF);
						Tag(PNADC)->dp.iVal = 0;

					}
					showRow();
					Tag(PNADC)->showRow();
					return;
				case TOGGLE:
					if(pT->dp.iVal==LOW) { 
						pT->dp.iVal = HIGH;
						if(pT->pPin) pT->pPin->write(pT->pPin->onVal);
					}
					else {
						pT->dp.iVal=LOW; 
						pT->dp.iVal = LOW;
						if(pT->pPin) pT->pPin->write(~(pT->pPin->onVal));
					}
					showRow();
					return;
				case PULSE:
					if(pT->pPin)  pT->pPin->pulse(PULSEMSECS);
					showRow();
					return;
				case YPULSE:
					if(pT->pPin) pT->pPin->YshiftPulse(PULSEMSECS);
					showRow();
					return;
				case INCR:
					pT->dp.iVal++;
					showRow();
					return;
				case INCR:
					if(pT->pPin) pT->pPin->read();
					showRow();
					return;
			} // END SWITCH on Action
	}// END SWITCH on Right
}

/*				case TROW:
					if(!Tag(TLOG)->ptext)  Tag(TLOG)->ptext = getPstr(P_NOLOG,Pbuffer) ;
					Tag(TSET)->tTag = pT->tag;
					Tag(TFAC)->tTag = pT->tag;
					Tag(TFAC)->pd.iVal = pT->factor;
					Tag(TFAC)->pd.iVal = pT->factor;
					Tag(TVAL)->tTag = pT->tag;
					Tag(TVAL)->pd.iVal = pT->getValue();
					Tag(TVAL)->format = pT->format;
					Tag(TINC)->format = FLOAT2;
					Tag(TINC)->pd.iVal = 0.01;
					Tag(TINP)->pd.iVal = pT->pd.iVal;
					Tag(TOPR)->tTag = pT->tag;
					Tag(TINP)->status |= MARK;

					switch(pT->format) {
						case INT5:   Tag(TINC)->pd.iVal = 1; break;
						case FLOAT1: Tag(TINC)->pd.iVal = .1; break;
						case FLOAT2: 
						default:     Tag(TINC)->pd.iVal = .01; break;
					}

						if(!(pT->flags & TINPUT))  {
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
								Tag(TFAC)->pd.iVal = 1;
							}
							else {
								ptext = menu.biasStr;
								Tag(TFAC)->pd.iVal = 0;
							}
							show();
							Tag(TFAC)->show();
							break;
						
				case TINC:	value*=10;
							if(value> 1000) value=1000;
							show();
							break;
				case TADJ:
							value += Tag(TINC)->pd.iVal ;
							show();
							break;
				case TSET:	
												if(pT->flags & TINPUT)  {
								if(!strcmp(Tag(TOPR)->ptext,menu.biasStr))  {
									Tag(TVAL)->pd.iVal = Tag(TINP)->pd.iVal + Tag(TFAC)->pd.iVal + ROUNDOFF;
									pT->flags |= TBIAS;
								}
								else  {
									Tag(TVAL)->pd.iVal = Tag(TINP)->pd.iVal * Tag(TFAC)->pd.iVal  + ROUNDOFF;
									pT->flags &= ~TBIAS;
								}
								pT->factor = Tag(TFAC)->pd.iVal;
							}
							else
				
							
							pT->pd.iVal = Tag(TVAL)->pd.iVal;
							pPage->selectHeader();
							Tag(TVAL)->show();
							break;
				//case SET:
				//			pT->pd.iVal = Tag((uint16_t)value)->pd.iVal;
				//			pPage->selectHeader();
				//			break;

		

			}


		case LEFT: 
			switch(type) {

				case TOGGLE:
					pT->pd.iVal=LOW; 
					value = LOW;
					if(pP) pP->write(~(pP->onVal));
					show();
					break;
				case TADJ:
					value -= Tag(TINC)->pd.iVal ;
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
void fB_Row:: hide() {
	flags &= ~VISIBLE;
	//tft.setAll2Bcolor();
	show(HIDE);
	//tft.resetDefColors();
	status &= ~VISIBLE;
}

void fB_Row:: select() {
	tft.setColor(FCOLOR,GCOLOR);
	tft.drawHLine(STARTX,getY()+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	//status |= SELECTED;
}

void fB_Row:: deselect() {
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,getY()+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	//status &= ~SELECTED;

}

void fB_Row:: frame() {

	tft.drawRect(STARTX,getY() ,STARTX + MAXPIXELWID,getY() + ROWHT);
}

void fB_Row:: unframe() {
	tft.setAll2Bcolor();
	tft.drawRect(STARTX,getY() ,STARTX + MAXPIXELWID,getY() + ROWHT);
	tft.resetDefColors();
}
void fB_Row:: clearRow() {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,getY()+1,STARTX +MAXPIXELWID-1,getY()+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	status &= ~VISIBLE;


}
void fB_Row:: clearRow1() {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,getY()+1,STARTX +GTITLEX-10,getY()+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	status &= ~VISIBLE;


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
	fB_Row* r = Row(mTag);
	r->pd.iVal = value;
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
	w->pd.iVal = value;
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
//	fB_Row* r = Row(mTag);
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
