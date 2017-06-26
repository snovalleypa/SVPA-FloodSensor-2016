/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

#include "application.h"//needed for all Particle libraries

#ifndef checkRange_h
#define checkRange_h

/******************************************************************************
  Basic Range finding with I2CXL-MaxSonar
  Assumes the sensor is using the default address

*******************************************************************************/

//The Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8-bit 0xE0
#define SensorAddress byte(0x70)
//The sensors ranging command has a value of 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)




void getRange();
void takeRangeReading();
uint16_t requestRange();
int getRangeVal();


#endif
