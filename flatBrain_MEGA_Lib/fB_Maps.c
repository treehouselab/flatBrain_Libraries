#ifndef FB_MAPS_H
#define FB_MAPS_H

#include <avr/pgmspace.h>
#include <fB_SYS_Define.h>
#include <avr/io.h>

// in this block,  D coresponds to MCP pin, A to CD pin

uint8_t XmapB[40] PROGMEM={
	12,COL_L,IO_A,8,  // BO
	12,COL_R,IO_A,9,  // B1
	13,COL_L,IO_A,10,  // B2
	13,COL_R,IO_A,11,  // B3
	14,COL_L,IO_D,0,   // VD
	14,COL_R,IO_D,0,   // K0
	15,COL_L,IO_D,44,  // K1
	15,COL_R,IO_D,42,  // K2
	16,COL_L,IO_D,2,  // K3
	16,COL_R,IO_D,3  // K4
};

uint8_t Xmap50[64] PROGMEM={
	17,COL_L,IO_D,9,
	17,COL_R,IO_D,8,
	18,COL_L,IO_D,11,
	18,COL_R,IO_D,10,
	19,COL_L,IO_D,13,
	19,COL_R,IO_D,12,
	20,COL_L,IO_D,15,
	20,COL_R,IO_D,14,
	21,COL_L,IO_A,7,
	21,COL_R,IO_A,5,
	22,COL_L,IO_A,4,
	22,COL_R,IO_A,6,
	23,COL_L,IO_A,0,
	23,COL_R,IO_A,2,
	24,COL_L,IO_A,3,
	24,COL_R,IO_A,1
};
	
