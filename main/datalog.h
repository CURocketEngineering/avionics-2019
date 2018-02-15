/*
 * Declaration file for datalogger
 */

/* DEFINE */
#ifndef DATALOG_H_
#define DATALOG_H_

/* LIBRARIES */
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "libraries/ArduinoJson.h"

#include "pins.h"

// Modified from https://github.com/PaulStoffregen/SD/blob/master/examples/Datalogger/Datalogger.ino

// SD module
const int chipSelect = BUILTIN_SDCARD;

/* SETTING UP JSON */
// Allocating buffer
DynamicJsonBuffer jsonBuffer;

// Creating object trees
// May have to place elsewhere (in a cpp for example)
JsonObject& acc_json = jsonBuffer.createObject();
JsonObject& baro_json = jsonBuffer.createObject();
JsonObject& gps_json = jsonBuffer.createObject();
JsonObject& ninedof_json = jsonBuffer.createObject();
JsonArray& gyro_data = ninedof_json.createNestedArray("gyro");
JsonArray& accel_data = ninedof_json.createNestedArray("accel");
JsonArray& mag_data = ninedof_json.createNestedArray("mag");
JsonArray& attitude_data = ninedof_json.createNestedArray("attitude");

/* DEFINE FUNCTIONS */
void datalog_setup();
void datalog_print();

#endif
