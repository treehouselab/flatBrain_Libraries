#ifndef FP_USER_DEFINES_H
#define FP_USER_DEFINES_H


/////////////////////////////////////////////////////////
//  USER DEFINES.  EDIT FOR EACH FLATPLANE CONFIGURATION
/////////////////////////////////////////////////////////
//#define MAXPINS		    8	// XIO
//#define MAXCARDS	    2	// XIO 
//#define MAXLOGS			5  // MAX LOGFILES
#define MAXAFILES		    10  
#define _ANALOGSAMPLESIZE	25
#define _ANALOGSAMPLEDELAY	10
#define _LOGDECIMALS		2

#define VREFADJ	0.9901

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
//////////////  USER-DEFINED TAGs //////////////////
///////////////////////////////////////////////////////////////////////////////


// Pin , Page, Row Tags, unique
// User Page tag range: 100-255)
// User Row tag range:	>= 600 )

#define RPULSE	120

#define Y1			601 
#define Y2			602 
#define Y3			603 
#define Y4			604 
#define Y5			605 
#define Y6			606
#define YRST		607
#define YSHFT		608

#define VSTATUS	121

#define Y1S			621
#define Y2S			622
#define Y3S			623
#define Y4S			624
#define Y5S			625 
#define Y6S			626
#define WARN		627
#define WLED		628
#define LBOOT		629

#define SENSORS	122

#define V0			641
#define V1			642
#define V2			643
#define V3			644
#define CZ			645
#define CX			646
#define CL			647 
#define IGN			648 
#define ALT			649 

#define VGLOBALS	123

#define CHLO			701
#define CHHI			702
#define VEXS			703 
#define VALT			704 
#define CPSEC			705  // VANDUINO CURRENT PAGE INTERVAL
#define BKSEC			706  // VANDUINO BACKGROUND INTERVAL
#define LGMIN			707  // VANDUINO LOG INTERVAL

#define LVDLIMITS	124

#define DLO1			710
#define DLO2			711
#define DLO3			712
#define DHI1			713
#define DHI2			714 
#define DHI3			715 

#endif
