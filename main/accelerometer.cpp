/*
 * Definition file for accelerometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include "accelerometer.h"
#include "pins.h"

void accelerometer_init() {
    // Set inputs and outputs
    pinMode(ACCEL_X, INPUT);
    pinMode(ACCEL_Y, INPUT);
    pinMode(ACCEL_Z, INPUT);
}

void accelerometer_read(bool filter) {
    acc_prev = acc; // For a simple low-pass filter on data to reduce jitter

    acc.x = analogRead(ACCEL_X)*400.0/1023.0 - 200.0;
    acc.y = analogRead(ACCEL_Y)*400.0/1023.0 - 200.0;
    acc.z = analogRead(ACCEL_Z)*400.0/1023.0 - 200.0;

    if (filter) {
        acc.x = ACCEL_GAIN*acc.x + (1.0 - ACCEL_GAIN)*acc_prev.x;
        acc.y = ACCEL_GAIN*acc.y + (1.0 - ACCEL_GAIN)*acc_prev.y;
        acc.z = ACCEL_GAIN*acc.z + (1.0 - ACCEL_GAIN)*acc_prev.z;
    }

    acc_prev = acc;
}
