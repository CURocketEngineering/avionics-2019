/*
 * Declaration file for GPS
 */

/* DEFINE */
#ifndef GPS_H_
#define GPS_H_

/* LIBRARIES */
#include <Arduino.h>

// Define struct type variables
static struct gps_s {
     float lat, lon; // Unit: degrees
     unsigned short hour, min, sec;
     unsigned short day, mon, year;
} gps;

/* DEFINE FUNCTIONS */
void gps_init();
void gps_read();

#endif
