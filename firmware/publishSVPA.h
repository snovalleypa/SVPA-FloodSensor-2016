/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

#include "application.h"//needed for all Particle libraries

#ifndef publishSVPA_h
#define publishSVPA_h

#endif

struct Reading {
  int timeStamp; // (unix time)
  int range; // cm
  float internalTemp; //Degrees Celsius
  float internalPressure;
  float internalHumidity;
  float soc; //percentage
  float voltage;
  int rssi;

};

struct Report {
  int schemaVersion;
  int firmwareVersion;
  char deviceId[24];
  int nextUpdateTime; // (unix time);
  Reading readings[10];

};



void saveNewReading(Reading);

Reading getLastReading(int i);

int getSchemaVersion();

String getJSON(Report newReport);
String getJSON(Reading newReading);

void publishSVPA();

void publishReading(Reading theReading);

void publishDebug(String debugString);
