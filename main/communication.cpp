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

void communication_send(char type, const char * data, unsigned int len) {
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

char communication_recv() {
     // Request and read a single char from base station
     if (Serial.available()) {
          return Serial.read();
     }
     else {
          return ' ';
     }
}

void communication_writeDebug() {
     // Send bits
     digitalWrite(PANEL_LATCH, LOW);
     shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, lowByte(debug));
     shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, highByte(debug));
     digitalWrite(PANEL_LATCH, HIGH);
     delay(50);
}

void communication_updateTelemetry() {
     static union data_u {
          char bytes[28];
          float values[7];
     } data;

     accelerometer_read(true);
     barometer_read(true);
     gps_read();

     data.values[0] = acc.x;
     data.values[1] = acc.y;
     data.values[2] = acc.z;
     data.values[3] = bar.p;
     data.values[4] = bar.alt;
     data.values[5] = gps.lat;
     data.values[6] = gps.lon;

     communication_send('u', data.bytes, 28);
}

void communication_init() {
     pinMode(PANEL_CLOCK, OUTPUT);
     pinMode(PANEL_DATA, OUTPUT);
     pinMode(PANEL_LATCH, OUTPUT);
     digitalWrite(PANEL_LATCH, HIGH);
     delay(50);

     Serial.begin(9600);
}
