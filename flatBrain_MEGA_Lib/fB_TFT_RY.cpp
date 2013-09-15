#include "fB_Include.h"

fB_TFT::fB_TFT() { }


void fB_TFT::init(uint8_t orientation=PORTRAIT)
{ 
	/*
   //initTpin(uint8_t  tDex, uint8_t   cAddr, uint8_t  cPin, uint8_t  iodir);
	initTpin(0, TL,  TLADDR,  0, _OUTPUT); 
	initTpin(1, TL,  TLADDR, 1, _OUTPUT);
	initTpin(2, TL,  TLADDR, 2, _OUTPUT);
	initTpin(3, TL,  TLADDR, 3, _OUTPUT);
	initTpin(4, TL,  TLADDR, 4, _OUTPUT);
	initTpin(5, TL,  TLADDR, 5, _OUTPUT);
	initTpin(6, TL,  TLADDR, 6, _OUTPUT);
	initTpin(7, TL,  TLADDR, 7, _OUTPUT);

	initTpin(8,  TH, THADDR, 0, _OUTPUT);
	initTpin(9,  TH, THADDR,1, _OUTPUT);
	initTpin(10, TH, THADDR,2, _OUTPUT);
	initTpin(11, TH, THADDR,3, _OUTPUT);
	initTpin(12, TH, THADDR,4, _OUTPUT);
	initTpin(13, TH, THADDR,5, _OUTPUT);
	initTpin(14, TH, THADDR,6, _OUTPUT);
	initTpin(15, TH, THADDR,7, _OUTPUT);

    // new
	initTpin(L_CS,  TC, TCADDR,0, _OUTPUT);
	initTpin(L_RST, TC, TCADDR,1, _OUTPUT);
	initTpin(T_CLK, TC, TCADDR,4, _OUTPUT);
	initTpin(T_CS,  TC, TCADDR,5, _OUTPUT);
	initTpin(T_DIN, TC, TCADDR,6, _OUTPUT);
	initTpin(T_OUT, TC, TCADDR,7, INPUT);
	
	initTpin(TSW_RT,  TS,  TSADDR,0, INPUT);
	initTpin(TSW_LF,  TS,  TSADDR,1, INPUT);
	initTpin(TSW_UP,  TS,  TSADDR,2, INPUT);
	initTpin(TSW_DN,  TS,  TSADDR,3, INPUT);

	*/
	   //initTpin( index, uint8_t   cAddr, uint8_t  cPin);
	initTpin(0, TLADDR,  0);  
	initTpin(1,  TLADDR, 1); 	
	initTpin(2,  TLADDR, 2);
	initTpin(3,  TLADDR, 3);
	initTpin(4,  TLADDR, 4);
	initTpin(5,  TLADDR, 5);
	initTpin(6,  TLADDR, 6);
	initTpin(7,  TLADDR, 7);

	initTpin(8,  THADDR, 0);
	initTpin(9,  THADDR,1);
	initTpin(10, THADDR,2);
	initTpin(11, THADDR,3);
	initTpin(12, THADDR,4);
	initTpin(13, THADDR,5);
	initTpin(14, THADDR,6);
	initTpin(15, THADDR,7);

    // new
	initTpin(L_CS,  TCADDR,0);
	initTpin(L_RST, TCADDR,1);
	initTpin(T_CLK, TCADDR,4);
	initTpin(T_CS,  TCADDR,5);
	initTpin(T_DIN, TCADDR,6);
	initTpin(T_OUT, TCADDR,7);
	
	initTpin(TSW_RT,   TSADDR,0);
	initTpin(TSW_LF,   TSADDR,1);
	initTpin(TSW_UP,   TSADDR,2);
	initTpin(TSW_DN,   TSADDR,3);

	i2c.write((uint8_t )TSADDR, (uint8_t )0xFF);// pullup all buttons
			

	//initTpin(TSW_DN,  TS,  TSADDR,3, INPUT);
	//initTpin(T_IRQ,		TM,NULL ,  M16, INPUT);

	WRDDR  |=  _BV(WRPIN) ; // set WR as output
	RSDDR  |=  _BV(RSPIN) ; // set RS as output //NEW

	tftCtrlReg = 0;

	orient=orientation;
	bangTFTbit(L_RST, HIGH);
	delay(5); 
	bangTFTbit(L_RST, LOW);
	delay(15);
	bangTFTbit(L_RST, HIGH);
	delay(15);
	bangTFTbit(L_CS, LOW);

	//itdb02s SSD1289


	LCD_Write_COM_DATA(0x00,0x0001);
	LCD_Write_COM_DATA(0x03,0xA8A4);
	LCD_Write_COM_DATA(0x0C,0x0000);
	LCD_Write_COM_DATA(0x0D,0x080C);
	LCD_Write_COM_DATA(0x0E,0x2B00);
	LCD_Write_COM_DATA(0x1E,0x00B7);
	LCD_Write_COM_DATA(0x01,0x2B3F);
	LCD_Write_COM_DATA(0x02,0x0600);
	LCD_Write_COM_DATA(0x10,0x0000);
	LCD_Write_COM_DATA(0x11,0x6070);
	LCD_Write_COM_DATA(0x05,0x0000);
	LCD_Write_COM_DATA(0x06,0x0000);
	LCD_Write_COM_DATA(0x16,0xEF1C);
	LCD_Write_COM_DATA(0x17,0x0003);
	LCD_Write_COM_DATA(0x07,0x0233);
	LCD_Write_COM_DATA(0x0B,0x0000);
	LCD_Write_COM_DATA(0x0F,0x0000);
	LCD_Write_COM_DATA(0x41,0x0000);
	LCD_Write_COM_DATA(0x42,0x0000);
	LCD_Write_COM_DATA(0x48,0x0000);
	LCD_Write_COM_DATA(0x49,0x013F);
	LCD_Write_COM_DATA(0x4A,0x0000);
	LCD_Write_COM_DATA(0x4B,0x0000);
	LCD_Write_COM_DATA(0x44,0xEF00);
	LCD_Write_COM_DATA(0x45,0x0000);
	LCD_Write_COM_DATA(0x46,0x013F);
	LCD_Write_COM_DATA(0x30,0x0707);
	LCD_Write_COM_DATA(0x31,0x0204);
	LCD_Write_COM_DATA(0x32,0x0204);
	LCD_Write_COM_DATA(0x33,0x0502);
	LCD_Write_COM_DATA(0x34,0x0507);
	LCD_Write_COM_DATA(0x35,0x0204);
	LCD_Write_COM_DATA(0x36,0x0204);
	LCD_Write_COM_DATA(0x37,0x0502);
	LCD_Write_COM_DATA(0x3A,0x0302);
	LCD_Write_COM_DATA(0x3B,0x0302);
	LCD_Write_COM_DATA(0x23,0x0000);
	LCD_Write_COM_DATA(0x24,0x0000);
	LCD_Write_COM_DATA(0x25,0x8000);
	LCD_Write_COM_DATA(0x4f,0x0000);
	LCD_Write_COM_DATA(0x4e,0x0000);
	LCD_Write_COM(0x22);   

	bangTFTbit(L_CS, HIGH);

	resetDefColors();
	setFont(BigFont);
	currY = 1;


}
void fB_TFT::bangTFTbit(uint8_t  tDex, int value) {

  unsigned nop;

	bitWrite(tftCtrlReg,(int)tPin[tDex].cpin,value);
	i2c.write((uint8_t )tPin[tDex].caddr, (uint8_t )tftCtrlReg); //////////////////////////////////////////////

	//nop++;
}
void fB_TFT::bangWR(uint8_t  value) {

	if(value==HIGH) WRPORT |= 1<<WRPIN;
	else WRPORT &= ~(1<<WRPIN);

}
void fB_TFT::WRbangLoHi() {

	WRPORT &= ~(1<<WRPIN);
	delayMicroseconds(10);
	WRPORT |= 1<<WRPIN;

}
void fB_TFT::bangRS(uint8_t  value) {

	//new
	if(value==HIGH) RSPORT |= 1<<RSPIN;
	else RSPORT &= ~(1<<RSPIN);

/* 
	//old
 	bangTFTbit(L_RS,value); 
*/
}

