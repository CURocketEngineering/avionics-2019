/*
 * Definition file for barometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>
#include "pins.h"
#include "barometer.h"
#include "BME280/SparkFunBME280.h"

BME280 barSensor; //The barometer

void barometer_init() {
     Wire.begin();

     //Default setup code for the BME280 library
     //This is I2C, BME280 also supports SPI
     mySensor.settings.commInterface = I2C_MODE;
     mySensor.settings.I2CAddress = BARO_ADDR;
     //For SPI enable the following and disable the I2C section
     //mySensor.settings.commInterface = SPI_MODE;
     //mySensor.settings.chipSelectPin = 10;
     
     //General Settings
     mySensor.settings.runMode = 3; //Run mode
     mySensor.settings.tStandby = 0; // 0.5ms
     mySensor.settings.filter = 0; //filter off **
     mySensor.settings.tempOverSample = 1;
     mySensor.settings.pressOverSample = 1;
     mySensor.settings.humidOverSample = 1;
     delay(10);
}

void barometer_read(bool filter) {
     static byte buf[3];

     static float pressure, temperature, altitude, humidity; //kpa, c, m, %
     static float temperaturef, altitudeft; //degree f, ft

     pressure = barSensor.readFloatPressure();
     temperature = barSensor.readTempC();
     altitude = barSensor.readFloatAltitudeMeters();
     humidity = barSensor.readFloatHumidity();
     altitudeft = barSensor.readFloatAltitudeFeet();
     temperaturef = barSensor.readTempf();
     
     // Calculate compensated pressure
     bar_prev = bar;
     bar.p = pressure;
     bar.alt = altitude;
     
     if (filter) {
          bar.p = BARO_GAIN*bar.p + (1.0 - BARO_GAIN)*bar_prev.p;
     }

     bar.dp = bar.p - bar_prev.p;
}
