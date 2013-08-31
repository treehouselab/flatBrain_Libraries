#include "fB_Include.h"


void defineSystem()  {

	defineCard(BRAIN,BRAIN,0,0);

	defineRow(HEADER,PAGE);

	definePage(SYSTEM,HOME);
		defineJump(APINS);
		defineJump(DPINS);
		defineJump(LOGS);
		defineRow(ARCHIVES,NULL);
		defineJump(CLOCK);
		defineRow(FRAM,INT5 | NOACT);
		defineRow(VCC,FLOAT2 | NOACT);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR, INCR | INT5);
		defineRow(CLKMO, INCR | INT5);
		defineRow(CLKDY, INCR | INT5);
		defineRow(CLKHH, INCR | INT5);
		defineRow(CLKMM, INCR | INT5);
		defineRow(CLKGET, BLANK);
		defineRow(CLKSET, BLANK);
		defineAlias(CLKYR,YEAR);
		defineAlias(CLKMO,MONTH);
		defineAlias(CLKDY,DAY);
		defineAlias(CLKHH,HOUR);
		defineAlias(CLKMM,MIN);
		defineAlias(CLKGET,GET DATETIME);
		defineAlias(CLKSET,SET DATETIME);


	definePage(DPINS,SYSTEM);
		defineRow(PNPIN,PTEXT );
		defineRow(PNCRD,PTEXT | NOACT);
		defineRow(PNROW,INT5  | NOACT);
		defineRow(PNCOL,PTEXT | NOACT);
		defineRow(PNTOG,BLAMP );
		if(secondPass){
			Tag(PNCRD)->flag16 |= MARK;
			Tag(PNROW)->flag16 |= MARK;
			Tag(PNCOL)->flag16 |= MARK;
		}
		defineAlias(PNPIN,PIN);
		defineAlias(PNCRD,CARD);
		defineAlias(PNROW,ROW);
		defineAlias(PNCOL,COL);
		defineAlias(PNTOG,TOGGLE);


	definePage(APINS,SYSTEM);
		defineRow(PNPIN,PTEXT );
		defineRow(PNCRD,PTEXT | NOACT);
		defineRow(PNROW,INT5  | NOACT);
		defineRow(PNCOL,PTEXT | NOACT);
		defineRow(PNGAT,BLAMP );
		defineRow(PNADC,INT5  | UPDATE);
		defineRow(PNVAL,FLOAT2 | NOACT);
		defineRow(PNFAC,D2STR | NOACT);
		defineRow(PNOFF,FLOAT2 | NOACT);
		if(secondPass){
			Tag(PNVAL)->flag16 |= MARK;
			Tag(PNFAC)->flag16 |= MARK;
			Tag(PNOFF)->flag16 |= MARK;
		}
		defineAlias(PNGAT,GATE);
		defineAlias(PNADC,ADC);
		defineAlias(PNVAL,VALUE);
		defineAlias(PNFAC,FACTOR);
		defineAlias(PNOFF,OFFSET);

	//definePage(RECORD,SYSTEM );
	//	defineJump(LOGS);

	defineRowList(LOGS,SYSTEM,TTITLE);

	definePage(FPANEL,LOGS);
		defineRow(FDATE, NOACT | TTITLE);
		defineRow(FSIZE, NOACT | TEXT);
		defineRow(FSTD,NULL);
		defineRow(FDUMP,BLANK);
		defineRow(FARCH,BLANK);
		if(secondPass)Tag(FDATE)->flag16 |= MARK;
		if(secondPass)Tag(FSIZE)->flag16 |= MARK;
		defineAlias(FDATE,DATE);
		defineAlias(FSIZE,SIZE);
		defineAlias(FDUMP,SERIAL DUMP);
		defineAlias(FARCH,ARCHIVE);
					
	//defineRowList(STAGS,SYSTEM,NULL); 
	
/*
	definePage(TPANEL,HOME);
		defineRow(TLOG,NOACT);
		defineRow(TINP,NOACT);
		defineRow(TVAL,FLOAT2);
		defineRow(TOPR,TEXT); 
		defineRow(TFAC,FLOAT2);
		if(secondPass)	Tag(TLOG)->flag16 |= MARK;
		*/

	defineRecord(TBOOT,SYSTAG,TSYS);// Probably also need to define row in fB_Menu.cpp		
	defineAlias(DPINS,DIGITAL PINS);
	defineAlias(APINS,ANALOG PINS);
	defineAlias(FRAM,FREE RAM);


}

