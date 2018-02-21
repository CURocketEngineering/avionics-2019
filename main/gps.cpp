/*
 * Definition file for accelerometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "pins.h"
#include "gps.h"
#include "util.h"

SoftwareSerial gpscomm(GPS_RX, GPS_TX);

void gps_init() {
     gpscomm.begin(9600);
}

void gps_read() {
     static char sentence[80];
     static char field[20];

     while (gpscomm.available()) {
          const char * ptr = sentence;

          util_readLine(gpscomm, sentence, sizeof(sentence));

          // ID
          ptr = util_getField(ptr, field, sizeof(field));

          if (strcmp(field, "$GPRMC") == 0) {
               // Time
               ptr = util_getField(ptr, field, sizeof(field));

               // Status
               ptr = util_getField(ptr, field, sizeof(field));

               // Latitude
               ptr = util_getField(ptr, field, sizeof(field));
               gps.lat = atof(field);

               // n/s
               ptr = util_getField(ptr, field, sizeof(field));
               if (field[0] == 'S') {
                    gps.lat = -gps.lat;
               }

               // Longitude
               ptr = util_getField(ptr, field, sizeof(field));
               gps.lon = atof(field);

               // e/w
               ptr = util_getField(ptr, field, sizeof(field));
               if (field[0] == 'W') {
                    gps.lon = -gps.lon;
               }
          }
     }
}
