#ifndef FP_USER_DEFINES_H
#define FP_USER_DEFINES_H


////////////////////////////////////////////////////////
////////// RESERVED ////////////////////////////////////
////////////////////////////////////////////////////////
// 
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


//////////////////////////////////////////////////////////
//////////////  ALL TAGS MUST BE > 500 //////////////////
////////////////////////////////////////////////////////


// LOGS

#define	LOG1	    541
#define	LOG2	    542
#define	LOG3	    543

// Card tags,unique
#define VMCU	501
#define TFTS	502
#define YCRD	503
#define	PRT1	504
#define	VLVD	505

// Pin tags, unique
#define Y1		501 
#define Y2		502 
#define Y3		503 
#define Y4		504 
#define Y5		505 
#define Y6		506
#define YRST	507
#define YSHFT	508

#define Y1S		510
#define Y2S		511
#define Y3S		512
#define Y4S		513
#define Y5S		514 
#define Y6S		515

#define V1		520
#define V2		521
#define V3		522
#define CZ		523
#define CC		524
#define CL		525 

// Page Tags, unique

#define RELAYPULS	501
#define RELAYSTAT	502
#define SENSORS	503

// Row Tags, unique


#endif
