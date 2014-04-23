#include "fB_PCF8574.h"
#include "fB_PCinterrupt.h"
#include "fB_Define.h"

extern fB_I2C  i2c;

PCF8574::PCF8574(){
	resetRegister();
}

PCF8574::~PCF8574(){
	/* DO NOTHING */
}

void PCF8574::begin(uint8_t  i2cAddr){
	PCFaddress=i2cAddr;
	//i2c.begin();
}

void PCF8574::pinMode(int pin, int mode){
	bitWrite(PCFTRISA,pin,mode);
}

void PCF8574::digitalWrite(int pin,int value){
	//if((pin >= 0) && (pin < 8)){
		bitWrite(PCFPORTA,pin,value);
		i2cSend();
	//}
}

int PCF8574::digitalRead(int pin){
	if((pin >= 0) && (pin < 8)){
		i2cRead();
		return bitRead(PCFPORTA,pin);
	}
}

void PCF8574::write(int value){
	PCFPORTA = value;
	i2cSend();
}

void PCF8574::pullUp(int pin){
	if((pin >= 0) && (pin < 8)){
		PCFPULL[pin] == 1;
		i2cRead();
		bitWrite(PCFPORTA,pin,1);
		i2cSend(PCFPORTA);
	}
}

void PCF8574::pullDown(int pin){
	if((pin >= 0) && (pin < 8)){
		PCFPULL[pin] == 2;
		i2cRead();
		bitWrite(PCFPORTA,pin,0);
		i2cSend(PCFPORTA);
	}
}

int PCF8574::read(){
	i2cRead();
	return PCFPORTA;
}

void PCF8574::clear(){
		PCFPORTA = 0x00;
		i2cSend();
}

void PCF8574::set(){
		PCFPORTA = 0xFF;
		i2cSend();
}

void PCF8574::toggle(int pin){
	if((pin >= 0) && (pin < 8)){
		i2cRead();
		bitWrite(PCFPORTA,pin,!bitRead(PCFPORTA,pin));
		i2cSend();
	}
}

void PCF8574::blink(int pin,int time,int wait){
	if((pin >= 0) && (pin < 8)){
		i2cRead();
		for(int i=0;i<time;i++){
			bitWrite(PCFPORTA,pin,0);
			i2cSend();
			delay(wait);
			bitWrite(PCFPORTA,pin,1);
			i2cSend();
			delay(wait);
		}
	}
}

void PCF8574::enableInterrupt(int pin,void(* selfCheckFunction)(void)){
	PCFIntPin = pin;
	pinMode(PCFIntPin,INPUT);
	PCattachInterrupt(PCFIntPin,selfCheckFunction,FALLING);
}

void PCF8574::disableInterrupt(){
	PCdetachInterrupt(PCFIntPin);
}

#ifdef INT_INPUT_ONLY
#define INT_CHECK (PCFTRISA[i] == INPUT)
#else
#define INT_CHECK true
#endif

void PCF8574::checkForInterrupt(){
	sei();
	PCFBUFFER = i2cRead(0x00);
	for(int i=0;i<8;i++){
		switch(PCFINT[i]){
			case CHANGE:
				if(INT_CHECK && (bitRead(PCFPORTA,i) != bitRead(PCFBUFFER,i)))
					PCFJMP[i]();
				break;
			case LOW:
				if(INT_CHECK && (bitRead(PCFBUFFER,i) == 0))
					PCFJMP[i]();
				break;
			case FALLING:
				if(INT_CHECK && (bitRead(PCFPORTA,i) == 1) && (bitRead(PCFBUFFER,i) == 0))
					PCFJMP[i]();
				break;
			case RISING:
				if(INT_CHECK && (bitRead(PCFPORTA,i) == 0) && (bitRead(PCFBUFFER,i) == 1))
					PCFJMP[i]();
				break;
			case 255:
				break;
		}
	}
}

void PCF8574::attachInterrupt(int pin,void (*userFunc)(void),int mode){
	if((pin >= 0) && (pin < 8)){
		PCFINT[pin] = mode;
		PCFJMP[pin] = userFunc;
	}
}

void PCF8574::detachInterrupt(int pin){
	if((pin >= 0) && (pin < 8)){
		PCFINT[pin] = 255;
	}
}

void PCF8574::resetRegister(){
	PCFPORTA =0;
	PCFTRISA =0;
	PCFBUFFER =0;
	for(int i=0;i<8;i++) {
		PCFPULL[i] = 0;
		PCFINT[i] = 255;
	}
}



void PCF8574::i2cSend(){
	PCFBUFFER = i2cRead(0x00);
	for(int i=0;i<8;i++){
		if(bitRead(PCFTRISA,i) == INPUT) bitWrite(PCFPORTA,i,bitRead(PCFBUFFER,i));
		if(PCFPULL[i] == 1) bitWrite(PCFPORTA,i,1); // Required for unblock pull level
		if(PCFPULL[i] == 2) bitWrite(PCFPORTA,i,0); // Required for unblock pull level
	}
	Wire1.beginTransmission(PCFaddress);
	Wire1.write((uint8_t )PCFPORTA);
	Wire1.endTransmission();

}


void PCF8574::i2cRead(){
  uint8_t  value = 0x00;
  Wire1.requestFrom((uint8_t )PCFaddress, (uint8_t )1));
  if (Wire1.available())  PCFPORTA = (int) Wire1.read();
  else PCFPORTA = (int)value; //error condition
}

int PCF8574::i2cRead(uint8_t  value){
  Wire1.requestFrom((uint8_t )PCFaddress, (uint8_t )1));
  if (Wire1.available())  PCFPORTA = (int) Wire1.read();
  else PCFPORTA = (int)value; //error condition
  //return value;
   return PCFPORTA;

}

