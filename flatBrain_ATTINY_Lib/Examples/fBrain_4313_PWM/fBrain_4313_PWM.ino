void setup() {
	pinMode(7,OUTPUT);
	pinMode(11,OUTPUT);
	pinMode(12,OUTPUT);
	pinMode(13,OUTPUT);
}

void loop() {
	analogWrite(7,60);
	analogWrite(11,120);
	analogWrite(12,180);
	analogWrite(13,254);
	delay(50);
	analogWrite(7,0);
	analogWrite(11,0);
	analogWrite(12,0);
	analogWrite(13,0);
	delay(50);
}


/*

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
 
#define    F_CPU    1000000UL
#include <util/delay.h>
 
void init_timer(){
// Set Fast PWM mode. 
TCCR0A |= (1<<WGM00) | (1<<WGM01);
// Clear 0C0A on compare. 
TCCR0A |= (1<<COM0A1);
// Start timer, no prescaling. 
TCCR0B |= (1<<CS00);
// Duty cycle 0% 
OCR0A = 0;
}
 
void init_led(){
DDRB |= (1<<PB2);
}
 
int main(int argc, char *argv[]){
int i;
 
init_led();
init_timer();
 
while(1) {
  for(i=0; i < 255; i++) {
      OCR0A = i;
      _delay_ms(10);
  }
 
  for(i=255; i > 0; i--) {
      OCR0A = i;
      _delay_ms(10);
  }
}
 
return 0;
}

*/