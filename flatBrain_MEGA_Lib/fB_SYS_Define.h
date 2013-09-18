//////////////////////////////////////
// SYSTEM DEFINES. EDIT AT YOUR PERIL.
//////////////////////////////////////
#ifndef FP_SYS_DEFINES_H
#define FP_SYS_DEFINES_H

#define STRINGIFY(x) #x
#define CONCAT(prefix, suffix) prefix##suffix



#define WRPIN	1     // for port banging WR pin on TFT ( corresponds to ARDUINO D23  )
#define WRPORT	PORTA // for port banging WR pin on TFT ( corresponds to ARDUINO D23  )
#define WRDDR	DDRA // for port banging WR pin on TFT ( corresponds to ARDUINO D23  )
#define RSPIN	2     // for port banging RS pin on TFT ( corresponds to ARDUINO D24  )
#define RSPORT	PORTA // for port banging RS pin on TFT ( corresponds to ARDUINO D24  )
#define RSDDR	DDRA // for port banging WR pin on TFT ( corresponds to ARDUINO D24  )

#define AT_SPISS 		    53 //  dedicated Mega d53 pin for Chip Select pin for ATTINYs

#define SD_SSPIN 		    15 //  dedicated Mega d15 pin for Chip Select pin for TFT-SD card 
#define SD_SSBIT 		    0 
#define SD_SSPORT 		    PORTJ
#define SD_SSDDR 		    DDRJ 
#define SPISPEED			0x01 //(0-VERY HIGH....3-VERY SLOW)

#define ALARM_PIN		11 // option: pin 45

#define NAV_INT			5 // Display Navigation button interrupt number, corresp. to Mega pin 18
#define NAV_INTPIN		18
#define WARN_INT		4 // Alarm warning interrupt number, corresp. to Mega pin 20
#define WARN_INTPIN		19
#define K0_INT			0 //  interrupt number, corresp. to Mega pin 2
#define K0_INTPIN		2 
#define K1_INT			1 //  interrupt number, corresp. to Mega pin 3
#define K1_INTPIN		3

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

#define _TIMER_MAX_EVENTS		10     
#define	_TIMER_UPDATE			0 
#define	_TIMER_WARNDELAY		1 
#define	_TIMER_WARN				2
#define	_TIMER_ALARM			3 
#define	_TIMER_FREEINDEXSTART	4 

#define fERR  -1

#define ATAG		60000     // reserved for Archive Tag
#define I2CFAST			1	  
#define I2CSLOW			0	  
#define I2CSPEED		1
#define I2CTIMEOUT		100  //ms	  
#define SERIALSPEED		9600  
#define ROUNDOFF		.0005  // added to calcs to round off displayed values
#define BASEGLOBAL		0 // allows 25 gSys objects
#define	PULSEMSECS		200

#define MAXEETAGS		25  // for eeprom runaway only
#define MAXCHARSDUMP	256  // reduce this if logfile dump collides with heap 
#define MAXLISTROWS		9 
#define	MESSAGE_Y		295
#define MAXCHARSLINE	15 
#define MAXCHARSTEXT	8 
#define MAXCHARSD2STR	6 
//#define MAXVDRDEX		6 
#define	MAXTEMPTAG		500
#define	MAXZEROADC		250 // max ADC reading that qualifies for binary zero


// Bus to Arduino Mega pin mapping
#define	_B0		A8
#define _B1		A9
#define	_B2		A10
#define	_B3		A11

#define	_K0		44
#define	_K1		42
#define	_K2		2
#define	_K3		3

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
#define _X76LD			1	// MCP PIN MAPS TO  BOARD LED
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


#define GAUGEHT		    60
#define GSTARTY			5
#define GSTARTX			5
#define GTITLEX			168
#define GTITLEY1		34
#define GTITLEY2		5
#define GMAXCHARSTITLE	4


#define	RTC			0x01   // brain status flags, unique uint8_t bits 
#define	SD			0x02   


#define	_PGATE			2
#define	ON				1
#define	OFF				0

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Tag->flags is 16 bits, top 4bits is used for row count buffer
#define	_PAGE			0x0001	 
#define	_TSYS			0x0002	// System Tag
#define	_STOREE			0x0004	// EEPROM Store 
#define	_LOG				0x0008	// Log to SD card
#define	_MARK			0x0010	// Mark row
#define	_HIDE			0x0020
#define	_TTITLE			0x0040	// use char* ptitle for row title instead of Ptitle
#define	_TTAG			0x0080  // has target tag stored in pin/tTag union
#define	_UNDEF			0x0100
#define	_LOCAL			0x0200 
#define	_DUBL			0x0400
//#define				0x0800
#define	_MASKP			0xF000	// 4 bits reserved ROWCOUNT
#define	_RCOFFSET		12	    // Offset of rowCount in Tag flag

