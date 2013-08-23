#include "fB_Include.h"


void defineSystem()  {

	defineCard(BRAIN,BRAIN,0,0);

	defineRow(HEADER,NULL,PAGE);

	definePage(SYSTEM,HOME);
		defineRow(FRAM,NULL,INT5 | NOACT);
		defineJump(CLOCK);
		defineJump(PINS);
		defineJump(LOGS);
		defineRow(ARCHIVES,NULL,NULL);

	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR, NULL,INCR | INT5);
		defineRow(CLKMO, NULL,INCR | INT5);
		defineRow(CLKDY, NULL,INCR | INT5);
		defineRow(CLKHH, NULL,INCR | INT5);
		defineRow(CLKMM, NULL,INCR | INT5);
		defineRow(CLKGET, NULL,BLANK);
		defineRow(CLKSET, NULL,BLANK);


	definePage(PINS,SYSTEM);
		defineRow(PNPIN,NULL,PTEXT );
		defineRow(PNCRD,NULL,PTEXT | NOACT);
		defineRow(PNROW,NULL,INT5  | NOACT);
		defineRow(PNCOL,NULL,PTEXT | NOACT);
		defineRow(PNTOG,NULL,BLAMP | PINTOG);
		defineRow(PNADC,NULL,INT5  | UPDATE);
		if(secondPass)Tag(PNROW)->flag16 |= MARK;
		if(secondPass)Tag(PNCOL)->flag16 |= MARK;

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
					
	//defineRowList(STAGS,SYSTEM,NULL); 
	

	definePage(TPANEL,HOME);
		defineRow(TLOG,NULL,NOACT);
		defineRow(TINP,NULL,NOACT);
		defineRow(TVAL,NULL,FLOAT2);
		defineRow(TOPR,NULL,TEXT); 
		defineRow(TFAC,NULL,FLOAT2);
		if(secondPass)	Tag(TLOG)->flag16 |= MARK;

	defineRecord(MTAG,MTAG2,TSYS);// Probably also need to define row in fB_Menu.cpp
	defineRecord(TBOOT,SYSTAG,TSYS);// Probably also need to define row in fB_Menu.cpp


}

