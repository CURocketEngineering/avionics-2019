/*
 * Main v0.1
 *
 * Main computer logic handling: button inputs, debug outputs, sensor inputs,
 * ignition, parachute ejection, and telemetry. Telemetry is communicated to
 * the payload computer for relay over the radios on a best effort basis.
 * Everything else is a critical function.
 */

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

#include <SPI.h>
#include <SoftSPI.h>

struct {
	unsigned int x, y, z;
} acc;

struct {
	unsigned int p;
} bar;

SoftSPI barometer(BARO_MOSI, BARO_MISO, BARO_SCK);

void readAccelerometer() {
	acc.x = map(analogRead(ACCEL_X), 0, 1023, -200, 200);
	acc.y = map(analogRead(ACCEL_Y), 0, 1023, -200, 200);
	acc.z = map(analogRead(ACCEL_Z), 0, 1023, -200, 200);
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
	unsigned short c22 = barometerRead(0x1c);

	// calculate compensated pressure
	bar.p = a0 + (b1 + c11*pressure + c12*temperature)*pressure + (b2 + c22*temperature)*temperature;
}

void setup() {
	barometer.begin();
}

void loop() {
}