// initTpin() creates element in tPin array
void fB_TFT::initTpin(uint8_t  tDex, uint8_t  cAddr ,uint8_t  cPin)
 {

	//tPin[tDex].cdex =	cDex;
	tPin[tDex].caddr =	cAddr;
	tPin[tDex].cpin =	cPin;
	//tPin[tDex].iodir =	iodir;

	//if(cDex != TM) tPCF[cDex]->pinMode(cPin,iodir);

}
/*
uint8_t  fB_TFT::readTpin(uint8_t  tDex) {
	int iRegister = 0;
	bitWrite(iRegister,(int)tPin[tDex].cpin,1);
	i2c.read((uint8_t ) tPin[tDex].caddr,(uint8_t )iRegister,(uint8_t )1);
	return  (uint8_t )i2c.receive();

}
*/
uint8_t  fB_TFT::readButtons() {
    static unsigned long last_interrupt_time = 0;
  	uint8_t  buffer = 0x00;
	uint8_t button = 0;
	static uint8_t  buttonTemp = 0;
	static uint8_t  buttonLast = 0;

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
	 unsigned long interrupt_time = millis();
	 if (interrupt_time - last_interrupt_time > 300) {
			if(!i2c.read((uint8_t )TSADDR,(uint8_t )1)) buffer = ~((uint8_t )i2c.receive() & 0xFF);
			switch (buffer) {
				case 0x00: buttonLast = buttonTemp;	   // Simple scheme, top button = 1, next button = 2, ...3...4
						   buttonTemp = 0;			   // if 2 buttons pressed (dual), concat values (eg; if 1&2, return value = 12, 1&3 = 13, 3&4 = 34, etc)
						   return(buttonLast);
				case 0x03: buttonTemp =  12; break; 
				case 0x05: buttonTemp =  13; break; 
				case 0x06: buttonTemp =  23; break; 
				case 0x09: buttonTemp =  14; break; 
				case 0x0A: buttonTemp =  24; break; 
				case 0x0C: buttonTemp =  34; break; 
				default:
					if(buttonTemp >4) break; // dual buttons trump singles
					switch(buffer) {
						case 0x01: buttonTemp =  1; break;
						case 0x02: buttonTemp =  2; break;
						case 0x04: buttonTemp =  3; break;
						case 0x08: buttonTemp =  4; break;
					}
					break;
			}
			return 0;
	}
}
	
