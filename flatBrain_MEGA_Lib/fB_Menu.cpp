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
	sListStart = 0;
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

	Tag(FPANEL)->flag16 |= TTITLE;  // for filename


}

void fB_Menu::jumpPage(uint16_t tag) {
	fB_Tag *pT;
	pT = Tag(tag);
	if(!pT ||!( pT->flag16 & PAGE)) return;
	dbug(F("Jump from %P to tag:%d , title:%P"),curr.pP->Ptitle,pT->tag,pT->Ptitle);
	if(curr.pageTag) clearPage();
	showPage(tag);

}
void fB_Menu::jumpPage(fB_Tag *pT) {
	if(!pT ||!( pT->flag16 & PAGE)) return;
	dbug(F("Jump from %P to tag:%d , title:%P"),curr.pP->Ptitle,pT->tag,pT->Ptitle);
	if(curr.pageTag) clearPage();
	showPage(pT->tag);

}

void fB_Menu::checkButtonCode() {
	if(!buttonCode) return;

	switch(buttonCode) {
		case 1: context(RIGHT);break;
		case 2: context(LEFT);break;
		case 3: curr.prevSwitch(); break;
		case 4: curr.nextSwitch(); break;
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
		}
		if(hand==RIGHT) {
			switch(curr.pageTag) {
				case FILES: 		
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
					if(	*pListStart >= MAXLISTROWS) {
						*pListStart -= MAXLISTROWS;	
						jumpPage(curr.pageTag); 
						return;
					}
			}				
		jumpPage(curr.parentTag);
		}
	}
	else curr.tag(curr.rowDex)->action(hand);
}

void fB_Curr:: nextSwitch() {
	int i;
	fB_Tag* pT;
	for(i=0;i <= rowCount;i++)	{
		pT = tag(i);
dbug(F("NX %P,  f16:%x , a:%L"),pT->Ptitle, pT->flag16,pT->getAction()) ;
//Serial.println(pT->getAction(),DEC);
		if((pT->flag16 & DISABLE || pT->getAction() == NOACT)  && !(pT->flag16 & PAGE)) continue;
//dbug(F("NX2 %P,  f16:%x , a:%L"),pT->Ptitle, pT->flag16,pT->getAction()) ;
		if( i > rowDex) {
//dbug(F("NX3 %P,  f16:%x , a:%L"),pT->Ptitle, pT->flag16,pT->getAction()) ;
			deselectRow();
			rowDex = i;
			selectRow();
			break;
		}
	}
}
void fB_Curr:: prevSwitch() {
	int i;
	fB_Tag* pT;
	if(!rowDex) return;
	for(i=rowCount;i >=0 ;i--)	{
		pT = tag(i);
		if((pT->flag16 & DISABLE || pT->getAction() == NOACT)  && !(pT->flag16 & PAGE)) continue;
		if( i < rowDex) {
			deselectRow();
			rowDex = i;
			selectRow();
			break;
		}
	}
}
void fB_Menu:: selectHeader() {
	curr.deselectRow();
	curr.rowDex = 0;			
	curr.selectRow();
}

