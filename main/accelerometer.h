/*
 * Declaration file for accelerometer
 */

/* DEFINE */
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

/* LIBRARIES */
#include <Arduino.h>
#include "global.h"

// Gain value for sensor filter
#define ACCEL_GAIN 0.2

// Define struct type variables
static struct acc_s {
     float x, y, z;
} acc, acc_prev;

/* DEFINE FUNCTION */
void accelerometer_read(bool filter);

#endif
