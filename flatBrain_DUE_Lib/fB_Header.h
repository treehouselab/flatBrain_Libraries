#ifndef FP_HEADER_H
#define FP_HEADER_H
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <pins_arduino.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//#include <Timer.h>
#include <Wire.h>
extern TwoWire Wire1;


#include "fB_PCF8574.h"
#include "fB_MCP23017.h"
#include "fB_Define.h"
#include "fB_TFT_RY.h"
//#include "fB_tFAT.h"
//#include "fB_mmc.h"
#include "fB_RTC.h"
#include "fB_EEPROM.h"
#include "fB_Card.h"
//#include "fB_Pin.h"
#include "fB_Tag.h"
//#include "fB_Alarm.h"
//#include "fB_Record.h"
#include "fB_Define.h"
#include "fB_Menu.h"
#include "fB_Timer.h"
//#include "fBT_Seg.h"


inline void digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

inline int digitalReadDirect(int pin){
  return !!(g_APinDescription[pin].pPort -> PIO_PDSR & g_APinDescription[pin].ulPin);
}

#endif
