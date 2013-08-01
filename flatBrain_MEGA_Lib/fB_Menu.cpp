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
	for(i=0;i< pageCount && mPage[i].pTag != tTag;i++); 
	if(i < pageCount) return &mPage[i];
	else return NULL;
}
fB_Row*	   fB_Menu::Row(uint16_t mTag){ // used to access Row at menu level
	int i;
	for(i=0;i< totalRows;i++) if(mRow[i].tag == mTag) return &mRow[i];
	return NULL;
}
void fB_Menu::showRow(uint16_t mTag) {
	fB_Row* r = Row(mTag);
	r->show(FORCE);
}	


void fB_Menu::init(){
	nullText[0] = '.';
	nullText[1] = '\0';
	pageCount   = 0;				// actual count of initialized pages
	rowCount    = 0;				// actual count of initialized pages
	totalFiles   = 0;				
	pCurrPage   = NULL;	
	sListStart = 0;
	fListStart = 0;
	gyListStart = 0;
	guListStart = 0;
	totalPages   =0;
	totalRows    =0;
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
	// Set MAXPSTRCOUNT in fB_SYS_Defines.h to highest N+1
	// use getPstr(uint16_t tag, char *buffer)


	passTog =0;
	defineSystem();
    defineGlobals();
    defineStacks();
	defineClock(); 

	defineMenu(); // 1st pass with passtog = 0, determine array sizes
	mPage = (fB_Page *) malloc(sizeof(fB_Page) * totalPages);
	mRow =  (fB_Row *)   calloc(totalRows,sizeof(fB_Row));

	passTog = 1;
	defineSystem();
    defineGlobals();
    defineStacks();
	defineClock(); 
	defineMenu(); // 2nd pass with passtog = 1, execute
	pCurrPage = &mPage[0];
	//for(int i = 0;i < brain.globalCount;i++) dbug("MI GLOBAL tag %d tagstr %s",brain.pGlobal[i]->gTag,brain.pGlobal[i]->tagStr);

}

//void fB_Menu::definePage(uint16_t pTag,  char *tagStr, uint8_t  type,uint16_t parentTag)  {
void fB_Menu::definePage(uint16_t pTag,const __FlashStringHelper* Ptitle, uint16_t parentTag, uint8_t  type)  {
	if(!passTog) {
		totalPages++;
		totalRows++;
		return;
	}

	fB_Page * p;
	p->Ptitle = Ptitle;
	p = &mPage[pageCount];
	p->pTag = pTag;
	p->parentTag = parentTag;
	p->type = type;
	p->pDex = pageCount;
	//p->win.status = NOSTATUS;
	//p->X =  STARTX;
	p->Y =  STARTY;
	p->currRowDex = 0;
	p->pageStackCount = 0;
	p->pageRowCount = 0;
	p->farY = 0;
	p->status |= INITED;
	pageCount++;  // next index of mPage array
	defineRow(NULL,Ptitle,HEADER,(uint16_t) NULL,NULL);


}


void fB_Menu::defineDrow(const __FlashStringHelper* Ptitle){  // dummy row
	fB_Row *pR;
	pR = defineRow(NULL, Ptitle,NOACT,TITLE,NULL, (float) NULL);
	pR->status |= MARK;

}


void fB_Menu::defineJrow(uint16_t tTag,const __FlashStringHelper* Ptitle){// 
	defineRow(NULL, Ptitle,JPAGE,TITLE,tTag, (float) NULL);
	Page(tTag)->parentTag = mPage[pageCount-1].pTag;
}
void fB_Menu::defineArow(uint8_t  type,const __FlashStringHelper* Ptitle){// 
	defineRow(NULL, Ptitle,type,TITLE,NULL, (float) NULL);
}
void fB_Menu::defineRow(uint16_t mTag,const __FlashStringHelper* Ptitle,uint8_t  type,uint8_t  format){// 
	defineRow(mTag, Ptitle,type,format,(uint16_t) NULL, (float) NULL);
}

