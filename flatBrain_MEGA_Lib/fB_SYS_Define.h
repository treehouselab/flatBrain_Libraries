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


#define MAXEETAGS		25  // for eeprom runaway only
#define MAXCHARSDUMP	256  // reduce this if logfile dump collides with heap 
#define MAXLISTROWS		10 
#define MAXCHARSTEXT	15 
#define MAXVDRDEX		6 
#define	MAXPSTRCOUNT	11

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



// Pin sides
#define	L		0
#define	R		1

//Pin types		unique4, 0..7
#define	D		0
#define A		1


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

//////////////////////////////////////////
// Tag->flags is 8 bits, LAST 2bits IS MASKED FOR FORMAT
#define	PAGE			0x01	// ROW PAGE JUMP

#define	MASKP			0x1E	// next 4 bits overwritte in case of PAGE to hold pagerowcount

#define	TSYS			0x02	// System Tag
#define	STOREE			0x04	// store in eeprom
#define	LOG				0x08	
//#define	AVAIL		0x10	

#define	MASKF			0xE0	// last 3 bits reserved for format, EXCLUSIVE
#define	BLANK			0x00	
#define	TEXT			0x20	// default
#define	INT5			0x40	
#define	FLOAT1			0x60
#define	FLOAT2			0x80	
#define	BINARY			0xB0	
#define	STRIKE			0xC0	
//#define	AVAIL		0xD0	

//////////////////////////////////////////
//////////////////////////////////////////
// Row->flags is 8 bits, LAST 2bits is unused ( format defaults to Tag
//#define	AVAIL		0x0O	
#define	MARK			0x01	
#define	VISIBLE			0x02	
//#define	avail  		0x04
//#define	avail  		0x08
//#define	avail  		0x10

#define	MASKA			0xE0	 // last 3 bits reserved for format, EXCLUSIVE
#define	NOACT			0x00	
#define	TOGGLE			0x20	
#define	INCR			0x40	
#define	SHFTPULSE		0x60	
#define	PULSE			0x80
#define	REFRESH			0xB0
#define	CGATE			0xD0
//#define AVAIL			0xE0	

//////////////////////////////////////////


#define GAUGEHT		    60
#define GSTARTY			5
#define GSTARTX			5
#define GTITLEX			168
#define GTITLEY1		34
#define GTITLEY2		5
#define GMAXCHARSTITLE	4

#define	NOTYPE		0		// page/button/block type
#define	NOSTATUS	0x00    // page/button/block status
#define	CREATED		0x01   
#define	INITED		0x02   
//#define	VISIBLE		0x04
#define	SELECTED	0x08
//#define	MARK		0x10
#define	LOG			0x20
#define	DISABLE		0x80
//#define	 		0x80

#define	RTC			0x01   // brain status flags, unique uint8_t bits 
#define	SD			0x02   

#define	GAUGE			1   // Block type
//#define SWITCH			111	// Block type	 
//#define MONITOR			112	// Block type	 
#define WINDOW			2	// Block type	 

#define	STRIKE			1	// row format  (unique from Action Types)
#define FLOAT1			2	// format
#define FLOAT2			3	// format
#define INT5			4	// format
#define TEXT			5	// format    
#define TITLE			6	// format    
#define LAMP			7	// format  

#define BINARY			1	// EEPROM column format    

#define	NOSTATE			9  //  Row state
#define	ON				1
#define	GATE				1
#define	OFF				0

#define	FORCE			0 // Row show states
#define	REFRESH			1 
#define	HIDE			2 


////////////////////////////////////////////////////////////////////////
////////////// ALL SYSTEM TAGS < 500 ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

#define	SYSTAG		1	// LOG tags

#define	HOME		0   // page tags,16 bit
#define	SYSTEM		1
#define	GLOBALS 	2
#define FILES		7
#define FPANEL		5	
#define TPANEL		5	
#define	DIAGNOSTICS	6
#define	CLOCK		8
#define	ALARM		9
#define	PINS		10

#define TLIST		20   // 10 numbers (MAXLISTROW)  RESERVED FOR LIST of TAG POINTER ARRAY INDEXES


#define TSTAGS		31
#define TUTAGS		32
#define TPINS		33
#define TLOGS		34
#define TPAGES		35
#define TROWS		36
#define FRAM		37

#define TLOG			81  //  Tags
#define TOPR			82
#define TINP			83
#define TVAL			84
#define TFAC			85
#define TINC			86
#define TSET			87
#define TIAT			90  // EEPROM Init all  TAGS
#define TSAT			91  // EEPROM Store all TAGS
#define TLAS			92	// SD LOG all TAGS
#define TLAU			93	// SD LOG all TAGS
#define TBOOT			94	// BOOT TAGS ON INIT

   


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
#define PGATE 			128		    



#define HEADER			130   
#define TOGGLE			131		    
#define JPAGE			132  
#define FPANEL			136		    
#define FSTAMP			137		    
#define FARCH			138		    
#define FSIZE			139	
#define FDATE			140	
#define FDEL			141	
#define FDUMP			142	
#define	FSTD			143  // tag for FPANEL row that can hgouse either FSTAMP or FDELETE subtype
#define CLK				144		    
#define VALUE			145		    
#define NOACT			146		    
#define TADJ			149		    
#define TARB 			150		
#define GATE 			152		    
#define PULSE 			154		    
#define YPULSE 			155		    

#endif


