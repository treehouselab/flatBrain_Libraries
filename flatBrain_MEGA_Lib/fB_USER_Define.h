#ifndef FP_USER_DEFINES_H
#define FP_USER_DEFINES_H


////////////////////////////////////////////////////////
////////// RESERVED ////////////////////////////////////
////////////////////////////////////////////////////////
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

///
/////////////////////////////////////////////////////////
//  USER DEFINES.  EDIT FOR EACH FLATPLANE CONFIGURATION
/////////////////////////////////////////////////////////
//#define MAXPINS		    8	// XIO
//#define MAXCARDS	    2	// XIO 
#define MAXSTACKPAGES	1	
#define MAXLOGS			5  // MAX LOGFILES
#define MAXAFILES		10  

#define ANALOGSAMPLESIZE	50
#define ANALOGSAMPLEDELAY	50



// LOGS

#define	GDEF	    40     // Reserved, Global Definition File
#define	LOG1	    41
#define	LOG2	    42
#define	LOG3	    43



// Card tags, unique
#define VMCU	1
#define TFTS	2
#define	PRT1	3
#define	PRT2	4
#define	VLVD	5

// Pin tags, unique, arbitrary MUST BE > 100

// UTILITY BOARD PIN TAGS

#define PLED1	105 
#define PLED2	106 
#define PANL1	106 
#define PANL2	107
#define PANL3	108
#define PANL4	109

#define LALR	110 // LVD ALARM LED (OUTPUT)
#define LRON	111 // LVD  RELAY ON (OUTPUT)
#define LOVR	112 // LVD OVERRIDE ON (INPUT)
#define LADR	113 // LVD DISPLAY ADR (OUTPUT)
#define LV0		114 // LVD V0 SENS (INPUT)

// Page Tags, used as indices for tPage array in fB_TFT.cpp
//#define P1	1
//#define	P2	2
#define	G1	    231		// USER GLOBALS START AT 100
#define	G2QWE	232
#define	G3	    233
#define	G4	    234
#define	G5	    235
#define	G6	    236



#endif
