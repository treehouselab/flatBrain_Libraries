#include "fB_Include.h"


void defineSystem()  {
	initTag(TAGZERO,F("TAGZERO"),_FLOAT2); // bad tag catch

	defineCard(BRAIN,BRAIN,0,0);

	defineRow(HEADER,_PAGE);
	defineRow(_MSG, _TEXT);

	definePage(SYSTEM,HOME);
		defineJump(APINS);
		defineJump(DPINS);
		defineJump(LOGS);
		defineRow(ARCHIVES,NULL);
		defineJump(CLOCK);
		defineRow(ELOAD,_BLANK);
		defineRow(ESTOR,_BLANK);
		defineRow(EDUMP,_BLANK);
		defineRow(FRAM,_INT5 | NOACT);
		defineRow(VCC,_FLOAT2 | NOACT);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR, _INCR | _INT5);
		defineRow(CLKMO, _INCR | _INT5);
		defineRow(CLKDY, _INCR | _INT5);
		defineRow(CLKHH, _INCR | _INT5);
		defineRow(CLKMM, _INCR | _INT5);
		defineRow(CLKGET, _BLANK);
		defineRow(CLKSET, _BLANK);
		defineAlias(CLKYR,YEAR);
		defineAlias(CLKMO,MONTH);
		defineAlias(CLKDY,DAY);
		defineAlias(CLKHH,HOUR);
		defineAlias(CLKMM,MIN);
		defineAlias(CLKGET,GET DATETIME);
		defineAlias(CLKSET,SET DATETIME);


	definePage(DPINS,SYSTEM);
		defineRow(PNPIN,_PTEXT );
		defineRow(PNCRD,_PTEXT | NOACT);
		defineRow(PNROW,_INT5  | NOACT);
		defineRow(PNCOL,_PTEXT | NOACT);
		defineRow(PNTOG,_BLAMP );
		if(secondPass){
			Tag(PNCRD)->flag16 |= _MARK;
			Tag(PNROW)->flag16 |= _MARK;
			Tag(PNCOL)->flag16 |= _MARK;
		}
		defineAlias(PNPIN,PIN);
		defineAlias(PNCRD,CARD);
		defineAlias(PNROW,ROW);
		defineAlias(PNCOL,COL);
		defineAlias(PNTOG,_TOGGLE);


	definePage(APINS,SYSTEM);
		defineRow(PNPIN,_PTEXT );
		defineRow(PNCRD,_PTEXT | NOACT);
		defineRow(PNROW,_INT5  | NOACT);
		defineRow(PNCOL,_PTEXT | NOACT);
		defineRow(PNGAT,_BLAMP );
		defineRow(PNADC,_INT5  | UPDATE);
		//defineRow(PNVAL,_FLOAT2 | NOACT);
		defineRow(PNVAL,_D2STR | NOACT);
		//defineRow(PNFAC,_D2STR | NOACT);
		//defineRow(PNOFF,_D2STR | NOACT);
		if(secondPass){
			Tag(PNVAL)->flag16 |= _MARK;
		//	Tag(PNFAC)->flag16 |= _MARK;
		//	Tag(PNOFF)->flag16 |= _MARK;
		}
		defineAlias(PNGAT,GATE);
		defineAlias(PNADC,ADC);
		defineAlias(PNVAL,VALUE);
		//defineAlias(PNFAC,FACTOR);
		//defineAlias(PNOFF,OFFSET);

	//definePage(RECORD,SYSTEM );
	//	defineJump(LOGS);

	defineRowList(LOGS,SYSTEM,_TTITLE);

	definePage(FPANEL,LOGS);
		defineRow(FDATE, NOACT | _TTITLE);
		defineRow(FSIZE, NOACT | _TEXT);
		defineRow(FSTD,NULL);
		defineRow(FDUMP,_BLANK);
		defineRow(FARCH,_BLANK);
		if(secondPass)Tag(FDATE)->flag16 |= _MARK;
		if(secondPass)Tag(FSIZE)->flag16 |= _MARK;
		defineAlias(FDATE,DATE);
		defineAlias(FSIZE,SIZE);
		defineAlias(FDUMP,SERIAL DUMP);
		defineAlias(FARCH,ARCHIVE);
					
	//defineRowList(STAGS,SYSTEM,NULL); 
	
/*
	definePage(TPANEL,HOME);
		defineRow(TLOG,NOACT);
		defineRow(TINP,NOACT);
		defineRow(TVAL,_FLOAT2);
		defineRow(TOPR,_TEXT); 
		defineRow(TFAC,_FLOAT2);
		if(secondPass)	Tag(TLOG)->flag16 |= _MARK;
		*/

	defineRecord(TBOOT,SYSTAG,_LOADEE);// Probably also need to define row in fB_Menu.cpp		
	defineAlias(DPINS,DIGITAL PINS);
	defineAlias(APINS,ANALOG PINS);
	defineAlias(FRAM,FREE RAM);


}

