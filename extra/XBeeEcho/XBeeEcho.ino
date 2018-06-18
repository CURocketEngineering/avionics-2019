/*
 * XBeeEcho v0.1
 *
 * A program that repeats anything received over the radio.
 */

//#define SS
#ifdef SS
#include <SoftwareSerial.h>

// note: these are the opposite of what is on the dev board
#define XBEE_RX 10
#define XBEE_TX 11

SoftwareSerial xbee(XBEE_RX, XBEE_TX);

#else
#define xbee Serial2
#endif
void setup() {
#ifdef SS
  pinMode(XBEE_RX, INPUT);
  pinMode(XBEE_TX, OUTPUT);

#endif
  xbee.begin(115200);
}

void loop() {
  if (xbee.available()) {
    String message = xbee.readStringUntil('\n');
    xbee.println(message);
  }
}
