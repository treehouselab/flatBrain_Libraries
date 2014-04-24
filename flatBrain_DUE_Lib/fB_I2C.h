#include <Arduino.h>
#include <inttypes.h>

#ifndef FB_I2C_H
#define FB_I2C_H


#define START           0x08
#define REPEATED_START  0x10
#define MT_SLA_ACK		0x18
#define MT_SLA_NACK		0x20
#define MT_DATA_ACK     0x28
#define MT_DATA_NACK    0x30
#define MR_SLA_ACK		0x40
#define MR_SLA_NACK		0x48
#define MR_DATA_ACK     0x50
#define MR_DATA_NACK    0x58
#define LOST_ARBTRTN    0x38
#define TWI_STATUS      (TWSR & 0xF8)
#define SLA_W(address)  (address << 1)
#define SLA_R(address)  ((address << 1) + 0x01)
#define cbi(sfr, bit)   (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)   (_SFR_BYTE(sfr) |= _BV(bit))

#define MAX_BUFFER_SIZE 32




class fB_I2C
{
  public:
	int status;
    void begin();
    void end();
   // void timeOut(uint16_t);
    //void setSpeed(uint8_t ); 
   // void pullup(uint8_t );
   // void scan();
    uint8_t  available();
	int  read(int address);
	int  read(uint8_t address);
	int  read(uint8_t address, uint8_t registerAddress);
	int  read(uint8_t  address, uint8_t  numberBytes, uint8_t  *dataBuffer);
	int  read(uint8_t  address,uint8_t  registerAddress, uint8_t  numberBytes, uint8_t  *dataBuffer);
	int  read(uint8_t  address,uint8_t  registerAddress1,uint8_t  registerAddress2, uint8_t  numberBytes, uint8_t  *dataBuffer);

	int  write(uint8_t  address, uint8_t  dataByte);
	int  write(int  address, int  dataByte);
	int  write(uint8_t address,uint8_t registerAddress, uint8_t data);
	int  write(int address,int registerAddress, int data);
	int  write(uint8_t address,uint8_t registerAddress, uint8_t numberBytes, uint8_t *buffer);
	int  write(uint8_t  address,uint8_t  registerAddress1,uint8_t  registerAddress2, uint8_t  numberBytes, uint8_t  *dataBuffer);




};


#endif
