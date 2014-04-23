
#include "pins_arduino.h"

#define LED_HB    9
#define PTIME 30
uint8_t hbval=128;
int8_t hbdelta=8;

void pulse(int pin, int times) {
  do {
    digitalWrite(pin, HIGH);
    delay(PTIME);
    digitalWrite(pin, LOW);
    delay(PTIME);
  } 
  while (times--);
}

void setup() {
  pinMode(LED_HB, OUTPUT);
  pulse(LED_HB, 2);
}


// this provides a heartbeat on pin 9, so you can tell the software is running.
void heartbeat() {
  if (hbval > 192) hbdelta = -hbdelta;
  if (hbval < 32) hbdelta = -hbdelta;
  hbval += hbdelta;
  analogWrite(LED_HB, hbval);
  delay(20);
}


void loop(void) {
   // light the heartbeat LED
  heartbeat();
}