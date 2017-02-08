/******************************************************************************
  2017 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

#include "application.h"//needed for all Particle libraries

#ifndef privateConfig_h
#define privateConfig_h

#endif

String targetServer = "mpzap.com";
int port = 80;
String targetResource = "/post.php";
