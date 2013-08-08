#include "fB_Include.h"

int compareFilename(const void *x1, const void *x2) { 
	fB_Log *f1,*f2;
	f1 = menu.mFile[*(uint8_t *)x1];
	f2 = menu.mFile[*(uint8_t *)x2];
	if(f1->fTag !=NULL && f2->fTag == NULL) return -1;
	if(f1->fTag == NULL && f2->fTag !=NULL) return  1;
	return strcmp(f1->filename,f2->filename);
}


/* 
As each row,gauge, or window is created at menu level, its pointer is added to the mMap table 
( fB_Row* for rows, fB_Window* for windows and gauges). Since the mTag values are user-defined and arbitrary, 
they would inefficient as lookup indexes, so we use the functions Row(mTag),Gauge(mTag), and Window(mTag) to scan through
the mMap table to find the pointer for a given mTag.

Note that Gauges and Windows are different types of the same fB_Window object.
Note that each entry mMap table contains either a pointer to an fB_Row or an fB_Window object.
*/

fB_Page*	fB_Menu::Page(uint16_t tTag) {  //// used to access Page at menu level, for consistency
	int i;
	//for(i=0;i< pageCount ;i++) {
		 
	//	dbug(F("Page  i:%d, mtag:%d"),i, mPage[i].tag);
	//}
	for(i=0;i< pageCount && mPage[i].tag != tTag;i++);
	if(i < pageCount) return &mPage[i];
	else return NULL;
}
fB_Row*	   fB_Menu::Row(uint16_t mTag){ // used to access Row at menu level
	int i;
	for(i=0;i< rowCount;i++) if(mRow[i].tag == mTag) return &mRow[i];
	return NULL;
}
void fB_Menu::showRow(uint16_t mTag) {
	fB_Row* r = Row(mTag);
	r->show(FORCE);
}	


void fB_Menu::init(){
	nullText[0] = '.';
	nullText[1] = '\0';
	// actual count of initialized pages

	//sListStart = 0;
	totalFiles   = 0;
	fListStart = 0;
	tUsrListStart = 0;
	tSysListStart = 0;
	buttonCode = 0;
	//pCurrPage = &mPage[0];
	pCurrPage = NULL;

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
	// Set MAXPSTRCOUNT in fB_SYS_Defines.h to highest N+1
	// use getPstr(uint16_t tag, char *buffer)



}

void fB_Menu::definePage(uint16_t tag,const __FlashStringHelper* Ptitle, uint16_t parentTag)  {
	if(!secondPass) {
		pageCount++;
		rowCount++;
		//	dbug(F("DP0 pageCount %d, %P, rc:%d"),pageCount,Ptitle,rowCount);
		return;
	}
	fB_Page * p;

	p = &mPage[pageCount];
	if(tag == HOME || tag == NULL) {
		if(tag == NULL) Ptitle = F("FLATBRAIN");
		p->parentTag = NULL;
		}
	else {
		if(parentTag == NULL) p->parentTag = HOME;
		else p->parentTag = parentTag;
	}
	p->tag = tag;
	p->Ptitle = Ptitle;
	p->Y =  STARTY;
	p->currRowDex = 0;
	p->pageRowCount = 0;
	p->farY = 0;
	//p->status |= INITED;
	pageCount++;  // next index of mPage array
	defineRow(NULL,Ptitle,HEADER, NULL,NULL,NULL);
	//dbug(F("DP1 pageCount %d, %P, tag:%d , rc:%d"),pageCount,Ptitle,tag,rowCount);
	//dbug(F("DP1 mtag:%d "),mPage[pageCount-1].tag);

}


void fB_Menu::defineSpace() { defineRow(NULL,NULL,NOACT,NULL,NULL); }

void fB_Menu::defineJump(uint16_t tTag) { defineRow(NULL,NULL,JPAGE,NULL,tTag); }

