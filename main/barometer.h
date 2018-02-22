/*
 * Declaration file for barometer
 */

/* DEFINE */
#ifndef BAROMETER_H_
#define BAROMETER_H_

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

#include "bme280.h"

#include "pins.h"

// National Weather Service altimeter reading (http://www.weather.gov/)
#define NWS_ALTI 1013.25
// Pin
#define BARO_ADDR 0x77
// Gain value for sensor filter
#define BARO_GAIN 0.2

// Define struct type variables
static struct bar_s {
     float p, dp;        // Unit: mb
     float alt, gnd;     // Unit: ft
     float temp;         // Unit: deg C
     float hum;          // Unit: %
} bar, bar_prev;

/* DEFINE FUNCTIONS */
void barometer_init();
void barometer_read(bool filter);

#endif
