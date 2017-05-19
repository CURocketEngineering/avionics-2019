/*
 * Main v0.1
 *
 * Main computer logic handling: button inputs, debug outputs, sensor inputs,
 * ignition, parachute ejection, and telemetry. Telemetry is communicated to
 * the payload computer for relay over the radios on a best effort basis.
 * Everything else is a critical function.
 */

// definitions

#define ACCEL_X -1
#define ACCEL_Y -1
#define ACCEL_Z -1

#define BARO_MOSI -1
#define BARO_MISO -1
#define BARO_SCK -1

#define CTRL -1

#define PAYLOAD_SCL -1
#define PAYLOAD_SDA -1

#define PANEL_CLOCK -1
#define PANEL_DATA -1
#define PANEL_LATCH -1

#define TERM_MAIN -1
#define TERM_DROGUE -1
#define TERM_IGNITE -1

#define PAYLOAD_ADDR 1

// libraries

#include <avr/sleep.h>

#include <SPI.h>
#include <SoftSPI.h>
#include <Wire.h>

// global data

static enum {
	INIT,
	IDLE,
	HALT,
	TEST,
	ARM,
	IGNITE,
	BURN,
	COAST,
	EJECT,
	FALL,
	RECOVER
} state;

static struct {
	// 0 -> -200 g, 1023 -> 200 g
	unsigned int x, y, z;
} acc;

static struct {
	// 0 -> 50 kPa, 1023 -> 115 kPa
	unsigned int p;
} bar;

// global variables

SoftSPI barometer(BARO_MOSI, BARO_MISO, BARO_SCK);

// sensor functions

void readAccelerometer() {
	acc.x = analogRead(ACCEL_X);
	acc.y = analogRead(ACCEL_Y);
	acc.z = analogRead(ACCEL_Z);
}

void barometerWrite(byte address, byte data) {
	// write mode
	address &= 0x7F;

	barometer.transfer(address);
	barometer.transfer(data);
}

unsigned short barometerRead(byte address) {
	// read mode
	address |= 0x80;

	unsigned short value = barometer.transfer(address);
	value <<= 8;
	value |= barometer.transfer(address + 0x02);

	return value;
}

void readBarometer() {
	// start read
	barometerWrite(0x24, 0x00);
	delay(10);

	// get pressure and temperature
	unsigned short pressure = barometerRead(0x00) >> 6;
	unsigned short temperature = barometerRead(0x04) >> 6;

	// get coefficients
	unsigned short a0 = barometerRead(0x08);
	unsigned short b1 = barometerRead(0x0c);
	unsigned short b2 = barometerRead(0x10);
	unsigned short c12 = barometerRead(0x14);
	unsigned short c11 = barometerRead(0x18);
	unsigned short c22 = barometerRead(0x1c);

	// calculate compensated pressure
	bar.p = a0 + (b1 + c11*pressure + c12*temperature)*pressure + (b2 + c22*temperature)*temperature;
}

// communication functions

void sendPayload(char type, long data) {
	// transmit message type and 32-bit data
	Wire.beginTransmission(PAYLOAD_ADDR);
	Wire.write(type);
	Wire.write((char *)(&data), 4);
	Wire.endTransmission();
}

char recvPayload() {
	// request and read a single char from payload
	Wire.requestFrom(PAYLOAD_ADDR, 1);
	return Wire.read();
}

// state functions

void idle() {
}

void halt() {
	// clear interrupts and put processor to sleep
	cli();
	sleep_enable();
	sleep_cpu();
}

void test() {
}

void arm() {
}

void ignite() {
}

void burn() {
}

void coast() {
}

void eject() {
}

void fall() {
}

void recover() {
}

// program functions

void setup() {
	// set inputs and outputs
	pinMode(ACCEL_X, INPUT);
	pinMode(ACCEL_Y, INPUT);
	pinMode(ACCEL_Z, INPUT);

	pinMode(BARO_MOSI, OUTPUT);
	pinMode(BARO_MISO, INPUT);
	pinMode(BARO_SCK, OUTPUT);

	pinMode(CTRL, INPUT_PULLUP);

	pinMode(PANEL_CLOCK, OUTPUT);
	pinMode(PANEL_DATA, OUTPUT);
	pinMode(PANEL_LATCH, OUTPUT);

	pinMode(TERM_MAIN, OUTPUT);
	pinMode(TERM_DROGUE, OUTPUT);
	pinMode(TERM_IGNITE, OUTPUT);

	// initialize communication with the barometer
	barometer.begin();

	// initialize communication with the payload
	Wire.begin();

	// set initial state
	state = INIT;
}

void loop() {
	// run appropriate state function
	switch (state) {
		// go to idle from init state
		case INIT:
			state = IDLE;
			break;

		case IDLE:
			idle();
			break;

		case HALT:
			halt();
			break;

		case TEST:
			test();
			break;

		case ARM:
			arm();
			break;

		case IGNITE:
			ignite();
			break;

		case BURN:
			burn();
			break;

		case COAST:
			coast();
			break;

		case EJECT:
			eject();
			break;

		case FALL:
			fall();
			break;

		case RECOVER:
			recover();
			break;

		// halt program in invalid state
		default:
			state = HALT;
	}
}