fB_Row * fB_Menu::defineRow(uint16_t mTag, const __FlashStringHelper* Ptitle,uint8_t  type,uint8_t  format,uint16_t tTag, float value){// general case, value defaults to NULL
	if(!passTog) {
		totalRows++;
		return NULL;
	}
	

	fB_Row *r;
	r = &mRow[rowCount++];  // get next row in array
	r->Ptitle =Ptitle;

	r->Ptext =NULL;
	r->ptitle = NULL;
	r->ptext = NULL;
	fB_Page *p;
	p = &mPage[pageCount-1]; // get last initialized page in array
	r->pPage = p;
	r->pDex = pageCount-1;
	r->tag = mTag;
	if(!p->pageRowCount)  p->pRow = r;  // first row for this page, set pRow
	//r->X = p->X ;
	r->Y = p->Y + (ROWHT) * p->pageRowCount; 
	p->pageRowCount++;
	p->farY =  (ROWHT) * p->pageRowCount;
	r->type = type;
	r->format = type; // default
	if(format) r->format = format;
	r->status = NOSTATUS;
	if(p->pTag == GPANEL) r->status = CREATED | INITED;
	r->value = value;
	r->tTag = tTag;
	return r;
 
}
//fB_Row*     defineRow(uint16_t mTag,uint8_t  type,char* text, uint8_t  format,uint16_t tTag, float value=0);

void fB_Menu::defineSystem()  {
	definePage(SYSTEM,F("SYSTEM"),HOME);
	defineJrow(CLOCK,F("CLOCK"));
	defineJrow(ALARM,F("ALARM"));
	defineJrow(VDIV,F("VDIV"));

	definePage(VDIV,F("VDIV"),SYSTEM);
	defineRow(VDCRD,F("CARD"),VDCRD,TEXT);
	//defineRow(VDCUT,NOACT,F("CUTOFF"),LAMP);
	defineRow(VDPIN,F("PIN"),VDPIN,TEXT);
	defineRow(VDROW,F("ROW"),NOACT,INT5);
	defineRow(VDCOL,F("COL"),NOACT,TEXT);
	defineRow(VDRES,F("RES"),VDRES,INT5);
	defineRow(VDGAT,F("GATE"),VDGAT,LAMP);
	defineRow(VDADC,F("ADC"),VDADC,INT5);
	defineArow(VDSET,F("SET"));

	if(passTog)Row(VDROW)->status |= MARK;
	if(passTog)Row(VDCOL)->status |= MARK;

}

void fB_Menu::defineGlobals()  {		
	definePage(GLOBALS,F("GLOBALS"),HOME );
	defineRow(NULL,F("VAR LIST"),JPAGE,TITLE,GLIST,PGUS);
	defineArow(GLAG,F("VAR LOG"));
	defineRow(NULL,F("SYS LIST"),JPAGE,TITLE,GLIST,PGSY);
	defineArow(GDEF,F("SYS LOG"));
	defineDrow(NULL);
	defineArow(GSAG,F("EEPROM STORE"));
	defineArow(GIAG,F("EEPROM READ"));
	defineJrow(FILES,F("FILES"));

	definePage(GLIST,F("GLIST"),GLOBALS);
	for(int i=0;i<min(brain.globalCount,MAXLISTROWS);i++) defineRow((uint8_t )GROW+i,NULL,(uint8_t )GROW,(uint16_t) GPANEL, (float) i);
	definePage(GPANEL,F("GPANEL"),GLIST);
	defineRow(GLOG,NULL,NOACT);
	defineRow(GINP,F("INPUT"),NOACT,VALUE);
	defineRow(GVAL,F("VALUE"),GADJ,VALUE);
	defineRow(GOPR,NULL,GARB,TEXT);
	defineRow(GFAC,F("FACTOR"),GADJ,VALUE);
	defineRow(GINC,F("INCR"),GINC,VALUE);
	//defineRow(GSET,GSET,F("SET"),TITLE);
	defineArow(GSET,F("SET"));
	if(passTog){
		Row(GLOG)->status |= MARK;
	}

definePage(FILES,F("FILES"),GLOBALS);
	for(int i=0;i<MAXLISTROWS;i++) defineRow((uint16_t)FROW+i,NULL,(uint8_t )FROW,(uint16_t) FPANEL, (float) i);
	definePage(FPANEL,NULL,FILES);
	defineRow(FDATE,F("TIME"),NOACT);
	defineRow(FSIZE,F("SIZE"),NOACT);
	defineRow(FDUMP,F("DUMP"),FDUMP);
	defineRow(FSTD,NULL,FSTD);
	defineRow(FARCH,F("ARCHIVE"),FARCH);
	if(passTog)Row(FDATE)->status |= MARK;
	if(passTog)Row(FSIZE)->status |= MARK;

}
void fB_Menu::defineStacks()  {
	definePage(STACK,F("STACK"),HOME);
	for(int i=0;i<MAXLISTROWS;i++) defineRow((uint16_t)SROW+i,NULL,NOACT,NULL,NULL,NULL);
}

