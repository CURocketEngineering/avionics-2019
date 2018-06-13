/*
 * XBeeLatency v0.1
 *
 * A program to test latency to a single radio running XBeeEcho.
 */

#ifdef SS
#include <SoftwareSerial.h>

// note: these are the opposite of what is on the dev board
#define XBEE_RX 10
#define XBEE_TX 11

SoftwareSerial xbee(XBEE_RX, XBEE_TX);
#else
#define xbee Serial1
#endif

#define TIMEOUT 1000
#define ATTEMPTS 5

void setup() {
#ifdef SS
  pinMode(XBEE_RX, INPUT);
  pinMode(XBEE_TX, OUTPUT);
#endif

  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
  if (Serial.available()) {
    byte attempt = 0;

    unsigned long startTime, stopTime;

    String message = Serial.readStringUntil('\n');
    Serial.print("Echo: ");
    Serial.println(message);

    while (xbee.available())
      xbee.read();

    while (attempt < ATTEMPTS) {
      startTime = millis();

      xbee.println(message);

      while (!xbee.available()) {
        if (millis() - startTime > TIMEOUT)
          break;
      }

      if (xbee.available())
        break;

      attempt++;
    }

    if (xbee.available()) {
      message = xbee.readStringUntil('\n');

      stopTime = millis();

      Serial.print("Rply: ");
      Serial.print(message);
      Serial.print(" (");
      Serial.print(stopTime - startTime);
      if (attempt > 1) {
        Serial.print(" ms) (");
        Serial.print(attempt);
        Serial.println(" attempts)");
      }
      else {
        Serial.println(" ms)");
      }
    }
    else {
      Serial.println("Fail: No Reply");
    }
  }
}
