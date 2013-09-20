#ifndef FP_USER_DEFINES_H
#define FP_USER_DEFINES_H


/////////////////////////////////////////////////////////
//  USER DEFINES.  EDIT FOR EACH FLATPLANE CONFIGURATION
/////////////////////////////////////////////////////////
//#define MAXPINS		    8	// XIO
//#define MAXCARDS	    2	// XIO 
//#define MAXLOGS			5  // MAX LOGFILES
#define MAXAFILES		    10  
#define ANALOGSAMPLESIZE	50
#define ANALOGSAMPLEDELAY	10

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

#define RSTATUS	121

#define Y1S			621
#define Y2S			622
#define Y3S			623
#define Y4S			624
#define Y5S			625 
#define Y6S			626
#define WARN		627
#define WLED		628

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

#define VLIMITS	123

#define CHLO			701
#define CHHI			702
#define DLO1			703
#define DLO2			704
#define DLO3			705
#define DHI1			706
#define DHI2			707 
#define DHI3			708 
#define VEXS			709 
#define VALT			710 



#endif
