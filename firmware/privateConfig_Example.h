/******************************************************************************
  2017 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

#include "application.h"//needed for all Particle libraries

#ifndef privateConfig_h
#define privateConfig_h

#endif

/* Update the info below to point to a valid end point
and save as privateConfig.h */
String targetServer = "example.com";
int port = 80;
String targetResource = "/post.php";
