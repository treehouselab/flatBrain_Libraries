#include "fB_Include.h"


void defineSystem()  {

	defineCard(BRAIN,BRAIN,0,0);
	defineLog(LOG);

	defineRow(HEADER,NULL,PAGE);

	definePage(SYSTEM,HOME);
		defineJump(CLOCK);
		defineJump(PINS);
		defineJump(FILES);
		defineJump(TLIST);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR, NULL,INCR | INT5);
		defineRow(CLKMO, NULL,INCR | INT5);
		defineRow(CLKDY, NULL,INCR | INT5);
		defineRow(CLKHH, NULL,INCR | INT5);
		defineRow(CLKMM, NULL,INCR | INT5);
		defineRow(CLKGET, NULL,BLANK);
		defineRow(CLKSET, NULL,BLANK);


	definePage(PINS,SYSTEM);
		defineRow(PNPIN,NULL,TEXT);
		defineRow(PNCRD,NULL,NOACT | TEXT);
		defineRow(PNROW,NULL,INT5 | NOACT);
		defineRow(PNCOL,NULL,TEXT | NOACT);
		defineRow(PNTOG,NULL,BLAMP);
		defineRow(PNADC,NULL,INT5);
		if(secondPass)Tag(PNROW)->flag16 |= MARK;
		if(secondPass)Tag(PNCOL)->flag16 |= MARK;

	//definePage(RECORD,SYSTEM );
	//	defineJump(FILES);

	definePage(FILES,SYSTEM);
	defineRowList(FROW,MAXLISTROWS,NULL,TEXT);
	definePage(FPANEL,FILES);
		defineRow(FDATE,NULL,TEXT | NOACT);
		defineRow(FSIZE,NULL,INT5 | NOACT);
		defineRow(FDUMP,NULL,BLANK);
		defineRow(FSTD,NULL,NULL);
		defineRow(FARCH,NULL,BLANK);
		if(secondPass)Tag(FDATE)->flag16 |= MARK;
		if(secondPass)Tag(FSIZE)->flag16 |= MARK;
					
	definePage(TLIST,SYSTEM);
	defineRowList(TROW,MAXLISTROWS,NULL,NULL); 
	definePage(TPANEL,TLIST);
		defineRow(TLOG,NULL,NOACT);
		defineRow(TINP,NULL,NOACT);
		defineRow(TVAL,NULL,FLOAT2);
		defineRow(TOPR,NULL,TEXT); 
		defineRow(TFAC,NULL,FLOAT2);
		if(secondPass)	Tag(TLOG)->flag16 |= MARK;

	defineRecord(TBOOT,SYSTAG,TSYS);// Probably also need to define row in fB_Menu.cpp


}

