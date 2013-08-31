#include "fB_Include.h"

uint8_t fB_Menu::dPlaces(double value, uint8_t size) {  // returns number if visible decimal places in double if format is "size" chars and point is one character width.
	int j = 0;
	double dint,test;

	while(true) {
		test = value/pow(10.0,(double)j);
		if(test < 1) break;
		j++;
	}
	return (size - ( j + 1));
}

char *fB_Menu::d2str(double value, uint8_t size, char* buffer) {//formats double to string of size length, strips leading zero
	uint8_t places, skipz = 0,i;
	char c;
	places = dPlaces(value,size);
	//dbug(F("d2str1 v:%f  p:%d"),value,places);
	doubleToStr(value,places,buffer);
	return buffer;
}

/*	
///use this code if youw want to strip leading zero 
char tempbuf[MAXCHARSLINE];
	
	places = dPlaces(value,size);
	dbug(F("d2str1 v:%f  p:%d"),value,places);
	doubleToStr(value,places,tempbuf);
	if(value < 1.0) skipz = 1;
	for ( i=0; i<size;i++) {
		if(i>= strlen(tempbuf)) buffer[i] = '0'; // pad 
		else buffer[i] = tempbuf[i+skipz]; // skip leading zero
	}
	buffer[i]= '\0';
dbug(F("d2str3 v:%f t:%s, s:%s"),value,tempbuf,buffer);
	return buffer;
*/

char* fB_Menu::doubleToStr(double value, int places,char *buffer) {
   // this is used to cast digits 
   int digit,dhit = 0;
   double tens = 0.1;
   int tenscount = 0;
   int i,j=0,k=0,abase= 48;
   double tempdouble = value;
   char dBuffer[places+1];

     // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
   // if this rounding step isn't here, the value  54.321 prints as 54.3209

   // calculate rounding term d:   0.5/pow(10,places)  
   double d = 0.5;
   if (value < 0)
     d *= -1.0;
   // divide by ten for each decimal place
   for (i = 0; i < places; i++)
     d/= 10.0;    
   // this small addition, combined with truncation will round our values properly 
   tempdouble +=  d;

   // first get value tens to be the large power of ten less than value
   // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

   if (value < 0)   tempdouble *= -1.0;
   while ((tens * 10.0) <= tempdouble) {
     tens *= 10.0;
     tenscount += 1;
   }
   if (value < 0) buffer[j++] = '-';// write out the negative if needed
   if (tenscount == 0) buffer[j++] = abase +0;
   for (i=0; i< tenscount; i++) {
     digit = (int) (tempdouble/tens);
     buffer[j++] = abase +digit;
     tempdouble = tempdouble - ((double)digit * tens);
     tens /= 10.0;
   }
   if (places <= 0){ // if no places after decimal, stop now and return
		buffer[j] = '\0';
		return buffer;
   }
   k = j;
   buffer[j++] = '.';// otherwise, write the point and continue on
   for (i = 0; i < places; i++) {   // write out each decimal place shifting digits into the ones place and writing the truncated value
     tempdouble *= 10.0; 
     digit = (int) tempdouble;
     buffer[j++] = abase + digit;
     tempdouble = tempdouble - (double) digit;      // once written, subtract off that digit
	 if(digit) dhit = 1;
   }
   if(!dhit) j=k;  // truncate if all decimals are zero
   buffer[j] = '\0';
   return buffer;
}

void fB_Menu::init(){

	fListStart = 0;
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
	#define P_INPUT 5
	createPstr("INPUT");
	#define P_AMP 6
	createPstr("AMP");
	#define P_STRIKE 7
	createPstr("----");
	#define P_TOGGLE 8
	createPstr("TOGGLE");
	#define P_GATE 9
	createPstr("GATE");
	#define P_LOGS 10
	createPstr("LOGS");

	Tag(FPANEL)->flag16 |= TTITLE;  // for filename


}

