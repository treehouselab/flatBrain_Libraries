//////////////////////////////////////
// SYSTEM DEFINES. FOR ARDUINO DUE.  EDIT AT YOUR PERIL.
//////////////////////////////////////
#ifndef FP_SYS_DEFINES_H
#define FP_SYS_DEFINES_H

#define STRINGIFY(x) #x
#define CONCAT(prefix, suffix) prefix##suffix



#define WR_PIN	29     // for port banging WR pin on TFT ( corresponds to DUE D29  )
#define RS_PIN	27     // for port banging RS pin on TFT ( corresponds to DUE D27 )

#define AT_SPISS 		    53 //  dedicated Mega d53 pin for Chip Select pin for ATTINYs

#define SD_SSPIN 		    30 //  dedicated Mega d15 pin for Chip Select pin for TFT-SD card 
#define SD_SSBIT 		    0 
#define SD_SSPORT 		    PORTJ
#define SD_SSDDR 		    DDRJ 
#define SPISPEED			0x01 //(0-VERY HIGH....3-VERY SLOW)

#define _ALARMPIN		7 // option: DUE pin 7

#define NAV_INT			24 // Display Navigation button interrupt number, corresp. to DUE pin 24
#define NAV_INTPIN		24
#define WARN_INT		4 // Alarm warning interrupt number, corresp. to Mega pin 20
#define WARN_INTPIN		19
#define K0_INT			0 //  interrupt number, corresp. to DUE pin 2
#define K0_INTPIN		2 
#define K1_INT			1 //  interrupt number, corresp. to DUE pin 3
#define K1_INTPIN		3

#define	INTK1			1	// Interrupt K1 active
#define	INTK1SHFT		2	// Interrupt K1 SHIFTED active

/* CUSTOMARY I2C ADDRESSES:
TFT-PCF		0x38
TFT-PCF		0x39
TFT-PCF		0x3A
TFT-PCF		0x3B
RTC-EEPROM  0x50
EEPROM		0x52
RTC			0x68
XCARDS		Ox20-27
*/

#define RTC_ADDRESS		0x68
#define EE_ADDRESS		0x52

// IOX Card I2C allowed card addresses: 
// CARD TYPE _X50 TAKES SINGLE ADDRESS, UP TO 8
// CARD TYPE _X76 RESERVES 2 ADDRESS PAIRS, UP TO 4
#define	_C0		0x20	// _X50 OR _X76
#define _C1		0x21	// _X50 IF C0 !_X76
#define	_C2		0x22	// _X50 OR _X76
#define _C3		0x23	// _X50 IF C2 !_X76
#define	_C4		0x24	// _X50 OR _X76  
#define _C5		0x25	// _X50 IF C4 !_X76 
#define	_C6		0x26	// _X50 OR _X76
#define _C7		0x27	// _X50 IF C6 !_X76   

#define	_APPENDLOG		0
#define	_APPENDDIF		1
#define	_NEWLOG			2
#define	_MINLOGMIN		1  // minimum log interval in minutes

#define fERR  -1

#define I2CFAST			1	  
#define I2CSLOW			0	  
#define I2CSPEED		1
#define I2CTIMEOUT		100  //ms	  
#define SERIALSPEED		9600  
#define BASEETAG		0       // base address EEPROM, load values
#define BASEELOG		1024    // base address EEPROM, last log values
#define	PULSEMSECS		200
#define	_CPSECLIM		10
#define	_BKSECLIM		30

#define MAXEETAGS		25  // for eeprom runaway only
#define MAXCHARSDUMP	200  // reduce this if logfile dump collides with heap 
#define MAXLISTROWS		9 
#define	MESSAGE_Y		295
#define MAXCHARSLINE	15 
#define MAXCHARSTEXT	8 
#define MAXCHARS_D2STR	6 
#define _PRECISION		4
//#define MAXVDRDEX		6 
#define	MAXTEMPTAG		256
#define	MAXTEMPLOG		24
#define	MAXZEROADC		250 // max ADC reading that qualifies for binary zero

// Bus to Arduino Mega pin mapping
#define	_B0		A8
#define _B1		A9
#define	_B2		A10
#define	_B3		A11

#define	_SH		6
#define	_K1		5
#define	_K2		4
#define	_K3		2
#define	_K4		3

// Card types
#define	BRAIN			0
#define _X50			1
#define _X76			2

// Pin DIRECTION
#define	_INPUT		0
#define	_OUTPUT		1

// Pin sides
#define	_L		0
#define	_R		1

//Pin types		unique4, 0..7
#define	_DIGITAL	0
#define _ANALOG		1


