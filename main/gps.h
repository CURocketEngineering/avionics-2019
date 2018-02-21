/*
 * Declaration file for GPS
 */

/* DEFINE */
#ifndef GPS_H_
#define GPS_H_

/* LIBRARIES */
#include <Arduino.h>
#include <SoftwareSerial.h>

// GPS pins
#define GPS_RX 8
#define GPS_TX 9

extern SoftwareSerial gpscomm;

// Define struct type variables
static struct gps_s {
     float lat, lon; // Unit: degrees
} gps;

/* DEFINE FUNCTIONS */
void gps_init();
void gps_read();

#endif
