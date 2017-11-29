/*
 * Definition file for accelerometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include "global.h"
#include "gps.h"

SoftwareSerial gpscomm(GPS_RX, GPS_TX);

const char * gpsGetField(const char * sentence, char * buf, unsigned short len) {
     unsigned short idx = 0;

     while (idx < len - 1) {
          if (*sentence == ',') {
               sentence++;
               break;
          }

          buf[idx] = *sentence;
          sentence++;
          idx++;
     }

     buf[idx] = '\0';

     return sentence;
}

void gpsReadLine(Stream & stream, char * buf, unsigned short len, bool crlf) {
     unsigned short idx = 0;
     int c;

     while (idx < len - 1) {
          while(!stream.available());

          c = stream.read();

          if (crlf) {
               if (c == '\r') {
                    continue;
               }
          }

          if (c == '\n') {
               break;
          }

          buf[idx] = c;
          idx++;
     }
     buf[idx] = '\0';
}

void readGps(bool debug, float lat, float lon) {
     static char sentence[80];
     static char field[20];

     if (!debug) {
          while (gpscomm.available()) {
               const char * ptr = sentence;

               gpsReadLine(gpscomm, sentence, sizeof(sentence));

               // ID
               ptr = gpsGetField(ptr, field, sizeof(field));

               if (strcmp(field, "$GPRMC") == 0) {
                    // Time
                    ptr = gpsGetField(ptr, field, sizeof(field));

                    // Status
                    ptr = gpsGetField(ptr, field, sizeof(field));

                    // Latitude
                    ptr = gpsGetField(ptr, field, sizeof(field));
                    gps.lat = atof(field);

                    // n/s
                    ptr = gpsGetField(ptr, field, sizeof(field));
                    if (field[0] == 'S') {
                    gps.lat = -gps.lat;
                    }

                    // Longitude
                    ptr = gpsGetField(ptr, field, sizeof(field));
                    gps.lon = atof(field);

                    // e/w
                    ptr = gpsGetField(ptr, field, sizeof(field));
                    if (field[0] == 'W') {
                    gps.lon = -gps.lon;
                    }
               }
          }
     }
     else {
          gps.lat = lat;
          gps.lon = lon;
     }
}
