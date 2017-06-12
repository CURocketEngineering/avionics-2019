/*
 * Main v0.1
 *
 * Main computer logic handling: button inputs, debug outputs, sensor inputs,
 * ignition, parachute ejection, and telemetry. Telemetry and commands are
 * communicated with the payload computer for relay over the radios on a best
 * effort basis. Everything else is a critical function.
 */

// definitions

// set when testing
#define DEBUG

// must be set before flight from Nation Weather Service altimeter reading (http://www.weather.gov/)
#define NWS_ALTI 30

// pins
#define ACCEL_X 14
#define ACCEL_Y 15
#define ACCEL_Z 16

#define BARO_MOSI 9
#define BARO_MISO 7
#define BARO_SCK 8

#define CTRL 6

#define PANEL_CLOCK 2
#define PANEL_DATA 10
#define PANEL_LATCH 3

#define TERM_MAIN 4
#define TERM_DROGUE 5
#define TERM_IGNITE 17

// header bytes for EEPROM
#define EEPROM_HEADER "MainRev3"

// I²C address of payload
#define PAYLOAD_ADDR 8

// initialization of sensor data
#define SENSOR_INIT 20
#define SENSOR_DELAY 10

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
#include <EEPROM.h>
#include <SoftSPI.h>
#include <Wire.h>

// global data

static enum state_e {
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
} state, state_prev;

static struct acc_s {
	// 0 -> -200 g, 1023 -> 200 g
	unsigned int x, y, z;
} acc, acc_prev;

static struct bar_s {
	// 0 -> 50 kPa, 1023 -> 115 kPa
	unsigned int p, dp;
	// ft
	unsigned int alt;
	unsigned int gnd;
} bar, bar_prev;

//                      1  2  3  4  A
//                     GBRGBRGBRGBRBXXX
unsigned int debug = 0b0000000000000000;

String eeprom_header = EEPROM_HEADER;
int eeprom_state = eeprom_header.length();
int eeprom_debug = eeprom_header.length() + sizeof(state);
int eeprom_ground = eeprom_header.length() + sizeof(state) + sizeof(debug);

SoftSPI barometer(BARO_MOSI, BARO_MISO, BARO_SCK);

// sensor functions

void readAccelerometer(bool filter, bool debug = false, unsigned int x = 0, unsigned int y = 0, unsigned int z = 0) {
	acc_prev = acc;

	if (!debug) {
		acc.x = analogRead(ACCEL_X);
		acc.y = analogRead(ACCEL_Y);
		acc.z = analogRead(ACCEL_Z);
	}
	else {
		acc.x = x;
		acc.y = y;
		acc.z = z;
	}

	if (filter) {
		acc.x = ACCEL_GAIN*acc.x + (1.0 - ACCEL_GAIN)*acc_prev.x;
		acc.y = ACCEL_GAIN*acc.y + (1.0 - ACCEL_GAIN)*acc_prev.y;
		acc.z = ACCEL_GAIN*acc.z + (1.0 - ACCEL_GAIN)*acc_prev.z;
	}
}

void barometerWrite(byte address, byte data) {
	// write mode
	address &= 0x7F;

	barometer.transfer(address);
	barometer.transfer(data);
}

unsigned short barometerRead(byte address) {
	static unsigned short value;

	// read mode
	address |= 0x80;

	value = barometer.transfer(address);
	value <<= 8;
	value |= barometer.transfer(address + 0x02);

	return value;
}

void readBarometer(bool filter, bool debug = false, unsigned int p = 0) {
	static unsigned short pressure, temperature, a0, b1, b2, c12, c11, c22;

	// start read
	barometerWrite(0x24, 0x00);
	delay(10);

	// get pressure and temperature
	pressure = barometerRead(0x00) >> 6;
	temperature = barometerRead(0x04) >> 6;

	// get coefficients
	a0 = barometerRead(0x08);
	b1 = barometerRead(0x0c);
	b2 = barometerRead(0x10);
	c12 = barometerRead(0x14);
	c11 = barometerRead(0x18);
	c22 = barometerRead(0x1c);

	// calculate compensated pressure
	bar_prev = bar;

	if (!debug) {
		bar.p = a0 + (b1 + c11*pressure + c12*temperature)*pressure + (b2 + c22*temperature)*temperature;
	}
	else {
		bar.p = p;
	}

	bar.dp = bar.p - bar_prev.p;

	if (filter) {
		bar.p = BARO_GAIN*bar.p + (1.0 - BARO_GAIN)*bar_prev.p;
	}

	// calculate altitude
	//                                                                 inHg/Pa
	bar.alt = (1 - pow(map(bar.p, 0, 1023, 50000, 115000) / NWS_ALTI / 0.000295299830714, (1 / 5.25587611))) / 0.0000068756;
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
	digitalWrite(PANEL_LATCH, LOW);
	shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, lowByte(debug));
	shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, highByte(debug));
	digitalWrite(PANEL_LATCH, HIGH);
	delay(50);
}

