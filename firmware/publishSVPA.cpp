/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/
#include "publishSVPA.h"
#include "privateConfig.h"


const int schemaVersion = 1;
const int debugLevel = 1;

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

retained int nextReadingIndex = 0;
retained Reading lastReadings[10];

TCPClient client;

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
  publishDebug("Saving new Reading...");
  publishDebug(String("index=" + String(nextReadingIndex)));
  lastReadings[nextReadingIndex] = newReading;
  nextReadingIndex++;
  if (nextReadingIndex > 10) nextReadingIndex = 0;
}

Reading getLastReading(int i){
  //@TODO reference index into circular array starting at nextReadingIndex
  return lastReadings[i];
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
  myJSON.concat("\"nextUpdateTime\":");
  myJSON.concat(String(newReport.nextUpdateTime));
  myJSON.concat(",");
  myJSON.concat("\"readings\":");
  myJSON.concat("[");
    // .concat readings 0 through 10
    for (int i=0; i <= 9; i++){
      myJSON.concat(getJSON(newReport.readings[i]));
      //@TODO add camas in JSON Array
      myJSON.concat(",");
    }
    myJSON.concat(getJSON(newReport.readings[10]));

  myJSON.concat("]");

  myJSON.concat("}");
  return myJSON;
}

String getJSON(Reading newReading){
  String myJSON = "{";
  myJSON.concat("\"timeStamp\":");
  myJSON.concat(String(newReading.timeStamp));
  myJSON.concat(",");
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

void publishDebug(String debugString){
  if(debugLevel>=1){
    if(debugString.length()<=250){
      Particle.publish("debug", debugString);
    } else {
      Particle.publish("debug", debugString.substring(0, 250));
      delay(2000);
      publishDebug(debugString.substring(250));
    }
  }
}

int pushReport(Report newReport){
  String putJSON;
   putJSON.reserve(2048);
   putJSON = getJSON(newReport);

  publishDebug(String("targetServer=" + targetServer));

  /*
  // Local Network debugging for Photon version
  byte targetIP[] = {192, 168, 11, 36};
  targetServer = "ProlCat";
  port = 2129;

  publishDebug(String("targetIP=" +
    String(targetIP[0]) + "."+ String(targetIP[1])
    + "."+ String(targetIP[2])+ "."+ String(targetIP[3])
    ));
  */

  publishDebug(String("port=" + String(port)));

  delay(1000);

  if (client.connect(targetServer, port))
  //if (client.connect(targetIP, port)) //for Photon version
  {
     String pushRequest = "POST ";
      pushRequest.concat(targetResource);
      pushRequest.concat("  HTTP/1.1");
      pushRequest.concat("\r\n");
      pushRequest.concat("Host: ");
      pushRequest.concat(targetServer);
      pushRequest.concat("\r\n");
      pushRequest.concat("Connection: keep-alive");
      pushRequest.concat("\r\n");
      //client.println("Content-Type:   application/json;");
      pushRequest.concat("Content-Type:   text/plain; charset=UTF-8");
      pushRequest.concat("\r\n");
      pushRequest.concat("Content-Length:  ");
      pushRequest.concat(String(putJSON.length()));

    publishDebug(String("pushRequest=" + pushRequest));
    //publishDebug(String("putJSON=" + putJSON));

    client.println(pushRequest);
    client.println();

    //int pushBytes = client.println(putJSON);
    int pushBytes;

    if(putJSON.length()<=1023){
      pushBytes = client.println(putJSON);
    } else if(putJSON.length()<=2047){
      pushBytes = client.print(putJSON.substring(0,1023));
      pushBytes = pushBytes + client.print(putJSON.substring(1023));
    }

    delay(500);
    client.println();
    client.println();

    publishDebug(String("pushbytes=" + String(pushBytes)));

    delay(5000);

    publishDebug(String("client.available=" + String(client.available())));

    String pushResponse = "";
    char nextResponseChar;
    int bytesRead = 0;

    while (client.available())
      {
        nextResponseChar = client.read();
        bytesRead++;
        pushResponse.concat(nextResponseChar);
        delay(15);
        if (pushResponse.length() > 254){
            publishDebug(String("pushResponse=" + pushResponse));
            pushResponse = "";
        }
      }

    publishDebug(String("pushResponse=" + pushResponse));
    publishDebug(String("bytesRead=" + String(bytesRead)));

    return 0;
  }
  else
  {
    publishDebug(String("putReportResult= Did not connect."));
    return -1;
  }
}
