/*
 * Definition file for communication functions
 */

/* LIBRARIES */
#include <Arduino.h>

#include "pins.h"
#include "accelerometer.h"
#include "barometer.h"
#include "gps.h"
#include "communication.h"

DynamicJsonBuffer json;

JsonObject & telemetry = json.createObject();

void communication_updateTelemetry() {
     ninedof_read(true);
     barometer_read(true);
     gps_read(true);

     obj["sensors"]["gyro"]["x"] = gyro.x;
     obj["sensors"]["gyro"]["y"] = gyro.x;
     obj["sensors"]["gyro"]["z"] = gyro.x;

     obj["sensors"]["acc"]["x"] = acc.x;
     obj["sensors"]["acc"]["y"] = acc.x;
     obj["sensors"]["acc"]["z"] = acc.x;

     obj["sensors"]["mag"]["x"] = mag.x;
     obj["sensors"]["mag"]["y"] = mag.x;
     obj["sensors"]["mag"]["z"] = mag.x;

     obj["sensors"]["bar"]["p"] = bar.p;
     obj["sensors"]["bar"]["dp"] = bar.dp;
     obj["sensors"]["bar"]["alt"] = bar.alt;
     obj["sensors"]["bar"]["gnd"] = bar.gnd;
     obj["sensors"]["bar"]["temp"] = bar.temp;
     obj["sensors"]["bar"]["hum"] = bar.hum;

     obj["sensors"]["gps"]["lat"] = gps.lat;
     obj["sensors"]["gps"]["lon"] = gps.lon;
}

void communication_init() {
     obj.createNestedObject("sensors");
     obj["sensors"].createNestedObject("gyro");
     obj["sensors"].createNestedObject("acc");
     obj["sensors"].createNestedObject("mag");
     obj["sensors"].createNestedObject("bar");
     obj["sensors"].createNestedObject("gps");

     obj["time"] = millis();
     obj["state"] = "init";

     obj["sensors"]["gyro"]["x"] = 0;
     obj["sensors"]["gyro"]["y"] = 0;
     obj["sensors"]["gyro"]["z"] = 0;

     obj["sensors"]["acc"]["x"] = 0;
     obj["sensors"]["acc"]["y"] = 0;
     obj["sensors"]["acc"]["z"] = 0;

     obj["sensors"]["mag"]["x"] = 0;
     obj["sensors"]["mag"]["y"] = 0;
     obj["sensors"]["mag"]["z"] = 0;

     obj["sensors"]["bar"]["p"] = 0;
     obj["sensors"]["bar"]["dp"] = 0;
     obj["sensors"]["bar"]["alt"] = 0;
     obj["sensors"]["bar"]["gnd"] = 0;
     obj["sensors"]["bar"]["temp"] = 0;
     obj["sensors"]["bar"]["hum"] = 0;

     obj["sensors"]["gps"]["lat"] = 0;
     obj["sensors"]["gps"]["lon"] = 0;

     Serial.begin(9600);
}
