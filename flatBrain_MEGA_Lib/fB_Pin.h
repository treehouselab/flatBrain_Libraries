#ifndef FP_PIN_H
#define FP_PIN_H

#include "fB_Header.h"

class fB_Pin {
	public:
		//fB_Card *pCard;
		//uint16_t pTag;
		//const __FlashStringHelper* Ptitle;
		uint8_t  flags; // roff-6bits, iodir - 1 bit, onVal-i bit
		void pull(unsigned int value);  // HIGH or LOW
		void pinMode(unsigned int iodir); //INPUT or OUTPUT
		void pulse(unsigned int msecs); 
		void YshiftPulse(unsigned int msecs); 
		//void pulse(unsigned int value,unsigned int msec);

		uint8_t		getCpin();  // get chip pin
		uint8_t		getOffset();
		uint8_t		getMode();
		uint8_t		getRow();
		uint8_t		getSide();

		void write(unsigned int value);
		void aWrite(unsigned int value);
 		void dWrite(unsigned int value);
		unsigned int read();
		unsigned int aRead();
		unsigned int dRead();
		bool isLatched();
		uint16_t avgAnalogIn();
		fB_Pin(uint16_t ctag,uint8_t   row,uint8_t   side, uint8_t  iodir, uint8_t  ival) ;
		~fB_Pin();
	private:
};
 


#endif