uint8_t Xmap76[128] PROGMEM={
	17,COL_L,IO_D,8,
	17,COL_R,IO_D,9,
	18,COL_L,IO_D,11,
	18,COL_R,IO_D,10,
	19,COL_L,IO_D,13,
	19,COL_R,IO_D,12,
	20,COL_L,IO_D,15,
	20,COL_R,IO_D,14,
	21,COL_L,IO_A,6,
	21,COL_R,IO_A,7,
	22,COL_L,IO_A,4,
	22,COL_R,IO_A,5,
	23,COL_L,IO_A,2,
	23,COL_R,IO_A,3,
	24,COL_L,IO_A,0,
	24,COL_R,IO_A,1,
	25,COL_L,IO_D,7,
	25,COL_R,IO_D,6,
	26,COL_L,IO_D,5,
	26,COL_R,IO_D,4,
	27,COL_L,IO_D,3,
	27,COL_R,IO_D,2,
	28,COL_L,IO_D,1,
	28,COL_R,IO_D,0,
	29,COL_L,IO_A,12,
	29,COL_R,IO_A,11,
	30,COL_L,IO_A,10,
	30,COL_R,IO_A,9,	
	31,COL_L,IO_A,13,  
	31,COL_R,IO_A,8,
	32,COL_L,IO_A,14,  
	32,COL_R,IO_A,15
};
/*
	f struct 
 { 
    unsigned short Min; 
    unsigned short Max; 
    unsigned short Def; 
    unsigned char iUM; 
 } MYSTRUCT; 

 const MYSTRUCT MyStruct_tab[] PROGMEM = 
 { 
    { 0, 10, 5, 0 }, 
    { 2, 20, 10, 1 }, 
    { 3, 30, 15, 2 } 
 }; 

 MYSTRUCT * pt; 

 int main(void) { 
    unsigned short Min, Max; 

    pt = &MyStruct_tab[1]; 
    Min = pgm_read_word(&pt->Min); 
    Max = pgm_read_word(&pt->Max); 
    PORTB = Min; 
    PORTC = Max; 
    while(1) { 
       asm("nop"); 
    } 
 }
// struct for mapping digital pins/
typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * pin;
  volatile uint8_t * port;
  uint8_t  bit;
}pin_map_t ;


pin_map_t *PmegaPinmap;

pin_map_t getMegaPinMapStruct(uint8_t pin) {
	

const pin_map_t MegaPinMap[] = PROGMEM {
  {&DDRE, &PINE, &PORTE, 0},  // E0  0
  {&DDRE, &PINE, &PORTE, 1},  // E1  1
  {&DDRE, &PINE, &PORTE, 4},  // E4  2
  {&DDRE, &PINE, &PORTE, 5},  // E5  3
  {&DDRG, &PING, &PORTG, 5},  // G5  4
  {&DDRE, &PINE, &PORTE, 3},  // E3  5
  {&DDRH, &PINH, &PORTH, 3},  // H3  6
  {&DDRH, &PINH, &PORTH, 4},  // H4  7
  {&DDRH, &PINH, &PORTH, 5},  // H5  8
  {&DDRH, &PINH, &PORTH, 6},  // H6  9
  {&DDRB, &PINB, &PORTB, 4},  // B4 10
  {&DDRB, &PINB, &PORTB, 5},  // B5 11
  {&DDRB, &PINB, &PORTB, 6},  // B6 12
  {&DDRB, &PINB, &PORTB, 7},  // B7 13
  {&DDRJ, &PINJ, &PORTJ, 1},  // J1 14
  {&DDRJ, &PINJ, &PORTJ, 0},  // J0 15
  {&DDRH, &PINH, &PORTH, 1},  // H1 16
  {&DDRH, &PINH, &PORTH, 0},  // H0 17
  {&DDRD, &PIND, &PORTD, 3},  // D3 18
  {&DDRD, &PIND, &PORTD, 2},  // D2 19
  {&DDRD, &PIND, &PORTD, 1},  // D1 20
  {&DDRD, &PIND, &PORTD, 0},  // D0 21
  {&DDRA, &PINA, &PORTA, 0},  // A0 22
  {&DDRA, &PINA, &PORTA, 1},  // A1 23
  {&DDRA, &PINA, &PORTA, 2},  // A2 24
  {&DDRA, &PINA, &PORTA, 3},  // A3 25
  {&DDRA, &PINA, &PORTA, 4},  // A4 26
  {&DDRA, &PINA, &PORTA, 5},  // A5 27
  {&DDRA, &PINA, &PORTA, 6},  // A6 28
  {&DDRA, &PINA, &PORTA, 7},  // A7 29
  {&DDRC, &PINC, &PORTC, 7},  // C7 30
  {&DDRC, &PINC, &PORTC, 6},  // C6 31
  {&DDRC, &PINC, &PORTC, 5},  // C5 32
  {&DDRC, &PINC, &PORTC, 4},  // C4 33
  {&DDRC, &PINC, &PORTC, 3},  // C3 34
  {&DDRC, &PINC, &PORTC, 2},  // C2 35
  {&DDRC, &PINC, &PORTC, 1},  // C1 36
  {&DDRC, &PINC, &PORTC, 0},  // C0 37
  {&DDRD, &PIND, &PORTD, 7},  // D7 38
  {&DDRG, &PING, &PORTG, 2},  // G2 39
  {&DDRG, &PING, &PORTG, 1},  // G1 40
  {&DDRG, &PING, &PORTG, 0},  // G0 41
  {&DDRL, &PINL, &PORTL, 7},  // L7 42
  {&DDRL, &PINL, &PORTL, 6},  // L6 43
  {&DDRL, &PINL, &PORTL, 5},  // L5 44
  {&DDRL, &PINL, &PORTL, 4},  // L4 45
  {&DDRL, &PINL, &PORTL, 3},  // L3 46
  {&DDRL, &PINL, &PORTL, 2},  // L2 47
  {&DDRL, &PINL, &PORTL, 1},  // L1 48
  {&DDRL, &PINL, &PORTL, 0},  // L0 49
  {&DDRB, &PINB, &PORTB, 3},  // B3 50
  {&DDRB, &PINB, &PORTB, 2},  // B2 51
  {&DDRB, &PINB, &PORTB, 1},  // B1 52
  {&DDRB, &PINB, &PORTB, 0},  // B0 53
  {&DDRF, &PINF, &PORTF, 0},  // F0 54
  {&DDRF, &PINF, &PORTF, 1},  // F1 55
  {&DDRF, &PINF, &PORTF, 2},  // F2 56
  {&DDRF, &PINF, &PORTF, 3},  // F3 57
  {&DDRF, &PINF, &PORTF, 4},  // F4 58
  {&DDRF, &PINF, &PORTF, 5},  // F5 59
  {&DDRF, &PINF, &PORTF, 6},  // F6 60
  {&DDRF, &PINF, &PORTF, 7},  // F7 61
  {&DDRK, &PINK, &PORTK, 0},  // K0 62
  {&DDRK, &PINK, &PORTK, 1},  // K1 63
  {&DDRK, &PINK, &PORTK, 2},  // K2 64
  {&DDRK, &PINK, &PORTK, 3},  // K3 65
  {&DDRK, &PINK, &PORTK, 4},  // K4 66
  {&DDRK, &PINK, &PORTK, 5},  // K5 67
  {&DDRK, &PINK, &PORTK, 6},  // K6 68
  {&DDRK, &PINK, &PORTK, 7}   // K7 69
};
*/
#endif
