/*
 * Definition file for accelerometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "global.h"
#include "gps.h"

SoftwareSerial gpscomm(GPS_RX, GPS_TX);

const char * gps_getField(const char * sentence, char * buf, unsigned short len) {
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

void gps_readLine(Stream & stream, char * buf, unsigned short len, bool crlf) {
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

void gps_init() {
     gpscomm.begin(9600);
}

void gps_read() {
     static char sentence[80];
     static char field[20];

     while (gpscomm.available()) {
          const char * ptr = sentence;

          gps_readLine(gpscomm, sentence, sizeof(sentence));

          // ID
          ptr = gps_getField(ptr, field, sizeof(field));

          if (strcmp(field, "$GPRMC") == 0) {
               // Time
               ptr = gps_getField(ptr, field, sizeof(field));

               // Status
               ptr = gps_getField(ptr, field, sizeof(field));

               // Latitude
               ptr = gps_getField(ptr, field, sizeof(field));
               gps.lat = atof(field);

               // n/s
               ptr = gps_getField(ptr, field, sizeof(field));
               if (field[0] == 'S') {
                    gps.lat = -gps.lat;
               }

               // Longitude
               ptr = gps_getField(ptr, field, sizeof(field));
               gps.lon = atof(field);

               // e/w
               ptr = gps_getField(ptr, field, sizeof(field));
               if (field[0] == 'W') {
                    gps.lon = -gps.lon;
               }
          }
     }
}
