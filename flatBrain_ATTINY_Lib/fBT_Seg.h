#ifndef FBRAIN_TSEG_H
#define FBRAIN_TSEG_H

#define SEG_DADDR		0x51 // I2C addr of Seg register for setting address and options, if the Seg DADDR pin is jumped low during power up, configuration commands can be sent. Remove jumper and repower to assume normal operations.
#define SEG_ADDR		0x37  // Current I2C address for rite operations. Set using SEG_DADDR, see above.
#define SEG_WRITE_CODE      0xAA  // command sent to SET_ADDR to display the 4 following bytes
#define SEG_WRITE_CMD       0x55	// command sent to SET_ADDR to set parameter


class fBT_Seg {


public:
	void displayDec(unsigned long val, unsigned short dplace = 0);  // val = integer, dplace = number of decimal places
	void displayFloat(double val,unsigned short dplace );


	void display(unsigned short *buf);
	void displayDashes();

	void setLumin( unsigned short OnDelay, unsigned short OffDelay);
	void setAddress(unsigned short val);
	void test();
};


#endif