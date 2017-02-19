/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

#include "application.h"//needed for all Particle libraries

#ifndef checkUpdateOTA_h
#define checkUpdateOTA_h

void checkUpdateOTA();
long getSecUntilPublish();
long getNextUpdateCheck();

#endif