void fB_Menu:: clearPage(uint8_t  full) {
	//curr.deselectRow();
	if(!full) tft.clear(curr.farY);
	else tft.clear();	
	curr.rowDex = 0;

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
dbug(F("gfl tf: %d"), totalFiles);
	mFile = (fB_Log **) malloc(sizeof(fB_Log*) * totalFiles);
	fat.restartDir();
	i=0;
	k=0;
	//while ((fat.findNextFile()== NO_ERROR) && i<totalFiles){
	while (!fat.findNextFile() && i<totalFiles) {
		dbug(F("gfl %s"), fat.DE.filename);

		mFile[i]= new fB_Log(NULL,fat.DE.filename); // non-active ( non-.txt) tags are NULL
		for(j=0;j<logFileCount && k<logFileCount;j++ ) {
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


void fB_Menu:: showPage(uint16_t tag, uint8_t pageOption) {
	int dr;
	uint8_t  i,x;
	uint8_t  rows = 0;  //  calculated rows on this page (may be less than default pageRowCount)
	uint8_t  count,listStart;
	float dv;
	fB_Tag *pT;
	if(pageOption != REFRESHPAGE) {
		Tag(HEADER)->Ptitle = curr.pP->Ptitle; 
		curr.deselectRow();
		curr.rowDex = 0;		
		menu.selectHeader();
		curr.setCurrPage(tag);

		switch(tag) {
			case FILES:
				menu.getFileList();
				count = menu.totalFiles;
				listStart = menu.fListStart;
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
			case PINS:
				if(cardCount<2) jumpPage(SYSTEM);//jumpPage(curr.parentTag);
				pinPageConstruct(0);
				break;

		}

		switch(tag) {
			case FILES:
				if(count <= listStart)listStart = max(count - MAXLISTROWS,0);
				if(MAXLISTROWS < count - listStart ) rows = MAXLISTROWS;
				else rows = count - listStart;
				for( i=0;i<rows;i++) {
					switch(tag) {
						case FILES:
							pT = curr.tag(i+1);
							x = menu.fSort[i+listStart];
							//pT->iVal = x;
							pT->ptitle = menu.mFile[x]->filename;
							pT->fTag = x; 
					dbug(F("sp %P  , %s"), curr.pP->Ptitle,pT->ptitle);
							// use fTag to store array index
							if(menu.mFile[x]->fTag == NULL) pT->flag16 |= MARK; // |=log
							else pT->flag16 &= ~MARK; 
							break;
					}
				}
				curr.putRowCount(rows);
				/*
				dr = curr.rowCount - (rows+1); // for page tags, pagerow count is stored in iVal
				iVal = rows+1;
				if(dr > 0) farY -= dr * ROWHT;
				else farY = iVal * ROWHT;
				*/
				break;
		}

	}

	for(int i = 0; i <= curr.rowCount; i++)	curr.tag(i)->showRow(i,pageOption);
}


 ///////////////////////////////////////// ROW METHODS //////////////////////////////////////////////

uint16_t fB_Tag::getY(uint8_t rowIndex) {  return ( STARTY + (ROWHT) * rowIndex); }

void fB_Menu::refreshRow(uint16_t tag) {  
	uint8_t rowIndex;
	fB_Tag* pT;
	if(!tag){
		rowIndex = curr.rowDex;
		pT = curr.tag(rowIndex); 
	}
	else 	{
		rowIndex = curr.row(tag);
		pT = Tag(tag);
	}
	pT->showRow(rowIndex);
}

void fB_Tag::showRow(uint8_t  rowIndex, uint8_t  option) {  //when option == REFRESH, only right col data is updated
dbug(F("SR %P f16: 0x%x"),Ptitle, flag16);

	int i=0;
	uint32_t format;
	char Pbuffer[MAXCHARSTEXT+1];
	char* pTitleText;
	tft.resetDefColors();
	if( (flag16 & DISABLE)) 	tft.setAll2Bcolor();
	else if(flag16 & MARK)	tft.setColor(FCOLOR,HCOLOR);
	format = getFormat();
//dbug(F("SR2 %P "),Ptitle);

	if(option != REFRESHPAGE) {
		if(!(flag16 & TTITLE)) pTitleText = getPtext(Ptitle,Pbuffer); // title points to Ptitle (usual case)
		else pTitleText = ptitle; // title points to text field
		if(!rowIndex) {  // row is page header
			char bufferTitle[MAXCHARSTEXT+1];
			sprintf(bufferTitle,"< %s >",pTitleText);
			tft.print(CENTER,getY(rowIndex),bufferTitle,MAXCHARSTEXT);
			return;
		} 
	}
	if( option != REFRESHPAGE || format == TEXT || format == PTEXT || flag16 & TTITLE) tft.print(STARTX +ROWTEXTX,getY(rowIndex),pTitleText,MAXCHARSTEXT);
		//dbug(F("SR format H: 0x%J"),format);
		//Serial.println(format,DEC);
		//Serial.println(format,HEX);

	if(format == BLAMP) {
		//dbug(F("SR %P ival:%d "),Ptitle,iVal);
			int x = STARTX +ROWSTATEX; int y = getY(rowIndex)+ROWSTATEY+2;
			tft.drawCircle(x,y,8);
			if(iVal) {
				tft.setColor(FCOLOR,GCOLOR);
				tft.fillCircle(x,y,6);
			}
			else  {
				tft.setAll2Bcolor();
				tft.fillCircle(x,y,6);
				tft.resetDefColors();
			}
		//}
	}
	else {		
		
		switch(format) {

			case INT5:
				if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex),getPstr(P_STRIKE,Pbuffer));
				else tft.printInt(STARTX +MONX,getY(rowIndex),iVal,6,RIGHT);
				break;
			case FLOAT1:
				if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex),getPstr(P_STRIKE,Pbuffer));
				else tft.printFloat(STARTX +MONX,getY(rowIndex),dVal->value,1,RIGHT);
				break;
			case FLOAT2:
				if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
				else tft.printFloat(STARTX +MONX,getY(rowIndex),dVal->value,AR4_2,RIGHT);
				break;
				break;
			case TEXT:
				if(flag16 & LJUST) 	tft.print( LEFT, getY(rowIndex), ptext);
				else 	tft.print( RIGHT, getY(rowIndex), ptext);
				break;
			case PTEXT:
				getPtext(Ptext,Pbuffer);
				if(flag16 & LJUST) 	tft.print(  LEFT, getY(rowIndex),Pbuffer );
				else 	tft.print(  RIGHT, getY(rowIndex),Pbuffer );
				break;
		}
	}
	tft.resetDefColors();
	flag16 |= VISIBLE;
}

