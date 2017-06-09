/*
 * Main v0.1
 *
 * Main computer logic handling: button inputs, debug outputs, sensor inputs,
 * ignition, parachute ejection, and telemetry. Telemetry and commands are
 * communicated with the payload computer for relay over the radios on a best
 * effort basis. Everything else is a critical function.
 */

// definitions

// must be set before flight from Nation Weather Service altimeter reading
#define NWS_ALTI 30

// pins
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

// I²C address of payload
#define PAYLOAD_ADDR 1

// gain values for sensor filter
#define ACCEL_GAIN 0.2
#define BARO_GAIN 0.2

// time to hold pin high for parachute charge
#define DELAY_PARACHUTE 1000

// time before allowing the rocket to be controlled from ignite to halt
#define MAX_IGNITE 30000

// accelerometer values to determine changes in state
#define MIN_ACCEL 514
#define THRUST_ACCEL 512

// barometer values to determine changes in state
#define APOGEE_DPRES 1
#define MIN_DPRES 1

// altitude to deploy main parachute
#define MAIN_ALT 2000

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
	APOGEE,
	WAIT,
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
	unsigned int p, dp;
	// ft
	unsigned int alt;
	unsigned int gnd;
} bar;

//                      1  2  3  4  A
//                     GBRGBRGBRGBRBXXX
unsigned int debug = 0b0000000000000000;

SoftSPI barometer(BARO_MOSI, BARO_MISO, BARO_SCK);

// sensor functions

void readAccelerometer() {
	acc.x = ACCEL_GAIN*analogRead(ACCEL_X) + (1.0 - ACCEL_GAIN)*acc.x;
	acc.y = ACCEL_GAIN*analogRead(ACCEL_Y) + (1.0 - ACCEL_GAIN)*acc.y;
	acc.z = ACCEL_GAIN*analogRead(ACCEL_Z) + (1.0 - ACCEL_GAIN)*acc.z;
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
	unsigned int p = a0 + (b1 + c11*pressure + c12*temperature)*pressure + (b2 + c22*temperature)*temperature;

	bar.dp = p - bar.p;
	bar.p = BARO_GAIN*p + (1.0 - BARO_GAIN)*bar.p;

	// calculate altitude
	//                                                           inHg/kPa
	bar.alt = (1 - pow(map(bar.p, 0, 1023, 50, 115) / NWS_ALTI / 0.295299830714, (1 / 5.25587611))) / 0.0000068756;
}

// communication functions

void sendPayload(char type, const char * data, unsigned int len) {
	// transmit message type and data
	Wire.beginTransmission(PAYLOAD_ADDR);
	Wire.write(type);
	Wire.write(data, len);
	Wire.endTransmission();
}

char recvPayload() {
	// request and read a single char from payload
	Wire.requestFrom(PAYLOAD_ADDR, 1);
	if (Wire.available())
		return Wire.read();
	else
		return ' ';
}

void sendDebug() {
	// send bits
	shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, debug | 0xff);
	shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, debug >> 8);

	// update output
	digitalWrite(PANEL_LATCH, LOW);
	delay(10);
	digitalWrite(PANEL_LATCH, HIGH);
}

void updateTelemetry() {
	static char data[10];

	readAccelerometer();
	readBarometer();

	data[0] = acc.x >> 8;
	data[1] = acc.x | 0xff;
	data[2] = acc.y >> 8;
	data[3] = acc.y | 0xff;
	data[4] = acc.z >> 8;
	data[5] = acc.z | 0xff;
	data[6] = bar.p >> 8;
	data[7] = bar.p | 0xff;
	data[8] = bar.alt >> 8;
	data[9] = bar.alt | 0xff;

	sendPayload('u', data, 10);
}

// state functions

void idle() {
	// update payload state
	sendPayload('s', "h", 1);

	while (state == IDLE) {
		// check on ctrl term
		if (digitalRead(CTRL) == LOW) {
			// wait for debounce and intent
			delay(500);

			// if button still held
			if (digitalRead(CTRL) == LOW) {
				state = ARM;

				// wait for unpress
				while (digitalRead(CTRL) != HIGH)
					delay(100);

				// wait for debounce
				delay(500);

				// skip remainder of processing
				break;
			}
		}

		// receive payload command
		switch (recvPayload()) {
			// do nothing if no command
			case 'h':
				// Debug 1 Green - idling
				debug &= ~(1 << 13);
				debug |= (1 << 14);
				break;

			// run test
			case 't':
				state = TEST;
				break;

			// arm rocket
			case 'a':
				state = ARM;
				break;

			// no communication with payload
			case ' ':
				// Debug 1 Red - no communication with payload
				debug |= (1 << 13);
				debug &= ~(1 << 14);
				break;

			// halt program in invalid state
			default:
				state = HALT;
		}

		// update debug lights
		sendDebug();
	}
}

void halt() {
	// Debug 1-4 Red - halt
	debug = 0b0010010010010000;
	sendDebug();

	// turn off all important lines
	digitalWrite(TERM_MAIN, LOW);
	digitalWrite(TERM_DROGUE, LOW);
	digitalWrite(TERM_IGNITE, LOW);

	// clear interrupts and put processor to sleep
	cli();
	sleep_enable();
	sleep_cpu();
}

