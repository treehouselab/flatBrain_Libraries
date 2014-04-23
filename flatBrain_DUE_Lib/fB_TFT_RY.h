
#ifndef FP_TFT_H
#define FP_TFT_H

#define AR4_2 9994	// autorange max 4 significant figures (default max 2 decimal places) , for printFloat()
//#define AR5 9995	// autorange max 5 significant figures , for printFloat()

#define LEFT 2		// justify
#define RIGHT 251
#define CENTER 250

#define PORTRAIT 0
#define LANDSCAPE 1

#define PERIOD	1  // for printSpecChar()
#define DASH	2
#define CURSOR	3

#define MAXPIXELWID		239 
#define MAXPIXELHT		319 

// TFT connector pin tags for indices into tPin array  ( declared in   )
// indices are arbitrary, do not conform to TFT connector pinout, 
// but must start at 16 and should be consecutive, max index is 39
// (0-15 used as indices for tft connector pins DB0 - DB15)


//#define L_RS		16  // for LCD
//#define L_WR		18
#define L_CS		17
#define L_RST		19
#define T_CLK		20 // FOR TOUCHSCREEN
#define T_CS		21
#define T_DIN		22
#define T_OUT		23
#define T_IRQ		24
#define	TSW_RT		26// FOR TFT SWITCHES
#define	TSW_LF		27
#define	TSW_UP		28
#define	TSW_DN		29
#define	TSW_INT		30

#define TC	0  // indices into TFT PCF array, 0..4
#define TL	1
#define TH	2
#define TS	3
#define TM  4  // not array index, designates signal to M0..M7

// TFT Card PCF8574A  I2C base adddress: 0x38
#define	TCADDR		0x38	// base | 0x00   HARDWIRED  TFT RS,WR,CLK....CONTROL
#define TLADDR		0x39	// base | 0x01   HARDWIRED	TFT DB0-7
#define	THADDR		0x3A	// base | 0x02   HARDWIRED  TFT DB8-15
#define TSADDR		0x3B	// base | 0x03   HARDWIRED	TFT SWITCHES 1..8

/*
// AVAILABLE PCF8547A ADDRESSES:
#define ?		0x3C	// base | 0x04  
#define ?		0x3D	// base | 0x05  
#define ?		0x3E	// base | 0x06  
#define ?		0x3F	// base | 0x07  
*/


#define swap(type, i, j) {type t = i; i = j; j = t;}

#define fontbyte(x) pgm_read_byte(&cfont.font[x])  

//#define regtype volatile uint8_t 
//#define regsize uint8_t 
#define bitmapdatatype unsigned int*



#include "Arduino.h"

struct _current_font
{
	uint8_t * font;
	uint8_t  x_size;
	uint8_t  y_size;
	uint8_t  offset;
	uint8_t  numchars;
};

// Declare which fonts we will be using
extern uint8_t  SmallFont[];
extern uint8_t  BigFont[];
extern uint8_t  SegFont[];

typedef struct TPIN {
	//uint8_t  cdex;  // index of PCF chip 
	uint8_t  caddr;  // i2c address of PCF chip 
	uint8_t  cpin;   // physical pin on PCC chip (0..7)
	//uint8_t  iodir;  // I/O direction
};


class fB_TFT
{
	public:

		unsigned int TP_X ,TP_Y, currY;
		TPIN tPin[35];		
		_current_font	cfont;
		uint8_t fColor,gColor,hColor,bColor;


		fB_TFT();
		void init(uint8_t orient);
		void clear(uint16_t ht=0);
		void drawPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		void fillScr(uint8_t c);
		void drawRect(int x1, int y1, int x2, int y2);
		void drawRoundRect(int x1, int y1, int x2, int y2);
		void fillRect(int x1, int y1, int x2, int y2, uint8_t color);
		void fillRoundRect(int x1, int y1, int x2, int y2);
		void drawCircle(int x, int y, int radius);
		void fillCircle(int x, int y, int radius);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void printDeg( int x, int y,char *st, int deg=0);
		void printLine( char* text, uint8_t  wid=0);
		void printNewLine( char* text, uint8_t  wid=0);
		void printInt(int x, int y,int num, int wid, int just =0 );
		void printFloat(int x, int y, float num, int dec,int just=0);
		void print( int x, int y, char *st,int n=0);

		//void packFont(uint8_t * font,int maxsize);
		void setFont(uint8_t * font);
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale=1);
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
		void lcdOff();
		void lcdOn();
		void setContrast(char c);
		void initDefColors();
		void resetDefColors();
		void setColor(uint8_t target, uint8_t color);
		void setAll2Bcolor();
		void testBits();

		friend class tPage;
		friend class tBlock;

		void Touchpin(int tclk,int tcs,int tdin,int dout, int irq);
		void printChar( int x, int y,uint8_t c);
		void printSpecChar(int x, int y,uint8_t  ccode);
		//void printSegPeriod( int x, int y);
		//void TouchGetaddress();

		void bangWR(uint8_t  value);
		void bangRS(uint8_t  value);

		uint8_t  readTouchPin(uint8_t  tDex);
		uint8_t  readTpin(uint8_t  tDex);
		uint8_t  readButtons();
		uint8_t  readTpinDbounce(uint8_t  tDex);

		bool TouchIRQ();
		void initTouch();
		void TouchWriteData(uint8_t data);
		word TouchReadData();

		void TouchGetAddress();

		unsigned int TouchGetX();
		unsigned int TouchGetY();



	protected:
		unsigned int _lastX,_lastY,_currLine;
		uint8_t orient;
		long disp_x_size, disp_y_size;
		//uint8_t display_model, display_transfer_mode, display_serial_mode;
		//regtype *P_RS, *P_WR, *P_CS, *P_RST, *P_SDA, *P_SCL;
		//regsize B_RS, B_WR, B_CS, B_RST, B_SDA, B_SCL;

		//void set_direction_registers(uint8_t mode);
		void LCD_Writ_LBus(char VL);
		void LCD_Writ_HBus(char VH);
		void LCD_Writ_Bus(char VH,char VL);
		void LCD_Write_COM(char VL);
		void LCD_Write_DATA(char VH,char VL);
		void LCD_Write_HDATA(char VH);
		void LCD_Write_COM_DATA(char com1,int dat1);
		void setPixel(uint8_t color);
		void setXY(word x1, word y1, word x2, word y2);
		void clrXY();
		void rotateChar(uint8_t c, int x, int y, int pos, int deg);
		//PCF8574 * tPCF[4];  // array 0f pointers to PCF8574 instances corresponding to 4 IC's on TFT board
		volatile int tftCtrlReg;
		volatile int tftTouchReg;
		void WriteDataTo7843(unsigned char num);
		unsigned int ReadDataFrom7843();
		///void initTpin(uint8_t  tDex, uint8_t   cDex, uint8_t  cAddr, uint8_t  cPin, uint8_t  iodir);
		void initTpin(uint8_t  tDex,  uint8_t  cAddr, uint8_t  cPin);
		void bangTFTbit(uint8_t  tDex, int value);
		void bangTouchBit(uint8_t  tDex, int value);
		void WRbangLoHi();

};


#endif