void updateTelemetry() {
	static char data[10];

#ifdef DEBUG
	if (Serial.available()) {
		static struct data_s {
			unsigned int acc_x;
			unsigned int acc_y;
			unsigned int acc_z;
			unsigned int bar_p;
		} data;

		Serial.readBytes((char *)&data, 8);

		readAccelerometer(true, true, data.acc_x, data.acc_y, data.acc_z);
		readBarometer(true, true, data.bar_p);
	}
#else
	readAccelerometer(true);
	readBarometer(true);
#endif

	data[0] = highByte(acc.x);
	data[1] = lowByte(acc.x);
	data[2] = highByte(acc.y);
	data[3] = lowByte(acc.y);
	data[4] = highByte(acc.z);
	data[5] = lowByte(acc.z);
	data[6] = highByte(bar.p);
	data[7] = lowByte(bar.p);
	data[8] = highByte(bar.alt);
	data[9] = lowByte(bar.alt);

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
				bitClear(debug, 13);
				bitSet(debug, 14);
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
				bitSet(debug, 13);
				bitClear(debug, 14);
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
	// Debug 1 Blue - run test
	bitSet(debug, 15);
	sendDebug();

	// update payload state
	sendPayload('s', "t", 1);

	char cmd;

	while ((cmd = recvPayload()) == 'h')
		updateTelemetry();

	if (cmd == 'e') {
		sendPayload('t', "p", 1);

		// Debug 2 Green - pass test
		bitSet(debug, 11);
	}
	else {
		sendPayload('t', "f", 1);

		// Debug 2 Red - fail test
		bitSet(debug, 10);
	}

	sendDebug();

	// wait for lights to be read
	delay(2000);

	// turn off lights
	bitClear(debug, 10);
	bitClear(debug, 11);
	bitClear(debug, 15);
	sendDebug();

	state = IDLE;
}

void arm() {
	// Arm Blue - armed
	bitSet(debug, 3);
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
				bitClear(debug, 3);
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
	bitSet(debug, 7);
	sendDebug();

	// update payload state
	sendPayload('s', "i", 1);

	// store ground in EEPROM
	EEPROM.put(eeprom_ground, bar.gnd);

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
	bitSet(debug, 8);
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
	bitClear(debug, 7);
	bitClear(debug, 8);
	bitSet(debug, 9);
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
	bitClear(debug, 9);
	bitSet(debug, 8);
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
	bitSet(debug, 6);
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
	bitClear(debug, 6);
	bitSet(debug, 4);
	bitSet(debug, 5);
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
	bitClear(debug, 4);
	sendDebug();

	// update telemetry during descent
	while (bar.dp > MIN_DPRES)
		updateTelemetry();

	// change to recover
	state = RECOVER;
}

void recover() {
	// Arm Green - recovery
	bitClear(debug, 3);
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
	digitalWrite(PANEL_LATCH, HIGH);
	delay(50);

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

	// define initial state
	state = INIT;
	debug = 0b0000000000000000;

	// initialize sensor data
	acc.x = acc.y = acc.z = 512;
	bar.p = 808;
	bar.dp = 0;
	bar.alt = 0;
	bar.gnd = 0;

	// sample filter seed data
	unsigned long acc_x = 0, acc_y = 0, acc_z = 0;
	unsigned long bar_p = 0, bar_dp = 0;
	unsigned long bar_alt = 0;

	for (byte cnt = 0; cnt < SENSOR_INIT; cnt++) {
		readAccelerometer(false);
		acc_x += acc.x;
		acc_y += acc.y;
		acc_z += acc.z;

		readBarometer(false);
		bar_p += bar.p;
		bar_dp += bar.dp;
		bar_alt += bar.alt;

		delay(SENSOR_DELAY);
	}

	acc.x = acc_x/SENSOR_INIT;
	acc.y = acc_y/SENSOR_INIT;
	acc.z = acc_z/SENSOR_INIT;

	bar.p = bar_p/SENSOR_INIT;
	bar.dp = bar_dp/SENSOR_INIT;
	bar.alt = bar_alt/SENSOR_INIT;

	// check for saved data in EEPROM
	bool stored = true;
	for (byte idx = 0; idx < eeprom_header.length(); idx++) {
		if (EEPROM[idx] != eeprom_header[idx]) {
			stored = false;
			EEPROM[idx] = eeprom_header[idx];
		}
	}

	// set state from EEPROM if CTRL is not pressed
	if (stored && digitalRead(CTRL) == HIGH) {
		EEPROM.get(eeprom_state, state);
		EEPROM.get(eeprom_debug, debug);
		EEPROM.get(eeprom_ground, bar.gnd);

		bitClear(debug, 12);
	}
	else {
		EEPROM.put(eeprom_state, state);
		EEPROM.put(eeprom_debug, debug);
		EEPROM.put(eeprom_ground, bar.gnd);
	}

	// wait on CTRL press
	if (digitalRead(CTRL) == HIGH) {
		// wait for unpress
		while(digitalRead(CTRL) != HIGH)
			delay(100);

		// wait for debounce
		delay(500);
	}

#ifdef DEBUG
	Serial.begin(9600);
	Serial.write('D');

	// Debug 2 Blue - debug
	bitSet(debug, 12);

#endif
	sendDebug();
}

void loop() {
	state_prev = state;

	// run appropriate state function
	switch (state) {
		// go to idle from init state
		case INIT:
			state = IDLE;
			debug = 0b0000000000000000;
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

	if (state != state_prev) {
		EEPROM.put(eeprom_state, state);
		EEPROM.put(eeprom_debug, debug);
	}
}
