#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include "fBT_Seg.h"

fBT_Seg seg;

#define OFF	0	// RELAY STATE
#define ON	1
#define ALARM	2
#define OVER	3

#define RELAY	3  //   pin 6 relay
#define DISPLAY	1  //   pin 3 seg display switch
#define BUZZER	2  //	pin 5 pwm buzzer
#define BUTTON	A5  //  pin 8 muxed switches
#define INTRPT	0  //	pin 2 interrupt
#define VZSENS	A3  //  pin 10 V0 sense
#define CALVZ	A2  //  pin 11 calib pot
#define CALON	A1  //	pin 12 calib pot
#define CALOFF	A0 //	pin 13 calib pot

#define MAXSAMPLES	10

long vRefScale = 1125300L; // default;


float  vButton;
float vFact  = .00408;
float bFact  = .00489;
float cFact = .001;
float zFact  = 4.424 ;
float zBias  = -1.393;
float vON  = 13; //  default
float vOFF = 11; //  default
float v0sense =0; 
float vCalib = 0;

unsigned long tAlarm = 0;  // used internally for alarm
unsigned long tOver  = 0;  // used internally for override
unsigned long tDisplay = 0;  // used internally for read delay
unsigned long tInit = 0;  // used internally for init display

unsigned long pAlarm   = 10000;  // period in millisecs
unsigned long pOver    = 20000;  
unsigned long pDisplay = 1000;  
unsigned long pInit    = 10000;  


uint8_t state = OFF;
uint8_t displayTog = 0;
uint8_t maxSamples = 200;

void setup() { 
  //  more accurate chip-specific scaling factor:
  //  scale = internal1.1Ref * 1023 * 1000
  //  where internal1.1Ref = 1.1 * Vcc1 (per voltmeter) / Vcc2 (per readVcc() function)
  vRefScale *= 4.22/4.06;
	

	TinyWireM.begin();
	seg.setAddress(SEG_ADDR);  // set segmented display address if necessary;

    pinMode(CALVZ,INPUT);
    pinMode(CALON,INPUT);
    pinMode(CALOFF,INPUT);
    pinMode(VZSENS,INPUT);
    pinMode(BUTTON,INPUT);
    pinMode(BUZZER,OUTPUT);
    pinMode(RELAY,OUTPUT);
    pinMode(DISPLAY,OUTPUT);

	digitalWrite(RELAY,LOW);
	startTimer(tInit,pInit);


}

void loop() {
    // Get Voltages
    vButton =	avgAnalog(BUTTON) * vFact;
    vON =	avgAnalog(CALON) * bFact + 10;
    vOFF =	avgAnalog(CALOFF) * bFact + 10;
    vCalib =	avgAnalog(CALVZ) * cFact - 0.5;
    v0sense =   avgAnalog(VZSENS)/1023.0 * readVcc() * zFact + zBias +vCalib;

    // Check for active button
	if(vButton > 3.2 ) {  // Boot/Override button
		startTimer(tOver,pOver);
		state = OVER;
	}
        if(vButton < 3 && vButton > 1.5) { //  ON voltage display button
    		 seg.displayFloat(vON,2);
    		 tDisplay = 0;
    	}
	if(vButton <= 1.5 && vButton > 1) { //  OFF voltage display button
		seg.displayFloat(vOFF,2);
		tDisplay = 0;
        }
	if(vButton <=  1 && timeout(tDisplay)) { //  no button press, display V0 if after display timeout
                if( state == ALARM) {		 // if Alarm state, toggle display and buzzer
                  if(displayTog) {
                    seg.displayDashes();
                    analogWrite(BUZZER,50);
                    displayTog = 0;
                  }
                  else { 
                    seg.displayFloat(v0sense,2);
                    anologWrite(BUZZER,0);
                    displayTog = 1;
                  }
                }
                else seg.displayFloat(v0sense,2);
                
		startTimer(tDisplay,pDisplay);
        }
	//else seg.displayFloat(readVcc(),2);


         // Check voltage, set state
	if(v0sense >= vON) state = ON;
        if(v0sense <= vOFF) {
		if(state == ON || ( state == OVER && timeout(tOver) )) {  // start Alarm
			state = ALARM;
			startTimer(tAlarm,pAlarm);   
		}
    }
	if(v0sense > vOFF && v0sense < vON) {
              if((state == ALARM && v0sense-vOFF > 0.15) || state == OVER) state = ON;
	}
         
        if(state != ALARM || timeout(tAlarm)) digitalWrite(BUZZER,LOW);
        if(state == ALARM && timeout(tAlarm))  state = OFF;
	if(state == ON || state == OVER)     {
		digitalWrite(RELAY,HIGH);
		digitalWrite(DISPLAY,HIGH);
	}
	if(state == OFF)   {
		digitalWrite(RELAY,LOW);	
		if(timeout(tInit)) digitalWrite(DISPLAY,LOW);
	}
}

void startTimer(unsigned long &t_millis,unsigned long period)
{
   t_millis = millis() + period;  // initial setup
}

uint8_t timeout(unsigned long t_millis)
{
   if( (long)( millis() - t_millis ) >= 0) return 1;
   else return 0;
}


uint16_t avgAnalog(uint8_t pin) {
	uint16_t sum = 0;
	for(uint8_t i=0;i<MAXSAMPLES;i++) sum += analogRead(pin);
	return ( (uint16_t) sum/MAXSAMPLES );
}


float readVcc() {


  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;
  return (float) (vRefScale/result)/1000; // Vcc in Volts
}
