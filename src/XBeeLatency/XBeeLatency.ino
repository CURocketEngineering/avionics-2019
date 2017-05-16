/*
 * XBeeLatency v0.1
 *
 * A program to test latency to a single radio running XBeeEcho.
 */

#include <SoftwareSerial.h>

// note: these are the opposite of what is on the dev board
#define XBEE_RX 10
#define XBEE_TX 11

#define TIMEOUT 5000

SoftwareSerial xbee(XBEE_RX, XBEE_TX);

void setup() {
  pinMode(XBEE_RX, INPUT);
  pinMode(XBEE_TX, OUTPUT);

  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
  if (Serial.available()) {
    unsigned long startTime, stopTime;

    String message = Serial.readStringUntil('\n');
    Serial.print("Echo: ");
    Serial.println(message);

    while (xbee.available())
      xbee.read();

    startTime = millis();

    xbee.println(message);

    while (!xbee.available()) {
      if (millis() - startTime > TIMEOUT) {
        Serial.println("ERROR: Timeout");
        return;
      }
    }

    message = xbee.readStringUntil('\n');

    stopTime = millis();

    Serial.print("Rply: ");
    Serial.print(message);
    Serial.print(" (");
    Serial.print(stopTime - startTime);
    Serial.println(" ms)");
  }
}