void fB_Menu::pinPageConstruct(uint8_t startDex, uint8_t hand) {
		int i;
		fB_Tag* pT;
		Tag(PNPIN)->Ptext=NULL;
		Tag(PNPIN)->fTag = startDex; //  fTag avail to store index
		Tag(PNCRD)->Ptext= NULL;
		Tag(PNROW)->iVal = NULL;
		Tag(PNCOL)->Ptext = NULL;
		Tag(PNTOG)->iVal= LOW;
		Tag(PNADC)->iVal=0;

		if(hand == RIGHT) {
			for( i = startDex+1;i<tagCount;i++) {
				pT = &tagRay[i];
				if(pT->pin) break;
			}
			if(i == tagCount) {
				for( i = 0;i<tagCount;i++) {
					pT = &tagRay[i];
					if(pT->pin) break;
				}
			}
		}
		else {
			for( i =startDex-1 ;i>=0;i--) {
				pT = &tagRay[i];
				if(pT->pin) break;
			}
			if(i < 0) {
				for( i =tagCount-1 ;i>=0;i--) {
					pT = &tagRay[i];
					if(pT->pin) break;
				}
			}
		}
		pT = &tagRay[i];  // pT is Pointer to PIN
		if(!pT->pin) return;
		uint8_t rowSide;
		pT->getRowSide(rowSide); // stuff row,side encoded byte into this variable
		Tag(PNPIN)->Ptext=pT->Ptitle;
		Tag(PNPIN)->fTag = i; //  fTag avail to store index
	//dbug(F("%P tag->fTag: %d"),pT->Ptitle,Tag(PNPIN)->fTag);
		Tag(PNTOG)->iVal= LOW;
		Tag(PNCRD)->Ptext= Card(pT->getCtag())->Ptitle;
		Tag(PNADC)->iVal=0;
		Tag(PNROW)->iVal = rowSide & 0x1F;
		if((rowSide >> 7) == COL_L) Tag(PNCOL)->Ptext = PstrRay[P_LEFT];
		else Tag(PNCOL)->Ptext = PstrRay[P_RIGHT];
		Tag(PNTOG)->putAction(PINTOG);
		if( pT->getMode() == IO_D) {
			Tag(PNADC)->flag16 |= DISABLE;
			Tag(PNTOG)->Ptitle=PstrRay[P_TOGGLE];
			if(pT->isLatched()) Tag(PNTOG)->iVal = HIGH;
			else Tag(PNTOG)->iVal = LOW;
		}
		else {
			Tag(PNADC)->flag16  &= ~DISABLE;
			Tag(PNTOG)->Ptitle = PstrRay[P_GATE];
			Tag(PNTOG)->putAction(TGATE);
			Card(pT->getCtag())->openCDchan(pT->getCpin());
			if(pT->getOnVal() == PGATE) {
				Tag(PNTOG)->iVal= HIGH;
				Tag(PNADC)->iVal= pT->read();
			}
			else {
				Tag(PNADC)->iVal= 0;
				Tag(PNTOG)->iVal= LOW;
			}
		}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void fB_Tag::action(uint8_t  hand) {
	if(flag16 & DISABLE ) return;
	char Pbuffer[7];
	fB_Tag *pT;
	fB_Log *pF =  NULL;   //  ptr to  logfile
	dbug(F("ra %P ,  cp: %P , iVal:%d"),Ptitle,curr.pP->ptitle,iVal);

	if(curr.pageTag == FPANEL || curr.pageTag == FILES) {
		pF = menu.mFile[fTag];
		if(pF) pF->getAttributes();
	}
	switch(curr.pageTag) {
		case FILES:	Tag(FPANEL)->ptitle = pF->filename;
					Tag(FDATE)->ptitle = dateStr;
					Tag(FSIZE)->ptext = sizeStr;
					Tag(FDUMP)->fTag = fTag; //iVal = index of the selected logfile
					Tag(FSTD)->fTag = fTag;
					Tag(FARCH)->fTag = fTag;
					//if(flag16 & LOG) {
						//Tag(FARCH)->flag16 |= VISIBLE;  // file is logfile
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
	}

	switch (hand)	{
		case RIGHT:
			//check page jump first 
			if(flag16 & PAGE) {	menu.jumpPage(tag); return; }
			// then check tag
			switch(tag) {
				case PNPIN: 
					menu.pinPageConstruct(Tag(PNPIN)->fTag);
					menu.refreshRow();
					menu.refreshRow(PNCRD);
					menu.refreshRow(PNROW);
					menu.refreshRow(PNCOL);
					menu.refreshRow(PNTOG);
					menu.refreshRow(PNADC);
					return;
				case PNADC:
					pT = &tagRay[ Tag(PNPIN)->fTag];
					Tag(PNADC)->iVal = pT->read();
					menu.refreshRow();
					return;
				case FSTAMP:
					dbug(F("ra stamp1"));

					pF->writeData();
										dbug(F("ra stamp2"));

					pF->getAttributes();
										dbug(F("ra stamp3"));

					menu.selectHeader();
										dbug(F("ra stamp4"));

					//Tag(FDATE)->text = pF->dateStr; // new atts
					//Tag(FSIZE)->text = pF->sizeStr;
					Tag(FDATE)->ptext = dateStr; // new atts
										dbug(F("ra stamp5"));

					Tag(FSIZE)->ptext = sizeStr;
										dbug(F("ra stamp6"));

					menu.refreshRow(FDATE);
										dbug(F("ra stamp7"));

					menu.refreshRow(FSIZE);
					return;
				case FARCH:
					pF->archive();
					//if(pF->archive()){
						//menu.selectHeader();
						//Tag(FDATE)->text = pF->dateStr; // new atts
						//Tag(FSIZE)->text = pF->sizeStr;
						//Tag(FDATE)->ptext = dateStr; // new atts
						//Tag(FSIZE)->ptext = sizeStr;
						//menu.refreshRow(FDATE);
						//menu.refreshRow(FSIZE);
					//}
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
					rtc.yOff = (uint8_t ) (Tag(CLKYR)->iVal - 2000);
					rtc.m = (uint8_t ) Tag(CLKMO)->iVal ;
					rtc.d = (uint8_t ) Tag(CLKDY)->iVal ;
					rtc.hh = (uint8_t ) Tag(CLKHH)->iVal ;
					rtc.mm = (uint8_t ) Tag(CLKMM)->iVal ;
					rtc.adjust();
					menu.showPage(curr.pP->tag,REFRESHPAGE);
					break;
				case CLKGET:
					rtc.set(__DATE__,__TIME__);
					menu.showPage(curr.pP->tag,REFRESHPAGE);
					break;
				case CLKYR: 
					iVal += 1;
					menu.refreshRow();
					return;
				case CLKMO: 
					if(iVal < 12) iVal += 1;
					else iVal = 1;
					menu.refreshRow();
					return;
				case CLKDY: 
					if(iVal < 31) iVal += 1;
					else iVal = 1;
					menu.refreshRow();
					return;
				case CLKHH: 
					if(iVal < 23) iVal += 1;
					else iVal = 1;
					menu.refreshRow();
					return;
				case CLKMM: 
					if(iVal < 59) iVal += 1;
					else iVal = 1;
					menu.refreshRow();
					return;

				//case TLAS:	createGdefLog() ; pPage->selectHeader();break;
				case TIAT:	record.EEinitTags(); menu.selectHeader();return;
				case TSAT:	record.EEwriteTags(); menu.selectHeader();return;
				case TLAU:	
							for(int i = 0;i<logFileCount;i++) logRay[i].pLog->writeData();
							menu.selectHeader();
							return;

			} // END SWITCH on Tag

			switch(getAction()) {
				case TGATE:
					pT = &tagRay[Tag(PNPIN)->fTag];
					if(iVal == LOW) iVal = HIGH;
					else iVal = LOW;
					Card(pT->getCtag())->AnalogGate(iVal);
					if(iVal == HIGH) {
						putOnVal(PGATE);
						Tag(PNADC)->iVal = pT->read();
					}
					else {
						putOnVal(LOW);
						Tag(PNADC)->iVal = 0;
					}
					menu.refreshRow();
					menu.refreshRow(PNADC);
					return;
				case PINTOG:

					pT = &tagRay[ Tag(PNPIN)->fTag];

					if(iVal==LOW) { 
						iVal = HIGH;
						pT->write(getOnVal());
					}
					else {
						iVal = LOW;
						pT->write(~getOnVal());
					}
					menu.refreshRow();
					return;
				case PULSE:
					if(pin)  pulse(PULSEMSECS);
					menu.refreshRow();
					return;
				case SHFTPULSE:
					if(pin) YshiftPulse(PULSEMSECS);
					menu.refreshRow();
					return;
				case INCR:
					iVal++;
					menu.refreshRow();
					return;
				case UPDATE:
					if(pin) read();
					menu.refreshRow();
					return;
			} // END SWITCH on Action
			break; 	// END SWITCH on Right

		case LEFT:
		
			switch(tag) {
				case PNPIN: 
					menu.pinPageConstruct(Tag(PNPIN)->fTag,LEFT);
					menu.refreshRow();
					menu.refreshRow(PNCRD);
					menu.refreshRow(PNROW);
					menu.refreshRow(PNCOL);
					menu.refreshRow(PNTOG);
					menu.refreshRow(PNADC);
					return;
			}
			break;
	}// END SWITCH ON HAND

}






//void fB_Tag::hideRow(uint8_t rowIndex) {
	//flag16 &= ~VISIBLE;
	//tft.setAll2Bcolor();
	//showRow(HIDE);
	//tft.resetDefColors();
	//flag16 &= ~VISIBLE;
//}

/*
void fB_Tag::frame() {

	tft.drawRect(STARTX,getY() ,STARTX + MAXPIXELWID,getY() + ROWHT);
}

void fB_Tag::unframe() {
	tft.setAll2Bcolor();
	tft.drawRect(STARTX,getY() ,STARTX + MAXPIXELWID,getY() + ROWHT);
	tft.resetDefColors();
}
*/
void fB_Tag::clearRow(uint8_t rowIndex) {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,getY(rowIndex)+1,STARTX +MAXPIXELWID-1,getY(rowIndex)+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	flag16 &= ~VISIBLE;


}
void fB_Tag::clearRow1(uint8_t rowIndex) {
	tft.setAll2Bcolor();
	tft.fillRect(STARTX +1,getY(rowIndex)+1,STARTX +GTITLEX-10,getY(rowIndex)+ROWHT-1,tft.bColor);
	tft.resetDefColors();
	flag16 &= ~VISIBLE;


}


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

				case PINTOG:
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
