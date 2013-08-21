#ifndef FP_USER_DEFINES_H
#define FP_USER_DEFINES_H


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




// LOG TAGS ("LOGDEF" IS RESERVED FOR DEFAULT LOG DEFINITION)
// RANGE: 10-255

#define	LOG1	    11
#define	LOG2	    12
#define	LOG3	    13

// Card tags,unique, RANGE: 1-255
#define VMCU	2
#define TFTS	3
#define YCRD	4
#define	PRT1	5
#define	VLVD	6

///////////////////////////////////////////////////////////////////////////////
//////////////  ALL USER-DEFINED TAG ARRAY Tags MUST BE > 500 //////////////////
///////////////////////////////////////////////////////////////////////////////


// Pin , Page, Row Tags, unique

#define RELAYPULS	600

#define Y1			601 
#define Y2			602 
#define Y3			603 
#define Y4			604 
#define Y5			605 
#define Y6			606
#define YRST		607
#define YSHFT		608

#define RELAYSTAT	620

#define Y1S			621
#define Y2S			622
#define Y3S			623
#define Y4S			624
#define Y5S			625 
#define Y6S			626

#define SENSORS		640

#define V1			641
#define V2			642
#define V3			643
#define CZ			644
#define CC			645
#define CL			646 



#endif
