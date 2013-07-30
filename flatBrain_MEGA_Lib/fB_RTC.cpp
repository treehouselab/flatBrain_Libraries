// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#include "fB_Include.h"


#define SECONDS_PER_DAY 86400L

#define SECONDS_FROM_1970_TO_2000 946684800

#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#else
 #include <WProgram.h>
#endif
extern fB_I2C    i2c;
extern uint8_t 	_i2cspeed;


static uint8_t  conv2d(const char* p) {
    uint8_t  v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}
static uint8_t  bcd2bin (uint8_t  val) { return val - 6 * (val >> 4); }
static uint8_t  bin2bcd (uint8_t  val) { return val + 6 * (val / 10); }
const uint8_t  daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 }; //has to be const or compiler compaints

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t  m, uint8_t  d) {
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t  i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

static long time2long(uint16_t days, uint8_t  h, uint8_t  m, uint8_t  s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

////////////////////////////////////////////////////////////////////////////////
// RTC implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

uint8_t  fB_RTC::init() {

	uint8_t  res;
	res = i2c.write((uint8_t ) RTC_ADDRESS, (uint8_t )0);
	if(res) { error = 1 ;dbug(F("RTC WRITE ERROR")); }

	else {
		res = i2c.read((uint8_t )RTC_ADDRESS, (uint8_t )1);
		if(res){ error = 1 ;dbug(F("RTC READ ERROR")); }
		else {
			  //i2c.setSpeed(I2CSLOW);
			  uint8_t  ss = i2c.receive();
			  //i2c.setSpeed(_i2cspeed);
			  if(ss>>7) {
				  error = 1;
				  dbug(F("RTC ERROR: %h"),ss);
			  }
		}
	}
	return error;
}
void fB_RTC::set(uint32_t t) {
  t -= SECONDS_FROM_1970_TO_2000;    // bring to 2000 timestamp from 1970

    ss = t % 60;
    t /= 60;
    mm = t % 60;
    t /= 60;
    hh = t % 24;
    uint16_t days = t / 24;
    uint8_t  leap;
    for (yOff = 0; ; ++yOff) {
        leap = yOff % 4 == 0;
        if (days < 365 + leap)
            break;
        days -= 365 + leap;
    }
    for (m = 1; ; ++m) {
        uint8_t  daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
        if (leap && m == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    d = days + 1;
	adjust();
}

void fB_RTC::set (uint16_t year, uint8_t  month, uint8_t  day, uint8_t  hour, uint8_t  min, uint8_t  sec) {
    if (n_yOff >= 2000)
        n_yOff -= 2000;
    yOff = n_yOff;
    m = n_m;
    d = n_d;
    hh = hour;
    mm = min;
    ss = sec;
	adjust();
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
void fB_RTC::set (const char* date, const char* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
        case 'F': m = 2; break;
        case 'A': m = date[2] == 'r' ? 4 : 8; break;
        case 'M': m = date[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
        default:  return;  // no date avail
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);
	adjust();
}


uint8_t  fB_RTC::dayOfWeek() const {    
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t fB_RTC::unixtime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2long(days, hh, mm, ss);
  t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000
  return t;
}
/*
Serial.print("yOff: ");
Serial.println(data[6],HEX);
Serial.print("m: ");
Serial.println(data[5],HEX);
Serial.print("d: ");
Serial.println(data[4],HEX);
Serial.print("hh: ");
Serial.println(data[2],HEX);
Serial.print("mm: ");
Serial.println(data[1],HEX);


}
*/
void fB_RTC::adjust() {
	if(error) return;
	uint8_t  res;
	data[0] = bin2bcd(ss);
	data[1] = bin2bcd(mm);
	data[2] = bin2bcd(hh);
	data[3] = bin2bcd(0);
	data[4] = bin2bcd(d);
	data[5] = bin2bcd(m);
	data[6] = bin2bcd(yOff);

   //i2c.setSpeed(I2CSLOW);
   res = i2c.write((uint8_t )RTC_ADDRESS,(uint8_t ) 0);
   res = i2c.write((uint8_t ) RTC_ADDRESS, (uint8_t )0, (uint8_t *) data, (uint8_t ) 7);
//Serial.print("rtc adjust i2cres2: ");
//Serial.println(res,HEX);
  // i2c.setSpeed(_i2cspeed);

}

void fB_RTC::now() {  
  if(error) return;
  i2c.write((uint8_t )RTC_ADDRESS,(uint8_t ) 0);
  i2c.read((uint8_t )RTC_ADDRESS, (uint8_t )7);
  ss = bcd2bin(i2c.receive() );
  mm = bcd2bin(i2c.receive());
  hh = bcd2bin(i2c.receive());// mask assumes 24hr clock
  i2c.receive();
  d = bcd2bin(i2c.receive());
  m = bcd2bin(i2c.receive());
  yOff = bcd2bin(i2c.receive());
    ///i2c.setSpeed(_i2cspeed);

}


char *fB_RTC::stamp(char *buffer){
    if(error) return "NO RTC,";
   // digital clock display of current time
	now();
	sprintf(buffer,"%d-%02d-%02d, %02d:%02d:%02d",yOff+2000,m,d,hh,mm,ss);
	return buffer;
}
