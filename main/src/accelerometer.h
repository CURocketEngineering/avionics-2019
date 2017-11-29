/*
 * Declaration file for accelerometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include "global.h"

/* DEFINE */
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

     // Define struct type variables
     static struct acc_s {
          float x, y, z;
     } acc, acc_prev;

     /* DEFINE FUNCTION */
     void readAccelerometer(bool filter, bool debug = false, float x = 0, float y = 0, float z = 0);

#endif
