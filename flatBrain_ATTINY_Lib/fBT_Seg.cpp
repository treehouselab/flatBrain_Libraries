
#include <Arduino.h>
#include <USI_TWI_Master.h>
#include <TinyWireM.h>
// includes are explicit so that this module can be compiled searately for ATTINY program
//#include <Arduino.h>
//#include <avr/pgmspace.h>
//#include <pins_arduino.h>
//#include <stdio.h>
//#include <stdarg.h>
#include "fBT_Seg.h"



#define SEG_DADDR		0x51 // I2C addr of Seg register for setting address and options, if the Seg DADDR pin is jumped low during power up, configuration commands can be sent. Remove jumper and repower to assume normal operations.
#define SEG_ADDR		0x37  // Current I2C address for rite operations. Set using SEG_DADDR, see above.
#define SEG_WRITE_CODE      0xAA  // command sent to SET_ADDR to display the 4 following bytes
#define SEG_WRITE_CMD       0x55	// command sent to SET_ADDR to set parameter

unsigned short segCodeTable[] = 
{
         0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F, 0x77, 0x7C,0x39,0x5E,0x79,0x71,0x00

};


void fBT_Seg::displayDec(unsigned long val,unsigned short dplace)
{
  unsigned short i;
  unsigned short segcode[5];
  segcode[0] = SEG_WRITE_CODE;
  segcode[1] = segCodeTable[val%10];
  segcode[2] = segCodeTable[(val%100)/10];
  segcode[3] = segCodeTable[(val/100)%10];
  segcode[4] =  segCodeTable[ val/1000];
  if(dplace) dplace++;
  TinyWireM.beginTransmission(SEG_ADDR);

  for (i=0; i<5; i++)
  {	
	if (i && i == dplace) segcode[i] |= 0x80; 
    TinyWireM.send(segcode[i]);    
  }
  TinyWireM.endTransmission();   
  

}

void fBT_Seg::display(unsigned short *buf)
{
  unsigned short i;
  TinyWireM.beginTransmission(SEG_ADDR); 
  TinyWireM.send(SEG_WRITE_CODE);
  for (i=0; i<4; i++)
  {
    TinyWireM.send(buf[i]);         
  }
  TinyWireM.endTransmission();              
}

void fBT_Seg::setLumin( unsigned short OnDelay, unsigned short OffDelay)
{
    TinyWireM.beginTransmission(SEG_ADDR); 
    TinyWireM.send(SEG_WRITE_CMD);
    TinyWireM.send(OnDelay);
    TinyWireM.send(~OnDelay); 
    TinyWireM.send(OffDelay);
    TinyWireM.send(~OffDelay);
    TinyWireM.endTransmission();              
}
void fBT_Seg::setAddress(unsigned short val)
{
      TinyWireM.beginTransmission(SEG_DADDR); 
      TinyWireM.send(0x61);
      TinyWireM.send(val);
      TinyWireM.send(~val);
      TinyWireM.endTransmission();              
}


void fBT_Seg::test() {
	unsigned short Nopdelay;
	unsigned short buff[4];

    unsigned short i;
    Nopdelay = 1000;
    for (i=0 ;i<8; i++)
    {
        buff[0]=buff[1]=buff[2]=buff[3] = (1<<i);
        display( buff);
        delay(Nopdelay);
    }
    Nopdelay = 100;
    for (i=3 ;i>0; i--)
    {
        buff[0]=buff[1]=buff[2]=buff[3] = 0;
        buff[i] = (1<<4);
        display( buff);
        delay(Nopdelay);
        buff[i] = (1<<5);
        display( buff);
        delay(Nopdelay);
        buff[i] = (1<<0);
        display( buff);
        delay(Nopdelay);
        buff[i] = (1<<1);
        display( buff);
        delay(Nopdelay);
        buff[i] = (1<<2);
        display( buff);
        delay(Nopdelay);
    }
     buff[0]=buff[1]=buff[2]=buff[3] = 0xFF;
     display( buff);
     delay(1000);
	 buff[0]=buff[1]=buff[2]=buff[3] = 0x00;
     display( buff);

}