// Pin mapping for IOX cards 
// CARD TYPE _X50 
#define _X50AA			7	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X50AB			6	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X50AC			5	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X50BA			4	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define _X50BB			3	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define _X50BC			2	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define _X50LD			1	// MCP PIN MAPS TO  BOARD LED
#define _X50GT			0	// MCP PIN MAPS TO  opiso gate

// CARD TYPE _X76 
#define _X76AA			11	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X76AB			12	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X76AC			9	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X76AD			8	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define _X76IN			10	// MCP PIN MAPS TO CD4067 A INHIBIT PIN
#define _X76BA			4	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define _X76BB			3	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define _X76BC			2	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define _X76RS			13	// MCP PIN MAPS TO 2nd MCP _RST pin
#define _X76LD			14	// MCP PIN MAPS TO  BOARD LED
#define _X76GT			0	// MCP PIN MAPS TO  op-iso gate

// Used when invoking analogRead/Write using an MPC digital line
#define  AD_HIGH    1024   // returned when MPC_analogRead senses HIGH
#define  AD_LOW     0      // returned when MPC_analogRead senses LOW
#define  AD_TRANS   512    // when MPC_analogWrite value > AD_TRANS, line sent HIGH, else sent LOW

//  TFT
#define RED				248
#define ORANGE			251
#define GOLD			253
#define YELLOW			255
#define MAGENTA			146
#define GREEN			7
#define BLACK			0
#define FCOLOR		255  //defaults
#define GCOLOR		248
#define HCOLOR		253
#define BCOLOR		0

#define STARTX			0 // TOP LEFT CORNER OF PAGE
#define STARTY			5 // TOP LEFT CORNER OF PAGE
#define ROWTEXTX		0  // X OFFSET OF TEXT 
#define ROWSTATEX		225  // X OFFSET OF  indicator
#define ROWSTATEY		6  // Y OFFSET OF  indicator
//#define ROWSP			10 // vertical pixels beween blocks
#define ROWHT			29
#define LINEHT			20
#define MONX		   60
#define ADJX		   52
#define REFRESHPAGE		53

#define LED_HB    9
#define LED_SD    8

#define GAUGEHT		    60
#define GSTARTY			5
#define GSTARTX			5
#define GTITLEX			168
#define GTITLEY1		34
#define GTITLEY2		5
#define GMAXCHARSTITLE	4


#define	_RTC			0x01   // brain status flags, unique uint8_t bits 
#define	_SD				0x02   


#define	_PGATE			2
#define	ON				1
#define	OFF				0

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Tag->flags is 16 bits, top 4bits is used for row count buffer
#define	_PAGE			0x0001	 
#define	_PIN			0x0002	
#define	_LOADEE			0x0004	// load values from EEPROM on Boot
#define	_UPSHOW			0x0008	// Update value when row shows
#define	_MARK			0x0010	// Mark row
#define	_HIDE			0x0020
#define	_TTITLE			0x0040	// use char* ptitle for row title instead of Ptitle
#define	_TTAG			0x0080  // has target tag stored in pin/tTag union
#define	_UNDEF			0x0100
#define	_ARCH			0x0200 
#define	_DUBL			0x0400
//#define				0x0800
#define	_MASKP			0xF000	// 4 bits reserved ROWCOUNT
#define	_RCOFFSET		12	    // Offset of rowCount 

////////////////////////////////////////////////////////////////////////////////
//Tag->flag8 is 8 bits. The low 4bits of Tag->flag8 contain one of 11 possible 
// action codes (incl. zero) in decimal format. 
// The high 4bits of  Tag->format contains one of 11 possible format codes in similar scheme.
// This code is converted to 32bit for run-time comparisons (listed below). 
// This allows for all the flag16, format, and 
// action flags to be entered on a single OR'd field at Tag definition arguments.


#define	MASK8A			0x0F	 
#define	MASK32A			0x003FF000L
//#define				0x00000000L	
//#define			0x00001000L	
#define	_INCR			0x00002000L	
#define	SHFTPULSE		0x00004000L	
#define	PULSE			0x00008000L
#define	_UPDATE			0x00010000L
#define	_PTEN			0x00020000L
#define	_NOACT			0x00040000L
#define	_TOGGLE			0x00080000L
//#define		       	0x00100000L
//#define				0x00200000L

#define	MASK8F			0xF0	 
#define	MASK32F			0xFFC00000L
#define	_BLANK			0x00000000L	
#define	_TEXT			0x00400000L	
#define	_INT5			0x00800000L	
#define	_FLOAT1			0x01000000L
#define	_FLOAT2			0x02000000L	
#define	_BLAMP			0x04000000L	
#define	_D2STR			0x08000000L	
#define	_PTEXT			0x10000000L	
#define	_STRIKE			0x20000000L	
//#define				0x20000000L
//#define				0x40000000L
//#define				0x80000000L