/*
	if(!i2c.read((uint8_t )TSADDR,(uint8_t )1)) buffer = ~((uint8_t )i2c.receive() & 0xFF);
	switch (buffer) {
			case 0x08: button =  4; break;// Simple scheme, top button = 1, next button = 2, ...3...4
			case 0x04: button =  3; break;
			case 0x02: button =  2; break;
			case 0x01: button =  1; break;
			default: button = 0;
	}
  }
  last_interrupt_time = interrupt_time;
  return button;
*/	


/*
uint8_t  fB_TFT::readTpinDbounce(uint8_t  tDex) {
	 int iRegister = 0;
	 uint8_t   state, last = HIGH; // HIGH = OFF
	 uint16_t period = 100; // total time allowed for dbounce test
	 uint16_t qualify = 20; //  time to qualify as stable
	 uint8_t   dint = 10;  // delay interval
	 uint16_t plap = 0;   // total msec elapsed
	 uint16_t qlap = 0;   // stable msec elapsed

	 bitWrite(iRegister,(int)tPin[tDex].cpin,1);
	// DEBOUNCE
	while(plap < period) {
		i2c.read((uint8_t ) tPin[tDex].caddr,(uint8_t )iRegister,(uint8_t )1);
		state = (uint8_t )i2c.receive();
		delay(dint);
		plap += dint;
		if(state != last) { 
			qlap = 0;
			last = state;
		}
		else qlap +=dint;
		if(qlap >= qualify) return state;
	}
	return HIGH;

}
*/
/*
void fB_TFT::writeTpin(uint8_t  tDex, uint8_t   value)
{
	//tPCF[tPin[tDex].cdex]->digitalWrite(tPin[tDex].cpin,value);
}
*/

void fB_TFT::resetDefColors()
{
	fColor = (byte)FCOLOR;
	gColor = (byte)GCOLOR;
	hColor = (byte)HCOLOR;
	bColor = (byte)BCOLOR;
}
void fB_TFT::setColor(uint8_t target, uint8_t color)
{
	switch(target) {
		case FCOLOR:
			fColor = (byte)color;
			break;
		case GCOLOR:
			gColor = (byte)color;
			break;
		case HCOLOR:
			hColor = (byte)color;
			break;
		case BCOLOR:
			bColor = (byte)color;
			break;
	}
}
void fB_TFT::setAll2Bcolor()
{
	fColor = bColor;
	gColor = bColor;
	hColor = bColor;
}


void fB_TFT::LCD_Writ_Bus(char VH,char VL)
{   
	i2c.write((uint8_t )TLADDR, (uint8_t ) VL); ///////////////////////
	i2c.write((uint8_t )THADDR, (uint8_t ) VH);  /////////////////////////////
	WRbangLoHi();

}
void fB_TFT::LCD_Writ_LBus(char VL)
{   
	i2c.write((uint8_t )TLADDR, (uint8_t ) VL);
	WRbangLoHi();

}
void fB_TFT::LCD_Writ_HBus(char VH)
{   
	i2c.write((uint8_t )THADDR, (uint8_t ) VH);
	WRbangLoHi();
}

void fB_TFT::LCD_Write_COM(char VL)  
{   
		bangRS( LOW);
		LCD_Writ_Bus(0x00,VL);
}

void fB_TFT::LCD_Write_DATA(char VH,char VL)
{
		bangRS( HIGH);
		LCD_Writ_Bus(VH,VL);
}

void fB_TFT::LCD_Write_HDATA(char VH)
{
		bangRS( HIGH);
		LCD_Writ_HBus(VH);
}

void fB_TFT::LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8,dat1);
}

