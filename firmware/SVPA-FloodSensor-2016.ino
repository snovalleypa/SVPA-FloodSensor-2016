/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************
  Some code taken heavely from the SparkFun Photon Weather Shield basic example
  by Joel Bartlett @ SparkFun Electronics
  https://github.com/sparkfun/SparkFun_Photon_Weather_Shield_Particle_Library

  Some code taken heavely from the  I2CXL-MaxSonar® - WR/WRCTM Series data sheet
  http://www.maxbotix.com/documents/I2CXL-MaxSonar-WR_Datasheet.pdf

*******************************************************************************/
PRODUCT_ID(1623);
PRODUCT_VERSION(8);


/******************************************************************************
  Basic SparkFun Photon Weather Shield
*******************************************************************************/
#include "SparkFun_Photon_Weather_Shield_Library.h"

#include "checkUpdateOTA.h"
#include "publishSVPA.h"

double humidity = 0;
double tempf = 0;
double pascals = 0;
double baroTemp = 0;
double altf = 0;


//#include "SparkFunMAX17043/SparkFunMAX17043.h" // Include the SparkFun MAX17043 library

double voltage = 0;
double soc = 0;
bool alert;

long lastPublish = 0;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barometric sensor
Weather weatherShield;

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

int rangValue = 0;

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2

FuelGauge fuel; // instantiate a FuelGauge object for use monitoring the battery

//---------------------------------------------------------------
void setup()
{
    /* Commenting out so as to not use up GSM Data
    // Create Particle.variables for each piece of data for easy access
    Particle.variable("humidity", humidity);
    Particle.variable("tempF", tempf);
    Particle.variable("pressurePascals", pascals);
    Particle.variable("baroTemp", baroTemp);
    Particle.variable("range", rangValue);
    */

    //Initialize the I2C sensors and ping them
    weatherShield.begin();

    /*You can only receive accurate barometric readings or accurate altitude
    readings at a given time, not both at the same time. The following two lines
    tell the sensor what mode to use. You could easily write a function that
    takes a reading in one made and then switches to the other mode to grab that
    reading, resulting in data that contains both accurate altitude and barometric
    readings. For this example, we will only be using the barometer mode. Be sure
    to only uncomment one line at a time. */
    weatherShield.setModeBarometer();//Set to Barometer Mode
    //weatherShield.setModeAltimeter();//Set to altimeter Mode

    //These are additional MPL3115A2 functions that MUST be called for the sensor to work.
    weatherShield.setOversampleRate(7); // Set Oversample rate
    //Call with a rate from 0 to 7. See page 33 for table of ratios.
    //Sets the over sample rate. Datasheet calls for 128 but you can set it
    //from 1 to 128 samples. The higher the oversample rate the greater
    //the time between data samples.


    weatherShield.enableEventFlags(); //Necessary register calls to enble temp, baro and alt

    pinMode(led2, OUTPUT);

    publishData();

    //delay(30000); // Wait for any over the air updates.

    digitalWrite(led2, HIGH);
    delay(500);
    digitalWrite(led2, LOW);
    delay(500);
    digitalWrite(led2, HIGH);
    delay(500);
    digitalWrite(led2, LOW);
    delay(500);
    digitalWrite(led2, HIGH);
    delay(1000);
    digitalWrite(led2, LOW);

    checkUpdateOTA();


    //System.sleep(SLEEP_MODE_DEEP,120); //shut down for 2 minutes
    int sleepSec = getSecUntilPublish();

    //@TODO Comment this publish event, once done debugging
    Particle.publish("Sleeping:",
      "Going to sleep for: " + String(sleepSec) + " seconds."
      );

    delay(2000); // to allow last events to reach the cloud

    System.sleep(SLEEP_MODE_DEEP,sleepSec); //shut down for 20 minutes



}
//---------------------------------------------------------------
void loop()
{

    publishData();

    delay(10000);


}
//---------------------------------------------------------------


void publishData(){


    digitalWrite(led2, HIGH);

    //Get readings from all sensors
    getWeather();

    getRange();

    getVoltage();



    publishRSSI();

    publishWeather();

    publishRange();

    publishVoltage();

    publishSVPA();


    digitalWrite(led2, LOW);

}

void publishWeather(){

    Particle.publish("humidity", String(humidity));
    Particle.publish("tempF", String(tempf));
    delay(1000);

//    Particle.publish("pressurePascals", String(pascals));
//    Particle.publish("baroTemp", String(baroTemp));

//    Particle.publish("altitudeFt", String(altf));
//    delay(1000);


}

void publishRange(){

    Particle.publish("Range", String(rangValue)); // Publish a range event
    delay(1000);


}



void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = weatherShield.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = weatherShield.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()

  //Measure the Barometer temperature in F from the MPL3115A2
  baroTemp = weatherShield.readBaroTempF();

  weatherShield.setModeBarometer();//Set to Barometer Mode
  //Measure Pressure from the MPL3115A2
  pascals = weatherShield.readPressure();


  weatherShield.setModeAltimeter();//Set to altimeter Mode
  //If in altitude mode, you can get a reading in feet with this line:
  altf = weatherShield.readAltitudeFt();
}

void takeRangeReading(){
    Wire.beginTransmission(SensorAddress);
    //Start addressing
    Wire.write(RangeCommand);
    //send range command
    Wire.endTransmission();
    //Stop and do something else now
}

void getRange() {


    takeRangeReading();
    //Tell the sensor to perform a ranging cycle
    delay(100);
    //Wait for sensor to finish
    uint16_t range = requestRange();
    //Get the range from the sensor


    rangValue = range;  //update the online range varable

    Serial.print("Range String: "); Serial.println(String(rangValue));
    //Print to the user

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

void getVoltage () {
    // fuel.getVCell() Returns the battery voltage as a float
	voltage = fuel.getVCell();
	// fuel.getSoC() returns the estimated state of charge (e.g. 79%)
	soc = fuel.getSoC();
	// fuel.getAlert() returns a 0 or 1 (0=alert not triggered)
	alert = fuel.getAlert();


}

void publishVoltage() {

    // fuel.getVoltage() returns a voltage value (e.g. 3.93)
	voltage = fuel.getVCell();
	// fuel.getSOC() returns the estimated state of charge (e.g. 79%)
	soc = fuel.getSoC();
	// lipo.getAlert() returns a 0 or 1 (0=alert not triggered)
	alert = fuel.getAlert();

    Particle.publish("voltage", String(voltage));
    Particle.publish("soc", String(soc));
    if (alert) {
        char resultstr[64]; //String to store the alret data
        sprintf(resultstr, "{\"voltage\":%d,\"soc\":%d}", voltage, soc); //Write sensor data to string
        Particle.publish("batteryAlert", resultstr);
    }


}

void publishRSSI() {

    CellularSignal myRSSI = Cellular.RSSI();

    //Serial.print("  RSSI = ");
    //Serial.print(myRSSI);

    Particle.publish("RSSI", String(myRSSI));

}
