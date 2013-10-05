/* $Id: fB_Alarm.cpp 119 2010-07-17 18:56:36Z bhagman@roguerobotics.com $

  A fB_Alarm Generator Library

  Written by Brett Hagman
  http://www.roguerobotics.com/
  bhagman@roguerobotics.com

*************************************************/
#include "fB_Include.h"

uint8_t bootBeepToggle;   
uint16_t bootBeepNote;   
uint8_t bootBeepEnabled = 1;   
uint8_t alarmEnabled = 1;   

prog_char alarm_0[] PROGMEM = ":d=16,o=5,b=120:c,p,g";
prog_char alarm_1[] PROGMEM = ":d=16,o=5,b=120:c,f,c6";
prog_char alarm_2[] PROGMEM = ":d=16,o=5,b=120:c,p,a,p,a,p,a";
prog_char alarm_3[] PROGMEM = ":d=16,o=5,b=120:c,d6";
prog_char alarm_4[] PROGMEM = ":d=16,o=4,b=120:f,g,p,g";
prog_char alarm_5[] PROGMEM = ":d=16,o=5,b=120:d,c,f6";
prog_char alarm_6[] PROGMEM = ":d=16,o=5,b=120:g,p,d6,p,d6,p,d6,p,g,p,d6,p,d6,p,d6";
//prog_char alarm_8[] PROGMEM = "";

/*
		defineAlias(_TALRMBT,ALARM BOOT);
		defineAlias(_TALRMIN,ALARM INIT);
		defineAlias(_TALRMFL,ALARM FAIL);
		defineAlias(_TALRMWN,ALARM WARN);
		defineAlias(_TALRMAC,ALARM ACTION);
		defineAlias(_TALRMQS,ALARM Q);
		defineAlias(_TALRMEG,ALARM EMERG);

To be recognized by ringtone programs, an RTTTL/Nokring format ringtone must contain three specific elements: name, settings, and notes.

For example, here is the RTTTL ringtone for Haunted House:

HauntHouse: d=4,o=5,b=108: 2a4, 2e, 2d#, 2b4, 2a4, 2c, 2d, 2a#4, 2e., e, 1f4, 1a4, 1d#, 2e., d, 2c., b4, 1a4, 1p, 2a4, 2e, 2d#, 2b4, 2a4, 2c, 2d, 2a#4, 2e., e, 1f4, 1a4, 1d#, 2e., d, 2c., b4, 1a4

The three parts are separated by a colon.
Part 1: Deleted
Part 2: settings (here: d=4,o=5,b=108), where "d=" is the default duration of a note. 
In this case, the "4" means that each note with no duration specifier (see below) is by default considered a quarter note. 
"8" would mean an eighth note, and so on. Accordingly, "o=" is the default octave. There are four octaves in the Nokring/RTTTL format. 
And "b=" is the tempo, in "beats per minute".

Part 3: the notes. Each note is separated by a comma and includes, in sequence: a duration specifier, a standard music note, 
either a, b, c, d, e, f or g, and an octave specifier. If no duration or octave specifier are present, the default applies.

Standard musical durations are denoted by the following notations:
1 - whole note
2 - half note
4 - quarter note
8 - eighth note
16 - sixteenth note
32 - thirty-second note

Dotted rhythm patterns can be formed by appending a period (".") character to the end of a duration/beat/octave element.
P - rest or pause

The RTTTL format allows octaves starting from the A below middle C and going up four octaves. 
This corresponds with the inability of cellphones to reproduce certain tones audibly. 
These octaves are numbered from lowest pitch to highest pitch from 4 to 7.
The octave should be left out of the notation in the case of a rest or pause in the pattern.

An example of the RTTTL format would be
fifth:d=4,o=5,b=63:8P,8G5,8G5,8G5,2D#5

*/
 const char *alarmTable[] PROGMEM= {   
  alarm_0,
  alarm_1,
  alarm_2,
  alarm_3,
  alarm_4,
  alarm_5,
  alarm_6,
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
    pin = _ALARMPIN;
	alarmEnabled = 1;
    bootBeepEnabled = 1;
    bootBeepToggle = 0;
	bootBeepNote = NOTE_C6;
}

void fB_Alarm::init()
{
	//enabled = 1;
	//bootBeepEnabled = 1;

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
		//TCCR4B |= (1 << CS12);

}
void fB_Alarm::enable()
{
	if( alarmEnabled) {
		init();	
		TCCR4B = 0x00;        //Disbale Timer4 
	}
	alarmEnabled = 1;

}
void fB_Alarm::disable()
{
	alarmEnabled = 0;
	bootBeepEnabled = 0;
	TCCR3B |= (1 << CS10);//Disbale Timer3 
	TCCR4B = 0x00;        //Disbale Timer4 
}
void fB_Alarm::bootBeepDisable()
{
	bootBeepEnabled = 0;
	TCCR4B = 0x00;        //Disbale Timer4 

}
void fB_Alarm::bootBeepEnable()
{
	if(bootBeepEnabled)	TCCR4B |= (1 << CS12); //Enable Timer4 

}



