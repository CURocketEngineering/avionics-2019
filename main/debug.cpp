/*
 * Definition file for accelerometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include "debug.h"
#include "pins.h"

//                      1  2  3  4  A
//                     GBRGBRGBRGBRBXXX
unsigned int debug = 0b1111111111111111;

void debug_init() {
     pinMode(PANEL_CLOCK, OUTPUT);
     pinMode(PANEL_DATA, OUTPUT);
     pinMode(PANEL_LATCH, OUTPUT);
     digitalWrite(PANEL_LATCH, HIGH);
     delay(50);
}

void debug_write() {
     // Send bits
     digitalWrite(PANEL_LATCH, LOW);
     shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, lowByte(debug));
     shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, highByte(debug));
     digitalWrite(PANEL_LATCH, HIGH);
     delay(50);
}
