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
#define YCRD	3
#define	PRT1	4
#define	VLVD	5


// Page Tags, used as indices for tPage array in fB_TFT.cpp >19
#define VANDUINO	20

// Pin tags, unique, arbitrary MUST BE > 100
#define Y1		101 
#define Y2		102 
#define Y3		103 
#define Y4		104 
#define Y5		105 
#define Y6		106
#define YRST	107
#define YSHFT	108

#define BY1		201 
#define BY2		202 
#define BY3		203 
#define BY4		204 
#define BY5		205 
#define BY6		206
#define BYRST	207
#define BYSHFT	208

#define Y1S		110
#define Y2S		111
#define Y3S		112
#define Y4S		113
#define Y5S		114 
#define Y6S		115




#define	G1	    231		
#define	G2QWE	232
#define	G3	    233
#define	G4	    234
#define	G5	    235
#define	G6	    236



#endif
