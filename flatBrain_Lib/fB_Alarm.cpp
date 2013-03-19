/* $Id: fB_Alarm.cpp 119 2010-07-17 18:56:36Z bhagman@roguerobotics.com $

  A fB_Alarm Generator Library

  Written by Brett Hagman
  http://www.roguerobotics.com/
  bhagman@roguerobotics.com

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*************************************************/
#include "fB_Include.h"

uint8_t bootBeepToggle;   
uint16_t bootBeepNote;   
uint8_t bootBeepEnabled;   
uint8_t alarmEnabled;   

prog_char alarm_0[] PROGMEM = ":d=16,o=5,b=140:g,f,c6";
prog_char alarm_1[] PROGMEM = ":d=16,o=5,b=120:c,p,g";
prog_char alarm_2[] PROGMEM = ":d=16,o=5,b=120:d,p,a,p,a";
prog_char alarm_3[] PROGMEM = ":d=16,o=5,b=120:g,p,c6,p,c6,p,c6";
prog_char alarm_4[] PROGMEM = ":d=16,o=5,b=140:2e6,4p,2e6,4p,2e6";

 const char *alarmTable[] PROGMEM= {   
  alarm_0,
  alarm_1,
  alarm_2,
  alarm_3,
  alarm_4,
 };


// timerx_toggle_count:
//  > 0 - duration specified
//  = 0 - stopped
//  < 0 - infinitely (until stop() method called, or new playTone() called)


prog_uint16_t notes[] PROGMEM= { 0,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

volatile int32_t timer3_togCount;
volatile uint8_t  *timer3pinPort;
volatile uint8_t  timer3pinMask;

ISR(TIMER4_OVF_vect) {
   extern fB_Brain brain; 
   if(bootBeepEnabled && alarmEnabled) {   
		bootBeepToggle = !bootBeepToggle;
		digitalWrite(13,bootBeepToggle); 
		tone(11,bootBeepNote,60);
		TCNT4 = 0;             //Reset Timer to 130 out of 255
		TIFR4 = 0x00;          //Timer4 INT Flag Reg: Clear Timer Overflow Flag
   }
} 

ISR(TIMER3_COMPA_vect)
{
  if (timer3_togCount != 0) {
    // toggle the pin
    *timer3pinPort ^= timer3pinMask;
    if (timer3_togCount > 0)  timer3_togCount--;
  }
  else  {
    TIMSK3 &= ~(1 << OCIE3A);                 // disable the interrupt
    *timer3pinPort &= ~(timer3pinMask);   // keep pin low after stop
  }
}


fB_Alarm::fB_Alarm() {
    pin = ALARM_PIN;
	alarmEnabled = brain.alarmEnabled;
    bootBeepEnabled = 0;
    bootBeepToggle = 0;
	bootBeepNote = NOTE_C6;
}

void fB_Alarm::enable()
{
	//enabled = 1;
	bootBeepEnabled = 1;

    // Set timer specific stuff
    // All timers in CTC mode
    // 8 bit timers will require changing prescalar values,
    // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar
        // 16 bit timer
        TCCR3A = 0;
        TCCR3B = 0;
        //bitWrite(TCCR3B, WGM12, 1);
		TCCR3B |= (1 << WGM12);
		TCCR3B |= (1 << CS10);
        //bitWrite(TCCR3B, CS10, 1);
        timer3pinPort = portOutputRegister(digitalPinToPort(pin));
        timer3pinMask = digitalPinToBitMask(pin);
		//Setup Timer4 to fire every 1ms
		TCCR4B = 0x00;        //Disbale Timer4 while we set it up
		TCNT4  = 0;         //Reset Timer Count to x out of 255
		TIFR4  = 0x00;        //Timer4 INT Flag Reg: Clear Timer Overflow Flag
		TIMSK4 = 0x01;        //Timer4 INT Reg: Timer4 Overflow Interrupt Enable
		TCCR4A = 0x00;        //Timer4 Control Reg A: Wave Gen Mode normal
		TCCR4B |= (1 << CS12);
		//TCCR4B |= (1 << CS10);

}
void fB_Alarm::disable()
{
	//enabled = 0;
	bootBeepEnabled = 0;
}
void fB_Alarm::disableBootBeep()
{
	bootBeepEnabled = 0;
	TCCR4B = 0x00;        //Disbale Timer4 

}

// frequency (in hertz) and duration (in milliseconds).

void fB_Alarm::playTone(uint16_t frequency, uint32_t duration)
{
  uint8_t  prescalarbits = 0b001;
  int32_t toggle_count = 0;
  uint32_t ocr = 0;

     
	// Set the pinMode as OUTPUT
	pinMode(pin, OUTPUT);

	// two choices for the 16 bit timers: ck/1 or ck/64
	ocr = F_CPU / frequency / 2 - 1;

	prescalarbits = 0b001;
	if (ocr > 0xffff) {
		ocr = F_CPU / frequency / 2 / 64 - 1;
		prescalarbits = 0b011;
	}

    TCCR3B = (TCCR3B & 0b11111000) | prescalarbits;

    // Calculate the toggle count
    if (duration > 0)   toggle_count = 2 * frequency * duration / 1000;
    else   toggle_count = -1;

    // Set the OCR for the given timer,
    // set the toggle count,
    // then turn on the interrupts
        OCR3A = ocr;
        timer3_togCount = toggle_count;
        bitWrite(TIMSK3, OCIE3A, 1);
}


void fB_Alarm::stop()
{
      TIMSK3 &= ~(1 << OCIE3A);
      digitalWrite(pin, 0);
}


bool fB_Alarm::isPlaying(void)
{
  return (TIMSK3 & (1 << OCIE3A));
}


void fB_Alarm::play(uint8_t  alarmTag)
{
  // Absolutely no error checking in here

  uint8_t default_dur = 4;
  uint8_t default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  uint8_t note;
  uint8_t scale;
  char *p;

  strcpy_P(alarmBuffer, (char*)pgm_read_word(&(alarmTable[alarmTag])));
  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)
  //Serial.print("buffer: "); 
  //Serial.println(alarmBuffer);

  p = &alarmBuffer[0];

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  //Serial.print("ddur: "); Serial.println(default_dur, 10);

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  //Serial.print("doct: "); Serial.println(default_oct, 10);

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  //Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  //Serial.print("wn: "); Serial.println(wholenote, 10);


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      //Serial.print("Playing: ");
      //Serial.print(scale, 10); Serial.print(' ');
      //Serial.print(note, 10); Serial.print(" (");
      //Serial.print(notes[(scale - 4) * 12 + note], 10);
      //Serial.print(") ");
     // Serial.println(duration, 10);
      playTone(pgm_read_word_near(notes + (scale - 4) * 12 + note));
      //playTone(notes[(scale - 4) * 12 + note]);
      delay(duration);
      stop();
    }
    else
    {
      //Serial.print("Pausing: ");
      //Serial.println(duration, 10);
      delay(duration);
    }
  }
}

