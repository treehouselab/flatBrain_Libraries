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

// 
#define AT_SPISS 		    53 //  dedicated Mega d53 pin for Chip Select pin for ATTINYs
#define SD_SPISS 		    15 //  dedicated Mega d15 pin for Chip Select pin for TFT-SD card 
#define SPISPEED			0x01 //(0-VERY HIGH....3-VERY SLOW)

#define ALARM_PIN		11 // option: pin 45

#define NAV_INT			5 // Display Navigation button interrupt number, corresp. to Mega pin 18
#define NAV_INTPIN		18
#define K0_INT			0 // Display Navigation button interrupt number, corresp. to Mega pin 2
#define K0_INTPIN		2 
#define K1_INT			1 // Display Navigation button interrupt number, corresp. to Mega pin 3
#define K1_INTPIN		3

#define RTC_ADDRESS		0x68
#define EE_ADDRESS		0x52

#define fERR  -1

#define ATAG		60000     // reserved for Archive Tag
#define I2CFAST			1	  
#define I2CSLOW			0	  
#define I2CTIMEOUT		2000  //ms	  
#define SERIALSPEED		9600  
#define ROUNDOFF		.0005  // added to calcs to round off displayed values
#define BASEGLOBAL		0 // allows 25 gSys objects


#define MAXGLOBALS		25  // for eeprom runaway only
#define MAXPINXMAP		44     
#define MAXCHARSDUMP	256  // reduce this if logfile dump collides with heap 
#define MAXLISTROWS		10 
#define MAXCHARSTEXT	15 
#define MAXVDRDEX		6 

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
#define X2				1
#define X3				2

// IOX Card I2C allowed card addresses: 
// CARD TYPE X2 TAKES SINGLE ADDRESS, UP TO 8
// CARD TYPE X3 RESERVES 2 ADDRESS PAIRS, UP TO 4
#define	C0		0x20	// X2 OR X3
#define C1		0x21	// X2 IF C0 !X3
#define	C2		0x22	// X2 OR X3
#define C3		0x23	// X2 IF C2 !X3
#define	C4		0x24	// X2 OR X3  
#define C5		0x25	// X2 IF C4 !X3 
#define	C6		0x26	// X2 OR X3
#define C7		0x27	// X2 IF C6 !X3   


// Pin sides
#define	L		0
#define	R		1

//Pin types		unique4, 0..7
#define	D		0
#define A		1


// Pin mapping for IOX cards 
// CARD TYPE X2 
#define X2AA			7	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X2AB			6	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X2AC			5	// MCP PIN MAPS TO CD4051 A ADDRESS PIN, FOR ANALOG CHANNEL
#define X2BA			4	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X2BB			3	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X2BC			2	// MCP PIN MAPS TO CD4051 B ADDRESS PIN, FOR VD CHANNEL
#define X2LD			1	// MCP PIN MAPS TO  BOARD LED
#define X2GT			0	// MCP PIN MAPS TO  opiso gate

#define X3GT			0	// MCP PIN MAPS TO N-Chan FET gate




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
#define	VISIBLE		0x04
#define	SELECTED	0x08
#define	MARK		0x10
#define	LOG			0x20
#define	DISABLE		0x40
//#define	 		0x80

#define	RTC			0x01   // brain status flags, unique uint8_t bits 
#define	SD			0x02   

#define	GAUGE			1   // Block type
//#define SWITCH			111	// Block type	 
//#define MONITOR			112	// Block type	 
#define WINDOW			2	// Block type	 


#define	HOME		0   // page tags,16 bit
#define	SYSTEM		1
#define	GLOBALS 	2
#define GPAGE		3	
#define	GLIST		4   
#define GDIR		5	
#define	STACK   	6
#define FILES		7
#define	CLOCK		8
#define	ALARM		9
#define	VDIV		10

#define	PGSY		1  // page type,8 bit
#define	PGUS		2  


#define FROW			30   // 10 numbers RESERVED FOR FILE LIST ROW INDEXES
#define SROW			40   // 10 numbers RESERVED FOR STACK LIST ROW INDEXES
#define GROW			50   // 10 numbers RESERVED FOR GLOBAL LIST ROW INDEXES
#define YROW			60   // 10 numbers RESERVED FOR GLOBAL LIST ROW INDEXES

#define GLOG			81  // Global Tags
#define GOPR			82
#define GINP			83
#define GVAL			84
#define GFAC			85
#define GINC			86
#define GSET			87
#define GIAG			90  // EEPROM Init all  Globals
#define GSAG			91  // EEPROM Store all Globals
#define GLAG			92	// SD LOG all Globals
#define GDEF			93	// SD LOG all Globals
#define GBOO			94	// BOOT GLOBALS ON INIT

#define	NOFLAG			0x00  //  GLOBAL  flags, unique uint8_t bits
#define	GPIN			0x01  // 
#define	GSYS			0x02
#define	GINIT			0x04  // store and init from eeprom
#define	GINPUT			0x08  // 
#define	GBIAS			0x10  // 
   


#define CLKYR			100	// Clock Set row index	    
#define CLKMO			101	// Clock Set row index	    
#define CLKDY			102	// Clock Set row index	    
#define CLKHH			103	// Clock Set row index	    
#define CLKMM			104	// Clock Set row index	    
#define CLKSET			105	// Clock Set row index	    
#define CLKGET			106	// Clock Set row index	    

#define VDCUT 			120	// Action Type (atype)	    
#define VDPIN 			121	// Action Type (atype)	    
#define VDROW 			122	// Action Type (atype)	    
#define VDCOL 			123	// Action Type (atype)	    
#define VDRES 			124	// Action Type (atype)	    
#define VDCRD 			125	// Action Type (atype)	    
#define VDGAT 			126	// Action Type (atype)	    
#define VDSET 			127	// Action Type (atype)	    


#define HEADER			130   // Action Type (atype)
#define TOGGLE			131	// Action Type (atype)	    
#define JPAGE			132  // Action Type (atype)
#define GPANEL			135	// Action Type (atype)	    
#define FPANEL			136	// Action Type (atype)	    
#define FSTAMP			137	// Action Type (atype)	    
#define FARCH			138	// Action Type (atype)	    
#define FSIZE			139	// Action Type (atype)
#define FDATE			140	// Action Type (atype)
#define FDEL			141	// Action Type (atype)
#define FDUMP			142	// Action Type (atype)
#define	FSTD			143  // tag for FPANEL row that can hgouse either FSTAMP or FDELETE subtype
#define CLK				144	// Action Type (atype)	    
#define VALUE			145	// Action Type (atype)	    
#define NOACT			146	// Action Type (atype)	    
#define GADJ			149	// Action Type (atype)	    
#define GARB 			150	// Action Type (atype)	
#define SET 			152	// Action Type (atype)	    
#define TOGL 			153	// Action Type (atype)	    

#define STACK			167	// Action Type (atype)    
#define SDDIR			168	// Action Type (atype)    


#define	STRIKE			1	// row format  (unique from Action Types)
#define FLOAT1			2	// format
#define FLOAT2			3	// format
#define INT5			4	// format
#define TEXT			5	// format    
#define TITLE			6	// format    
#define LAMP			7	// format    

#define	NOSTATE			9  //  Row state
#define	ON				1
#define	OFF				0

#define	FORCE			0 // Row show states
#define	REFRESH			1 
#define	HIDE			2 

#endif


