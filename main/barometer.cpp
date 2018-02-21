/*
 * Definition file for barometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

#include "pins.h"
#include "barometer.h"

BME280 barometer;

void barometer_init() {
     Wire.begin();

     barometer.settings.commInterface = I2C_MODE;
     barometer.settings.I2CAddress = BARO_ADDR;
     // SPI mode
     //barometer.settings.commInterface = SPI_MODE;
     //barometer.settings.chipSelectPin = 10;

     barometer.settings.runMode = 3; // run mode
     barometer.settings.tStandby = 0; // 0.5ms
     barometer.settings.filter = 0; // filter off **

     barometer.settings.tempOverSample = 1;
     barometer.settings.pressOverSample = 1;

     barometer.settings.humidOverSample = 1;

     delay(10);
}

void barometer_read(bool filter) {
     bar_prev = bar;

     bar.p = barometer.readFloatPressure();
     bar.alt = barometer.readFloatAltitudeMeters();
     bar.t = barometer.readTempC();
     bar.h = barometer.readFloatHumidity();

     if (filter) {
          bar.p = BARO_GAIN*bar.p + (1.0 - BARO_GAIN)*bar_prev.p;
     }

     bar.dp = bar.p - bar_prev.p;
}
