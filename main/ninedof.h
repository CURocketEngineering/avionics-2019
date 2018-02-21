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

// Define struct type variables
static struct acc_s {
     float x, y, z;
} acc, acc_prev;

static struct gyro_s {
    float x, y, z;
} gyro, gyro_prev;

static struct gyro_s {
    float x, y, z;
} mag;

static struct att_s {
    float r, p, y;
} att;

void ninedof_init();
void ninedof_read();

#endif
