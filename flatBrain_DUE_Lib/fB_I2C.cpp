
#include <Arduino.h>
#include <inttypes.h>
#include "fB_I2C.h"
#include <Wire.h>
extern TwoWire Wire1;




////////////// Public Methods ////////////////////////////////////////



void fB_I2C::begin()   { Wire1.begin(); }

void fB_I2C::end() {  Wire1.endTransmission(); }

uint8_t  fB_I2C::available()
{
  return((uint8_t) Wire1.available());
}

int  fB_I2C::read(uint8_t address)
{
  int value = -1;
  Wire1.requestFrom(address, (uint8_t) 1);
  if(Wire1.available()) value =  Wire1.read(); 
  return value;

}
int  fB_I2C::read(int address)
{
  return read((uint8_t) address);
}

int  fB_I2C::read(uint8_t  address,uint8_t  registerAddress)
{
  write(address,registerAddress);
  return read((uint8_t) address);
}

int  fB_I2C::read(uint8_t  address, uint8_t  numberBytes, uint8_t  *dataBuffer)
{
  uint8_t nBytes,i;
  Wire1.requestFrom(address, numberBytes);
  nBytes = Wire1.available();
  for (i=0;i<nBytes;i++)  dataBuffer[i] = Wire1.read(); 
  return nBytes;
}
int  fB_I2C::read(uint8_t  address,uint8_t  registerAddress, uint8_t  numberBytes, uint8_t  *dataBuffer)
{
  write(address,registerAddress);
  uint8_t nBytes,i;
  Wire1.requestFrom(address, numberBytes);
  nBytes = Wire1.available();
  for (i=0;i<nBytes;i++)  dataBuffer[i] = Wire1.read(); 
  return nBytes;
}
int  fB_I2C::read(uint8_t  address,uint8_t  registerAddress1,uint8_t  registerAddress2, uint8_t  numberBytes, uint8_t  *dataBuffer)
{
  write(address,registerAddress1);
  write(address,registerAddress2);
  uint8_t nBytes,i;
  Wire1.requestFrom(address, numberBytes);
  nBytes = Wire1.available();
  for (i=0;i<nBytes;i++)  dataBuffer[i] = Wire1.read(); 
  return nBytes;
}

int  fB_I2C::write(uint8_t  address, uint8_t  dataByte)
{
  status = -1;
  Wire1.beginTransmission(address);
  status = Wire1.write(dataByte);
  Wire1.endTransmission();
  return status;
}
int  fB_I2C::write(int address, int data)
{
  status = -1;
  Wire1.beginTransmission((uint8_t)address);
  status = Wire1.write((uint8_t)data);
  Wire1.endTransmission(); 
  return status;
}
int  fB_I2C::write(uint8_t address,uint8_t registerAddress, uint8_t data)
{
  Wire1.beginTransmission(address);
  status = Wire1.write(registerAddress);
  if(status != -1) status = Wire1.write(data);
  Wire1.endTransmission();
  return status;
}
int  fB_I2C::write(int address,int registerAddress, int data)
{
  Wire1.beginTransmission((uint8_t)address);
  status = Wire1.write((uint8_t)registerAddress);
  if(status != -1) status = Wire1.write((uint8_t)data);
  Wire1.endTransmission();
  return status;
}
int  fB_I2C::write(uint8_t address,uint8_t registerAddress, uint8_t numberBytes, uint8_t *buffer)
{
  Wire1.beginTransmission(address);
  status = Wire1.write(registerAddress);
  for(int i=0; i < numberBytes && status != -1 ; i++) 	status = Wire1.write(buffer[i]);
  Wire1.endTransmission();
  return status;
}
int  fB_I2C::write(uint8_t address,uint8_t registerAddress1,uint8_t registerAddress2, uint8_t numberBytes, uint8_t *buffer)
{
  Wire1.beginTransmission(address);
  status = Wire1.write(registerAddress1);
  status = Wire1.write(registerAddress2);
  for(int i=0; i < numberBytes && status ; i++) 	status = Wire1.write(buffer[i]);
  Wire1.endTransmission();
  return status;
}



/////////////// Private Methods ////////////////////////////////////////


/*
uint8_t  fB_I2C::sendAddress(uint8_t  i2cAddress)
{
  TWDR = i2cAddress;
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((TWI_STATUS == MT_SLA_ACK) || (TWI_STATUS == MR_SLA_ACK))
  {
    return(0);
  }
  uint8_t  bufferedStatus = TWI_STATUS;
  if ((TWI_STATUS == MT_SLA_NACK) || (TWI_STATUS == MR_SLA_NACK))
  {
    stop();
    return(bufferedStatus);
  }
  else
  {
    lockUp();
    return(bufferedStatus);
  } 
}

//I2C I2c = I2C();


//void fB_I2C::timeOut(uint16_t _timeOut)
//{
//  timeOutDelay = _timeOut;
//}

void fB_I2C::setSpeed(uint8_t  _fast)
{
  if(!_fast)
  {
    TWBR = ((F_CPU / 100000) - 16) / 2;
  }
  else
  {
    TWBR = ((F_CPU / 400000) - 16) / 2;
  }
}
  
void fB_I2C::pullup(uint8_t  activate)
{
  if(activate)
  {
    #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
      // activate internal pull-ups for twi
      // as per note from atmega8 manual pg167
      sbi(PORTC, 4);
      sbi(PORTC, 5);
    #else
      // activate internal pull-ups for twi
      // as per note from atmega128 manual pg204
      sbi(PORTD, 0);
      sbi(PORTD, 1);
    #endif
  }
  else
  {
    #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
      // deactivate internal pull-ups for twi
      // as per note from atmega8 manual pg167
      cbi(PORTC, 4);
      cbi(PORTC, 5);
    #else
      // deactivate internal pull-ups for twi
      // as per note from atmega128 manual pg204
      cbi(PORTD, 0);
      cbi(PORTD, 1);
    #endif
  }
}

void fB_I2C::scan()
{
  uint16_t tempTime = timeOutDelay;
  timeOut(80);
  uint8_t  totalDevicesFound = 0;
  Serial.println("Scanning for devices...");
  Serial.println();
  for(uint8_t  s = 0; s <= 0x7F; s++)
  {
    returnStatus = 0;
    returnStatus = start();
    if(!returnStatus)
    { 
      returnStatus = sendAddress(SLA_W(s));
    }
    if(returnStatus)
    {
      if(returnStatus == 1)
      {
        Serial.println("Bus Problem, could not complete scan");
        timeOutDelay = tempTime;
        return;
      }
    }
    else
    {
      Serial.print("Found device @ ");
      Serial.print(" 0x");
      Serial.println(s,HEX);
      totalDevicesFound++;
    }
    stop();
  }
  if(!totalDevicesFound){Serial.println("No devices found");}
  timeOutDelay = tempTime;
}

*/