#define	_MANOVR		1	// fB interrupt selectors used by fBinterruptHandler() called by pin K1
#define	_WARNDELAY	2

////////////////////////////////////////////////////////////////////???
////////////////////////////////////////////////////////////////////
/*
#define	ADATE	60002
#define	ASIZE	60004
#define	ADEL	60006
#define	DTAG	60010
#define	DIR		60012
#define	LTAG	60020
#define	LDATE	60022
#define	LSIZE	60024
#define	LSTAMP	60026
#define	LARCH	60028
*/
////////////////////////////////////////////////////////////////////////
////////////// ALL SYSTEM TAGS < 500 ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

// SYSTEM LOG tags, Range 1-9
#define	SYSTAG		1	

///// TAG ARRAY tags, unique////////////////////////////////////////////
///////////////////////////////////////////////////////////
#define TAGZERO		0
// PAGE tags (SYSTEM Page tag range: 1-99)
#define	HOME		1   
#define	SYSTEM		2
#define FPANEL		3	
#define TPANEL		4	
#define	DIAGNOSTICS	5
#define	CLOCK		6
#define	ALARMP		7
#define	SOUNDS		8
#define	DPINS		9
#define	APINS		10
#define	ARCHIVES	11
#define	EEPROM		12
#define	LOGSYS		13

#define LOGS		29   // 29->40  11 RESERVED FOR LOGS

///////////////////////////////////////////////////////////
// row & pin tags  (SYSTEM  range: 256-599)

#define _WARN_OVR	258  // used by warnDelay system, can be pins defined in fB_USR_defines.cpp or fB_SYS_Defines.cpp
#define _TALLED	259
#define HEADER		260
#define _FRAM		267
#define _MOUNT		268
#define _MSG		271
#define VCC			272
#define DRST		273
#define	_EAUTO		274  // required


#define CLKYR		301		    
#define CLKMO		302		    
#define CLKDY		303		    
#define CLKHH		304		    
#define CLKMM		305		    
#define CLKSET		306		    
#define CLKGET		307		    

#define PNCUT 		320		    
#define PNPIN 		321	    
#define PNROW 		322		    
#define PNCOL 		323		    
#define PNCRD 		324	    
#define PNTOG 		325		    
#define PNGAT 		326		    
#define PNADC 		327		    
#define PNVAL 		328		    
#define PNBIA 		329		    
#define PNOFF 		330		    
#define PNINC 		331		    

#define FSTAMP		337		    
#define FARCH		338		    
#define FSIZE		339	
#define FDATE		340	
#define FDEL		341	
#define FDUMP		342	
#define	FSTD		343  // tag for FPANEL row that can house either FSTAMP or FDELETE subtype
#define CLK			344		

#define	ELOAD		350
#define	_ESTOR		351
#define	EDUMP		352
#define	ECLR		353

#define	_SECWD		368		// waning delay duration
#define	_SECWN		369		// warning duration
#define	_SECAL		370		// alarm period

#define	_TALBT		360
#define	_TALIN		361
#define	_TALFL		362
#define	_TALWN		363
#define	_TALAC		364
#define	_TALQS		365
#define	_TALEG		366
#define	_TALON		367

//////////////////////////////////////////////////////WARN CONSTANTS

#define	_ALRMBT		0
#define	_ALRMIN		1
#define	_ALRMFL		2
#define	_ALRMWN		3
#define	_ALRMAC		4
#define	_ALRMQS		5
#define	_ALRMEG		6

#define G_LEFT			0
#define G_RIGHT			1
#define G_STAMP			2
#define G_DELETE		3
#define G_NOLOG			4
#define G_INPUT			5
#define G_AMP			6
#define G_STRIKE		7
#define G_TOGGLE		8
#define G_GATE			9
#define G_LOGS			10
#define G_SHUTDOWN		11
#define G_SWITCHTO		12
#define G_DELAYSHUT		13
#define G_DELAYSW2		14
#define G_CHGALT		15
#define G_CHGEXT		16
#define G_BLANK			17
#define G_MANUAL		18
#define G_FAIL			19
#define G_FAIL_RTC		20
#define G_FAIL_SD		21
#define G_ALARM			22
#define G_INVERTER		23

#define	MAXGRAYCOUNT	24

#define _TIMER_MAX_EVENTS		10     
#define	_TIMER_UPDATE			0 
#define	_TIMER_WARNDELAY		1 
#define	_TIMER_WARN				2
#define	_TIMER_ALARM			3 
#define	_TIMER_FREEINDEXSTART	4 

#define _WD_OFF			0  // FOR WARNDELAY ACTION CODE
#define _WD_SKIP		1  
#define _WD_WARN		2
#define _WD_DELAY		3
#define _WD_ACT			4




	
#endif


