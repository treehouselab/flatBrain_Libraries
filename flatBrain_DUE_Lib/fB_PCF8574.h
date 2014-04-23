#ifndef PCF8574_H
#define PCF8574_H

#include <fB_I2C.h>

typedef void (*FunctionPointer)(void);

class PCF8574 {
public:
        PCF8574();
        ~PCF8574();
		void begin(uint8_t  SADR);
		void pinMode(int pin, int mode);
		void digitalWrite(int pin,int value);
		int digitalRead(int pin);
		void write(int value);
		int read();
		void clear();
		void set();
		void pullUp(int pin);
		void pullDown(int pin);
		void toggle(int pin);
		void blink(int pin,int time,int wait);
		void enableInterrupt(int pin,void(* selfCheckFunction)(void)); // Require pointer to an user create gateway function because checkForInterrupt() is in PCF8574 namespace.
		void disableInterrupt();
		void attachInterrupt(int pin,void (*userFunc)(void),int mode);
		void detachInterrupt(int pin);
		void checkForInterrupt();

		volatile int PCFPORTA;
		volatile int PCFPULL[8];
		volatile int PCFTRISA;
		volatile int PCFBUFFER;
		uint8_t   PCFaddress;
		int PCFIntPin;
		volatile int PCFINT[8];
		volatile FunctionPointer PCFJMP[8];
		void resetRegister();
		void i2cSend();
		void i2cSend(int mask);
		void i2cRead();
		int i2cRead(uint8_t  value);
private:
};
 

#endif
