/******************************************************************************
  2016 SVPA Flood Sensor Prototype Firmware
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/
#include "checkUpdateOTA.h"

STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

retained int nextUpdateCheck;
int updateDelayOTA = 3600; //miliseconds, aka 1 minute

int runTimeAdjSec = -118; // adjustment time from wake-up to publish

void checkUpdateOTA(){
  //@TODO Add routine to ensure that OTA updates are receivable once per day.

  //Check the current time
  int nowTime = Time.now();

  if (nextUpdateCheck < nowTime){
    // Wait for an over the air update
    delay(updateDelayOTA);
  }


};

long getSecUntilUpdate(){
  //Publish data events only once every 15 minutes

  int minute = 0;
  int nextPublishTime = 0;
  long secUntilUpdate = 900; // aka 15 minutes

  //Check the current time, in minutes
  minute = Time.minute();

  // Compute the next publish time

  if (minute > 45) {
    minute = minute - 60;
    nextPublishTime = 0;
  } else if (minute > 30) {
    nextPublishTime = 45;
  } else if (minute > 15) {
    nextPublishTime = 30;
  } else if (minute > 0) {
    nextPublishTime = 15;
  }

  // compute the number of minuted until the next publish time
  secUntilUpdate = (nextPublishTime - minute) * 60 + runTimeAdjSec;

  //@TODO Adjust secUntilUpdate by a random amount to reduce network contention

  return secUntilUpdate;
};
