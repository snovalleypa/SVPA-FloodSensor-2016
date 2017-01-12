/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/
#include "checkUpdateOTA.h"

STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

retained int nextUpdateCheck;
int updateDelayOTA = 60000; //miliseconds, aka 1 minute
int updateCadanceOTA = 25; //hours, aka once per day


int runTimeAdjSec = -10; // adjustment time from wake-up to publish
int publishTimeMin = 60;   // minimum sleep seconds between publish events
int publishTimeMax = 3600; // maximum sleep seconds between publish events

void checkUpdateOTA(){
  //@TODO Add routine to ensure that OTA updates are receivable once per day.

  //Check the current time
  int nowTime = Time.now();

  // Set time zone to Pacific USA standard time
  Time.zone(-8);

  if (nextUpdateCheck < nowTime){
    // Wait for an over the air update
    nextUpdateCheck = nowTime + (updateCadanceOTA * 3600);

    Particle.publish("UpdateOTA:",
      "Wait for next OTA Update after: " + Time.format(nextUpdateCheck, TIME_FORMAT_DEFAULT)
      );

    delay(updateDelayOTA);

  } else {
    //@TODO Comment this publish event, once done debugging
    Particle.publish("UpdateOTA:",
      "Next OTA Update planned after: " + Time.format(nextUpdateCheck, TIME_FORMAT_DEFAULT)
      );

  }


};

long getSecUntilPublish(){
  //Publish data events only once every 15 minutes

  int minute = 0;
  int nextPublishMinute = 0;
  long secUntilPublish = 900; // aka 15 minutes

  struct tm t;
  //Check the current time, in minutes
  minute = Time.minute() + 1;
  //@TODO Fix time check to account for runTimeAdjSec

  // Compute the next publish time
  //@TODO Refactor to work with an actual target time
  //@TODO Impliment runTimeAdjSec

  time_t t_of_day;
   t.tm_year = Time.year()-1900;
   t.tm_mon = Time.month() -1;           // Month, 0 - jan
   t.tm_mday = Time.day();          // Day of the month
   t.tm_hour = Time.hour() + 8; // w/ TZ correction
   t.tm_min = 0;
   t.tm_sec = 0;
   t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown


  if (minute > 45) {
    minute = minute - 60;
    nextPublishMinute = 0;
    t.tm_hour = t.tm_hour + 1;
    t.tm_min = 0;
  } else if (minute > 30) {
    nextPublishMinute = 45;
    t.tm_min = 45;
  } else if (minute > 15) {
    nextPublishMinute = 30;
    t.tm_min = 30;
  } else if (minute > 0) {
    nextPublishMinute = 15;
    t.tm_min = 15;
  }

  t_of_day = mktime(&t);

  // compute the number of minute until the next publish time

  secUntilPublish = (int)t_of_day - Time.now() + runTimeAdjSec;

  //@TODO Comment this publish event, once done debugging
  Particle.publish("Debug:",
    "Target:" + String((int)t_of_day) + " - Now: " + String(Time.now()) +
    " = secUntilPublish:"+ String(secUntilPublish)
    );

  //secUntilPublish = ((nextPublishMinute - minute) * 60) + runTimeAdjSec;

  //@TODO Adjust secUntilUpdate by a random amount to reduce network contention


  // check that getSecUntilUpdate is in a reasonable range
  if (secUntilPublish < publishTimeMin){secUntilPublish = publishTimeMin;}
  if (secUntilPublish > publishTimeMax){secUntilPublish = publishTimeMax;}


  return secUntilPublish;
};
