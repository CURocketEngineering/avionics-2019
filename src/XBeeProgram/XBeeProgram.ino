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
  if (Serial.available())
    xbee.write(Serial.read());

  if (xbee.available())
    Serial.write(xbee.read());
}

