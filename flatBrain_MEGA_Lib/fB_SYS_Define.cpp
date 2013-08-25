#include "fB_Include.h"


void defineSystem()  {

	defineCard(BRAIN,BRAIN,0,0);

	defineRow(HEADER,NULL,PAGE);

	definePage(SYSTEM,HOME);
		defineRow(FRAM,NULL,INT5 | NOACT);
		defineJump(CLOCK);
		defineJump(APINS);
		defineJump(DPINS);
		defineJump(LOGS);
		defineRow(ARCHIVES,NULL,NULL);
		defineAlias(DPINS,DIGITAL PINS);
		defineAlias(APINS,ANALOG PINS);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR, NULL,INCR | INT5);
		defineRow(CLKMO, NULL,INCR | INT5);
		defineRow(CLKDY, NULL,INCR | INT5);
		defineRow(CLKHH, NULL,INCR | INT5);
		defineRow(CLKMM, NULL,INCR | INT5);
		defineRow(CLKGET, NULL,BLANK);
		defineRow(CLKSET, NULL,BLANK);
		defineAlias(CLKYR,YEAR);
		defineAlias(CLKMO,MONTH);
		defineAlias(CLKDY,DAY);
		defineAlias(CLKHH,HOUR);
		defineAlias(CLKMM,MIN);
		defineAlias(CLKGET,GET DATETIME);
		defineAlias(CLKSET,SET DATETIME);



	definePage(DPINS,SYSTEM);
		defineRow(PNPIN,NULL,PTEXT );
		defineRow(PNCRD,NULL,PTEXT | NOACT);
		defineRow(PNROW,NULL,INT5  | NOACT);
		defineRow(PNCOL,NULL,PTEXT | NOACT);
		defineRow(PNTOG,NULL,BLAMP );
		if(secondPass){
			Tag(PNCRD)->flag16 |= MARK;
			Tag(PNROW)->flag16 |= MARK;
			Tag(PNCOL)->flag16 |= MARK;
		}
		defineAlias(PNPIN,PIN);
		defineAlias(PNCRD,CARD);
		defineAlias(PNROW,COL);
		defineAlias(PNTOG,TOGGLE);


	definePage(APINS,SYSTEM);
		defineRow(PNPIN,NULL,PTEXT );
		defineRow(PNCRD,NULL,PTEXT | NOACT);
		defineRow(PNROW,NULL,INT5  | NOACT);
		defineRow(PNCOL,NULL,PTEXT | NOACT);
		defineRow(PNGAT,NULL,BLAMP );
		defineRow(PNADC,NULL,INT5  | UPDATE);
		defineRow(PNVAL,NULL,FLOAT2 | NOACT);
		defineRow(PNFAC,NULL,D2STR | NOACT);
		defineRow(PNOFF,NULL,FLOAT2 | NOACT);
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
		defineRow(FDATE,NULL, NOACT | TTITLE);
		defineRow(FSIZE,NULL,INT5 | NOACT);
		defineRow(FDUMP,NULL,BLANK);
		defineRow(FSTD,NULL,NULL);
		defineRow(FARCH,NULL,BLANK);
		if(secondPass)Tag(FDATE)->flag16 |= MARK;
		if(secondPass)Tag(FSIZE)->flag16 |= MARK;
		defineAlias(FDATE,DATE);
		defineAlias(FSIZE,SIZE);
		defineAlias(FDUMP,SERIAL DUMP);
		defineAlias(FARCH,ARCHIVE);
					
	//defineRowList(STAGS,SYSTEM,NULL); 
	
/*
	definePage(TPANEL,HOME);
		defineRow(TLOG,NULL,NOACT);
		defineRow(TINP,NULL,NOACT);
		defineRow(TVAL,NULL,FLOAT2);
		defineRow(TOPR,NULL,TEXT); 
		defineRow(TFAC,NULL,FLOAT2);
		if(secondPass)	Tag(TLOG)->flag16 |= MARK;
		*/

	defineRecord(TBOOT,SYSTAG,TSYS);// Probably also need to define row in fB_Menu.cpp

}

