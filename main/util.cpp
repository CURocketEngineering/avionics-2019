/*
 * Definition file for barometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

#include "util.h"

void util_readLine(Stream & stream, char * buf, unsigned short len, bool crlf) {
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

const char * util_getField(const char * sentence, char * buf, unsigned short len) {
     unsigned short idx = 0;

     while (idx < len - 1) {
          if (*sentence == ',') { sentence++; break;
          }

          buf[idx] = *sentence;
          sentence++;
          idx++;
     }

     buf[idx] = '\0';

     return sentence;
}
