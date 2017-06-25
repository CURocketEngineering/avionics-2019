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
	while (!Serial1.available());
	Serial1.read();
	Serial.println("INIT");

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

		char type = Serial2.read();

		if (type == 's') {
			phs = Serial2.read();
		}
		else if (type == 'u') {
			for (byte idx = 0; idx < 20; idx++)
				data.bytes[idx] = Serial2.read();
		}
		else {
			phs = '%';
		}

		Serial.print("{'time': ");
		Serial.print(time);
		Serial.print(", 'payload': {'sensor': {'acc': {'x': ");
		Serial.print(stream_data.values[0]);
		Serial.print(", 'y': ");
		Serial.print(stream_data.values[1]);
		Serial.print(", 'z': ");
		Serial.print(stream_data.values[2]);
		Serial.print("}, 'bar': {'p': ");
		Serial.print(stream_data.values[3]);
		Serial.print(", 'alt': ");
		Serial.print(data.values[4]);
		Serial.print("}, 'gps': {'lat': 0.00, 'lon': 0.00}, 'mag': {'x': 0.00, 'y': 0.00, 'z': 0.00}}}, 'main': {'state': ");
		Serial.print(phs);
		Serial.print(", 'sensor': {'acc': {'x': ");
		Serial.print(data.values[0]);
		Serial.print(", 'y': ");
		Serial.print(data.values[1]);
		Serial.print(", 'z': ");
		Serial.print(data.values[2]);
		Serial.print("}, 'bar': {'p': ");
		Serial.print(data.values[3]);
		Serial.print(", 'alt': ");
		Serial.print(data.values[4]);
		Serial.print("}}}}");
	}
}