void fB_Menu::defineRow(uint16_t tag, const __FlashStringHelper* Ptitle,uint8_t  type,uint8_t  format,uint16_t tTag, float value){// general case, value defaults to NULL
	
	fB_Tag	*pT;
	pT = Tag(tag);
	if(!pT && tag) pT = initTag(tag,Ptitle,NULL,NULL,NULL);
	if(!secondPass) { 
		rowCount++ ;
		//dbug(F("DR0 rowCount %d, %P"),rowCount, Ptitle);
	}

	//else if(pT) {
	else if(pT || !tag) {
		fB_Row *r;
		r = &mRow[rowCount++];  // get next row in array
		r->Ptitle =Ptitle;
		r->Ptext =NULL;
		r->ptitle = NULL;
		r->ptext = NULL;
		r->tag = tag;
		r->type = type;
		r->format = type; // default
		if(format) r->format = format;
		r->status = VISIBLE;
		r->value = value;
		r->tTag = tTag;

		fB_Page *p;
		p = &mPage[pageCount-1]; // get last initialized page in array
		if(!p->pageRowCount)  p->pRow = r;  // first row for this page, set pRow
		r->pPage = p;
		r->Y = p->Y + (ROWHT) * p->pageRowCount; 
		p->pageRowCount++;
		p->farY =  (ROWHT) * p->pageRowCount;
		dbug(F("DR1 tag %d, ttag %d, %P"),r->tag,r->tTag, r->Ptitle);
		
	}

	//char Pbuffer[20];
	//getPtext(r->Ptitle,Pbuffer);
	//dbug(F("%d, %s,c%d"),mTag,Pbuffer, p->pageRowCount);
 
}
//defineRow(uint16_t mTag,const __FlashStringHelper* pTitle,uint8_t  type,uint8_t  format=NULL);
//defineRow(uint16_t mTag,const __FlashStringHelper* pTitle,uint8_t  type, uint8_t  format,uint16_t tTag, float value=0);

void fB_Menu::defineSystem()  {

	definePage(SYSTEM,F("SYSTEM"),HOME);
		defineRow(NULL,F("CLOCK"),JPAGE,NULL,CLOCK,NULL);
		defineRow(NULL,F("GLOBALS"),JPAGE,NULL,GLOBALS,NULL);
		defineRow(NULL,F("PINS"),JPAGE,NULL,PINS,NULL);

	definePage(PINS,F("PINS"),SYSTEM);
		defineRow(PNCRD,F("CARD"),PNCRD,TEXT,NULL,NULL);
		defineRow(PNPIN,F("PIN"),PNPIN,TEXT,NULL,NULL);
		defineRow(PNROW,F("ROW"),NOACT,INT5,NULL,NULL);
		defineRow(PNCOL,F("COL"),NOACT,TEXT,NULL,NULL);
		//defineRow(PNRES,F("RESISTOR"),PNRES,INT5,NULL,NULL);
		defineRow(PNTOG,F("GATE"),PNTOG,LAMP,NULL,NULL);
		defineRow(PNADC,F("ADC"),PNADC,INT5,NULL,NULL);
		//defineRow(PNSET,F("SET"),PNSET,TITLE,NULL,NULL);
		if(secondPass)Row(PNROW)->status |= MARK;
		if(secondPass)Row(PNCOL)->status |= MARK;

	definePage(CLOCK,F("CLOCK"),SYSTEM);
		defineRow(CLKYR,F("YEAR"),CLK,INT5,NULL,NULL);
		defineRow(CLKMO,F("MONTH"),CLK,INT5,NULL,NULL);
		defineRow(CLKDY,F("DAY"),CLK,INT5,NULL,NULL);
		defineRow(CLKHH,F("HOUR"),CLK,INT5,NULL,NULL);
		defineRow(CLKMM,F("MIN"),CLK,INT5,NULL,NULL);
		defineRow(CLKGET,F("GET USB"),CLKGET,TITLE,NULL,NULL);
		defineRow(CLKSET,F("SET"),CLKSET,TITLE,NULL,NULL);
		//Row(FDATE)->status |= MARK;

	
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

}

void fB_Menu::erase() {
	pCurrPage->hide();
}

void fB_Menu::show(uint16_t tTag) {
	jumpPage(tTag);
}

void fB_Menu::jumpPage(uint16_t tTag) {
	fB_Page *pP;
	pP = Page(tTag);
	if(!pP) return;
	if(pCurrPage == pP) return;
	//if(pCurrPage->tag == PNIV) pCardRay[(uint8_t )Row(PNCRD)->value]->VDcutoff(false);
dbug(F("Jump from %P to tag:%d , title:%P"),pCurrPage->Ptitle,pP->tag,pP->Ptitle);
	pCurrPage->hide();
	pP->show();
	pCurrPage = pP;
//dbug(F("Jump to %d  %P"),pCurrPage->tag,pCurrPage->Ptitle);

}

