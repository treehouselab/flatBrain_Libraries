#include "fB_Include.h"


void defineSystem()  {
	initTag(TAGZERO,"TAGZERO",_FLOAT2); // bad tag catch

	defineCard(BRAIN,BRAIN,0,0);

	defineRow(HEADER,_PAGE);
	defineRow(_MSG, _TEXT);

	definePage(SYSTEM,HOME);
		defineJump(APINS);
		defineJump(DPINS);
		defineJump(LOGSYS);
		defineJump(EEPROM);
		defineJump(CLOCK);
		defineJump(ALARMP);
		defineRow(_FRAM,_INT5);
		defineRow(VCC,_FLOAT2 | _NOACT);

	definePage(LOGSYS,SYSTEM);
		defineRow(_MOUNT, _BLAMP);
		defineJump(LOGS);
		defineRow(ARCHIVES,NULL);


	definePage(ALARMP,SYSTEM);
		defineJump(SOUNDS);
		defineRow(_SECAL, _INT5 | _INCR | _LOADEE);
		defineRow(_SECWN, _INT5 | _INCR | _LOADEE);
		defineRow(_SECWD, _INT5 | _INCR | _LOADEE);
		defineRow(_TALON,_BLAMP | _LOADEE);
		defineRow(_ESTOR,_BLANK);

		defineValue(_TALON,1);
		defineValue(_SECAL,4,1);
		defineValue(_SECWN,8,1);
		defineValue(_SECWD,30,1);
		defineAlias(ALARMP,ALARM);
		defineAlias(_SECAL,ALARM SEC);
		defineAlias(_SECWN,WARN  SEC);
		defineAlias(_SECWD,DELAY SEC);
		defineAlias(_TALON,ALARM ENABLE);

	definePage(SOUNDS,ALARMP);
		defineRow(_TALBT,_BLANK);
		defineRow(_TALIN,_BLANK);
		defineRow(_TALFL,_BLANK);
		defineRow(_TALWN,_BLANK);
		defineRow(_TALAC,_BLANK);
		defineRow(_TALQS,_BLANK);
		defineRow(_TALEG,_BLANK);

		defineValue(_TALBT,0);
		defineValue(_TALIN,1);
		defineValue(_TALFL,2);
		defineValue(_TALWN,3);
		defineValue(_TALAC,4);
		defineValue(_TALQS,5);
		defineValue(_TALEG,6);

		defineAlias(_TALBT,ALARM BOOT);
		defineAlias(_TALIN,ALARM INIT);
		defineAlias(_TALFL,ALARM FAIL);
		defineAlias(_TALWN,ALARM WARN);
		defineAlias(_TALAC,ALARM ACTION);
		defineAlias(_TALQS,ALARM QUESTION);
		defineAlias(_TALEG,ALARM EMERGENCY);

	definePage(EEPROM,SYSTEM);
		defineRow(_EAUTO,_BLAMP | _LOADEE); // required tag
		defineRow(_ESTOR,_BLANK);
		defineRow(ELOAD,_BLANK);
		defineRow(ECLR,_BLANK);
		defineRow(EDUMP,_BLANK);
		defineAlias(_EAUTO,LOAD ON BOOT);
		defineAlias(_ESTOR,SAVE ALL EEPROM);
		defineAlias(ELOAD,LOAD ALL EEPROM);
		defineAlias(ECLR,CLR  ALL EEPROM);
		defineAlias(EDUMP,SERIAL DUMP);


	definePage(CLOCK,SYSTEM);
		defineRow(CLKYR,  _INT5);
		defineRow(CLKMO,  _INT5);
		defineRow(CLKDY,  _INT5);
		defineRow(CLKHH,  _INT5);
		defineRow(CLKMM,  _INT5);
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
		//defineRow(PNCRD,_PTEXT | _NOACT);
		defineRow(PNROW,_TEXT  | _NOACT);
		//defineRow(PNCOL,_PTEXT | _NOACT);
		defineRow(PNTOG,_BLAMP );
		if(secondPass){
			Tag(PNCRD)->flag16 |= _MARK;
			Tag(PNROW)->flag16 |= _MARK;
			Tag(PNCOL)->flag16 |= _MARK;
		}
		defineAlias(PNPIN,PIN);
		//defineAlias(PNCRD,CARD);
		defineAlias(PNROW,ROW);
		//defineAlias(PNCOL,COL);
		defineAlias(PNTOG,TOGGLE);


	definePage(APINS,SYSTEM);
		defineRow(PNPIN,_PTEXT );
		//defineRow(PNCRD,_PTEXT | _NOACT);
		defineRow(PNROW,_TEXT  | _NOACT);
		//defineRow(PNCOL,_PTEXT | _NOACT);
		defineRow(PNGAT,_BLAMP );
		defineRow(PNADC,_INT5  | _UPDATE);
		//defineRow(PNVAL,_FLOAT2 | _NOACT);
		defineRow(PNVAL,_D2STR | _NOACT);
		defineRow(PNBIA,_D2STR );
		defineRow(PNOFF,_D2STR );
		defineRow(PNINC,_D2STR | _PTEN);
		defineRow(_ESTOR,_BLANK);

		if(secondPass){
			Tag(PNVAL)->flag16 |= _MARK;
		//	Tag(PNFAC)->flag16 |= _MARK;
		//	Tag(PNOFF)->flag16 |= _MARK;
		}
		defineAlias(PNGAT,GATE);
		defineAlias(PNADC,ADC);
		defineAlias(PNVAL,VALUE);
		defineAlias(PNBIA,BIAS);
		defineAlias(PNOFF,OFFS);
		defineAlias(PNINC,INCR);
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


	defineAlias(LOGSYS,LOG FILES);
	defineAlias(DPINS,DIGITAL PINS);
	defineAlias(APINS,ANALOG PINS);
	defineAlias(_FRAM,FREE RAM);
	defineAlias(_MOUNT,SD MOUNTED);


}

