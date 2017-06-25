/*
 * XBeeChat v0.1
 *
 * A simple radio chat using the serial monitor.
 */

#include <SoftwareSerial.h>

// note: these are the opposite of what is on the dev board
#define XBEE_RX 10
#define XBEE_TX 11

SoftwareSerial xbee(XBEE_RX, XBEE_TX);

void setup() {
  pinMode(XBEE_RX, INPUT);
  pinMode(XBEE_TX, OUTPUT);

  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    Serial.print("Local: ");
    Serial.println(message);
    xbee.println(message);
  }

  if (xbee.available()) {
    String message = xbee.readStringUntil('\n');
    Serial.print("Remote: ");
    Serial.println(message);
  }
}
