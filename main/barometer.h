/*
 * Declaration file for barometer
 */

/* DEFINE */
#ifndef BAROMETER_H_
#define BAROMETER_H_

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>
#include "pins.h"
#include "BME280/SparkFunBME280.h"

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
} bar, bar_prev;
static struct bar_init_s {
     // Coefficients
n     short AC1, AC2, AC3, VB1, VB2, MB, MC, MD;
     unsigned short AC4, AC5, AC6;
     float c3, c4, b1, c5, c6, mc, md, x0, x1, x2, y0, y1, y2, p0, p1, p2;
} bar_init;

/* DEFINE FUNCTIONS */
void barometer_writeByte(byte address, byte data);
void barometer_readBytes(byte address, byte * buf, unsigned short len);
short barometer_readInt(byte address);
unsigned short barometer_readUint(byte address);
void barometer_init();
void barometer_read(bool filter);

#endif
