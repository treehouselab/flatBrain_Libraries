
#ifndef FBT_YCARD_H
#define FBT_YCARD_H

#define	 REG	0x01  // button modes
#define  MOM	0x02
#define  RAD	0x04
#define  DEP	0x08


class fBT_yButton {
	public:
		uint8_t Bd;
		uint8_t Yd;
		uint8_t mode;		// SOLO, GANG, MOM, 
};

class fBT_yCard {
	public:
		uint8_t		mapCount;
		uint8_t		enableDelay;
		uint8_t	bState;  // button state 8-bit encoded
		uint8_t	sState;	 // shift state 
		uint8_t	aState;  // active state 8-bit encoded
		fBT_yButton yMap[6];
		fBT_yButton shiftBut;

		fBT_yCard();
		void insertYmap(uint8_t bDg, uint8_t sDg);
		void setButtonMode(uint8_t bnum,  uint8_t mod);
		void buildCardMap();
		void initCard();
		void pollButtons();
		void getAstate();
		void applyAstate();
};

#endif