void fB_TFT::clear(int ht)
{
	long i,m;
	int y;
	
	bangTFTbit(L_CS, LOW);
	bangRS( HIGH);
	
	if(!ht) y = (long) MAXPIXELHT+1;
	else y = (long)ht;
	m = ((long)MAXPIXELWID+1)*y;

	for (i=0; i<m; i++)	{
		i2c.write((uint8_t )TLADDR,(uint8_t )0);
		//i2c.write((uint8_t )THADDR,(uint8_t )0);
	WRbangLoHi();
	}
	
	currY = 1;
	clrXY();
	bangTFTbit(L_CS, HIGH);

}
void fB_TFT::fillScr(uint8_t color)
{
	int i;

	bangTFTbit(L_CS, LOW);

	bangRS( HIGH);
	for (i=0; i<((MAXPIXELWID+1)*(MAXPIXELHT+1)); i++)	LCD_Writ_LBus(color);
	clrXY();
	bangTFTbit(L_CS, HIGH);

}
void fB_TFT::setFont(uint8_t * font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}
/*
void fB_TFT::packFont(uint8_t * font, int maxbytes) {
	int i=0;
	int g=0;
	int f=0;

	uint8_t fbuf[1200] = { NULL };
	uint8_t gbuf[32] = { NULL };
	//uint8_t fbuf[2000] = { NULL };
	//uint8_t gbuf[33] = { NULL };
	uint8_t cb=0,ca=0,cc=0;

	setFont(font);

	for(i=3; i< 1600 ;i++ ) {
		if(i>4) cb = ca;
		ca = pgm_read_byte(&cfont.font[i]); 
		if(ca==cb && g<32 && (ca == 0x00 || ca == 0xFF)) {
			if(!g && f)	gbuf[g++] = fbuf[--f];
			gbuf[g++] = ca;
			continue;
		}
		else {
			if(!g ) { fbuf[f++] = ca; continue; }
			cc=0;
			if(cb==0x00) cc |= 0xA0;
			else		 cc |= 0x40;
			cc |= g -2;
			fbuf[f++] = cc;
			g=0;
			fbuf[f++] = ca; continue;
		}
	}
	if(g ) {
		cc=0;
		if(cb==0x00) cc |= 0xA0;
		else		 cc |= 0x40;
		cc |= g -2;
		fbuf[f++] = cc;
	}

	Serial.print(" f: ");
	Serial.println(f,DEC);
	Serial.println("*");
	for(int j=0; j< f ;j++ ){
		Serial.print("0x");
		Serial.print(fbuf[j],HEX);
		Serial.println(",");
	}
	Serial.println("*");

}
*/
void fB_TFT::printDeg( int x, int y,char *st, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (x==RIGHT)   x=(MAXPIXELWID+1)-(stl*cfont.x_size);
	if (x==CENTER)	x=((MAXPIXELWID+1)-(stl*cfont.x_size))/2;

	for (i=0; i<stl; i++)  rotateChar(*st++, x, y, i, deg);
}
	
void fB_TFT::printInt(int x, int y,int num, int wid, int just )
{
  char buf[25];
  char st[27];
  boolean neg=false;
  int i,c=0;

  if(just) x= just;

  if (num==0)  {
	  if(wid){ 
		  for(i=0;i<wid-1;i++) st[i] = 32;
		  st[i]=48;
	      st[i+1]=0;
	  }
	  else {	  
		st[0]=48;
		st[1]=0;
	  }
  }
  else  {
	  if (num<0)	  {
		neg=true;
		num=-num;
	  }
	  while (num>0)	  {
		buf[c]=48+(num % 10);
		c++;
		num=(num-(num % 10))/10;
	  }
	  buf[c]=0;

	  if(wid) {
		  for(i = 0;i<wid;i++) st[i]=32; 
		  st[i]=0;
		  for(i = wid-c;i<wid;i++) st[i] = buf[wid -i -1];
	  }
	  else {
		  for (i=0; i<c; i++)	st[i+neg]=buf[c-i-1];
		  st[c+neg]=0;
	  }		  
	  if (neg)	st[0]=45;

  }
  print(x,y,st);
}

void fB_TFT::printFloat(int x, int y, float num, int dec,int just)
{
  char buf[25];
  char st[27];
  boolean neg=false;
  int c=0, s=0,mult,i;
  unsigned long inum;

  num += .0000005; // for pesky roundoff error
  
  if(just) x= just;
  if (num==0)  {
	  st[0]=32;
	  st[1]=48;
	  st[2]='.';
	  if(dec == AR4_2) dec = 2;
	  for (int i=0; i<dec; i++)  {
		  st[3+i]=48;
		  st[4+i]=0;
	  }
  }
  else  {
	  if (num<0)  {
		st[s++]=45;
		num=-num;
	  }
	  else st[s++] = 32;

	  if( dec == AR4_2){
		  if((int) num > 99) dec = 1;
		  else dec = 2;
	  }
	  else {
		  if (dec<1)dec=1;
		  if (dec>5)dec=5;
	  }
	  mult = 1; 
      for (int j = 0; j < dec; j++) mult *= 10;
      inum=long(num*mult + 0.5);  
	  while (inum>0)  {
		buf[c++]=48+(inum % 10);
		inum=(inum-(inum % 10))/10;
	  }
	  for(;c<dec;c++) buf[c]=48;

	  if(num <1) {
		  st[s++] = 48;
		  st[s++] = '.';
		  for(i=dec-1;i>=0;i--)   st[s++] = buf[i];
	  }
	  else {
		  for(i=c-1;i>=0;i--) {
			  if(i==dec-1) st[s++] = '.';
			  st[s++] = buf[i];
		  }
	  }	
	  st[s++]=0;
  }
  print(x,y,st);
}

