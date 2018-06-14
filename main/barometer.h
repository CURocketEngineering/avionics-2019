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

#define BARO_ADDR 0x77
#define BARO_GAIN 0.9

// Define struct type variables
struct bar_s {
     float p, dp;        // Unit: mb
     float alt, gnd;     // Unit: ft
     float temp;         // Unit: deg C
     float hum;          // Unit: %
};
extern struct bar_s bar, bar_prev;

/* DEFINE FUNCTIONS */
void barometer_init();
void barometer_read(bool filter);

#endif
