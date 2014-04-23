/*************************************************** 
  This is a library for the MCP23017 i2c port expander

  These displays use i2c.to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <fB_I2C.h>
#include <fB_Define.h>
#include <avr/pgmspace.h>
#include "fB_MCP23017.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

extern fB_I2C  i2c;
////////////////////////////////////////////////////////////////////////////////

void MCP23017::begin(uint8_t  addr) {
  
  /*
  if (addr > 7) {
    addr = 7;
  }
	This is where MCP23017.cpp differs from Adafuit_MCP23017.cpp.
	In the latter library, addr is passed in as the lower 4 bits of the i2c.address, 
	and ORed with a defined base address to pass to the i2c.functions. In this library,
	I am passing the full 8-bit i2c.address as addr. 
	"i2c.requestFrom(i2cAddr,..." must be replaced with
	"i2c.requestFrom((int)i2cAddr,..." to avoid compiler ambiguity errors.
  */

  i2cAddr = addr;

  //i2c.begin();
  
  // set defaults!

  i2c.write((uint8_t )i2cAddr, (uint8_t )MCP23017_IODIRA, (uint8_t )0xFF);  // all inputs on port A
  i2c.write((uint8_t )i2cAddr, (uint8_t )MCP23017_IODIRB, (uint8_t )0xFF);  // all inputs on port B
}

void MCP23017::begin(void) {
  begin(0);
}

void MCP23017::pinMode(uint8_t  p, uint8_t  d) {
  uint8_t  iodir;
  uint8_t  iodiraddr;

  // only 16 bits!
  if (p > 15)    return;

  if (p < 8)   iodiraddr = MCP23017_IODIRA;
  else {
    iodiraddr = MCP23017_IODIRB;
    p -= 8;
  }
  // read the current IODIR
  iodir = i2c.read((uint8_t )i2cAddr,(uint8_t )iodiraddr);	

  // set the pin and direction
  if (d == _INPUT) {
    iodir |= 1 << p; 
  } else {
    iodir &= ~(1 << p);
  }

  // write the new IODIR
  i2c.write((uint8_t )i2cAddr,(uint8_t )iodiraddr,(uint8_t)iodir);	
}

uint16_t MCP23017::readGPIOAB() {
  uint8_t data[2];
  uint16_t value;
  // read the current GPIO output latches
  i2c.read((uint8_t )i2cAddr,(uint8_t )MCP23017_GPIOA,2, data);
  value = data[1];
  value <<=8;
  value |= data[0];
  return value;
}

void MCP23017::writeGPIOAB(uint16_t ba) {
  i2c.write((uint8_t )i2cAddr,(uint8_t )MCP23017_GPIOA , (uint8_t )(ba & 0xFF) );
  i2c.write((uint8_t )i2cAddr,(uint8_t )MCP23017_GPIOB , (uint8_t )(ba >> 8));

}

void MCP23017::allOff() {
	for(int i=0;i<16;i++) this->digitalWrite(i,LOW);
}

void MCP23017::digitalWrite(uint8_t  p, uint8_t  d) {
  uint8_t  gpio;
  uint8_t  gpioaddr, olataddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8) {
    olataddr = MCP23017_OLATA;
    gpioaddr = MCP23017_GPIOA;
  } else {
    olataddr = MCP23017_OLATB;
    gpioaddr = MCP23017_GPIOB;
    p -= 8;
  }

  // read the current GPIO output latches
  gpio = (uint8_t) i2c.read((uint8_t )i2cAddr,(uint8_t )olataddr);

  // set the pin and direction
  if (d == HIGH) {
    gpio |= 1 << p; 
  } else {
    gpio &= ~(1 << p);
  }

  // write the new GPIO
	
   //Serial.print("i2c write: ");
  //Serial.println(i2c.write((uint8_t )i2cAddr,(uint8_t )gpioaddr,(uint8_t )gpio),DEC);
 //dbug("MCPdigwrite addr 0x%h, gpioaddr 0x%h,  gpio 0x%h \n",i2cAddr,gpioaddr,gpio);
  
   i2c.write((uint8_t )i2cAddr,(uint8_t )gpioaddr,(uint8_t )gpio);

}

void MCP23017::pull(uint8_t  p, uint8_t  d) {
  uint8_t  gppu;
  uint8_t  gppuaddr;

  // only 16 bits!
  if (p > 15)
    return;

  if (p < 8)
    gppuaddr = MCP23017_GPPUA;
  else {
    gppuaddr = MCP23017_GPPUB;
    p -= 8;
  }


  // read the current pullup resistor set
  gppu = (uint8_t) i2c.read((uint8_t )i2cAddr,(uint8_t )gppuaddr);

  // set the pin and direction
  if (d == HIGH) {
    gppu |= 1 << p; 
  } else {
    gppu &= ~(1 << p);
  }

  // write the new GPIO
	i2c.write((uint8_t )i2cAddr,(uint8_t )gppuaddr, (uint8_t )gppu);	
}

uint8_t  MCP23017::digitalRead(uint8_t  p) {
  uint8_t  gpioaddr;
  uint8_t data;

  // only 16 bits!
  if (p > 15)
    return 0;

  if (p < 8)
    gpioaddr = MCP23017_GPIOA;
  else {
    gpioaddr = MCP23017_GPIOB;
    p -= 8;
  }

  // read the current GPIO
  data = (uint8_t) i2c.read((uint8_t )i2cAddr, (uint8_t )gpioaddr);
  return (data >> p) & 0x1;
}
