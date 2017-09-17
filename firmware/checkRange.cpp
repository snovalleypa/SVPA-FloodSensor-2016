/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

#include "checkRange.h"

int rangeRep = 30; // Number of range readings to take and average.
int rangeUpLimit = 765; //Upper limit of valid ranges.
int rangeValue;

void getRange() {

    int rangeReadingIndex;
    int rangeReading[rangeRep];
    uint16_t range;
    int rangeSum = 0;
    int rangeCount,rangeMax;
    float rangeAvg;
    for (int i=0; i<=rangeRep; i++){
      //currentReport.readings[i] =  getLastReading(i);

      takeRangeReading();
      //Tell the sensor to perform a ranging cycle
      delay(100);
      //Wait for sensor to finish
      range = requestRange();
      //Get the range from the sensor

      rangeReading[i] = range;

      // @TODO Add debugging feedback. LEDs?
      Serial.print("Range String - ");
      Serial.println(String(i));
      Serial.print(": ");
      Serial.println(String(rangeReading[i]));
      //Print to the user

      delay(500);
    }

    // @TODO Screen for outliers and compute the average range.

    rangeMax = 0;
    rangeSum = 0;
    for (int i=0; i<=rangeRep; i++){

      rangeSum = rangeSum + rangeReading[i];
      if (rangeReading[i] > rangeMax && rangeReading[i] < rangeUpLimit)
        {
          rangeMax = rangeReading[i];
        }
    }

    rangeAvg = rangeSum / rangeRep;  //update the online range varable

    Serial.print("Range Average: "); Serial.println(String(rangeAvg));
    //Print to the user
    Serial.print("Range Max: "); Serial.println(String(rangeMax));
    //Print to the user

    //r1 = (int) rangeAvg;
    rangeValue = rangeMax;

}



int getRangeVal(){
  return rangeValue;
}


void takeRangeReading(){
    Wire.beginTransmission(SensorAddress);
    //Start addressing
    Wire.write(RangeCommand);
    //send range command
    Wire.endTransmission();
    //Stop and do something else now
}


//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.
uint16_t requestRange(){
    Wire.requestFrom(SensorAddress, byte(2));
    if(Wire.available() >= 2){
        //Sensor responded with the two bytes
        byte HighByte = Wire.read();
        //Read the high byte back
        byte LowByte = Wire.read();
        //Read the low byte back
        //uint16_t range = uint16_t(HighByte, LowByte);
        uint16_t range = 256 * HighByte + LowByte;
        //Make a 16-bit word out of the two bytes for the range
        return range;
    }
    else {
        return uint16_t(0);
        //Else nothing was received, return 0
    }
}