////////////////////////////////////////////////////////////////////////////////
//Tag->format is 8 bits. The low 4bits of Tag->format contain one of 11 possible 
// action codes (incl. zero) in decimal format. This code is converted to 32bit BLAMP
// for run-time comparisons (listed below). This allows for all the Tflag, format, and 
// action flags to be entered on a single OR'd field at Tag definition arguments.

// The high 4bits of  Tag->format contains one of 11 possible format codes in similar scheme.

#define	MASK8A			0x0F	 
#define	MASK32A			0x003FF000L
//#define				0x00000000L	
//#define			0x00001000L	
#define	INCR			0x00002000L	
#define	SHFTPULSE		0x00004000L	
#define	PULSE			0x00008000L
#define	UPDATE			0x00010000L
//#define			0x00020000L
#define	NOACT			0x00040000L
#define	TOGGLE			0x00080000L
//#define		       	0x00100000L
//#define				0x00200000L

#define	MASK8F			0xF0	 
#define	MASK32F			0xFFC00000L
#define	BLANK			0x00000000L	
#define	TEXT			0x00400000L	
#define	INT5			0x00800000L	
#define	FLOAT1			0x01000000L
#define	FLOAT2			0x02000000L	
#define	BLAMP			0x04000000L	
#define	D2STR			0x08000000L	
#define	PTEXT			0x10000000L	
//#define				0x20000000L
//#define				0x40000000L
//#define				0x80000000L

////////////////////////////////////////////////////////////////////???
////////////////////////////////////////////////////////////////////

#define	ATAG	60000
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

////////////////////////////////////////////////////////////////////////
////////////// ALL SYSTEM TAGS < 500 ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

// SYSTEM LOG tags, Range 1-9
#define	SYSTAG		1	

///// TAG ARRAY tags, unique////////////////////////////////////////////
///////////////////////////////////////////////////////////
// PAGE tags (SYSTEM Page tag range: 1-28)
#define	HOME		1   
#define	SYSTEM		2
#define FPANEL		3	
#define TPANEL		4	
#define	DIAGNOSTICS	5
#define	CLOCK		6
#define	ALARM		7
#define	DPINS		8
#define	APINS		9
#define	ARCHIVES	10

#define LOGS		29   //   11 RESERVED FOR LOGS

#define TAGZERO		0
#define _WARN_OVR	58  // used by warnDelay system, can be pins defined in fB_USR_defines.cpp or fB_SYS_Defines.cpp
#define _ALARM_LED	59
#define HEADER		60
#define TBOOT		61	//SYSTEM TAGS
#define TUTAGS		62
#define TPINS		63
#define TLOGS		64
#define TPAGES		65
#define TROWS		66
#define FRAM		67
#define TIAT		69
#define TSAT		70
#define _MSG			71
#define VCC			72


#define CLKYR			100		    
#define CLKMO			101		    
#define CLKDY			102		    
#define CLKHH			103		    
#define CLKMM			104		    
#define CLKSET			105		    
#define CLKGET			106		    

#define PNCUT 			120		    
#define PNPIN 			121		    
#define PNROW 			122		    
#define PNCOL 			123		    
#define PNCRD 			124	    
#define PNTOG 			125		    
#define PNGAT 			126		    
#define PNADC 			127		    
#define PNFAC 			128		    
#define PNOFF 			129		    
#define PNVAL 			130		    

#define FSTAMP			137		    
#define FARCH			138		    
#define FSIZE			139	
#define FDATE			140	
#define FDEL			141	
#define FDUMP			142	
#define	FSTD			143  // tag for FPANEL row that can hgouse either FSTAMP or FDELETE subtype
#define CLK				144		 

#define P_LEFT			0
#define P_RIGHT			1
#define P_STAMP			2
#define P_DELETE		3
#define P_NOLOG			4
#define P_INPUT			5
#define P_AMP			6
#define P_STRIKE		7
#define P_TOGGLE		8
#define P_GATE			9
#define P_LOGS			10
#define P_SHUTDOWN		11
#define P_SWITCHTO		12
#define P_DELAYSHUT		13
#define P_DELAYSW2		14
#define P_CHGALT		15
#define P_CHGEXT		16
#define P_BLANK			17

#define	MAXPSTRCOUNT	18


	
#endif


