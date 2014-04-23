// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

// RTC based on the DS1307 chip connected via I2C 

class fB_RTC {
public:
	uint8_t  error;
	uint8_t  data[7];
	void set(uint32_t t =0); // Simple general-purpose date/time class (no TZ / DST / leap second handling!)
    void set(uint16_t year, uint8_t  month, uint8_t  day,
                uint8_t  hour =0, uint8_t  min =0, uint8_t  sec =0);
    void set(const char* date, const char* time);	
    uint8_t  dayOfWeek() const;

    // 32-bit times as seconds since 1/1/2000
    long secondstime() const;   
    // 32-bit times as seconds since 1/1/1970
    uint32_t unixtime(void) const;
    
	void adjust();
    uint8_t  init();
    void now();
	char *stamp(char *buffer);
    uint8_t  yOff, m, d, hh, mm, ss;
    uint8_t  n_yOff, n_m, n_d, n_hh, n_mm;
	fB_RTC() { error=0; };


protected:
};
