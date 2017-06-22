/*
 * Test v0.1
 *
 * Test program to simulate sensor data for the main computer.
 */

#include <avr/pgmspace.h>

#include "flight.h"

char cmd;
char phs;

unsigned long cur;
unsigned long start;

unsigned short time_cur;

union data_u {
	char bytes[20];
	float values[5];
} data;

union stream_data_u {
	char bytes[16];
	float values[4];
} stream_data;

void receive(int bytes) {
}

void setup() {
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	Serial.begin(9600);

	Serial1.begin(9600);
	Serial.println("Waiting on computer...");
	while (!Serial1.available());
	Serial1.read();
	Serial.println("Computer connected.");

	Serial2.begin(9600);

	digitalWrite(13, HIGH);

	cur = 0;
	start = 0;

	time_cur = pgm_read_word_near(time);
	stream_data.values[0] = pgm_read_float_near(acc_x);
	stream_data.values[1] = pgm_read_float_near(acc_y);
	stream_data.values[2] = pgm_read_float_near(acc_z);
	stream_data.values[3] = pgm_read_float_near(bar_p);
}

void loop() {
	if (Serial.available())
		cmd = Serial.read();

	if (time_cur == 65535)
		return;

	if (phs == 'h' || phs == 't' || phs == 'f' || phs == 'p' || phs == 'a') {
		for (byte idx = 0; idx < 16; idx++)
			Serial1.write(stream_data.bytes[idx]);
	}
	else if (millis() - start > time[cur]) {
		for (byte idx = 0; idx < 16; idx++)
			Serial1.write(stream_data.bytes[idx]);

		cur++;

		time_cur = pgm_read_word_near(time + cur*sizeof(unsigned short));
		stream_data.values[0] = pgm_read_float_near(acc_x + cur*sizeof(float));
		stream_data.values[1] = pgm_read_float_near(acc_y + cur*sizeof(float));
		stream_data.values[2] = pgm_read_float_near(acc_z + cur*sizeof(float));
		stream_data.values[3] = pgm_read_float_near(bar_p + cur*sizeof(float));
	}

	Serial2.write(cmd);
	cmd = 'h';

	if (Serial2.available()) {
		unsigned long time = millis();

		Serial.print("[");
		Serial.print(time / 1000);
		Serial.print(".");
		Serial.print(time % 1000);
		Serial.print("] ");

		char type = Serial2.read();

		if (type == 's') {
			Serial.print("Phase: ");

			String phase = "Err";

			phs = Serial2.read();

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
		else if (type == 'u') {
			Serial.print("Data: ");

			for (byte idx = 0; idx < 20; idx++)
				data.bytes[idx] = Serial.read();

			Serial.print(data.values[0]);
			Serial.print(",");
			Serial.print(data.values[1]);
			Serial.print(",");
			Serial.print(data.values[2]);
			Serial.print(",");
			Serial.print(data.values[3]);
			Serial.print(",");
			Serial.println(data.values[4]);
		}
		else {
			Serial.print("Unknown Type: ");
			Serial.println(type);
		}
	}
}