//fB_Row*     defineRow(uint16_t mTag,uint8_t  type,char* text, uint8_t  format,uint16_t tTag, float value=0);

void fB_Menu::defineClock()  {

	definePage(CLOCK,F("CLOCK"),SYSTEM);
	defineRow(CLKYR,F("YEAR"),CLK,INT5);
	defineRow(CLKMO,F("MONTH"),CLK,INT5);
	defineRow(CLKDY,F("DAY"),CLK,INT5);
	defineRow(CLKHH,F("HOUR"),CLK,INT5);
	defineRow(CLKMM,F("MIN"),CLK,INT5);
	defineArow(CLKGET,F("GET USB"));
	defineArow(CLKSET,F("SET"));
	//Row(FDATE)->status |= MARK;


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
	//if(pCurrPage->pTag == VDIV) brain.pCard[(uint8_t )Row(VDCRD)->value]->VDcutoff(false);

//bug(F("Jump from %d  %P"),pCurrPage->pTag,pCurrPage->Ptitle);
	pCurrPage->hide();
	pP->show();
	pCurrPage = pP;
//dbug(F("Jump to %d  %P"),pCurrPage->pTag,pCurrPage->Ptitle);

}

void fB_Menu::checkButtonCode() {
	//fB_Page *p;
	//fB_Row *r;
	//p = pCurrPage;
	if(!buttonCode) return;
	dbug(F("Cbc cp %d"),pCurrPage->pTag);

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
		switch(p->pTag) {
			case HOME: return; break;
			case FILES: 	pListStart = &fListStart; totalLines = totalFiles; break;
			case STACK: 	pListStart = &sListStart; totalLines = brain.stackCount;break;
			case GLIST: 	
				if(p->type == PGSY) { pListStart = &gyListStart; totalLines = brain.gSysCount; }
				else { pListStart = &guListStart; totalLines = brain.gUsrCount; }
				break;
		}
		if(hand==RIGHT) {
			switch(p->pTag) {
				case FILES: 		
				case STACK: 	
				case GLIST: 
					if(	totalLines >  *pListStart+MAXLISTROWS) {
						*pListStart += MAXLISTROWS; 
						jumpPage(p->pTag); 
					}
						
					break;
			}
		}
		else {
			switch(p->pTag) {
				case FILES: 		
				case STACK: 	
				case GLIST: 	
					if(	*pListStart >= MAXLISTROWS) {
						*pListStart -= MAXLISTROWS;	
						jumpPage(p->pTag); 
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

	tft.currY = y1 +1;
	len = (int) ((x2-x1+1)/tft.cfont.x_size);
	maxlines = (int) ((y2-y1+1)/LINEHT);

	fat.restartDir();
	while(linecount < maxlines) {
		res = fat.findNextFile();
		if (res==NO_ERROR){
			sprintf(strBuffer,"%4.4s.%3s %d",fat.DE.basename,fat.DE.fileext,fat.DE.fileSize/1000);
			if(!linecount) tft.printLine(strBuffer,len);
			else tft.printNewLine(strBuffer,len);
			linecount++;
		}
		else {
			if(!linecount) tft.printNewLine("* NO FILE *",len);
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
		for(j=0;j<brain.totalLogs && k<brain.totalLogs;j++ ) {
			if(!strcmp(brain.Logs[j].name,mFile[i]->filename)) {
				mFile[i]->fTag = brain.Logs[j].tag;
				brain.Logs[j].pLog = mFile[i];
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

/*void fB_Page:: update() {
	int i;
	pageStackCount = 0;
	for(i=0;i < pageRowCount;i++)	{ pRow[i].update();pRow[i].show(REFRESH);}
	if(win.status & CREATED) { win.update(); win.show(REFRESH); }
}
*/
void fB_Page:: show() {
	int dr;
	uint8_t  i,x;
	uint8_t  rows = 0;  //  calculated rows on this page (may be less than default pageRowCount)
	uint8_t  count,listStart,format;
	float dv;

	menu.pCurrPage = this;

	switch(pTag) {
		case FILES:
			menu.getFileList();
			count = menu.totalFiles;
			listStart = menu.fListStart;
			break;
		case STACK:
			count = brain.stackCount;	
			listStart = menu.sListStart;
			break;
		case GLIST:
			if(type == PGSY){
				count = brain.gSysCount;
				listStart = menu.gyListStart;
			}
			else {
				count = brain.gUsrCount;
				listStart = menu.guListStart;
			}
			break;
		case CLOCK:
			if(!(brain.status & RTC)) return;
			rtc.now();
			menu.Row(CLKYR)->value = rtc.yOff+2000 ;
			menu.Row(CLKMO)->value = rtc.m ;
			menu.Row(CLKDY)->value = rtc.d ;
			menu.Row(CLKHH)->value = rtc.hh ;
			menu.Row(CLKMM)->value = rtc.mm ;
		
			menu.Row(CLKYR)->status |= (CREATED | INITED) ;
			menu.Row(CLKMO)->status |= (CREATED | INITED) ;
			menu.Row(CLKDY)->status |= (CREATED | INITED) ;
			menu.Row(CLKHH)->status |= (CREATED | INITED) ;
			menu.Row(CLKMM)->status |= (CREATED | INITED) ;
			break;
		case VDIV:{
	//dbug(F("ps brain totalCards %d"),brain.totalCards);
				if(brain.totalCards<2) menu.jumpPage(parentTag);
				uint8_t  index;
				index = (uint8_t )  menu.Row(VDCRD)->value;
				if(index <1 || index >= brain.totalCards) index = 1;
				fB_Card* pC;
				pC = brain.pCard[index];

				//brain.pCard[index]->VDcutoff(true);
				menu.Row(VDCRD)->Ptext= pC->Ptitle;
				menu.Row(VDCRD)->value= index;
				//menu.Row(VDCUT)->value=(float) brain.pCard[index]->PCF->digitalRead(CUTOFF);
				//menu.Row(VDPIN)->title= NULL;
				menu.Row(VDPIN)->value=0;
				menu.Row(VDRES)->value=0;
				menu.Row(VDADC)->value=0;
				
				for( int i=0;i<brain.totalPins;i++) {
					if(brain.pPin[i]->bcard->index == index) {
						menu.Row(VDPIN)->Ptext= brain.pPin[i]->Ptitle;
						menu.Row(VDPIN)->value= (float) i;
						break;
					}
				}
				menu.Row(VDROW)->value = brain.pPin[i]->getRow();
				if(brain.pPin[i]->getSide() == L) menu.Row(VDCOL)->Ptext = PstrRay[P_LEFT];
				else menu.Row(VDCOL)->Ptext = PstrRay[P_RIGHT];
				menu.Row(VDRES)->value = brain.pPin[i]->vdr;
				pC->openCDchan(brain.pPin[i]->getCpin());
				pC->VDselectR(pgm_read_byte(&VDRmap[brain.pPin[i]->vdr]));
	
				//menu.Row(VDCUT)->status |= (CREATED | INITED) ;
				menu.Row(VDCRD)->status |= (CREATED | INITED) ;
				menu.Row(VDPIN)->status |= (CREATED | INITED) ;
				menu.Row(VDRES)->status |= (CREATED | INITED) ;
				menu.Row(VDADC)->status |= (CREATED | INITED) ;
			}
			break;

	}

	switch(pTag) {
		case FILES:
		case STACK:
		case GLIST:
			if(count <= listStart) listStart = max(count - MAXLISTROWS,0);
			if(MAXLISTROWS < count - listStart ) rows = MAXLISTROWS;
			else rows = count - listStart;
			for( i=0;i<rows;i++) {
				switch(pTag) {
					case FILES:
						x = menu.fSort[i+listStart];
						menu.Row(FROW+i)->ptitle = menu.mFile[x]->filename;
						menu.Row(FROW+i)->value = (float) x;
						if(menu.mFile[x]->fTag != NULL) menu.Row(FROW+i)->status |= LOG; 
						else menu.Row(FROW+i)->status &= ~LOG; 
						break;
					case STACK:
						dv = brain.stack[i+menu.sListStart].value - (int) brain.stack[i+menu.sListStart].value;
						if(dv<0.001) format = INT5;
						else if(dv < .1) format = FLOAT2;
						else format = FLOAT1;
						menu.Row(SROW+i)->Ptitle = brain.stack[i+menu.sListStart].Ptitle;
						menu.Row(SROW+i)->value = brain.stack[i+menu.sListStart].value;	
						menu.Row(SROW+i)->format = format;
						menu.Row(SROW+i)->status |= (CREATED | INITED) ;
						break;
					case GLIST:
						uint8_t  index;
						if(type == PGSY) index = brain.gSys[i];
						else index = brain.gUsr[i];
						menu.Row(GROW+i)->Ptitle = brain.pGlobal[index]->Ptitle;
						menu.Row(GROW+i)->tTag = brain.pGlobal[index]->gTag;
						menu.Row(GROW+i)->value = brain.pGlobal[index]->getValue();
						menu.Row(GROW+i)->format = brain.pGlobal[index]->format;
						switch(brain.pGlobal[index]->format) {
							case TOGL:  menu.Row(GROW+i)->type = TOGL; break;
							case LAMP:  menu.Row(GROW+i)->type = NOACT; break;
							default:	menu.Row(GROW+i)->type = GROW; break;
						}
						menu.Row(GROW+i)->status |= (CREATED | INITED) ;
						break;
				}
			}
			dr = pageRowCount - (rows+1);
			pageRowCount = rows+1;
			if(dr > 0) farY -= dr * ROWHT;
			else farY = pageRowCount * ROWHT;
			break;

	}
	selectHeader();
	for(i=0;i < pageRowCount;i++) pRow[i].show();
	//if(win.status & CREATED)  win.show();
	status |= VISIBLE;
	//dbug(F("ps r %d i %d"),pageRowCount,i);

}
void fB_Page:: hide(uint8_t  flag) {
	pRow[currRowDex].deselect();
	for(int i=0;i < pageRowCount;i++)	pRow[i].status &= ~VISIBLE;
	//if(win.status & CREATED) win.status &= ~VISIBLE;
	currRowDex = 0;
	status &= ~VISIBLE;
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
	if( status & DISABLE) 	return;
	fB_Global* pG;
	int i;
	char bufferTitle[MAXCHARSTEXT+1];

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
	if( flag != HIDE) 	tft.resetDefColors();
	if( flag == HIDE ) 	tft.setAll2Bcolor();
	if( (status & MARK) || (status & LOG))	tft.setColor(FCOLOR,HCOLOR);
	if( flag != REFRESH && format != HEADER ) tft.print(STARTX +ROWTEXTX,Y,bufferTitle,MAXCHARSTEXT);

	switch(format) {
		case HEADER:
			sprintf(header,"< %s >",bufferTitle);
			tft.print(CENTER,Y,header,MAXCHARSTEXT);
			break;
		case STRIKE:
			tft.print( RIGHT, Y, menu.strikeStr);
			break;
		case INT5:
			if(status == NOSTATUS) tft.print( RIGHT, Y, menu.strikeStr);
			else tft.printInt(STARTX +MONX,Y,(long) value,6,RIGHT);
			break;
		case FLOAT1:
			if(status == NOSTATUS) tft.print( RIGHT, Y,menu.strikeStr);
			else tft.printFloat(STARTX +MONX,Y,value,1,RIGHT);
			break;
		case VALUE:  // default
		case FLOAT2:
			if(status == NOSTATUS) tft.print( RIGHT, Y, menu.strikeStr);
			else tft.printFloat(STARTX +MONX,Y,value,AR4_2,RIGHT);
			break;
		case TOGL:
		case LAMP: { 
				pG = Global(tTag);
				if(pG) {
					if(pG->flags & GPIN) pG->value = (float) Pin(tTag)->read();
					value = pG->value;
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
	//   value = index of the selected logfile, if coming from FLIST
	fB_Log *pF;   //  ptr to selected logfile
	fB_Global *pG;//  ptr to selected global, if coming from GLIST
	fB_Pin *pP;//   ptr selected PIN, if coming from GLIST and global has targeted pin
	pF = menu.mFile[(int)value];//   value = index of the selected logfile
	pG = Global(tTag);  
	pP = Pin(pG->gTag);  
	uint8_t  tstate;
	dbug(F("ACTION flag %d , tTag %d,  type %d"),flag,tTag,type);

	switch (flag)	{
		case RIGHT:
			switch(type) {
				case VDCRD: {
						uint8_t  index;
						index = (uint8_t )  value;
						//brain.pCard[index++]->VDcutoff(false);
						index++;
						if(index >= brain.totalCards) index=1; // 0 is BRAIN card, don't want that
						//brain.pCard[index]->VDcutoff(true);
						Ptext =  brain.pCard[index]->Ptitle;
						value = (float) index;
						show();
					}
					break;
				case VDPIN: {
						brain.pCard[(uint8_t ) menu.Row(VDCRD)->value]->VDgate(OFF);
						menu.Row(VDGAT)->value= LOW;
						menu.Row(VDGAT)->show();

						fB_Card* pC;
						pC= brain.pCard[(uint8_t ) menu.Row(VDCRD)->value];
						uint8_t  i,j,index;
						index = (uint8_t )  value;
						j = brain.totalPins;
						if(index+1 < j) i = index+1;
						else i = 0;
						for(;i<j;i++) {
							if( brain.pPin[i]->bcard->index== (uint8_t ) menu.Row(VDCRD)->value) {
								Ptext= brain.pPin[i]->Ptitle;
								value = (float) i;
								menu.Row(VDROW)->value = brain.pPin[i]->getRow();
								if(brain.pPin[i]->getSide() == L) menu.Row(VDCOL)->Ptext = PstrRay[P_LEFT];
								else menu.Row(VDCOL)->Ptext = PstrRay[P_RIGHT];
								menu.Row(VDRES)->value = brain.pPin[i]->vdr;
								pC->openCDchan(brain.pPin[i]->getCpin());
								pC->VDselectR(pgm_read_byte(&VDRmap[brain.pPin[i]->vdr]));

								//menu.Row(VDRES)->value = 0;
								//pC->openCDchan(0);
								break;
							}
							if(i == brain.totalPins-1) {
								i = 0;
								j= index+1;
							}
						}
						show();
						menu.Row(VDROW)->show();
						menu.Row(VDCOL)->show();
						menu.Row(VDRES)->show();


					}
					break;
				case VDRES: {
						brain.pCard[(uint8_t ) menu.Row(VDCRD)->value]->VDgate(OFF);
						menu.Row(VDGAT)->value= LOW;
						menu.Row(VDGAT)->show();

						uint8_t index = (uint8_t )  value;
						if(++index > MAXVDRDEX) index = 0;
						value = (float) index;
						brain.pCard[(uint8_t ) menu.Row(VDCRD)->value]->VDselectR(pgm_read_byte(&VDRmap[index]));
						show();
						}
					break;
				case VDSET:
						if(menu.Row(VDPIN)->ptitle != NULL) {
							brain.pPin[(uint8_t) menu.Row(VDPIN)->value]->vdr = (uint8_t) menu.Row(VDRES)->value;
						}
						pPage->selectHeader();
						break;
				case VDGAT:
					//putTagState(tTag,HIGH); // works for both Pin and Global tags
					pP = brain.pPin[(uint8_t) menu.Row(VDPIN)->value];
					if(pP->gate==OFF) { 
						pP->gate = ON;
						value = HIGH;
						brain.pCard[(uint8_t ) menu.Row(VDCRD)->value]->VDgate(ON);
						menu.Row(VDADC)->value = brain.pPin[(uint8_t) menu.Row(VDPIN)->value]->read();
					}
					else {
						pP->gate = OFF;
						value = LOW;
						brain.pCard[(uint8_t ) menu.Row(VDCRD)->value]->VDgate(OFF);
						menu.Row(VDADC)->value = 0;

					}
					show();
					menu.Row(VDADC)->show();
					break;
				case VDADC:
					//putTagState(tTag,HIGH); // works for both Pin and Global tags
					menu.Row(VDADC)->value = brain.pPin[(uint8_t) menu.Row(VDPIN)->value]->read();
					show();
					break;
					/*
				case VDCUT: {
						uint8_t  

					pCard(UTIL)->VDcutoff(true);
					value = (float) Card(UTIL)->PCF->digitalRead(CUTOFF);
					show();
					break;
*/
				case TOGL:
					//putTagState(tTag,HIGH); // works for both Pin and Global tags
					if(pG->value==LOW) { 
						value = HIGH;
						if(pG->flags & GPIN) if(pP) pP->write(pP->onVal);
					}
					else {
						pG->value=LOW; 
						value = LOW;
						if(pG->flags & GPIN) if(pP) pP->write(~(pP->onVal));
					}
					show();
					break;
				case PULSE:
					if((pG->flags & GPIN )&& pP) pP->pulse((unsigned int)value);
					show();
					break;
				case YPULSE:
					if((pG->flags & GPIN )&& pP) pP->YshiftPulse((unsigned int)value);
					show();
					break;
				case JPAGE:
					if(tTag==GLIST) menu.Page(tTag)->type = value; // do this here to fork page content
					menu.jumpPage(tTag);
					break;
				case FROW: {
						menu.Page(FPANEL)->pRow[0].ptitle = pF->filename;
						//menu.Page(FPANEL)->ptitle = pF->filename;
						pF->getAttributes();
						menu.Row(FDATE)->ptext = pF->dateStr;
						menu.Row(FSIZE)->ptext = pF->sizeStr;
						menu.Row(FDUMP)->value = value;
						menu.Row(FSTD)->value = value;
						menu.Row(FARCH)->value = value;
						if(status & LOG) {
							menu.Row(FARCH)->status &= ~DISABLE;  // file is logfile
							menu.Row(FSTD)->Ptext = PstrRay[P_STAMP];  
							menu.Row(FSTD)->type = FSTAMP;  
						}
						else {
							menu.Row(FARCH)->status |= DISABLE;  // file is other file
							menu.Row(FSTD)->Ptext = PstrRay[P_DELETE];  
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
				case GROW:{
						//pG = Global(tTag);//   value = index of the selected global
			//dbug("RA grow  %s",pG->tagStr);

				//menu.Page(GPANEL)->pRow[0].title = pG->tagStr;
						//sprintf(fbuffer,"%s.TXT",pG->tagStr);
////////////////////////menu.Row(GLOG)->title = brain.getLogName(pG->fTag);  // is this legal?
						if(!menu.Row(GLOG)->ptext)  menu.Row(GLOG)->ptext = menu.noLogStr ;
						menu.Row(GSET)->tTag = pG->gTag;
						menu.Row(GFAC)->tTag = pG->gTag;
						menu.Row(GFAC)->value = pG->factor;
						menu.Row(GFAC)->value = pG->factor;
						menu.Row(GVAL)->tTag = pG->gTag;
						menu.Row(GVAL)->value = pG->getValue();
						menu.Row(GVAL)->format = pG->format;
						menu.Row(GINC)->format = FLOAT2;
						menu.Row(GINC)->value = 0.01;
						menu.Row(GINP)->value = pG->value;
						menu.Row(GOPR)->tTag = pG->gTag;
						menu.Row(GINP)->status |= MARK;

						switch(pG->format) {
							case INT5:   menu.Row(GINC)->value = 1; break;
							case FLOAT1: menu.Row(GINC)->value = .1; break;
							case FLOAT2: 
							default:     menu.Row(GINC)->value = .01; break;
						}

						if(!(pG->flags & GINPUT))  {
							menu.Row(GINP)->format = STRIKE;
							menu.Row(GFAC)->format = STRIKE;
							menu.Row(GOPR)->format = STRIKE;
							menu.Row(GFAC)->type = NOACT;
							menu.Row(GOPR)->type = NOACT;
							menu.Row(GVAL)->type =  GADJ;
							menu.Row(GFAC)->status |= MARK;
							menu.Row(GOPR)->status |= MARK;
							menu.Row(GVAL)->status &= ~MARK;
						}
						else {
							menu.Row(GFAC)->status &= ~MARK;
							menu.Row(GOPR)->status &= ~MARK;
							menu.Row(GVAL)->status |= MARK;

							menu.Row(GINP)->format = pG->format;
							menu.Row(GOPR)->format = TEXT;
							menu.Row(GFAC)->format = pG->format;
							menu.Row(GFAC)->type = GADJ;
							menu.Row(GOPR)->type = GARB;
							menu.Row(GVAL)->type = NOACT;
							if(pG->flags & GBIAS)  {
								menu.Row(GOPR)->ptext = menu.biasStr; 
								menu.Row(GINC)->format = INT5;
							}
							else {
								menu.Row(GOPR)->ptext = menu.ampStr;
								if(!pG->factor) pG->factor = 1;
								menu.Row(GINC)->format = FLOAT2;
							}
 					    }
						menu.jumpPage(GPANEL);

					}
					break;
				case GARB:	if (!strcmp(ptext,menu.biasStr)) {
								ptext = menu.ampStr;
								menu.Row(GFAC)->value = 1;
							}
							else {
								ptext = menu.biasStr;
								menu.Row(GFAC)->value = 0;
							}
							show();
							menu.Row(GFAC)->show();
							break;
				case GINC:	value*=10;
							if(value> 1000) value=1000;
							show();
							break;
				case GADJ:
							value += menu.Row(GINC)->value ;
							show();
							break;
				case GSET:	
							if(pG->flags & GINPUT)  {
								if(!strcmp(menu.Row(GOPR)->ptext,menu.biasStr))  {
									menu.Row(GVAL)->value = menu.Row(GINP)->value + menu.Row(GFAC)->value + ROUNDOFF;
									pG->flags |= GBIAS;
								}
								else  {
									menu.Row(GVAL)->value = menu.Row(GINP)->value * menu.Row(GFAC)->value  + ROUNDOFF;
									pG->flags &= ~GBIAS;
								}
								pG->factor = menu.Row(GFAC)->value;
							}
							else pG->value = menu.Row(GVAL)->value;
							pPage->selectHeader();
							menu.Row(GVAL)->show();
							break;
				case SET:
							pG->value = menu.Row((uint16_t)value)->value;
							pPage->selectHeader();
							break;
				case VALUE:
							//update();
							show();
							break;
				case CLKSET:
					dbug(F("clkset"));
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

				case GDEF:	brain.createGdefLog() ; pPage->selectHeader();break;
				case GIAG:	brain.EEinitGlobals(); pPage->selectHeader();break;
				case GSAG:	brain.EEwriteGlobals(); pPage->selectHeader();break;
				case GLAG:	
							for(int i = 0;i<brain.totalLogs;i++) brain.Logs[i].pLog->writeData();
							pPage->selectHeader();
							break;

			} // END SWITCH type
			break;

		case LEFT: 
			switch(type) {

				case TOGL:
					pG->value=LOW; 
					value = LOW;
					if(pG->flags & GPIN) if(pP) pP->write(~(pP->onVal));
					show();
					break;
				case GADJ:
					value -= menu.Row(GINC)->value ;
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
				case GINC:
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
	status |= SELECTED;
}

void fB_Row:: deselect() {
	tft.setAll2Bcolor();
	tft.drawHLine(STARTX,Y+ROWHT-8,MAXPIXELWID);
	tft.resetDefColors();	
	status &= ~SELECTED;

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
	
	if(!passTog) {
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