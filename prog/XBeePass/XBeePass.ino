/*
 * XBeePass v0.2
 *
 * A program to allow one to communicate with the XBee over the Arduino's
 * serial line. This is generally used to modify the XBee using Digi's
 * software.
 */

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if (Serial.available())
    Serial1.write(Serial.read());

  if (Serial1.available())
    Serial.write(Serial1.read());
}
