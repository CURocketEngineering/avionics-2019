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

#include "lsm9ds1.h"

// Declination (http://www.ngdc.noaa.gov/geomag-web/#declination)
#define DECLINATION 6.21

#define EPSILON 0.01

#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

#define GYRO_GAIN 0.2
#define ACC_GAIN 0.2
#define MAG_GAIN 0.2

// Define struct type variables
struct acc_s {
     float x, y, z;
};
extern struct acc_s acc, acc_prev;

struct gyro_s {
    float x, y, z;
};
extern struct gyro_s gyro, gyro_prev;

struct mag_s {
    float x, y, z;
};
extern struct mag_s mag, mag_prev;

struct att_s {
    float r, p, y;
};
extern struct att_s att, att_prev;

void ninedof_init();
void ninedof_read(bool filter);

#endif
