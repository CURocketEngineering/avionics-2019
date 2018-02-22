/*
 * Definition file for communication functions
 */

/* LIBRARIES */
#include <Arduino.h>

#include "pins.h"

#include "datalog.h"
#include "ninedof.h"
#include "barometer.h"
#include "gps.h"
#include "communication.h"

#include "json.h"

DynamicJsonBuffer json;

JsonObject & error = json.createObject();
JsonObject & telemetry = json.createObject();

void communication_updateTelemetry() {
     ninedof_read(true);
     barometer_read(true);
     gps_read(true);

     telemetry["sensors"]["gyro"]["x"] = gyro.x;
     telemetry["sensors"]["gyro"]["y"] = gyro.x;
     telemetry["sensors"]["gyro"]["z"] = gyro.x;

     telemetry["sensors"]["acc"]["x"] = acc.x;
     telemetry["sensors"]["acc"]["y"] = acc.x;
     telemetry["sensors"]["acc"]["z"] = acc.x;

     telemetry["sensors"]["mag"]["x"] = mag.x;
     telemetry["sensors"]["mag"]["y"] = mag.x;
     telemetry["sensors"]["mag"]["z"] = mag.x;

     telemetry["sensors"]["bar"]["p"] = bar.p;
     telemetry["sensors"]["bar"]["dp"] = bar.dp;

     telemetry["sensors"]["bar"]["alt"] = bar.alt;
     telemetry["sensors"]["bar"]["gnd"] = bar.gnd;

     telemetry["sensors"]["bar"]["temp"] = bar.temp;
     telemetry["sensors"]["bar"]["hum"] = bar.hum;

     telemetry["sensors"]["gps"]["lat"] = gps.lat;
     telemetry["sensors"]["gps"]["lon"] = gps.lon;

     String str;
     telemetry.printTo(str);
     datalog_write(str);
}

void communication_init() {
     error["type"] = "error";
     error["time"] = millis();
     error["message"] = "none";

     telemetry.createNestedObject("sensors");
     telemetry["sensors"].createNestedObject("gyro");
     telemetry["sensors"].createNestedObject("acc");
     telemetry["sensors"].createNestedObject("mag");
     telemetry["sensors"].createNestedObject("bar");
     telemetry["sensors"].createNestedObject("gps");

     telemetry["type"] = "telemetry";
     telemetry["time"] = millis();
     telemetry["state"] = "init";

     telemetry["sensors"]["gyro"]["x"] = 0;
     telemetry["sensors"]["gyro"]["y"] = 0;
     telemetry["sensors"]["gyro"]["z"] = 0;

     telemetry["sensors"]["acc"]["x"] = 0;
     telemetry["sensors"]["acc"]["y"] = 0;
     telemetry["sensors"]["acc"]["z"] = 0;

     telemetry["sensors"]["mag"]["x"] = 0;
     telemetry["sensors"]["mag"]["y"] = 0;
     telemetry["sensors"]["mag"]["z"] = 0;

     telemetry["sensors"]["bar"]["p"] = 0;
     telemetry["sensors"]["bar"]["dp"] = 0;
     telemetry["sensors"]["bar"]["alt"] = 0;
     telemetry["sensors"]["bar"]["gnd"] = 0;
     telemetry["sensors"]["bar"]["temp"] = 0;
     telemetry["sensors"]["bar"]["hum"] = 0;

     telemetry["sensors"]["gps"]["lat"] = 0;
     telemetry["sensors"]["gps"]["lon"] = 0;

     Serial.begin(9600);
}
