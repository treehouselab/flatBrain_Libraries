
#include <avr/pgmspace.h>
#include <fB_SYS_Define.h>
/*
// Pin sides
#define	L		0
#define	R		1

//Pin types		unique4, 0..7
#define	D		0
#define A		1


	// VDIV RESISTOR MAP TO CD4051 pin

	
	VDRmap[0] =		 5; /// NCON
	VDRmap[1] =		 0; /// no resistor
	VDRmap[2] = 	 2; 
	VDRmap[3] = 	 1; 
	VDRmap[4] = 	 4; 
	VDRmap[5] = 	 3; 
	VDRmap[6] = 	 6; 
*/
uint8_t VDRmap[7] PROGMEM={5,0,2,1,4,3,6 };

// in this block,  D coresponds to MCP pin, A to CD pin

uint8_t XmapB[40] PROGMEM={
	12,L,A,8,  // BO
	12,R,A,9,  // B1
	13,L,A,10,  // B2
	13,R,A,11,  // B3
	14,L,D,0,   // VD
	14,R,D,0,   // K0
	15,L,D,44,  // K1
	15,R,D,42,  // K2
	16,L,D,2,  // K3
	16,R,D,3  // K4
};

uint8_t Xmap50[64] PROGMEM={
	17,L,D,9,
	17,R,D,8,
	18,L,D,11,
	18,R,D,10,
	19,L,D,13,
	19,R,D,12,
	20,L,D,15,
	20,R,D,14,
	21,L,A,7,
	21,R,A,5,
	22,L,A,4,
	22,R,A,6,
	23,L,A,0,
	23,R,A,2,
	24,L,A,3,
	24,R,A,1
};
	
uint8_t Xmap76[128] PROGMEM={
	17,L,D,8,
	17,R,D,9,
	18,L,D,11,
	18,R,D,10,
	19,L,D,13,
	19,R,D,12,
	20,L,D,15,
	20,R,D,14,
	21,L,A,6,
	21,R,A,7,
	22,L,A,4,
	22,R,A,5,
	23,L,A,2,
	23,R,A,3,
	24,L,A,0,
	24,R,A,1,
	25,L,D,7,
	25,R,D,6,
	26,L,D,5,
	26,R,D,4,
	27,L,D,3,
	27,R,D,2,
	28,L,D,1,
	28,R,D,0,
	29,L,A,12,
	29,R,A,11,
	30,L,A,10,
	30,R,A,9,	
	31,L,A,13,  
	31,R,A,8,
	32,L,A,14,  
	32,R,A,15
};