void test() {
	// Debug 1 Green - run test
	debug |= (1 << 15);
	sendDebug();

	// update payload state
	sendPayload('s', "t", 1);

	char cmd;

	while ((cmd = recvPayload()) == 'h')
		updateTelemetry();

	if (cmd == 'e') {
		sendPayload('t', "p", 1);

		// Debug 2 Green - pass test
		debug |= (1 << 11);
	}
	else {
		sendPayload('t', "f", 1);

		// Debug 2 Red - fail test
		debug |= (1 << 10);
	}

	sendDebug();

	// wait for lights to be read
	delay(2000);

	// turn off lights
	debug &= ~(1 << 10);
	debug &= ~(1 << 11);
	debug &= ~(1 << 15);
	sendDebug();

	state = IDLE;
}

void arm() {
	// Arm Blue - armed
	debug |= (1 << 3);
	sendDebug();

	// update payload state
	sendPayload('s', "a", 1);

	while (state == ARM) {
		updateTelemetry();

		// sample ground altitude
		bar.gnd = bar.alt;

		// check on ctrl term
		if (digitalRead(CTRL) == LOW) {
			// wait for debounce and intent
			delay(500);

			// if button still held
			if (digitalRead(CTRL) == LOW) {
				state = IGNITE;

				// wait for unpress
				while (digitalRead(CTRL) != HIGH)
					delay(100);

				// wait for debounce
				delay(500);

				// skip remainder of processing
				break;
			}
		}

		// receive payload command
		switch (recvPayload()) {
			// do nothing if no command
			case 'h':
				break;

			// disarm rocket
			case 'd':
				debug &= ~(1 << 3);
				state = IDLE;
				break;

			// ignite rocket
			case 'i':
				state = IGNITE;
				break;

			// no communication with payload
			case ' ':
				// revert to idle state
				state = IDLE;
				break;

			// halt program in invalid state
			default:
				state = HALT;
		}
	}
}

void ignite() {
	// Debug 3 Red - igniting
	debug |= (1 << 7);
	sendDebug();

	// update payload state
	sendPayload('s', "i", 1);

	// send ignition signal
	digitalWrite(TERM_IGNITE, HIGH);

	// ignition time for measuring
	unsigned long start = millis();

	// wait for rocket to move up
	updateTelemetry();
	while (acc.z < MIN_ACCEL) {
		// halt if button pressed after still time
		if (millis() - start > MAX_IGNITE && digitalRead(CTRL) == LOW) {
			// wait for debounce and intent
			delay(500);

			// cancel launch if button still pressed
			if (digitalRead(CTRL) == LOW) {
				state = HALT;
				return;
			}
		}

		updateTelemetry();
	}

	// end ignition signal
	digitalWrite(TERM_IGNITE, LOW);

	// change to burn
	state = BURN;
}

void burn() {
	// Debug 3 Yellow - burning
	debug |= (1 << 8);
	sendDebug();

	// update payload state
	sendPayload('s', "b", 1);

	// update telemetry during burn
	while (acc.z > THRUST_ACCEL)
		updateTelemetry();

	// change to coast
	state = COAST;
}

void coast() {
	// Debug 3 Blue - coasting
	debug &= ~(1 << 7);
	debug &= ~(1 << 8);
	debug |= (1 << 9);
	sendDebug();

	// update payload state
	sendPayload('s', "c", 1);

	// update telemetry during coast
	while (bar.dp < APOGEE_DPRES)
		updateTelemetry();

	// change to apogee
	state = APOGEE;
}

void apogee() {
	// Debug 3 Green - apogee
	debug &= ~(1 << 9);
	debug |= (1 << 8);
	sendDebug();

	// update payload state
	sendPayload('s', "d", 1);

	// send parachute signal
	digitalWrite(TERM_DROGUE, HIGH);
	delay(DELAY_PARACHUTE);
	digitalWrite(TERM_DROGUE, LOW);

	// change to wait
	state = WAIT;
}

void wait() {
	// Debug 4 Blue - waiting
	debug |= (1 << 6);
	sendDebug();

	// update payload state
	sendPayload('s', "w", 1);

	// update telemetry during descent
	while (bar.alt > MAIN_ALT + bar.gnd)
		updateTelemetry();

	// change to eject
	state = EJECT;
}

void eject() {
	// Debug 4 Yellow - ejecting
	debug &= ~(1 << 6);
	debug |= (1 << 4);
	debug |= (1 << 5);
	sendDebug();

	// update payload state
	sendPayload('s', "e", 1);

	// send parachute signal
	digitalWrite(TERM_MAIN, HIGH);
	delay(DELAY_PARACHUTE);
	digitalWrite(TERM_MAIN, LOW);

	// change to fall
	state = FALL;
}

void fall() {
	// Debug 4 Green - falling
	debug &= ~(1 << 4);
	sendDebug();

	// update telemetry during descent
	while (bar.dp > MIN_DPRES)
		updateTelemetry();

	// change to recover
	state = RECOVER;
}

void recover() {
	// Arm Green - recovery
	debug &= (1 << 3);
	sendDebug();

	// clear interrupts and put processor to sleep
	cli();
	sleep_enable();
	sleep_cpu();
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
	digitalWrite(TERM_MAIN, LOW);
	pinMode(TERM_DROGUE, OUTPUT);
	digitalWrite(TERM_DROGUE, LOW);
	pinMode(TERM_IGNITE, OUTPUT);
	digitalWrite(TERM_IGNITE, LOW);

	// initialize communication with the barometer
	barometer.begin();

	// initialize communication with the payload
	Wire.begin();

	// intialize sensor data
	acc.x = acc.y = acc.z = 512;
	bar.p = 808;
	bar.dp = 0;
	bar.alt = 0;
	bar.gnd = 0;

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

		case APOGEE:
			apogee();
			break;

		case WAIT:
			wait();
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
