/*
 * Definition file for barometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

#include "pins.h"
#include "barometer.h"

BME280 barSensor; //The barometer

void barometer_init() {
     Wire.begin();

     //Default setup code for the BME280 library
     //This is I2C, BME280 also supports SPI
     barSensor.settings.commInterface = I2C_MODE;
     barSensor.settings.I2CAddress = BARO_ADDR;
     //For SPI enable the following and dissable the I2C section
     //barSensor.settings.commInterface = SPI_MODE;
     //barSensor.settings.chipSelectPin = 10;

     //General Settings
     barSensor.settings.runMode = 3; //Run mode
     barSensor.settings.tStandby = 0; // 0.5ms
     barSensor.settings.filter = 0; //filter off **

     barSensor.settings.tempOverSample = 1;
     barSensor.settings.pressOverSample = 1;

     barSensor.settings.humidOverSample = 1;
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
     temperaturef = barSensor.readTempF();


     // Calculate compensated pressure **
     bar_prev = bar;
     bar.p = pressure;
     bar.alt = altitude;

     if (filter) {
          bar.p = BARO_GAIN*bar.p + (1.0 - BARO_GAIN)*bar_prev.p;
     }

     bar.dp = bar.p - bar_prev.p;
}