/*
void fB_TFT::printChar( int x, int y,uint8_t c)
{
	uint8_t i,ch;
	word j;
	word temp; 

	bangTFTbit(L_CS, LOW);

	if(cfont.font == SegFont) {
		if(c>57) c = '-';
		switch(c) {
			case '.': printSpecChar( x, y,PERIOD); return;
			case '-': printSpecChar( x, y,DASH);return;
			case ' ': return;
			case '+': return;
		}
	}
	if(cfont.font == BigFont) if(c>96) c-=32; // force all caps
    
	setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
  
	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
	{
		ch=pgm_read_byte(&cfont.font[temp]);
		for(i=0;i<8;i++)
		{   
			if((ch&(1<<(7-i)))!=0)  LCD_Write_HDATA(fColor);
			else LCD_Write_HDATA(bColor);
		}
		temp++;
	}
	clrXY();	
	bangTFTbit(L_CS, HIGH);

}
*/
void fB_TFT::print( int x, int y, char *st,int n){
	uint8_t  stl,xsize,i,j,k,ch;
	uint8_t  dx=0,blen = 0;
	char c,last = 0;
	word temp; 

	stl= strlen(st);
	if(!stl) return;
	if(n) {
		if(n< stl) stl = n;
		else if(n> stl && x!= CENTER && x!= RIGHT) blen = n - stl;
	}
	//dbug(F("TFT %d,%d s:%s"),x,y,st);

	if (x==RIGHT)   x=(MAXPIXELWID+1)-(stl*cfont.x_size);
	if (x==CENTER)	x=((MAXPIXELWID+1)-(stl*cfont.x_size))/2;

	for (int i=0; i<stl+blen; i++) {
		if(i > 0) {
			if(cfont.font==SegFont) {
				switch(last) {
					case '.': dx += cfont.x_size/4; break;
					case '-': dx += 3*cfont.x_size/4; break;
					case '+': dx += 3*cfont.x_size/4; break;
					case ' ': dx += 3*cfont.x_size/4; break;
					default : dx += cfont.x_size;
					}
			}
			else dx += cfont.x_size;
		}
		last = *st;
		if(i<stl) c = *st++;
		else c = ' ';
		
		if(cfont.font == SegFont) {
			if(c>57) c = '-';
			xsize = 4;
			switch(c) {
				case '.': printSpecChar( x + dx, y,PERIOD); return;
				case '-': printSpecChar( x + dx, y,DASH);return;
				case ' ': 
				case '+': return;
			}
		}
		else if(cfont.font == BigFont) {
			if(c>93) c-=32; // force all caps
			if(c<32 || c>93) c=35; // illegal char displays  # char
			xsize = 2;
		}

		bangTFTbit(L_CS, LOW);
		setXY(x+dx,y,x+dx+cfont.x_size-1,y+cfont.y_size-1);
 
		temp=((c-cfont.offset)*(xsize*cfont.y_size))+4;
		for(j=0;j<(xsize *cfont.y_size);j++)
		{
			ch=pgm_read_byte(&cfont.font[temp]);
			for(k=0;k<8;k++)
			{   
				if((ch&(1<<(7-k)))!=0)  LCD_Write_HDATA(fColor);
				else LCD_Write_HDATA(bColor);
			}
			temp++;
		}
		clrXY();	
		bangTFTbit(L_CS, HIGH);
		
	}
}

void fB_TFT::printLine( char* text,uint8_t  wid) {
	print(LEFT,currY,text,(int)MAXPIXELWID/cfont.y_size);
}
void fB_TFT::printNewLine( char* text,uint8_t  wid) {
	int y  = currY + LINEHT;
	print(LEFT,y,text,(int)MAXPIXELWID/cfont.y_size);
	currY = y;
}

void fB_TFT::rotateChar(uint8_t c, int x, int y, int pos, int deg)
{
	uint8_t i,j,ch;
	word temp; 
	int newx,newy;
	float radian;
	radian=deg*0.0175;  

	bangTFTbit(L_CS, LOW);
	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++)	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)	{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++){   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0) 	LCD_Write_HDATA(fColor);
				else 	LCD_Write_HDATA(bColor);
			}
		}
		temp+=(cfont.x_size/8);
	}
	clrXY();
	bangTFTbit(L_CS, HIGH);
}


void fB_TFT::printSpecChar(int x, int y,uint8_t  ccode)
{
	uint8_t i,k,offX,offY,len;
	word j;
	byte* pcc;
	uint8_t period[5] = {0x38,0x7E,0xFF,0x7E,0x38};
	uint8_t dash[10] = {0x3F,0xFC,0x7F,0xFE,0xFF,0xFF,0x7F,0xFE,0x3F,0xFC};
	//uint8_t cursor[32] = {0x00,0x00,0x1C,0x00,0x1E,0x00,0x1F,0x00,0x1F,0x80,0x1F,0xC0,0x1F,0xE0,0x1F,0xF0 ,0x1F ,0xF0,0x1F,0xE0,0x1F,0xC0,0x1F,0x80,0x1F,0x00,0x1E,0x00,0x1C,0x00,0x00,0x00};
	switch (ccode) {
		case PERIOD:
				pcc = period;
				len = sizeof(period);
				y+= 27;
				offX =  7;
				offY =  4;
			break;
		case DASH:
				len = sizeof(dash);
				pcc = dash;
				y += 0;
				offX =  15;
				offY =  0;
			break;
			/*
		case CURSOR:
				len = sizeof(cursor);
				pcc = cursor;
				offX =  15;
				offY =  15;
			break;
			*/
		default: return;
	}
	bangTFTbit(L_CS, LOW);
  
	setXY(x,y,x+offX,y+offY);
  
	for(j=0;j<len;j++)
	{
		for(i=0;i<8;i++)
		{   
			if((pcc[j]&(1<<(7-i)))!=0)  LCD_Write_HDATA(fColor);
			else LCD_Write_HDATA(bColor);
		}
	}
	clrXY();	
	bangTFTbit(L_CS, HIGH);
}