void fB_Menu::jumpPage(uint16_t tag) {
	fB_Tag *pT;
	pT = Tag(tag);
	if(!pT ||!( pT->flag16 & PAGE)) return;
	//dbug(F("Jump from %P to tag:%d , title:%P"),curr.pP->Ptitle,pT->tag,pT->Ptitle);
	if(curr.pageTag) clearPage();
	showPage(tag);

}
void fB_Menu::jumpPage(fB_Tag *pT) {
	if(!pT ||!( pT->flag16 & PAGE)) return;
	//dbug(F("Jump from %P to tag:%d , title:%P"),curr.pP->Ptitle,pT->tag,pT->Ptitle);
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

	if(!curr.rowDex) { // page header selected
		switch(curr.pageTag) {
			case HOME:		return; 
			case LOGS: 	pListStart = &fListStart; totalLines = rec.fileCount; break;
		}
		if(hand==RIGHT) {  // move forward only in cases of multi-page output
			switch(curr.pageTag) {
				case LOGS: 		
					if(	totalLines >  *pListStart+MAXLISTROWS) {
						*pListStart += MAXLISTROWS; 
						jumpPage(curr.pageTag);
						return;
					}
			}
		}
		else { /// LEFT,  back to parent page, unless in middle of multi-page output
			switch(curr.pageTag) {
				case LOGS: 		
					if(	*pListStart >= MAXLISTROWS) {
						*pListStart -= MAXLISTROWS;	
						jumpPage(curr.pageTag); 
						return;
					}
			}				
		jumpPage(curr.parentTag); // back to parent page
		}
	}
	else curr.tag(curr.rowDex)->action(hand); // not header row, go to row actions
}