void fB_Menu::checkButtonCode() {
	if(!buttonCode) return;

	switch(buttonCode) {
		case 1: context(RIGHT);break;
		case 2: context(LEFT);break;
		case 3: pCurrPage->prevSwitch(); break;
		case 4: pCurrPage->nextSwitch(); break;
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
	fB_Page *p;
	p = pCurrPage;
	int* pListStart;
	uint8_t  totalLines;

	if(p->currRowDex == 0) { // jump  page if row[0]
		switch(p->tag) {
			case HOME: return; break;
			case FILES: 	pListStart = &fListStart; totalLines = totalFiles; break;
			case TLIST: 	
				if(p->type == PSYS) { pListStart = &tSysListStart; totalLines = sysTagCount; }
				else { pListStart = &tUsrListStart; totalLines = usrTagCount; }
				break;
		}
		if(hand==RIGHT) {
			switch(p->tag) {
				case FILES: 		
				case TLIST: 
					if(	totalLines >  *pListStart+MAXLISTROWS) {
						*pListStart += MAXLISTROWS; 
						jumpPage(p->tag); 
					}
						
					break;
			}
		}
		else {
			switch(p->tag) {
				case FILES: 		
				case TLIST: 	
					if(	*pListStart >= MAXLISTROWS) {
						*pListStart -= MAXLISTROWS;	
						jumpPage(p->tag); 
					}
					break;
			}				
			jumpPage(p->parentTag);
		}
	}
	else p->pRow[p->currRowDex].action(hand);
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


 ///////////////////////////////////////// PAGE METHODS //////////////////////////////////////////////

void fB_Page:: show() {
	int dr;
	uint8_t  i,x;
	uint8_t  rows = 0;  //  calculated rows on this page (may be less than default pageRowCount)
	uint8_t  count,listStart,format;
	float dv;

	menu.pCurrPage = this;

	switch(tag) {
		case FILES:
			menu.getFileList();
			count = menu.totalFiles;
			listStart = menu.fListStart;
			break;
		case TLIST:
			if(type == PSYS){
				count = sysTagCount;
				listStart = menu.tSysListStart;
			}
			else {
				count = usrTagCount;
				listStart = menu.tUsrListStart;
			}
			break;
		case CLOCK:
			if(!(bootStatus & RTC)) return;
			rtc.now();
			menu.Row(CLKYR)->value = rtc.yOff+2000 ;
			menu.Row(CLKMO)->value = rtc.m ;
			menu.Row(CLKDY)->value = rtc.d ;
			menu.Row(CLKHH)->value = rtc.hh ;
			menu.Row(CLKMM)->value = rtc.mm ;
			break;
		case PINS:{
			if(cardCount<2) menu.jumpPage(parentTag);
			uint8_t  index;
			fB_Tag* pT;
			fB_Pin* pP;
			fB_Card* pC;
			for(index=0;index<usrTagCount;index++) if(pUsrTagRay[index]->pPin) break;
			if(i==usrTagCount) break;
			pP = pUsrTagRay[index]->pPin;
			pC = pUsrTagRay[index]->pPin->pCard;
			menu.Row(PNCRD)->Ptext= pC->Ptitle;
			menu.Row(PNPIN)->Ptext= pT->Ptitle;
			menu.Row(PNPIN)->value= (float) index;
			menu.Row(PNADC)->value=0;
			menu.Row(PNROW)->value = pP->getRow();
			if(pP->getSide() == L) menu.Row(PNCOL)->Ptext = PstrRay[P_LEFT];
			else menu.Row(PNCOL)->Ptext = PstrRay[P_RIGHT];

			if( pP->getMode() == D) {
				menu.Row(PNADC)->status |= DISABLE;
				menu.Row(PNTOG)->type = TOGGLE;
				menu.Row(PNTOG)->Ptitle=PstrRay[P_TOGGLE];
			}
			else {
				menu.Row(PNADC)->status  &= ~DISABLE;
				menu.Row(PNTOG)->Ptitle = PstrRay[P_GATE];
				menu.Row(PNTOG)->type = PGATE;
				pC->openCDchan(pP->getCpin());
				if(pP->onVal ==GATE) {
					menu.Row(GATE)->value= HIGH;
					menu.Row(PNADC)->value= pP->read();
				}
				else {
					menu.Row(PNADC)->value= 0;
					menu.Row(GATE)->value= LOW;
				}
			}		
			break;

	}

	switch(tag) {
		case FILES:
		case TLIST:
			if(count <= listStart) listStart = max(count - MAXLISTROWS,0);
			if(MAXLISTROWS < count - listStart ) rows = MAXLISTROWS;
			else rows = count - listStart;
			for( i=0;i<rows;i++) {
				switch(tag) {
					case FILES:
						x = menu.fSort[i+listStart];
						menu.Row(FROW+i)->ptitle = menu.mFile[x]->filename;
						menu.Row(FROW+i)->value = (float) x;
						if(menu.mFile[x]->fTag != NULL) menu.Row(FROW+i)->status |= LOG; 
						else menu.Row(FROW+i)->status &= ~LOG; 
						break;
					case TLIST:
						fB_Tag* pT;
						uint8_t  index;
						if(type == PSYS) pT = pSysTagRay[i];
						else pT = pUsrTagRay[i];
						menu.Row(TROW+i)->Ptitle = pT->Ptitle;
						menu.Row(TROW+i)->tTag = pT->tag;
						menu.Row(TROW+i)->value = pT->getValue();
						menu.Row(TROW+i)->format = pT->format;
						switch(pT->format) {
							case TOGGLE:  menu.Row(TROW+i)->type = TOGGLE; break;
							case LAMP:  menu.Row(TROW+i)->type = NOACT; break;
							default:	menu.Row(TROW+i)->type = TROW; break;
						}
						//menu.Row(GROW+i)->status |= (CREATED | INITED) ;
						break;
				}
			}
			dr = pageRowCount - (rows+1);
			pageRowCount = rows+1;
			if(dr > 0) farY -= dr * ROWHT;
			else farY = pageRowCount * ROWHT;
			break;
		}

	}

	selectHeader();
	for(i=0;i < pageRowCount;i++) 	pRow[i].show();
	//status |= VISIBLE;
	//dbug(F("ps r %d i %d"),pageRowCount,i);

}
void fB_Page:: hide(uint8_t  flag) {
	pRow[currRowDex].deselect();
	//for(int i=0;i < pageRowCount;i++)	pRow[i].status &= ~VISIBLE;
	//if(win.status & CREATED) win.status &= ~VISIBLE;
	currRowDex = 0;
	//status &= ~VISIBLE;
	if(!flag) tft.clear(farY);
	else tft.clear();
}
void fB_Page:: nextSwitch() {
	int i;
	for(i=0;i < pageRowCount;i++)	{
		if(pRow[i].status & DISABLE) continue;
		if((pRow[i].type != NOACT ) && i > currRowDex) {
			pRow[currRowDex].deselect();
			pRow[i].select();
			currRowDex = i;
			break;
		}
	}
}
void fB_Page:: prevSwitch() {
	int i;
	if(!currRowDex) return;
	for(i=pageRowCount;i >=0 ;i--)	{
		if(pRow[i].status & DISABLE) continue;
		if((pRow[i].type != NOACT ) && i < currRowDex) {
			pRow[currRowDex].deselect();
			pRow[i].select();
			currRowDex = i;			
			break;
		}
	}

}
void fB_Page:: selectHeader() {
	pRow[currRowDex].deselect();
	pRow[0].select();
	currRowDex = 0;			
}



 ///////////////////////////////////////// ROW METHODS //////////////////////////////////////////////
void fB_Row:: show(uint8_t  flag) {  //when flag ==1, page is being updated only
	fB_Tag* pT;
	fB_Pin* pP;
	int i;
	char bufferTitle[MAXCHARSTEXT+1];
	char Pbuffer[6];

	if(ptitle != NULL) strcpy(bufferTitle,ptitle);  // use non-PROGMEM title (left side string) if avail
	else {
		if(Ptitle != NULL) getPtext(Ptitle,bufferTitle);
		else bufferTitle[0] = '\0';	
	}	
	char bufferText[MAXCHARSTEXT+1];
	char header[MAXCHARSTEXT+1];
	if(ptext != NULL) strcpy(bufferText,ptext);  // use non-PROGMEM title (left side string) if avail
	else {
		if(Ptext != NULL) getPtext(Ptext,bufferText);
		else bufferText[0] = '\0';
	}
	if( flag != HIDE  ) 	tft.resetDefColors();
	if( (flag == HIDE)  || (status & DISABLE)) 	tft.setAll2Bcolor();
	else if( (status & MARK) || (status & LOG) )	tft.setColor(FCOLOR,HCOLOR);
	if( flag != REFRESH && format != HEADER ) tft.print(STARTX +ROWTEXTX,Y,bufferTitle,MAXCHARSTEXT);

dbug(F("Show Row tag %d, ttag %d, tit: %s"),tag,tTag,bufferTitle);

	switch(format) {
		case HEADER:
			sprintf(header,"< %s >",bufferTitle);
			tft.print(CENTER,Y,header,MAXCHARSTEXT);
			break;
		case STRIKE:
			tft.print( RIGHT, Y, getPstr(P_STRIKE,Pbuffer));
			break;
		case INT5:
			if(status == NOSTATUS) tft.print( RIGHT, Y,getPstr(P_STRIKE,Pbuffer));
			else tft.printInt(STARTX +MONX,Y,(long) value,6,RIGHT);
			break;
		case FLOAT1:
			if(status == NOSTATUS) tft.print( RIGHT, Y,getPstr(P_STRIKE,Pbuffer));
			else tft.printFloat(STARTX +MONX,Y,value,1,RIGHT);
			break;
		case VALUE:  // default
		case FLOAT2:
			if(status == NOSTATUS) tft.print( RIGHT, Y, getPstr(P_STRIKE,Pbuffer));
			else tft.printFloat(STARTX +MONX,Y,value,AR4_2,RIGHT);
			break;
		case TOGGLE:
		case LAMP: { 
				pT = Tag(tTag);
				pP = pT->pPin;
				if(pT) {
					if(pP) pT->value = (float) pP->read();
					value = pT->value;
				}
				int x = STARTX +ROWSTATEX; int y = Y+ROWSTATEY+2;
				tft.drawCircle(x,y,8);
				if(value == LOW) {
					tft.setAll2Bcolor();
					tft.fillCircle(x,y,6);
					tft.resetDefColors();
				}
				else  {
					tft.setColor(FCOLOR,GCOLOR);
					tft.fillCircle(x,y,6);
				}
			}
			break;
		case PULSE:
		case TEXT:
			tft.print( RIGHT, Y, bufferText);
			break;

	}
	tft.resetDefColors();
	status |= VISIBLE;
}

void fB_Row::action(uint8_t  flag) {
	if(status & DISABLE ) return;
	char Pbuffer[7];
	fB_Log *pF = NULL;   //  ptr to  logfile
	fB_Tag *pT = NULL;
	fB_Pin *pP = NULL;
	fB_Card* pC = NULL;

	pF = menu.mFile[(int)value];//   value = index of the selected logfile
	pT = Tag(tTag);  
	pP = pT->pPin; 
	pC = pP->pCard;
	uint8_t  tstate;
	//dbug(F("ACTION flag %d , tTag %d,  type %d"),flag,tTag,type);

	switch (flag)	{
		case RIGHT:
			switch(type) {
				case PNCRD: {
						uint8_t  index;
						index = (uint8_t )  value;
						//pCardRay[index++]->VDcutoff(false);
						index++;
						if(index >= cardCount) index=1; // 0 is BRAIN card, don't want that
						//pCardRay[index]->VDcutoff(true);
						Ptext =  pCardRay[index]->Ptitle;
						value = (float) index;
						show();
					}
					break;
				case PNPIN: {
						pCardRay[(uint8_t ) menu.Row(PNCRD)->value]->AnalogGate(OFF);
						menu.Row(PNTOG)->value= LOW;
						menu.Row(PNTOG)->show();
						uint8_t  index;
						index = (int) menu.Row(PNPIN)->value;
						if(index++ >= usrTagCount) index=0;
						pT = pUsrTagRay[index];
						pP = pT->pPin;
						pC =pP->pCard;
						menu.Row(PNCRD)->Ptext= pC->Ptitle;
						menu.Row(PNPIN)->Ptext= pT->Ptitle;
						menu.Row(PNPIN)->value= (float) index;
						menu.Row(PNROW)->value = pP->getRow();
						if(pP->getSide() == L) menu.Row(PNCOL)->Ptext = PstrRay[P_LEFT];
						else menu.Row(PNCOL)->Ptext = PstrRay[P_RIGHT];

						if( pP->getMode() == D) {
							menu.Row(PNADC)->status |= DISABLE;
							menu.Row(PNTOG)->type = TOGGLE;
							menu.Row(PNTOG)->Ptitle=PstrRay[P_TOGGLE];
						}
						else {
							menu.Row(PNADC)->status  &= ~DISABLE;
							menu.Row(PNTOG)->Ptitle = PstrRay[P_GATE];
							menu.Row(PNTOG)->type = PGATE;
							pC->openCDchan(pP->getCpin());
							if(pP->onVal == GATE) {
								menu.Row(PNTOG)->value= HIGH;
								menu.Row(PNADC)->value= pP->read();
							}
							else {
								menu.Row(PNADC)->value= 0;
								menu.Row(PNTOG)->value= LOW;
							}
						}
						show();
						menu.Row(PNROW)->show();
						menu.Row(PNCOL)->show();
						menu.Row(PNTOG)->show();
						menu.Row(PNADC)->show();


					}
					break;
					/*
				case PNRES: {
						pCardRay[(uint8_t ) menu.Row(PNCRD)->value]->VDgate(OFF);
						menu.Row(PNTOG)->value= LOW;
						menu.Row(PNTOG)->show();

						uint8_t index = (uint8_t )  value;
						if(++index > MAXVDRDEX) index = 0;
						value = (float) index;
						//pCardRay[(uint8_t ) menu.Row(PNCRD)->value]->VDselectR(pgm_read_byte(&VDRmap[index]));
						show();
						}
					break;
				case PNSET:
						if(menu.Row(PNPIN)->ptitle != NULL) {
							pPin[(uint8_t) menu.Row(PNPIN)->value]->vdr = (uint8_t) menu.Row(PNRES)->value;
						}
						pPage->selectHeader();
						break;
						*/
				case PGATE:
					pP = pUsrTagRay[(uint8_t ) menu.Row(PNPIN)->value]->pPin;
					if(pP->onVal!=GATE) { 
						pP->onVal = GATE;
						value = HIGH;
						pCardRay[(uint8_t ) menu.Row(PNCRD)->value]->AnalogGate(ON);
						menu.Row(PNADC)->value =pP->read();
					}
					else {
						pP->onVal = OFF;
						value = LOW;
						pCardRay[(uint8_t ) menu.Row(PNCRD)->value]->AnalogGate(OFF);
						menu.Row(PNADC)->value = 0;

					}
					show();
					menu.Row(PNADC)->show();
					break;
				case PNADC:
					pP = pUsrTagRay[(uint8_t ) menu.Row(PNPIN)->value]->pPin;
					menu.Row(PNADC)->value = pP->read();
					show();
					break;
					/*
				case PNCUT: {
						uint8_t  

					pCard(UTIL)->VDcutoff(true);
					value = (float) Card(UTIL)->PCF->digitalRead(CUTOFF);
					show();
					break;
*/
				case TOGGLE:
					//putTagState(tTag,HIGH); // works for both Pin and Global tags
					if(pT->value==LOW) { 
						value = HIGH;
						if(pP) pP->write(pP->onVal);
					}
					else {
						pT->value=LOW; 
						value = LOW;
						if(pP) pP->write(~(pP->onVal));
					}
					show();
					break;
				case PULSE:
					if(pP) 	pP->pulse((unsigned int)value);
					show();
					break;
				case YPULSE:
					if( pP) pP->YshiftPulse((unsigned int)value);
					show();
					break;
				case JPAGE:
					if(tTag==TLIST) menu.Page(tTag)->type = value; // do this here to fork page content
					menu.jumpPage(tTag);
					break;
				case FROW: {
						menu.Page(FPANEL)->pRow[0].ptitle = pF->filename;
						//menu.Page(FPANEL)->ptitle = pF->filename;
						pF->getAttributes();
						menu.Row(FDATE)->ptitle = pF->dateStr;
						menu.Row(FSIZE)->ptext = pF->sizeStr;
						menu.Row(FDUMP)->value = value;
						menu.Row(FSTD)->value = value;
						menu.Row(FARCH)->value = value;
						if(status & LOG) {
							menu.Row(FARCH)->status |= VISIBLE;  // file is logfile
							menu.Row(FSTD)->Ptitle = PstrRay[P_STAMP];  
							menu.Row(FSTD)->type = FSTAMP;  
						}
						else {
							menu.Row(FARCH)->status &= ~VISIBLE;  // file is other file
							menu.Row(FSTD)->Ptitle = PstrRay[P_DELETE];  
							menu.Row(FSTD)->type = FDEL;  
						}
						menu.jumpPage(FPANEL);
						

					}
					break;
				case FSTAMP:
					pF->writeData();
					pF->getAttributes();
					pPage->selectHeader();
					menu.Row(FDATE)->ptext = pF->dateStr; // new atts
					menu.Row(FSIZE)->ptext = pF->sizeStr;
					menu.Row(FDATE)->show();
					menu.Row(FSIZE)->show();
					break;
				case FARCH:
					if(pF->archive()){
						pF->getAttributes();
						pPage->selectHeader();
						menu.Row(FDATE)->ptext = pF->dateStr; // new atts
						menu.Row(FSIZE)->ptext = pF->sizeStr;
						menu.Row(FDATE)->show();
						menu.Row(FSIZE)->show();
					}
					//else Serial.println("ARCHIVE FAIL");
					break;
				case FDEL:
					pF->remove();
					menu.jumpPage(menu.Page(FPANEL)->parentTag);					
					break;
				case FDUMP:
					pF->dump();
					pPage->selectHeader();
					break;
				case TROW:{
						//pG = Global(tTag);//   value = index of the selected global
			//dbug("RA grow  %s",pG->tagStr);

				//menu.Page(GPANEL)->pRow[0].title = pG->tagStr;
						//sprintf(fbuffer,"%s.TXT",pG->tagStr);
////////////////////////menu.Row(GLOG)->title = getLogName(pG->fTag);  // is this legal?
						if(!menu.Row(TLOG)->ptext)  menu.Row(TLOG)->ptext = getPstr(P_NOLOG,Pbuffer) ;
						menu.Row(TSET)->tTag = pT->tag;
						menu.Row(TFAC)->tTag = pT->tag;
						menu.Row(TFAC)->value = pT->factor;
						menu.Row(TFAC)->value = pT->factor;
						menu.Row(TVAL)->tTag = pT->tag;
						menu.Row(TVAL)->value = pT->getValue();
						menu.Row(TVAL)->format = pT->format;
						menu.Row(TINC)->format = FLOAT2;
						menu.Row(TINC)->value = 0.01;
						menu.Row(TINP)->value = pT->value;
						menu.Row(TOPR)->tTag = pT->tag;
						menu.Row(TINP)->status |= MARK;

						switch(pT->format) {
							case INT5:   menu.Row(TINC)->value = 1; break;
							case FLOAT1: menu.Row(TINC)->value = .1; break;
							case FLOAT2: 
							default:     menu.Row(TINC)->value = .01; break;
						}
/*
						if(!(pT->flags & TINPUT))  {
							menu.Row(TINP)->format = STRIKE;
							menu.Row(TFAC)->format = STRIKE;
							menu.Row(TOPR)->format = STRIKE;
							menu.Row(TFAC)->type = NOACT;
							menu.Row(TOPR)->type = NOACT;
							menu.Row(TVAL)->type =  TADJ;
							menu.Row(TFAC)->status |= MARK;
							menu.Row(TOPR)->status |= MARK;
							menu.Row(TVAL)->status &= ~MARK;
						}
						else {
							*/
							menu.Row(TFAC)->status &= ~MARK;
							menu.Row(TOPR)->status &= ~MARK;
							menu.Row(TVAL)->status |= MARK;

							menu.Row(TINP)->format = pT->format;
							menu.Row(TOPR)->format = TEXT;
							menu.Row(TFAC)->format = pT->format;
							menu.Row(TFAC)->type = TADJ;
							menu.Row(TOPR)->type = TARB;
							menu.Row(TVAL)->type = NOACT;
								menu.Row(TOPR)->ptext =getPstr(P_AMP,Pbuffer);
								if(!pT->factor) pT->factor = 1;
								menu.Row(TINC)->format = FLOAT2;
 					    //}
						menu.jumpPage(TPANEL);

					}
					break;
					/*
				case TARB:	if (!strcmp(ptext,menu.biasStr)) {
								ptext = menu.ampStr;
								menu.Row(TFAC)->value = 1;
							}
							else {
								ptext = menu.biasStr;
								menu.Row(TFAC)->value = 0;
							}
							show();
							menu.Row(TFAC)->show();
							break;
							*/
				case TINC:	value*=10;
							if(value> 1000) value=1000;
							show();
							break;
				case TADJ:
							value += menu.Row(TINC)->value ;
							show();
							break;
				case TSET:	
					/*
							if(pT->flags & TINPUT)  {
								if(!strcmp(menu.Row(TOPR)->ptext,menu.biasStr))  {
									menu.Row(TVAL)->value = menu.Row(TINP)->value + menu.Row(TFAC)->value + ROUNDOFF;
									pT->flags |= TBIAS;
								}
								else  {
									menu.Row(TVAL)->value = menu.Row(TINP)->value * menu.Row(TFAC)->value  + ROUNDOFF;
									pT->flags &= ~TBIAS;
								}
								pT->factor = menu.Row(TFAC)->value;
							}
							else
						*/
							
							pT->value = menu.Row(TVAL)->value;
							pPage->selectHeader();
							menu.Row(TVAL)->show();
							break;
				//case SET:
				//			pT->value = menu.Row((uint16_t)value)->value;
				//			pPage->selectHeader();
				//			break;
				case VALUE:
							//update();
							show();
							break;
				case CLKSET:
					//dbug(F("clkset"));
					rtc.yOff = (uint8_t ) (menu.Row(CLKYR)->value - 2000);
					rtc.m = (uint8_t ) menu.Row(CLKMO)->value ;
					rtc.d = (uint8_t ) menu.Row(CLKDY)->value ;
					rtc.hh = (uint8_t ) menu.Row(CLKHH)->value ;
					rtc.mm = (uint8_t ) menu.Row(CLKMM)->value ;
					rtc.adjust();
					pPage->show();
					break;
				case CLKGET:
					rtc.set(__DATE__,__TIME__);
					pPage->show();
					break;
				case CLKYR: 
					value += 1;
					show();
					break;
				case CLKMO: 
					if(value < 12) value += 1;
					else value = 1;
					show();
					break;
				case CLKDY: 
					if(value < 31) value += 1;
					else value = 1;
					show();
					break;
				case CLKHH: 
					if(value < 23) value += 1;
					else value = 1;
					show();
					break;
				case CLKMM: 
					if(value < 59) value += 1;
					else value = 1;
					show();
					break;

				//case TLAS:	createGdefLog() ; pPage->selectHeader();break;
				case TIAT:	record.EEinitTags(); pPage->selectHeader();break;
				case TSAT:	record.EEwriteTags(); pPage->selectHeader();break;
				case TLAU:	
							for(int i = 0;i<logCount;i++) logRay[i].pLog->writeData();
							pPage->selectHeader();
							break;

			} // END SWITCH type
			break;

		case LEFT: 
			switch(type) {

				case TOGGLE:
					pT->value=LOW; 
					value = LOW;
					if(pP) pP->write(~(pP->onVal));
					show();
					break;
				case TADJ:
					value -= menu.Row(TINC)->value ;
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


void fB_Row:: hide() {
	status &= ~VISIBLE;
	//tft.setAll2Bcolor();
	show(HIDE);
	//tft.resetDefColors();
	status &= ~VISIBLE;
}

void fB_Row:: select() {
	tft.setColor(FCOLOR,GCOLOR);
	tft.drawHLine(STARTX,Y+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	//status |= SELECTED;
}

void fB_Row:: deselect() {
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,Y+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	//status &= ~SELECTED;

}

void fB_Row:: frame() {

	tft.drawRect(STARTX,Y ,STARTX + MAXPIXELWID,Y + ROWHT);
}

void fB_Row:: unframe() {
	tft.setAll2Bcolor();
	tft.drawRect(STARTX,Y ,STARTX + MAXPIXELWID,Y + ROWHT);
	tft.resetDefColors();
}
void fB_Row:: clearRow() {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,Y+1,STARTX +MAXPIXELWID-1,Y+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	status &= ~VISIBLE;


}
void fB_Row:: clearRow1() {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,Y+1,STARTX +GTITLEX-10,Y+ROWHT-1,tft.bColor);
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
	r->value = value;
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
	w->value = value;
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
