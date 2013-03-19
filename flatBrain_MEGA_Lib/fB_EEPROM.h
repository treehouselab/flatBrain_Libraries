#ifndef FB_EEPROM_H
#define FB_EEPROM_H
// 
//    FILE: I2C_eeprom.h
//  AUTHOR: Rob Tillaart
// PURPOSE: Simple I2C_eeprom library for Arduino with EEPROM 24LC256 et al.
// VERSION: 0.2.00
//     URL: http://arduino.cc/playground/Main/LibraryForI2CEEPROM
// HISTORY: See I2C_eeprom.cpp
//
// Released to the public domain
//

// BLOCKSIZE must be 16
#define BLOCKSIZE 16

class fB_EEPROM  {
	public:
		// (mem_address, value)
		void writeByte(unsigned int, uint8_t  );
		// (mem_address, buffer, length)
		void writeBlock(unsigned int, uint8_t *, int ); 
		// (mem_address, value, count)
		void setBlock(unsigned int, uint8_t , int ); 
		// (mem_address)
		uint8_t  readByte(unsigned int );
		// (mem_address, buffer, length)
		void readBlock(unsigned int, uint8_t *, int );
		void dump(unsigned int addr, unsigned int length);

	private:
		int endOfPage(unsigned int);  
		// (mem_address, buffer, length)
		void _WriteBlock(unsigned int, uint8_t *, uint8_t  );
		void _ReadBlock(unsigned int, uint8_t *, uint8_t  );
};

#endif