void fB_Curr:: nextSwitch() {
	int i;
	fB_Tag* pT;
	for(i=0;i <= rowCount;i++)	{
		pT = tag(i);
		if((pT->flag16 & HIDE || pT->getAction() == NOACT)  && !(pT->flag16 & PAGE)) continue;
//dbug(F("NX2 %P,  f16:%x , a:%L"),pT->Ptitle, pT->flag16,pT->getAction()) ;
		if( i > rowDex) {
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
		if((pT->flag16 & HIDE || pT->getAction() == NOACT)  && !(pT->flag16 & PAGE)) continue;
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

void fB_Menu:: showPage(uint16_t tag, uint8_t pageOption) {
	int dr;
	uint8_t  i,x;
	uint8_t  rows = 0;  //  calculated rows on this page (may be less than default pageRowCount)
	uint8_t  count,listStart;
	float dv;
	fB_Tag *pT;
	char Pbuffer[4];

	if(pageOption != REFRESHPAGE) {
		//Tag(HEADER)->Ptitle = curr.pP->Ptitle; 
		curr.deselectRow();
		curr.rowDex = 0;		
		curr.setCurrPage(tag);
		Tag(HEADER)->Ptitle = curr.pP->Ptitle; 
//dbug(F("SP %P t:%d rc:%d, cdx:%d"),curr.pP->Ptitle, curr.pageTag,curr.getRowCount(),curr.rowDex);

		switch(tag) {
			case SYSTEM:
				menu.fListStart = 0 ;
				Tag(LOGS)->flag16 &= ~LOCAL; // default to log file list ( LOCAL ==1 displays archives
				Tag(LOGS)->Ptitle = PstrRay[P_LOGS];
				break;
			case LOGS:
				if(Tag(LOGS)->flag16 & LOCAL)  rec.buildFileRay(P("A"));
				else  rec.buildFileRay(P("LOG"));
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
			case APINS:
				if(cardCount<2) jumpPage(SYSTEM);//jumpPage(curr.parentTag);
				pinPageConstruct(IO_A,0);
				break;
			case DPINS:
				if(cardCount<2) jumpPage(SYSTEM);//jumpPage(curr.parentTag);
				pinPageConstruct(IO_D,0);
				break;

		}

		switch(tag) {
			case LOGS:
				listStart = menu.fListStart;
				if(rec.fileCount <= listStart)listStart = max(rec.fileCount - MAXLISTROWS,0);
				if(MAXLISTROWS < max(rec.fileCount - listStart,0) )	rows = MAXLISTROWS;
				else rows = rec.fileCount - listStart;
				for( i=0;i<rows;i++) {
					pT = curr.tag(i+1); // step through each row on page
					pT->buf8 = rec.sortRay[i+listStart];	// use buf to store FAT index
///dbug(F("SP %s, fc:%d, r:%d, b8:%d"),rec.filename,rec.fileCount,i+1, pT->buf8);

				}
				curr.putRowCount(rows);
				free(rec.sortRay);
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
	menu.selectHeader();

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
//dbug(F("SR %P t:%d , f16: 0x%x"),Ptitle, tag,flag16);

	int i=0;
	uint32_t format;
	char Pbuffer[MAXCHARSLINE+1];
	char* pTitleText;
	tft.resetDefColors();
	if( (flag16 & HIDE)) 	tft.setAll2Bcolor();
	else if(flag16 & MARK)	tft.setColor(FCOLOR,HCOLOR);
	format = getFormat();
//dbug(F("SR2 %P "),Ptitle);
	

	if(option != REFRESHPAGE) {
		if(!(flag16 & TTITLE)) pTitleText = getPtext(Ptitle,Pbuffer); // title points to Ptitle (usual case)
		else {
			if(curr.pageTag == LOGS && rowIndex) {
					strcpy(Pbuffer,rec.fileFind(buf8)); 
					pTitleText = Pbuffer;
			}
			else pTitleText = ptitle; // title points to text field
		}
		if(!rowIndex) {  // row is page header
			char bufferTitle[MAXCHARSLINE+1];
			if(strlen(pTitleText) > MAXCHARSLINE - 4) 	sprintf(bufferTitle,"<%s>",pTitleText);
			else sprintf(bufferTitle,"< %s >",pTitleText);
			tft.print(CENTER,getY(rowIndex),bufferTitle,MAXCHARSLINE);
			return;
		} 
	}
	if( option != REFRESHPAGE || format == TEXT || format == PTEXT || flag16 & TTITLE) tft.print(STARTX +ROWTEXTX,getY(rowIndex),pTitleText,MAXCHARSLINE);
		//dbug(F("SR format H: 0x%J"),format);
		//Serial.println(format,DEC);
		//Serial.println(format,HEX);
	if(getAction() == UPDATE && pin){
		//dbug(F("SR0 %P ival:%d "),Ptitle,iVal);
		read();
	}
	if(format == BLAMP) {
			//dbug(F("SR1 %P ival:%d "),Ptitle,iVal);
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
	}
		
	else switch(format) {
		case INT5:
			if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
			else tft.printInt(STARTX +MONX,getY(rowIndex),iVal,6,RIGHT);
			break;
		case FLOAT1:
			if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
			else tft.printFloat(STARTX +MONX,getY(rowIndex),dVal->value,1,RIGHT);
			break;
		case FLOAT2:
			if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
			else tft.printFloat(STARTX +MONX,getY(rowIndex),dVal->value,AR4_2,RIGHT);
			break;
		case D2STR:
			if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
			else tft.print( RIGHT, getY(rowIndex),menu.d2str(dVal->value,MAXCHARSD2STR,Pbuffer));
			break;
		case TEXT:
			if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
			else tft.print( RIGHT, getY(rowIndex), ptext);
			break;
		case PTEXT:
			if(flag16 & UNDEF) tft.print( RIGHT, getY(rowIndex), getPstr(P_STRIKE,Pbuffer));
			else tft.print(  RIGHT, getY(rowIndex),getPtext(Ptext,Pbuffer));
			break;
	}
	tft.resetDefColors();
	//flag16 |= VISIBLE;
}

void fB_Menu::pinPageConstruct(uint8_t mode,uint8_t startDex, uint8_t hand ) {
		int i,j=0;
		fB_Tag* pT;

		if(hand == RIGHT) j=1;
		else if(hand == LEFT) j= -1;
		i = startDex + j;

		if(hand == RIGHT) for(;i<=tagCount;i++) {
				if(i >= tagCount) { i = 0; continue; }
				pT = &tagRay[(uint8_t)i];
				if(pT->pin && pT->getMode() == mode) break;
		}
		if(hand == LEFT) for( ;i>=-1;i--) {
			if(i < 0) { i= tagCount - 1; continue; }
			pT = &tagRay[(uint8_t)i];
			if(pT->pin && pT->getMode() == mode) break;
		}

		Tag(PNPIN)->Ptext=NULL;
		Tag(PNCRD)->Ptext= NULL;
		Tag(PNROW)->iVal = NULL;
		Tag(PNCOL)->Ptext = NULL;
		Tag(PNTOG)->iVal= LOW;
		Tag(PNADC)->iVal=0;

		pT = &tagRay[(uint8_t)i];  // pT is Pointer to PIN
		//if(!pT->pin) return;
		uint8_t rowSide;
		pT->getRowSide(rowSide); // stuff row,side encoded byte into this variable
		Tag(PNPIN)->Ptext= pT->Ptitle;
		Tag(PNPIN)->fTag =  (uint8_t)i; //  fTag avail to store index
		Tag(PNTOG)->iVal= LOW;
		Tag(PNCRD)->Ptext= Card(pT->getCtag())->Ptitle;
		Tag(PNADC)->iVal=0;
		Tag(PNROW)->iVal = rowSide & 0x1F;
		if((rowSide >> 7) == COL_L) Tag(PNCOL)->Ptext = PstrRay[P_LEFT];
		else Tag(PNCOL)->Ptext = PstrRay[P_RIGHT];
		if(pT->getMode() == IO_D) {
			if(pT->getDir() == OUTPUT) {
				if(pT->isLatched()) Tag(PNTOG)->iVal = HIGH;
				else Tag(PNTOG)->iVal = LOW;
				Tag(PNTOG)->putAction(TOGGLE);
				Tag(PNTOG)->Ptitle = PstrRay[P_TOGGLE];
			}
			if(pT->getDir() == INPUT){
				Tag(PNTOG)->iVal = pT->readInt();
				Tag(PNTOG)->putAction(NOACT);
				Tag(PNTOG)->Ptitle = PstrRay[P_INPUT];
			}
		}
		else  {
			Tag(PNVAL)->dVal->value = pT->dVal->value;		
			Tag(PNFAC)->dVal->value = pT->dVal->factor;
			Tag(PNOFF)->dVal->value = pT->dVal->offset;
			if(pT->getOnVal() == PGATE) {
				Tag(PNGAT)->iVal= HIGH;
				uint16_t aVal = pT->readInt();
				Tag(PNADC)->iVal= aVal;
				Tag(PNVAL)->dVal->value = pT->calibrate(aVal);
				Tag(PNFAC)->dVal->value = pT->dVal->factor;
				Tag(PNOFF)->dVal->value = pT->dVal->offset;
				Tag(PNADC)->flag16 &= ~( NOACT | MARK | UNDEF);
				Tag(PNVAL)->flag16 &= ~( NOACT | MARK | UNDEF);
			}
			else {
				Tag(PNADC)->flag16 |= (UNDEF | NOACT | MARK);
				Tag(PNVAL)->flag16 |= (UNDEF | NOACT | MARK);
				Tag(PNGAT)->iVal= LOW;
			}
		}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void fB_Tag::action(uint8_t  hand) {
	if(flag16 & HIDE ) return;
	char Pbuffer[7];
	fB_Tag *pT;
	//dbug(F("ra %P ,  cp: %P , iVal:%d"),Ptitle,curr.pP->ptitle,iVal);

	switch(curr.pageTag) {
		case LOGS:	
				// FAT index of file is in buf8 of calling Tag
				if(!rec.fileFind(buf8)) return;  // rec filename->fat.DE.filename and lasts only so long
				Tag(FPANEL)->ptitle = rec.filename;
				Tag(FSTD)->buf8 = buf8;// page tag fTag/buf8 is used for parentTag, so stuffing FAT index in FSTD tag
		//dbug(F("ra DATIME %s"),rec.dateStr);
				Tag(FDATE)->ptitle = rec.dateStr;
				Tag(FSIZE)->ptext = rec.sizeStr;
				if(curr.pP->flag16 & LOCAL) {  // Archive file display
					Tag(FSTD)->Ptitle = PstrRay[P_DELETE];  
					Tag(FSTD)->iVal = FDEL;  
					Tag(FARCH)->flag16 |= HIDE;  
				}
				else {  // Log file display
					Tag(FSTD)->Ptitle = PstrRay[P_STAMP];  
					Tag(FSTD)->iVal = FSTAMP;  
					Tag(FARCH)->flag16 &= ~HIDE; 
				}
				menu.jumpPage(FPANEL);
				return;
		case FPANEL:
				if(!rec.fileFind(Tag(FSTD)->buf8)) return;
			    switch(tag) {
					case FSTD:
						if(iVal == FSTAMP) {
							rec.logStamp();
							Tag(FDATE)->ptext = rec.dateStr; 
							Tag(FSIZE)->ptext = rec.sizeStr;
							menu.refreshRow(FDATE);
							menu.refreshRow(FSIZE);
							menu.selectHeader();
							return;
						}
						else if(iVal == FDEL && Ptitle == PstrRay[P_DELETE]) {
							rec.logRemove();
							menu.jumpPage(curr.parentTag);
						}
						return;
					case FARCH:
						rec.logArchive();
						menu.jumpPage(curr.parentTag);
						return;
					case FDUMP:
						rec.logDump();
						menu.selectHeader();
						return;
				}
				break;
	}
	switch (hand)	{
		case RIGHT:
			//check page jump first 
			if(flag16 & PAGE) {	menu.jumpPage(tag); return; }
			// then check tag
			uint16_t aVal;
			switch(tag) {
				case ARCHIVES:
					Tag(LOGS)->flag16 |= LOCAL; // set Files Page local tag to display archives
					Tag(LOGS)->Ptitle = Ptitle;
					menu.jumpPage(LOGS);
					return;
				case PNPIN: 
					curr.deselectRow();
					menu.pinPageConstruct(tagRay[Tag(PNPIN)->fTag].getMode(),Tag(PNPIN)->fTag,RIGHT);
					menu.refreshRow();
					menu.refreshRow(PNCRD);
					menu.refreshRow(PNROW);
					menu.refreshRow(PNCOL);
					if(tagRay[Tag(PNPIN)->fTag].getMode() == IO_D) menu.refreshRow(PNTOG);
					else {
						menu.refreshRow(PNGAT);
						menu.refreshRow(PNADC);
						menu.refreshRow(PNVAL);
						menu.refreshRow(PNFAC);
						menu.refreshRow(PNOFF);
					}
					curr.selectRow();
					return;
				case PNGAT:
					pT = &tagRay[Tag(PNPIN)->fTag];
					if(tag == PNGAT) {
						if(iVal == LOW) iVal = HIGH;
						else iVal = LOW;
					}
					if(iVal == HIGH) {
						pT->putOnVal(PGATE);
						aVal = pT->readInt();
						Tag(PNADC)->iVal= aVal;
						Tag(PNVAL)->dVal->value = pT->calibrate(aVal);
						//if(Tag(PNVAL)->dVal->value < 0) = Tag(PNVAL)->dVal->value == 0;
						Tag(PNFAC)->dVal->value = pT->dVal->factor;
						Tag(PNOFF)->dVal->value = pT->dVal->offset;
						Tag(PNADC)->flag16 &= ~( NOACT | MARK | UNDEF);
						Tag(PNVAL)->flag16 &= ~( NOACT | MARK | UNDEF);
					}
					else {
						pT->putOnVal(OFF);
						Tag(PNADC)->flag16 |= (UNDEF | NOACT | MARK);
						Tag(PNVAL)->flag16 |= (UNDEF | NOACT | MARK);
					}
					menu.refreshRow();
					menu.refreshRow(PNADC);
					menu.refreshRow(PNVAL);
					return;
				case PNADC:
					pT = &tagRay[ Tag(PNPIN)->fTag];
					aVal = pT->readInt();
					Tag(PNADC)->iVal= aVal;
					Tag(PNVAL)->dVal->value = pT->calibrate(aVal);
					menu.refreshRow();
					menu.refreshRow(PNVAL);
					return;
				case PNTOG:
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
				case TIAT:	rec.EEinitTags(); menu.selectHeader();return;
				case TSAT:	rec.EEwriteTags(); menu.selectHeader();return;
				//case TLAU:	
				//			for(int i = 0;i<logFileCount;i++) logRay[i].pLog->writeData();
				//			menu.selectHeader();
				//			return;

			} // END SWITCH on Tag

			switch(getAction()) {
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
					curr.deselectRow();
					menu.pinPageConstruct(tagRay[Tag(PNPIN)->fTag].getMode(),Tag(PNPIN)->fTag,LEFT);
					menu.refreshRow();
					menu.refreshRow(PNCRD);
					menu.refreshRow(PNROW);
					menu.refreshRow(PNCOL);
					if(tagRay[Tag(PNPIN)->fTag].getMode() == IO_D) menu.refreshRow(PNTOG);
					else {
						menu.refreshRow(PNGAT);
						menu.refreshRow(PNADC);
						menu.refreshRow(PNVAL);
					}
					curr.selectRow();
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
	//flag16 &= ~VISIBLE;


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
