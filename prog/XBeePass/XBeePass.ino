/*
 * XBeePass v0.2
 *
 * A program to allow one to communicate with the XBee over the Arduino's
 * serial line. This is generally used to modify the XBee using Digi's
 * software.
 */

#define SS

#ifdef SS
#include <SoftwareSerial.h>

// note: these are the opposite of what is on the dev board
#define XBEE_RX 10
#define XBEE_TX 11

SoftwareSerial xbee(XBEE_RX, XBEE_TX);

#else
#define xbee Serial1
#endif

void setup() {
  Serial.begin(9600);

#ifdef SS
  pinMode(XBEE_RX, INPUT);
  pinMode(XBEE_TX, OUTPUT);

#endif
  xbee.begin(9600);
  xbee.println();
}

void loop() {
  if (Serial.available())
    xbee.write(Serial.read());

  if (xbee.available())
    Serial.write(xbee.read());
}
