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



void saveNewReading(Reading newReading){
  for(int i=9; i=1; i--){
    lastReadings[i] = lastReadings[i-1];
  }
  lastReadings[0] = newReading;

}

int getSchemaVersion(){
  return schemaVersion;
}

String getJSON(Report newReport){
  String myJSON = "{";
  myJSON.concat("\"schemaVersion\":\"");
  myJSON.concat(String(newReport.schemaVersion));
  myJSON.concat("\",");
  myJSON.concat("\"firmwareVersion\":\"");
  myJSON.concat(String(newReport.firmwareVersion));
  myJSON.concat("\",");
  myJSON.concat("\"deviceId\":\"");
  myJSON.concat(String(newReport.deviceId));
  myJSON.concat("\",");
  myJSON.concat("\"nextUpdateTime\":\"");
  myJSON.concat(String(newReport.nextUpdateTime));
  myJSON.concat("\"");
  myJSON.concat("}");
  return myJSON;
}

String getJSON(Reading newReading){
  String myJSON = "{";
  myJSON.concat("\"timeStamp\":\"");
  myJSON.concat(String(newReading.timeStamp));
  myJSON.concat("\",");
  myJSON.concat("\"range\":\"");
  myJSON.concat(String(newReading.range));
  myJSON.concat("\",");
  myJSON.concat("\"internalTemp\":\"");
  myJSON.concat(String(newReading.internalTemp));
  myJSON.concat("\",");
  myJSON.concat("\"internalPressure\":\"");
  myJSON.concat(String(newReading.internalPressure));
  myJSON.concat("\"");
  myJSON.concat(",");
  myJSON.concat("\"internalHumidity\":\"");
  myJSON.concat(String(newReading.internalHumidity));
  myJSON.concat("\"");
  myJSON.concat(",");
  myJSON.concat("\"soc\":\"");
  myJSON.concat(String(newReading.soc));
  myJSON.concat("\"");
  myJSON.concat(",");
  myJSON.concat("\"voltage\":\"");
  myJSON.concat(String(newReading.voltage));
  myJSON.concat("\"");
  myJSON.concat(",");
  myJSON.concat("\"rssi\":\"");
  myJSON.concat(String(newReading.rssi));
  myJSON.concat("\"");
  myJSON.concat("}");
  return myJSON;
}

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

void publishReading(Reading theReading){
    Particle.publish("Reading", getJSON(theReading));
}
