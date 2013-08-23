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
#define K0_INT			0 // Display Navigation button interrupt number, corresp. to Mega pin 2
#define K0_INTPIN		2 
#define K1_INT			1 // Display Navigation button interrupt number, corresp. to Mega pin 3
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
// CARD TYPE X50 TAKES SINGLE ADDRESS, UP TO 8
// CARD TYPE X76 RESERVES 2 ADDRESS PAIRS, UP TO 4
#define	C0		0x20	// X50 OR X76
#define C1		0x21	// X50 IF C0 !X76
#define	C2		0x22	// X50 OR X76
#define C3		0x23	// X50 IF C2 !X76
#define	C4		0x24	// X50 OR X76  
#define C5		0x25	// X50 IF C4 !X76 
#define	C6		0x26	// X50 OR X76
#define C7		0x27	// X50 IF C6 !X76   


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
#define MAXLISTROWS		10 
#define MAXCHARSTEXT	15 
#define MAXVDRDEX		6 
#define	MAXPSTRCOUNT	11
#define	MAXTEMPTAG		500

// Bus to Arduino Mega pin mapping
#define	B0		A8
#define B1		A9
#define	B2		A10
#define	B3		A11

#define	K0		44
#define	K1		42
#define	K2		2
#define	K3		3

// Card types
#define	BRAIN			0
#define X50				1
#define X76				2

// Pin DIRECTION
#define	INPUT		0
#define	OUTPUT		1

// Pin sides
#define	COL_L		0
#define	COL_R		1

//Pin types		unique4, 0..7
#define	IO_D		0
#define IO_A		1


// Pin mapping for IOX cards 
// CARD TYPE X50 
#define X50AA			7	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X50AB			6	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X50AC			5	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X50BA			4	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X50BB			3	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X50BC			2	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X50LD			1	// MCP PIN MAPS TO  BOARD LED
#define X50GT			0	// MCP PIN MAPS TO  opiso gate

// CARD TYPE X76 
#define X76AA			11	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X76AB			12	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X76AC			9	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X76AD			8	// MCP PIN MAPS TO CD4067 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X76IN			10	// MCP PIN MAPS TO CD4067 A INHIBIT PIN
#define X76BA			4	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X76BB			3	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X76BC			2	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X76RS			13	// MCP PIN MAPS TO 2nd MCP _RST pin
#define X76LD			1	// MCP PIN MAPS TO  BOARD LED
#define X76GT			0	// MCP PIN MAPS TO  op-iso gate

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


#define	HIDE			1 
#define	PGATE			2
#define	ON				1
#define	OFF				0

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Tag->flags is 16 bits, top 4bits is used for fow count buffer
#define	PAGE			0x0001	 
#define	TSYS			0x0002	// System Tag
#define	STOREE			0x0004	// EEPROM Store 
#define	LOG				0x0008	// Log to SD card
#define	MARK			0x0010	// Mark row
#define	HIDE			0x0020
#define	TTITLE			0x0040	// use char* ptitle for row title instead of Ptitle
//#define			    0x0080
#define	UNDEF			0x0100
#define	LOCAL			0x0200 
//#define				0x0400
//#define				0x0800
#define	MASKP			0xF000	// 4 bits reserved ROWCOUNT
#define	RCOFFSET		12	    // Offset of rowCount in Tag flag

////////////////////////////////////////////////////////////////////////////////
//Tag->format is 8 bits. The low 4bits of Tag->format contain one of 11 possible 
// action codes (incl. zero) in decimal format. This code is converted to 32bit BLAMP
// for run-time comparisons (listed below). This allows for all the Tflag, format, and 
// action flags to be entered on a single OR'd field at Tag definition arguments.

// The high 4bits of  Tag->format contains one of 11 possible format codes in similar scheme.

#define	MASK8A			0x0F	 
#define	MASK32A			0x003FF000L
#define	UNDEF			0x00000000L	
#define	PINTOG			0x00001000L	
#define	INCR			0x00002000L	
#define	SHFTPULSE		0x00004000L	
#define	PULSE			0x00008000L
#define	UPDATE			0x00010000L
#define	TGATE			0x00020000L
#define	NOACT			0x00040000L
#define	TOGGLE			0x00080000L
#define	FROW	       	0x00100000L
//#define				0x00200000L

#define	MASK8F			0xF0	 
#define	MASK32F			0xFFC00000L
#define	BLANK			0x00000000L	
#define	TEXT			0x00400000L	
#define	INT5			0x00800000L	
#define	FLOAT1			0x01000000L
#define	FLOAT2			0x02000000L	
#define	BLAMP			0x04000000L	
//#define				0x08000000L	
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
#define	MTAG2		2	 //test
#define	MTAG		1009	 //test

///// TAG ARRAY tags, unique////////////////////////////////////////////
///////////////////////////////////////////////////////////
// PAGE tags
#define	HOME		1   
#define	SYSTEM		2
#define FPANEL		3	
#define TPANEL		4	
#define	DIAGNOSTICS	5
#define	CLOCK		6
#define	ALARM		7
#define	PINS		8
#define	ARCHIVES	9


#define LOGS		19   //   11 RESERVED FOR LOGS

#define TBOOT		60	//SYSTEM TAGS
#define TSTAGS		61
#define TUTAGS		62
#define TPINS		63
#define TLOGS		64
#define TPAGES		65
#define TROWS		66
#define FRAM		67
#define HEADER		68

#define TLOG			81  //  Tags
#define TOPR			82
#define TINP			83
#define TVAL			84
#define TFAC			85
#define TINC			86
#define TSET			87
#define TIAT			90  
#define TSAT			91  
#define TLAS			92	
#define TLAU			93	
#define TADJ			95		    
#define TARB 			96		

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
#define PNCRD 			125		    
#define PNTOG 			126		    
#define PNADC 			127		    

//#define HEADER			130   
#define FSTAMP			137		    
#define FARCH			138		    
#define FSIZE			139	
#define FDATE			140	
#define FDEL			141	
#define FDUMP			142	
#define	FSTD			143  // tag for FPANEL row that can hgouse either FSTAMP or FDELETE subtype
#define CLK				144		  

	
#endif


