#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "datalog.h"
#include "ninedof.h"
#include "pins.h"

DynamicJsonBuffer jsonBuffer;

void datalog_setup() {
    acc_json = jsonBuffer.createObject();
    baro_json = jsonBuffer.createObject();
    gps_json = jsonBuffer.createObject();
    ninedof_json = jsonBuffer.createObject();

    gyro_data = ninedof_json.createNestedArray("gyro");
    accel_data = ninedof_json.createNestedArray("accel");
    mag_data = ninedof_json.createNestedArray("mag");
    attitude_data = ninedof_json.createNestedArray("attitude");

    SD.begin(BUILTIN_SDCARD);
}

void datalog_print() {
    File file = SD.open("box.json", FILE_WRITE);

    String str;

    ninedof_json.printTo(str);
    file.println(str);

    file.close();
}
