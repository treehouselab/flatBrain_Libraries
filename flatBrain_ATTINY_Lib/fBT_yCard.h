
#ifndef FBT_YCARD_H
#define FBT_YCARD_H

#define  MULTI  0x01  // button modes
#define  SOLO	0x02
#define  RADIO	0x04


class fBT_yButton {
	public:
		uint8_t bDg;
		uint8_t sDg;
		uint8_t mode;		// SOLO, GANG, MOM, 
};

class fBT_yCard {
	public:
		uint8_t		mapCount;
		uint8_t		enableDelay;
		uint16_t	bState;
		uint16_t	sState;
		//uint16_t	sState;
		fBT_yButton yMap[7];
		fBT_yButton altBut;

		fBT_yCard();
		void insertYmap(uint8_t bDg, uint8_t sDg, uint8_t mod);
		void buildCardMap();
		void initCard();
		void pollButtons();
		void getSstate();
		void applySstate();
		void allOff();
};

#endif

/*
#define RELAY76_B1			12	
#define RELAY76_B2			13	// D13, ATTINY pin 16 maps to RELAY76 button 2, from top of card
#define RELAY76_B3			11	// ATTINY pin 14 maps to RELAY76 button 3, from top of card
#define RELAY76_B4			10	// ATTINY pin 13 maps to RELAY76 button 4, from top of card
#define RELAY76_B5			9	// ATTINY pin 12 maps to RELAY76 button 5, from top of card
#define RELAY76_B6   		1	// ATTINY pin 3  maps to RELAY76 button 6, from top of card
#define RELAY76_B7   		8	// ATTINY pin 11 maps to RELAY76 button 7, from top of card

#define RELAY76_S1			6	// ATTINY pin 8 maps to RELAY76 switch 1
#define RELAY76_S2			3	// ATTINY pin 5 maps to RELAY76 switch 2
#define RELAY76_S3			2	// ATTINY pin 4 maps to RELAY76 switch 3
#define RELAY76_S4			4	// ATTINY pin 6 maps to RELAY76 switch 4
#define RELAY76_S5			7	// ATTINY pin 9 maps to RELAY76 switch 5
#define RELAY76_S6			5	// ATTINY pin 7 maps to RELAY76 switch 6
*/