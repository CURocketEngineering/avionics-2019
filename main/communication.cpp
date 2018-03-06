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

void communication_sendResult(bool pass) {
     telemetry["type"] = "result";
     telemetry["time"] = millis();
     telemetry["pass"] = pass;
}

void communication_sendState(enum state_e state) {
     telemetry["type"] = "state";
     telemetry["time"] = millis();
     telemetry["state"] = states[state];
}

enum command_e communication_recvCommand() {
  static char buf[256];
  static char * ptr;
  static unsigned int comm = COMM_COUNT;

  if (Serial.available()) {
    comm = 0;

    JsonObject & command = json.parse(Serial);

    for (int idx = 0; idx < sizeof(commands)/sizeof(const char *); idx++) {
      if (commands[idx] == command["command"])
       return (enum command_e)idx;
    }
  }
  else if (comm >= COMM_COUNT) {
    return NO_COMM;
  }
  else {
    comm++;

    return CMD_NONE;
  }
}

void communication_updateTelemetry() {
     ninedof_read(true);
     barometer_read(true);
     gps_read();

     telemetry["type"] = "telemetry";
     telemetry["time"] = millis();

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

     telemetry["sensors"]["gps"]["lat"] = gps.lat;
     telemetry["sensors"]["gps"]["lon"] = gps.lon;

     telemetry["sensors"]["gps"]["hour"] = gps.hour;
     telemetry["sensors"]["gps"]["min"] = gps.min;
     telemetry["sensors"]["gps"]["sec"] = gps.sec;

     telemetry["sensors"]["gps"]["day"] = gps.day;
     telemetry["sensors"]["gps"]["mon"] = gps.mon;
     telemetry["sensors"]["gps"]["year"] = gps.year;

     String str;
     telemetry.printTo(str);

     Serial.println(str);
     datalog_write(str);
}

void communication_init() {
     error["type"] = "error";
     error["time"] = millis();
     error["message"] = "none";

     JsonObject & sensors = telemetry.createNestedObject("sensors");
     sensors.createNestedObject("gyro");
     sensors.createNestedObject("acc");
     sensors.createNestedObject("mag");
     sensors.createNestedObject("bar");
     sensors.createNestedObject("gps");

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

     telemetry["sensors"]["gps"]["hour"] = 0;
     telemetry["sensors"]["gps"]["min"] = 0;
     telemetry["sensors"]["gps"]["sec"] = 0;

     telemetry["sensors"]["gps"]["day"] = 0;
     telemetry["sensors"]["gps"]["mon"] = 0;
     telemetry["sensors"]["gps"]["year"] = 0;

     Serial.begin(9600);
}
