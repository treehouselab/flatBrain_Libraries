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
		defineJump(EEPROM);
		defineJump(CLOCK);
		defineJump(ALARMP);
		defineRow(FRAM,_INT5 | _NOACT);
		defineRow(VCC,_FLOAT2 | _NOACT);

	definePage(ALARMP,SYSTEM);
		defineJump(SOUNDS);
		defineRow(_SECAL, _INT5 | _INCR | _LOADEE);
		defineRow(_SECWN, _INT5 | _INCR | _LOADEE);
		defineRow(_SECWD, _INT5 | _INCR | _LOADEE);
		defineRow(_TALRMON,_BLAMP | _LOADEE);
		defineRow(ESTOR,_BLANK);

		defineValue(_TALRMON,1);
		defineValue(_SECAL,4,1);
		defineValue(_SECWN,8,1);
		defineValue(_SECWD,30,1);
		defineAlias(ALARMP,ALARM);
		defineAlias(_SECAL,ALARM SEC);
		defineAlias(_SECWN,WARN  SEC);
		defineAlias(_SECWD,DELAY SEC);
		defineAlias(_TALRMON,ALARM ENABLE);

	definePage(SOUNDS,ALARMP);
		defineRow(_TALRMBT,_BLANK);
		defineRow(_TALRMIN,_BLANK);
		defineRow(_TALRMFL,_BLANK);
		defineRow(_TALRMWN,_BLANK);
		defineRow(_TALRMAC,_BLANK);
		defineRow(_TALRMQS,_BLANK);
		defineRow(_TALRMEG,_BLANK);

		defineValue(_TALRMBT,0);
		defineValue(_TALRMIN,1);
		defineValue(_TALRMFL,2);
		defineValue(_TALRMWN,3);
		defineValue(_TALRMAC,4);
		defineValue(_TALRMQS,5);
		defineValue(_TALRMEG,6);

		defineAlias(_TALRMBT,ALARM BOOT);
		defineAlias(_TALRMIN,ALARM INIT);
		defineAlias(_TALRMFL,ALARM FAIL);
		defineAlias(_TALRMWN,ALARM WARN);
		defineAlias(_TALRMAC,ALARM ACTION);
		defineAlias(_TALRMQS,ALARM QUESTION);
		defineAlias(_TALRMEG,ALARM EMERGENCY);

	definePage(EEPROM,SYSTEM);
		defineRow(ESTOR,_BLANK);
		defineRow(ELOAD,_BLANK);
		defineRow(EDUMP,_BLANK);
		defineRow(ECLR,_BLANK);
		defineRow(EAUTO,_BLAMP | _LOADEE); // required tag
		defineAlias(ESTOR,SAVE ALL EEPROM);
		defineAlias(ELOAD,LOAD ALL EEPROM);
		defineAlias(ECLR,CLEAR ALL EEPROM);
		defineAlias(EDUMP,SERIAL DUMP);


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
		defineRow(PNCRD,_PTEXT | _NOACT);
		defineRow(PNROW,_INT5  | _NOACT);
		defineRow(PNCOL,_PTEXT | _NOACT);
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
		defineAlias(PNTOG,TOGGLE);


	definePage(APINS,SYSTEM);
		defineRow(PNPIN,_PTEXT );
		defineRow(PNCRD,_PTEXT | _NOACT);
		defineRow(PNROW,_INT5  | _NOACT);
		defineRow(PNCOL,_PTEXT | _NOACT);
		defineRow(PNGAT,_BLAMP );
		defineRow(PNADC,_INT5  | UPDATE);
		//defineRow(PNVAL,_FLOAT2 | _NOACT);
		defineRow(PNVAL,_D2STR | _NOACT);
		//defineRow(PNFAC,_D2STR | _NOACT);
		//defineRow(PNOFF,_D2STR | _NOACT);
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
		defineRow(FDATE, _NOACT | _TTITLE);
		defineRow(FSIZE, _NOACT | _TEXT);
		defineRow(FSTD,NULL);
		defineRow(FDUMP,_BLANK);
		defineRow(FARCH,_BLANK);
		if(secondPass)Tag(FDATE)->flag16 |= _MARK;
		if(secondPass)Tag(FSIZE)->flag16 |= _MARK;
		defineAlias(FDATE,DATE);
		defineAlias(FSIZE,SIZE);
		defineAlias(FDUMP,SERIAL DUMP);
		defineAlias(FARCH,ARCHIVE);

	defineAlias(DPINS,DIGITAL PINS);
	defineAlias(APINS,ANALOG PINS);
	defineAlias(FRAM,FREE RAM);

}

