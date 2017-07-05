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
PRODUCT_VERSION(13); //Remember to update const below

// Impliments SEMI_AUTOMATIC sleep mode.
SYSTEM_MODE(SEMI_AUTOMATIC);

/******************************************************************************
  Basic SparkFun Photon Weather Shield
*******************************************************************************/
#include "SparkFun_Photon_Weather_Shield_Library.h"

#include "checkUpdateOTA.h"
#include "checkRange.h"
#include "publishSVPA.h"

const int firmwareVersion = 10;
const int debugLevel = 0;

double humidity = 0;
double tempf = 0;
double tempC = 0;
double pascals = 0;
double baroTemp = 0;
double altf = 0;

Reading currentReading;

Report currentReport;

//#include "SparkFunMAX17043/SparkFunMAX17043.h" // Include the SparkFun MAX17043 library

double voltage = 0;
double soc = 0;
bool alert;

long lastPublish = 0;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barometric sensor
Weather weatherShield;


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
    Particle.variable("range", );
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

}
//---------------------------------------------------------------
void loop()
{

  digitalWrite(led2, HIGH);
  delay(5000);
  digitalWrite(led2, LOW);
  delay(500);
  digitalWrite(led2, HIGH);
  delay(500);
  digitalWrite(led2, LOW);

  getData();

  Particle.connect();

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
/****************************************************************************
****************** System sleeps and resets.                *****************
*****************************************************************************
****************** The following code should never execute. *****************
*****************************************************************************/

    delay(10000);


}
//---------------------------------------------------------------

void getData(){

      digitalWrite(led2, HIGH);

      //Get readings from all sensors
      getWeather();

      getRange();

      getVoltage();


      currentReading.timeStamp = Time.now();
      currentReading.range = getRangeVal();
      currentReading.internalTemp = tempC;
      currentReading.internalPressure = pascals;
      currentReading.internalHumidity = humidity;
      currentReading.soc = soc;
      currentReading.voltage = voltage;
      CellularSignal sig = Cellular.RSSI();
      currentReading.rssi = sig.rssi;

      saveNewReading(currentReading);

      currentReport.schemaVersion = getSchemaVersion();
      currentReport.firmwareVersion = firmwareVersion;
      String strDeviceID = System.deviceID();
      strDeviceID.toCharArray(currentReport.deviceId, 24);
      currentReport.nextUpdateTime = getNextUpdateCheck();
      currentReport.readings[0] = currentReading;
      for (int i=0; i<=10; i++){
        currentReport.readings[i] =  getLastReading(i);
      }

}

void publishData(){

    //publishRSSI();

    //publishWeather();

    publishRange();

    publishVoltage();

    //publishDebug(getJSON(currentReport));
    //delay(1000);
    publishReading(currentReading);

    int pushReportResult = pushReport(currentReport);

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

    Particle.publish("Range", String(getRangeVal())); // Publish a range event
    delay(1000);


}



void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = weatherShield.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = weatherShield.getTempF();
  tempC = (tempf - 32) * 5 / 9;
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