// frequency (in hertz) and duration (in milliseconds).

void fB_Alarm::playTone(uint16_t frequency, uint32_t duration)
{
  uint8_t  prescalarbits = 0b001;
  int32_t toggle_count = 0;
  uint32_t ocr = 0;

     
	// Set the pinMode as _OUTPUT
	pinMode(pin, _OUTPUT);

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


void fB_Alarm::playTag(uint16_t  tag )
{
		if(!alarmEnabled) return;
		play(Tag(tag)->iVal);
}

void fB_Alarm::play(uint8_t  alarmIndex )
{
	if(!alarmEnabled) return;
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

  strcpy_P(alarmBuffer, (char*)pgm_read_word(&(alarmTable[alarmIndex])));
  //dbug(F("ALARM p:%s"),p);
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

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

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

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  //  begin note loop
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

    if(note)
    {
      playTone(pgm_read_word_near(notes + (scale - 4) * 12 + note));
      delay(duration);
      stop();
    }
    else
    {
      delay(duration);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////


void fB_WarnDelay::init() {
	currID = NULL;
	action = _WD_OFF;
	ptLED = Tag(_TALRMLED);
	if(ptLED) {
		LEDonVal = ptLED->getOnVal();
		ptLED->write(~LEDonVal);
	}
}


void fB_WarnDelay::reset() {
	//dbug(F("warn RESET"));
	stop();
	currID = 0;
	action = _WD_OFF;
}

void fB_WarnDelay::stop() {
	//dbug(F("warn STOP"));
	timer.stop(_TIMER_ALARM);
	timer.stop(_TIMER_WARN);
	timer.stop(_TIMER_WARNDELAY);
	action = _WD_OFF;
	if(ptLED) 	ptLED->write(~LEDonVal);
	//warn.setMsg(P_BLANK);

}

uint8_t fB_WarnDelay::warning(uint8_t id, uint8_t wdSecs,uint8_t wSecs, uint8_t aSecs, uint16_t tled) {
	dbug(F("warn ENTRY"));
	if(currID && id != currID) return _WD_SKIP;
	if(!currID) {
		currID = id;
		warnDelaySecs = wdSecs;
		warnSecs = wSecs;
		alarmIntervalSecs = aSecs;
		startWarning(NULL);	
		action = _WD_WARN;

	}
	dbug(F("warn ACTION %d"), action);
	if(action == _WD_ACT) warn.currID = NULL;
	return action;
}
void fB_WarnDelay::startWarnDelay() {  // alarm delay interrupt handler
	//dbug(F("STRTwarndelay INT ENTRY"));
	if(!currID) return;
	stop();
	//timer.after(_TIMER_WARNDELAY,(unsigned long)warnDelaySecs * 1000, startWarning,NULL);
	timer.after(_TIMER_WARNDELAY,(unsigned long)warnDelaySecs * 1000, endWarnDelay,NULL);
	action = _WD_DELAY;
	//menu.showMessage(warn.msgIndex,warn.msgText);
	if(ptLED) warn.ptLED->write(LEDonVal);
}

//////////////////// these methods are not in class because they need to be argument callable functions ////////////
void playWarning(uint16_t arg16) { 
	if(warn.ptLED) warn.ptLED->write(warn.LEDonVal);
	alarm.playTag(_TALRMWN);
	if(warn.ptLED) warn.ptLED->write(~warn.LEDonVal);
}

void endWarning(uint16_t arg16) {
	warn.stop();
	warn.action = _WD_ACT;
	alarm.playTag(_TALRMAC);
	//warn.setMsg(P_BLANK);

}
void startWarning(uint16_t arg16) {
	//dbug(F("startWARN"));
	_fBiK1 = 0;   // reset Warn Delay interrupt flag
	if(warn.ptLED) warn.ptLED->write(~warn.LEDonVal);
	timer.after(_TIMER_WARN,(unsigned long)warn.warnSecs * 1000, endWarning,NULL);
	timer.repeat(_TIMER_ALARM,(unsigned long)warn.alarmIntervalSecs * 1000, 25,playWarning,NULL);
	//menu.showMessage(warn.msgIndex,warn.msgText);
	playWarning(NULL);
	//dbug(F("end startWARN"));
}
void endWarnDelay(uint16_t arg16) {
	//dbug(F("startWARN"));
	warn.reset();
	if(warn.ptLED) warn.ptLED->write(~warn.LEDonVal);
}
