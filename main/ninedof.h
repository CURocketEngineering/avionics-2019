/*
 * Declaration file for nine degrees of freedom sensor
 */

/* DEFINE */
#ifndef NINEDOF_H_
#define NINEDOF_H_

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "libraries/LSM9DS1/SparkfunLSM9DS1.h"

// Create LSM9DS1 class
LSM9DS1 imu;

// I2C setup
#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

// Define struct type variables
static struct gyros {
    float x, /*y,*/ z;
} gyro_prev, gyro_dev, gyro_dev_prev; // dev = deviation & prev = previous

// Declination found using NGDC tool
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION 6.21    // Clemson, SC

/* DEFINE FUNCTIONS */
void ninedof_setup();
void ninedof_readData();
void ninedof_printData();
void ninedof_calcAttitude(float ax, float ay, float az,
                          float mx, float my, float mz);
float ninedof_calcDeviationX(float gx);     // I don't know how to use arrays with functions in C++
float ninedof_calcDeviationZ(float gz);     // Sorry

#endif
