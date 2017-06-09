/*
 * Test v0.1
 *
 * Test program to simulate sensor data for the main computer.
 */

#include <Wire.h>

#include "flight.h"

char cmd;
char phs;
unsigned long cur;
unsigned long start;

void request() {
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
	Serial.begin(9600);
	Serial1.begin(9600);

	while (!Serial1.available());
	Serial1.read();

	Wire.begin(1);
	Wire.onRequest(request);
	Wire.onReceive(receive);

	cur = 0;
	start = 0;
}

void loop() {
	if (Serial.available())
		cmd = Serial.read();

	if (phs == 'h' || phs == 't' || phs == 'f' || phs == 'p' || phs == 'a') {
		Serial1.write(acc_x[cur]);
		Serial1.write(acc_y[cur]);
		Serial1.write(acc_z[cur]);
		Serial1.write(bar_p[cur]);
	}
	else if (millis() - start > time[cur]) {
		Serial1.write(acc_x[cur]);
		Serial1.write(acc_y[cur]);
		Serial1.write(acc_z[cur]);
		Serial1.write(bar_p[cur]);

		cur++;
	}
}
