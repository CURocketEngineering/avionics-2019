/*
 * Test v0.1
 *
 * Test program to simulate sensor data for the main computer.
 */

#define PAYLOAD_ADDR 8

#include <avr/pgmspace.h>

#include <Wire.h>

#include "flight.h"

char cmd;
char phs;

unsigned long cur;
unsigned long start;

unsigned long time_cur;
unsigned long acc_x_cur;
unsigned long acc_y_cur;
unsigned long acc_z_cur;
unsigned long bar_p_cur;

void request() {
	Serial.print("Sending: ");
	Serial.println(cmd);
	Wire.write(cmd);
	cmd = 'h';
}

void receive(int bytes) {
	unsigned long time = millis();

	Serial.print("[");
	Serial.print(time / 1000);
	Serial.print(".");
	Serial.print(time % 1000);
	Serial.print("] ");

	if (bytes == 1) {
		Serial.print("Phase: ");

		String phase = "Err";

		phs = Wire.read();

		switch (phs) {
			case 'h':
				phase = "Idle";
				break;
			case 't':
				phase = "Test";
				break;
			case 'p':
				phase = "Test Pass";
				break;
			case 'f':
				phase = "Test Fail";
				break;
			case 'a':
				phase = "Arm";
				break;
			case 'i':
				phase = "Ignite";
				start = millis();
				break;

			case 'b':
				phase = "Burn";
				break;

			case 'c':
				phase = "Coast";
				break;

			case 'd':
				phase = "Apogee";
				break;

			case 'w':
				phase = "Wait";
				break;

			case 'e':
				phase = "Eject";
				break;
		}

		Serial.println(phase);
	}
	else {
		Serial.print("Data: ");

		unsigned int acc_x = Wire.read();
		acc_x <<= 8;
		acc_x |= Wire.read();
		unsigned int acc_y = Wire.read();
		acc_y <<= 8;
		acc_y |= Wire.read();
		unsigned int acc_z = Wire.read();
		acc_z <<= 8;
		acc_z |= Wire.read();
		unsigned int bar_p = Wire.read();
		bar_p <<= 8;
		bar_p |= Wire.read();
		unsigned int bar_alt = Wire.read();
		bar_alt <<= 8;
		bar_alt |= Wire.read();

		Serial.print(acc_x);
		Serial.print(",");
		Serial.print(acc_y);
		Serial.print(",");
		Serial.print(acc_z);
		Serial.print(",");
		Serial.print(bar_p);
		Serial.print(",");
		Serial.print(bar_alt);
	}
}

void setup() {
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	Serial.begin(9600);

	Serial1.begin(9600);
	Serial.println("Waiting on computer...");
	while (!Serial1.available());
	Serial1.read();

	digitalWrite(13, HIGH);

	Wire.begin(PAYLOAD_ADDR);
	Wire.onRequest(request);
	Wire.onReceive(receive);

	cur = 0;
	start = 0;

	time_cur = pgm_read_word_near(time);
	acc_x_cur = pgm_read_word_near(acc_x);
	acc_y_cur = pgm_read_word_near(acc_y);
	acc_z_cur = pgm_read_word_near(acc_z);
	bar_p_cur = pgm_read_word_near(bar_p);
}

void loop() {
	if (Serial.available())
		cmd = Serial.read();

	if (time_cur == 65535)
		return;

	if (phs == 'h' || phs == 't' || phs == 'f' || phs == 'p' || phs == 'a') {
		Serial1.write(acc_x_cur);
		Serial1.write(acc_y_cur);
		Serial1.write(acc_z_cur);
		Serial1.write(bar_p_cur);
	}
	else if (millis() - start > time[cur]) {
		Serial1.write(acc_x_cur);
		Serial1.write(acc_y_cur);
		Serial1.write(acc_z_cur);
		Serial1.write(bar_p_cur);

		cur++;

		time_cur = pgm_read_word_near(time + cur);
		acc_x_cur = pgm_read_word_near(acc_x + cur);
		acc_y_cur = pgm_read_word_near(acc_y + cur);
		acc_z_cur = pgm_read_word_near(acc_z + cur);
		bar_p_cur = pgm_read_word_near(bar_p + cur);
	}
}
