/*
 * Definition file for communication functions
 */

/* LIBRARIES */
#include <Arduino.h>
#include "global.h"
#include "accelerometer.h"
#include "barometer.h"
#include "gps.h"
#include "communication.h"

void sendBase(char type, const char * data, unsigned int len) {
     static union time_data_u {
     char bytes[4];
     float value;
     } time_data;

     time_data.value = millis()/1000.0;

     // Transmit message time type and data
     Serial.write(time_data.bytes, 4);
     Serial.write(type);
     Serial.write(data, len);
}

char recvBase() {
     // Request and read a single char from base station
     if (Serial.available()) {
          return Serial.read();
     }
     else {
          return ' ';
     }
}

void sendDebug() {
     // Send bits
     digitalWrite(PANEL_LATCH, LOW);
     shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, lowByte(debug));
     shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, highByte(debug));
     digitalWrite(PANEL_LATCH, HIGH);
     delay(50);
}

void updateTelemetry() {
     static union data_u {
     char bytes[28];
     float values[7];
     } data;

     readAccelerometer(true);
     readBarometer(true);
     readGps();

     data.values[0] = acc.x;
     data.values[1] = acc.y;
     data.values[2] = acc.z;
     data.values[3] = bar.p;
     data.values[4] = bar.alt;
     data.values[5] = gps.lat;
     data.values[6] = gps.lon;

     sendBase('u', data.bytes, 28);
}
