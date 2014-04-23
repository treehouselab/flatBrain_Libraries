// 
//    FILE: fB_EEPROM.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.00
// PURPOSE: Simple fB_EEPROM library for Arduino with EEPROM 24LC256 et al.
//
// HISTORY: 
// 0.1.00 - 2011-01-21 initial version
// 0.1.01 - 2011-02-07 added setBlock function
// 0.2.00 - 2011-02-11 fixed 64 bit boundary bug
// 
// Released to the public domain
//
#include <fB_I2C.h>
#include <fB_Define.h>
#include "fB_EEPROM.h"

extern fB_I2C	i2c;


void fB_EEPROM::writeByte(unsigned int address, uint8_t  data )
{
	_WriteBlock(address, &data, 1);
}

void fB_EEPROM::writeBlock(unsigned int address, uint8_t * buffer, int length)
{
	// determine length until end of page
	int le = endOfPage(address);
	if (le > 0)
	{
		_WriteBlock(address, buffer, le);
		address += le;
		buffer += le;
		length -= le;
	}

	// write the rest at BLOCKSIZE (16) uint8_t boundaries 
	while (length > 0)
	{
		_WriteBlock(address, buffer, min(length, BLOCKSIZE));
		address += BLOCKSIZE;
		buffer += BLOCKSIZE;
		length -= BLOCKSIZE;
	}
}

void fB_EEPROM::setBlock(unsigned int address, uint8_t  data, int length)
{
	uint8_t  buffer[BLOCKSIZE];
	for (uint8_t  i =0; i< BLOCKSIZE; i++) buffer[i] = data;

	// determine length until end of page
	int le = endOfPage(address);
	if (le > 0)
	{
		_WriteBlock(address, buffer, le);
		address += le;
		length -= le;
	}

	while (length > 0)
	{
		_WriteBlock(address, buffer, min(length, BLOCKSIZE));
		address += BLOCKSIZE;
		length -= BLOCKSIZE;
	}
}


uint8_t  fB_EEPROM::readByte(unsigned int address)
{
	uint8_t  rdata;
	_ReadBlock(address, &rdata, 1);
	return rdata;
}

// maybe let's not read more than 30 or 32 uint8_t s at a time!
void fB_EEPROM::readBlock(unsigned int address, uint8_t * buffer, int length)
{
	while (length > 0)
	{
		_ReadBlock(address, buffer, min(length, BLOCKSIZE));
		address += BLOCKSIZE;
		buffer += BLOCKSIZE;
		length -= BLOCKSIZE;
	}
}


void fB_EEPROM::dump(unsigned int addr, unsigned int length)
{
  // block to 8
  addr = addr / 8 * 8;
  length = (length + 7)/8 * 8;

  uint8_t b = readByte(addr); 
  for (int i = 0; i < length; i++) 
  {
    if (addr % 8 == 0)
    {
      Serial.println();
      Serial.print(addr);
      Serial.print(":\t");
    }
    Serial.print(b);
    b = readByte(++addr); 
    Serial.print("  ");
  }
  Serial.println();
}
////////////////////////////////////////////////////////////////////
//
// PRIVATE
//


// detemines length until first multiple of 16 of an address
// so writing allways occurs up to 16 uint8_t boundaries
// this is automatically 64 uint8_t boundaries
int fB_EEPROM::endOfPage(unsigned int address)
{
	const int m = BLOCKSIZE;
	unsigned int eopAddr = ((address + m - 1) / m) * m;  // "end of page" address
	return eopAddr - address;  // length until end of page
}

// pre: length < 32;
void fB_EEPROM::_WriteBlock(unsigned int address, uint8_t * buffer, uint8_t  length)
{
	i2c.write((uint8_t ) EE_ADDRESS, ( uint8_t )(address >> 8),(uint8_t )(address & 0xFF), length,buffer );
	delay(5);

}

// pre: buffer is large enough to hold length bytes
void fB_EEPROM::_ReadBlock(unsigned int address, uint8_t * buffer, uint8_t  length)
{
	i2c.read((uint8_t ) EE_ADDRESS, ( uint8_t )(address >> 8),(uint8_t )(address & 0xFF),length , buffer);

}
//
// END OF FILE
//