void fB_TFT::setXY(word x1, word y1, word x2, word y2)
{
	int tmp;

	LCD_Write_COM_DATA(0x44,(x2<<8)+x1);
	LCD_Write_COM_DATA(0x45,y1);
	LCD_Write_COM_DATA(0x46,y2);
	LCD_Write_COM_DATA(0x4e,x1);
	LCD_Write_COM_DATA(0x4f,y1);
	LCD_Write_COM(0x22); 
}

void fB_TFT::clrXY()
{
	bangTFTbit(L_CS, LOW);
	setXY(0,0,(word)MAXPIXELWID,(word)MAXPIXELHT);
	bangTFTbit(L_CS, HIGH);
}

void fB_TFT::drawRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)	swap(int, x1, x2);
	if (y1>y2)	swap(int, y1, y2);
	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void fB_TFT::drawRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)	swap(int, x1, x2);
	if (y1>y2)	swap(int, y1, y2);
	if ((x2-x1)>4 && (y2-y1)>4)	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void fB_TFT::fillRect(int x1, int y1, int x2, int y2,uint8_t color)
{
	bangTFTbit(L_CS, LOW);
  
	setXY(x1,y1,x2,y2);
	  
	for(long i=0;i< (y2-y1+1)*(x2-x1+1);i++) LCD_Write_HDATA(color);
	//clrXY();	
	bangTFTbit(L_CS, HIGH);
}

