/*
 * Declaration file for GPS
 */

/* LIBRARIES */
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "global.h"

/* DEFINE */
#ifndef GPS_H_
#define GPS_H_

     // GPS pins
     #define GPS_RX 8
     #define GPS_TX 9
     
     extern SoftwareSerial gpscomm;

     // Define struct type variables
     static struct gps_s {
          float lat, lon; // Unit: degrees
     } gps;

     /* DEFINE FUNCTIONS */
     const char * gpsGetField(const char * sentence, char * buf, unsigned short len);
     void gpsReadLine(Stream & stream, char * buf, unsigned short len, bool crlf = true);
     void readGps(bool debug = false, float lat = 0, float lon = 0);

#endif
