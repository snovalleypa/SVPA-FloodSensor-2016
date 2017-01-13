/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/
#include "publishSVPA.h"

const int schemaVersion = 1;
/*
struct Reading {
  int timeStamp; // (unix time)
  int range; // cm
  float internalTemp; //Degrees Celsius
  float internalPressure;
  float internalHumidity;
  float soc; //percentage
  float voltage;
  float rssi;

};

struct Report {
  int schemaVersion;
  int firmwareVersion;
  char deviceId[24];
  int nextUpdateTime; // (unix time);
  Reading readings[10];

};
*/

retained Reading lastReadings[10];


/*
{
  "schemaVersion": "1",
  "firmwareVersion": "8",
  "deviceId": "Dummy"
  "nextUpdateTime": "1484196300",
  "readings": [
      "timeStamp": "1484196330",
      "range": "79",
      "internalTemp": "20.05",
      "internalPressure": "1010.10",
      "internalHumidity": "32.122272",
      "soc": "80.585938",
      "rssi": "-81,37",
      "voltage": "3.858300"
    ]
}

@TODO
  externalTemp [ ]
    id (string)
    temp (float)

*/

void publishSVPA(){
  String reportJSON =     "{"
      "\"schemaVersion\":\"1\","
      "\"firmwareVersion\":\"8\","
      "\"deviceId\":\"Dummy\","
      "\"nextUpdateTime\":\"1484196300\","
      "\"readings\":[{"
      "\"timeStamp\":\"1484196330\","
      "\"range\":\"79\","
      "\"internalTemp\":\"20.05\","
      "\"internalPressure\":\"1010.10\","
      "\"internalHumidity\":\"32.122272\","
      "\"soc\":\"80.585938\","
      "\"rssi\":\"-81,37\","
      "\"voltage\":\"3.858300\""
      "}]"
      "}"
      ;

  Particle.publish("SVPA:",reportJSON);

}

void saveNewReading(Reading newReading){
  for(int i=9; i=1; i--){
    lastReadings[i] = lastReadings[i-1];
  }
  lastReadings[0] = newReading;

}

int getSchemaVersion(){
  return schemaVersion;
}