void fB_TFT::fillRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)	swap(int, x1, x2);
	if (y1>y2)	swap(int, y1, y2);

	if ((x2-x1)>4 && (y2-y1)>4)	{
		for (int i=0; i<((y2-y1)/2)+1; i++)		{
			switch(i)	{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void fB_TFT::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	char ch, cl;
	
	bangTFTbit(L_CS, LOW);
	setXY(x, y + radius, x, y + radius);
	LCD_Write_HDATA(fColor);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_HDATA(fColor);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_HDATA(fColor);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_HDATA(fColor);
 
	while(x1 < y1)	{
		if(f >= 0) 	{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_HDATA(fColor);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_HDATA(fColor);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_HDATA(fColor);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_HDATA(fColor);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_HDATA(fColor);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_HDATA(fColor);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_HDATA(fColor);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_HDATA(fColor);
	}
	clrXY();
	bangTFTbit(L_CS, HIGH);
}

void fB_TFT::fillCircle(int x, int y, int radius)
{
	bangTFTbit(L_CS, LOW);
	for(int y1=-radius; y1<=radius; y1++) 
		for(int x1=-radius; x1<=radius; x1++) 
			if(x1*x1+y1*y1 <= radius*radius) {
				setXY(x+x1, y+y1, x+x1, y+y1);
				LCD_Write_HDATA(fColor);
			}
	clrXY();
	bangTFTbit(L_CS, HIGH);
}


void fB_TFT::drawPixel(int x, int y)
{
	bangTFTbit(L_CS, LOW);
	setXY(x, y, x, y);
	LCD_Write_HDATA(fColor);
	clrXY();
	bangTFTbit(L_CS, HIGH);
}

void fB_TFT::drawLine(int x1, int y1, int x2, int y2)
{
	int tmp;
	float delta, tx, ty;
	float m, b, dx, dy;
	char ch;
	
	if (((x2-x1)<0))	{
		swap(int, x1, x2);
		swap(int, y1, y2);
	}
    if (((y2-y1)<0))	{
		swap(int, x1, x2);
		swap(int, y1, y2);
	}

	if (y1==y2)	{
		if (x1>x2)	{
			swap(int, x1, x2);
		}
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2){
		if (y1>y2)	swap(int, y1, y2);
		drawVLine(x1, y1, y2-y1);
	}
	else if (abs(x2-x1)>abs(y2-y1))	{
		bangTFTbit(L_CS, LOW);

		delta=(float(y2-y1)/float(x2-x1));
		ty=float(y1);
		if (x1>x2)	{
			for (int i=x1; i>=x2; i--)	{
				setXY(i, int(ty+0.5), i, int(ty+0.5));
				LCD_Write_HDATA(fColor);
        		ty=ty-delta;
			}
		}
		else{
			for (int i=x1; i<=x2; i++)	{
				setXY(i, int(ty+0.5), i, int(ty+0.5));
				LCD_Write_HDATA(fColor);
        		ty=ty+delta;
			}
		}
		clrXY();
		bangTFTbit(L_CS, HIGH);
	}
	else
	{
		bangTFTbit(L_CS, LOW);
		delta=(float(x2-x1)/float(y2-y1));
		tx=float(x1);
        if (y1>y2)   {
			for (int i=y2+1; i>y1; i--)	{
		 		setXY(int(tx+0.5), i, int(tx+0.5), i);
				LCD_Write_HDATA(fColor);
        		tx=tx+delta;
			}
        }
        else   {
			for (int i=y1; i<y2+1; i++)	{
		 		setXY(int(tx+0.5), i, int(tx+0.5), i);
				LCD_Write_HDATA(fColor);
        		tx=tx+delta;
			}
        }
		clrXY();
		bangTFTbit(L_CS, HIGH);
	}
}

void fB_TFT::drawHLine(int x, int y, int l)
{

	bangTFTbit(L_CS, LOW);
	setXY(x, y, x+l, y);
	for (int i=0; i<l+1; i++){
		LCD_Write_HDATA(fColor);
	}
	clrXY();
	bangTFTbit(L_CS, HIGH);
}

void fB_TFT::drawVLine(int x, int y, int l)
{

	bangTFTbit(L_CS, LOW);
	setXY(x, y, x, y+l);
	for (int i=0; i<l; i++)	LCD_Write_HDATA(fColor);
	clrXY();
	bangTFTbit(L_CS, HIGH);
}


void fB_TFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;
	uint8_t r, g, b;

	bangTFTbit(L_CS, LOW);

	if (scale==1)	{
		if (orient==PORTRAIT)	{
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)	{
				col=pgm_read_word(&data[tc]);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		}
	}
	else{
			for (ty=0; ty<sy; ty++)	{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++) {
					for (tx=0; tx<sx; tx++)	{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)	LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}
	}
	clrXY();
	bangTFTbit(L_CS, HIGH);
}

void fB_TFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	uint8_t r, g, b;
	float radian;
	radian=deg*0.0175;  

	if (deg==0)	drawBitmap(x, y, sx, sy, data);
	else {
		bangTFTbit(L_CS, LOW);
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)	{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		clrXY();
		bangTFTbit(L_CS, LOW);
	}
}

void fB_TFT::lcdOff()
{
		bangTFTbit(L_CS, LOW);
		bangTFTbit(L_CS, HIGH);
}

void fB_TFT::lcdOn()
{
		bangTFTbit(L_CS, LOW);
		bangTFTbit(L_CS, HIGH);
}

void fB_TFT::setContrast(char c)
{		
	
		bangTFTbit(L_CS, LOW);
		bangTFTbit(L_CS, HIGH);

}





/////////////////////////// Touchscreen routines ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/*
void fB_TFT::bangTFTbit(uint8_t  tDex, int value) {
	bitWrite(tftCtrlReg,(int)tPin[tDex].cpin,value);
	i2c.write((uint8_t )tPin[tDex].caddr, (uint8_t )tftCtrlReg);

}
	initTpin(T_CLK, TC, TCADDR,0, _OUTPUT);
	initTpin(T_DIN,  TC, TCADDR,1, _OUTPUT);
	initTpin(T_CS,  TC, TCADDR,2, _OUTPUT);
	initTpin(T_OUT, TC, TCADDR,3, INPUT);
	initTpin(T_IRQ,		TM,NULL ,  M5, INPUT);


uint8_t  fB_TFT::readTouchpin(uint8_t  tDex) {
	uint8_t iregister = 0;
	//bitWrite(iRegister,(int)tPin[tDex].cpin,1);
	//i2c.read((uint8_t ) tPin[tDex].caddr,(uint8_t )tftCtrlReg,(uint8_t )1);
	i2c.read((uint8_t ) tPin[tDex].caddr,(uint8_t )1);
Serial.print("in Reg: ");
Serial.println(iregister,HEX);
	iregister = (uint8_t )i2c.receive();
	return bitRead(iregister,tPin[tDex].cpin);

}
*/

uint8_t  fB_TFT::readTouchPin(uint8_t  tDex) {
  uint8_t  iregister = 0;
  i2c.read((uint8_t )tPin[tDex].caddr, (uint8_t )1);
  iregister = (uint8_t )i2c.receive();

  return bitRead(iregister,tPin[tDex].cpin);

}

/*
int PCF8574::digitalRead(int pin){
	if((pin >= 0) && (pin < 8)){
		i2cRead();
		return bitRead(PCFPORTA,pin);
	}
}

int PCF8574::i2cRead(uint8_t  value){
  i2c.read(PCFaddress, 1);
  return (int)i2c.receive();
}

*/
void fB_TFT::initTouch()
{
  /*
  bangTFTbit(T_CS,LOW);
  bangTFTbit(T_CLK,LOW);
  bangTFTbit(T_DIN,LOW);
  bangTFTbit(T_CLK,LOW);
  bangTFTbit(T_IRQ,LOW);

  delay(1000);
  */
  unsigned nop;

  bangTFTbit(T_CS,HIGH);
		bangTFTbit(T_CLK, LOW);                
		delayMicroseconds(30);
		bangTFTbit(T_CLK, HIGH);
		delayMicroseconds(30);
  bangTFTbit(T_DIN,HIGH);
		delayMicroseconds(30);
		bangTFTbit(T_CLK, LOW);                
		delayMicroseconds(30);
		bangTFTbit(T_CLK, HIGH);
		delayMicroseconds(30);

  pinMode(2,_INPUT);
  digitalWrite(2,HIGH);

}
void fB_TFT::TouchWriteData(uint8_t data)
{
	uint8_t temp;
	uint8_t nop;

	temp=data;
	bangTFTbit(T_CLK,LOW);

	for(uint8_t count=0; count<8; count++)
	{
		if(temp & 0x80)
			bangTFTbit(T_DIN, HIGH);
		else
			bangTFTbit(T_DIN, LOW);
		temp = temp << 1; 
				delay(20);

		bangTFTbit(T_CLK, LOW);                
		delayMicroseconds(30);
		bangTFTbit(T_CLK, HIGH);
		delayMicroseconds(30);
	}
}

word fB_TFT::TouchReadData()
{
	int bit;
	word data = 0;
	for(uint8_t count=0; count<12; count++)
	{
		data <<= 1;

		bangTFTbit(T_CLK, HIGH);               
		delayMicroseconds(30);
		bangTFTbit(T_CLK, LOW);
		delayMicroseconds(30);
		bit = digitalRead(A3);
		if (bit) data++;

  //Serial.print(" A3: " );
  //Serial.println(bit,DEC);

	}
	return(data);
}

void fB_TFT::TouchGetAddress()
{
	unsigned long tx=0;
	unsigned long ty=0;

	bangTFTbit(T_CS,LOW);                    

		TouchWriteData(0x90);        
		bangTFTbit(T_CLK,HIGH);
		delayMicroseconds(30);

		bangTFTbit(T_CLK,LOW); 
		ty+=TouchReadData();

		TouchWriteData(0xD0);      
		bangTFTbit(T_CLK,HIGH);
		delayMicroseconds(30);

		bangTFTbit(T_CLK,LOW);
		tx+=TouchReadData();
		delayMicroseconds(30);

	bangTFTbit(T_CS,HIGH);

	TP_X=tx;
	TP_Y=ty;
}

bool fB_TFT::TouchIRQ()
{
  bool avail;
  avail = !digitalRead(5);
  return avail;
}
unsigned int fB_TFT::TouchGetX()
{
  unsigned int lx;
  lx =  TP_X/14.4 - 24;
  return lx;
  
}

unsigned int fB_TFT::TouchGetY()
{
  unsigned int ly;
  ly = 370 - TP_Y/10 ;
  return ly;  
}


/*
void ITDB02_Touch::TouchPrecision(uint8_t precision)
{
	switch (precision)
	{
		case PREC_LOW:
			prec=1;
			break;
		case PREC_MEDIUM:
			prec=10;
			break;
		case PREC_HI:
			prec=25;
			break;
		case PREC_EXTREME:
			prec=100;
			break;
		default:
			prec=10;
			break;
	}
}

void fB_TFT::WriteDataTo7843(unsigned char num)
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
  bangTFTbit(T_CLK,LOW);
	for(count=0;count<8;count++)
  {
    if(temp&0x80)
      bangTFTbit(T_DIN,HIGH);
    else
      bangTFTbit(T_DIN,LOW);

    temp=temp<<1; 

	bangTFTbit(T_CLK,LOW);                
nop++;
	bangTFTbit(T_CLK,HIGH);
nop++;
 
  }
}

unsigned int fB_TFT::ReadDataFrom7843()
{
  unsigned nop;
  unsigned char count=0;
  unsigned int Num=0;
  for(count=0;count<12;count++)
  {
	//bangTFTbit(T_OUT,LOW);
    //nop++;

    Num<<=1;
    bangTFTbit(T_CLK,HIGH);               
    nop++;
    bangTFTbit(T_CLK,LOW);
    nop++;
    if(digitalRead(A3)) Num++;
	  
  }
  return(Num);
}

void fB_TFT::TouchGetaddress()
{
  unsigned nop;
  bangTFTbit(T_CS,LOW); 

 WriteDataTo7843(0x90);  
    nop++;

 
  bangTFTbit(T_CLK,HIGH);
     nop++;
 
  bangTFTbit(T_CLK,LOW); 

    nop++;

  TP_Y=ReadDataFrom7843();
    nop++;

  WriteDataTo7843(0xD0);      

 bangTFTbit(T_CLK,HIGH);
   
    nop++;

  bangTFTbit(T_CLK,LOW);
    nop++;
  

 TP_X=ReadDataFrom7843();
    nop++;

  bangTFTbit(T_CS,HIGH);
    nop++;

  Serial.print("tX,tY" );
  Serial.print(TP_X,DEC);
  Serial.print(" , " );
  Serial.println(TP_Y,DEC);

}

bool fB_TFT::TouchIRQ()
{
  bool ir;
  //ir=readTpin(T_IRQ);
  ir=digitalRead(2);

  return ir;
}



*/
