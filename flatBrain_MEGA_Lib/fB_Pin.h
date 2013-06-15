#ifndef FP_PIN_H
#define FP_PIN_H

class fB_Pin {
	public:
		fB_Card *bcard;
		//uint8_t  row;
		//uint8_t  side;
		uint16_t pTag;
		char* tagStr;
		uint8_t  mode;
		//uint8_t  type; //?
		uint8_t  row;
		uint8_t  side;
		uint8_t  cpin;
		uint8_t  iodir;
 		uint8_t  onVal;
 		uint8_t  vdr;
 		uint8_t  gate;
		void pull(unsigned int value);  // HIGH or LOW
		void pulse(unsigned int value,unsigned int msec);
		void yButtonON();
		void yButtonOFF();


		void write(unsigned int value);
		void aWrite(unsigned int value);
 		void dWrite(unsigned int value);
		unsigned int read();
		unsigned int aRead();
		unsigned int dRead();
		bool isLatched();
		uint16_t avgAnalogIn(uint8_t  pnum);
		fB_Pin(uint16_t ptag,char * tagStr, uint16_t ctag,uint8_t   row,uint8_t   side,uint8_t   pin,  uint8_t  mode, uint8_t  iodir, uint8_t  ival) ;
		~fB_Pin();
	private:
};
 


#endif

