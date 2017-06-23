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

// National Weather Service altimeter reading (http://www.weather.gov/)
#define NWS_ALTI 1013.25

// pins
#define ACCEL_X 14
#define ACCEL_Y 15
#define ACCEL_Z 16

#define BARO_ADDR 0x77

#define CTRL 7

#define DEBUG_RX 8
#define DEBUG_TX 9

#define PANEL_CLOCK 3
#define PANEL_DATA 4
#define PANEL_LATCH 2

#define TERM_MAIN 5
#define TERM_DROGUE 6
#define TERM_IGNITE 10

// header bytes for EEPROM
#define EEPROM_HEADER "MainRev4"

// initialization of sensor data
#define SENSOR_INIT 20
#define SENSOR_DELAY 10

// gain values for sensor filter
#define ACCEL_GAIN 0.2
#define BARO_GAIN 0.2

// time to hold pin high for parachute charge
#define DELAY_PARACHUTE 1000

// time before allowing the rocket to be controlled from ignite to halt
#define MAX_IGNITE 300

// accelerometer values to determine changes in state
#define MIN_ACCEL 2.0
#define THRUST_ACCEL 0.0

// barometer values to determine changes in state
#define APOGEE_DPRES 1.0
#define MIN_DPRES 1.0

// altitude to deploy main parachute
#define MAIN_ALT 2000.0

// libraries

#include <avr/sleep.h>

#include <EEPROM.h>
#include <Wire.h>
#ifdef DEBUG
#include <SoftwareSerial.h>
#endif

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
    // g
    float x, y, z;
} acc, acc_prev;

static struct bar_s {
    // mb
    float p, dp;
    // ft
    float alt, gnd;
} bar, bar_prev;

static struct bar_init_s {
    // coefficients
    short AC1, AC2, AC3, VB1, VB2, MB, MC, MD;
    unsigned short AC4, AC5, AC6;
    float c3, c4, b1, c5, c6, mc, md, x0, x1, x2, y0, y1, y2, p0, p1, p2;
} bar_init;

//                      1  2  3  4  A
//                     GBRGBRGBRGBRBXXX
unsigned int debug = 0b1111111111111111;

String eeprom_header = EEPROM_HEADER;
int eeprom_state = eeprom_header.length();
int eeprom_debug = eeprom_header.length() + sizeof(state);
int eeprom_ground = eeprom_header.length() + sizeof(state) + sizeof(debug);

#ifdef DEBUG
SoftwareSerial stream(DEBUG_RX, DEBUG_TX);

#endif
// sensor functions

void readAccelerometer(bool filter, bool debug = false, float x = 0, float y = 0, float z = 0) {
    acc_prev = acc;

    if (!debug) {
	acc.x = analogRead(ACCEL_X)*400.0/1023.0 - 200.0;
	acc.y = analogRead(ACCEL_Y)*400.0/1023.0 - 200.0;
	acc.z = analogRead(ACCEL_Z)*400.0/1023.0 - 200.0;
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

    acc_prev = acc;
}

void barometerWrite(byte address, byte data) {
    Wire.beginTransmission(BARO_ADDR);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

void barometerRead(byte address, byte * buf, unsigned short len) {
    Wire.beginTransmission(BARO_ADDR);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(BARO_ADDR, len);

    for(byte idx = 0; idx < len; idx++) {
	buf[idx] = Wire.read();
    }
}

short barometerReadInt(byte address) {
    static byte buf[2];

    barometerRead(address, buf, 2);

    return ((short)buf[0] << 8) | buf[1];
}

unsigned short barometerReadUInt(byte address) {
    static byte buf[2];

    barometerRead(address, buf, 2);

    return ((unsigned short)buf[0] << 8) | buf[1];
}

void initBarometer() {
    // datasheet magic
    bar_init.AC1 = barometerReadInt(0xAA);
    bar_init.AC2 = barometerReadInt(0xAC);
    bar_init.AC3 = barometerReadInt(0xAE);
    bar_init.AC4 = barometerReadUInt(0xB0);
    bar_init.AC5 = barometerReadUInt(0xB2);
    bar_init.AC6 = barometerReadUInt(0xB4);
    bar_init.VB1 = barometerReadInt(0xB6);
    bar_init.VB2 = barometerReadInt(0xB8);
    bar_init.MB = barometerReadInt(0xBA);
    bar_init.MC = barometerReadInt(0xBC);
    bar_init.MD = barometerReadInt(0xBE);

    bar_init.c3 = 160.0*pow(2, -15)*bar_init.AC3;
    bar_init.c4 = pow(10, -3) * pow(2, -15)*bar_init.AC4;
    bar_init.b1 = pow(160, 2)*pow(2, -30)*bar_init.VB1;
    bar_init.c5 = (pow(2, -15)/160)*bar_init.AC5;
    bar_init.c6 = bar_init.AC6;
    bar_init.mc = (pow(2, 11)/pow(160, 2))*bar_init.MC;
    bar_init.md = bar_init.MD/160.0;
    bar_init.x0 = bar_init.AC1;
    bar_init.x1 = 160.0*pow(2, -13)*bar_init.AC2;
    bar_init.x2 = pow(160, 2)*pow(2, -25)*bar_init.VB2;
    bar_init.y0 = bar_init.c4*pow(2, 15);
    bar_init.y1 = bar_init.c4*bar_init.c3;
    bar_init.y2 = bar_init.c4*bar_init.b1;
    bar_init.p0 = (3791.0 - 8.0)/1600.0;
    bar_init.p1 = 1.0 - 7357.0*pow(2, -20);
    bar_init.p2 = 3038.0*100.0*pow(2, -36);
}

void readBarometer(bool filter, bool debug = false, float p = 0) {
    static byte buf[3];
    static float tu, a, pu, s, x, y, z;

    static float pressure, temperature;

    // start temperature reading
    barometerWrite(0xF4, 0x2E);

    // wait on reading
    delay(10);

    // retrieve reading
    barometerRead(0xF6, buf, 2);

    // calculate temperature
    tu = (buf[0]*256.0) + buf[1];
    a = bar_init.c5*(tu - bar_init.c6);
    temperature = a + (bar_init.mc/(a + bar_init.md));

    // start pressure reading
    barometerWrite(0xF4, 0xF4);

    // wait on reading
    delay(30);

    // retrieve reading
    barometerRead(0xF6, buf, 3);

    // calculate pressure
    pu = (buf[0]*256.0) + buf[1] + (buf[2]/256.0);
    s = temperature - 25.0;
    x = (bar_init.x2*pow(s, 2)) + (bar_init.x1*s) + bar_init.x0;
    y = (bar_init.y2*pow(s, 2)) + (bar_init.y1*s) + bar_init.y0;
    z = (pu - x) / y;
    pressure = (bar_init.p2*pow(z, 2)) + (bar_init.p1*z) + bar_init.p0;

    // calculate compensated pressure
    bar_prev = bar;

    if (!debug) {
	bar.p = pressure;
    }
    else {
	bar.p = p;
    }

    if (filter) {
	bar.p = BARO_GAIN*bar.p + (1.0 - BARO_GAIN)*bar_prev.p;
    }

    bar.dp = bar.p - bar_prev.p;

    // calculate altitude
    bar.alt = 145440.0*(1 - pow(pressure/NWS_ALTI, 1/5.255));
}

// communication functions

void sendPayload(char type, const char * data, unsigned int len) {
    // transmit message type and data
    Serial.write(type);
    Serial.write(data);
}

char recvPayload() {
    // request and read a single char from payload
    if (Serial.available())
	return Serial.read();
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
    static union data_u {
	char bytes[20];
	float values[5];
    } data;

#ifdef DEBUG
    if (stream.available()) {
	static union stream_data_u {
	    char bytes[16];
	    float values[4];
	} stream_data;

	stream.readBytes(stream_data.bytes, 16);

	readAccelerometer(true, true, stream_data.values[0], stream_data.values[1], stream_data.values[2]);
	readBarometer(true, true, stream_data.values[3]);
    }
#else
    readAccelerometer(true);
    readBarometer(true);
#endif

    data.values[0] = acc.x;
    data.values[1] = acc.y;
    data.values[2] = acc.z;
    data.values[3] = bar.p;
    data.values[4] = bar.alt;

    sendPayload('u', data.bytes, 16);
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
	if (millis() - start > MAX_IGNITE*1000) {
	    if (digitalRead(CTRL) == LOW) {
		// wait for debounce and intent
		delay(500);

		// cancel launch if button still pressed
		if (digitalRead(CTRL) == LOW) {
		    state = HALT;
		    return;
		}
	    }

	    if (recvPayload() == 'c') {
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

    // update payload state
    sendPayload('s', "l", 1);

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

    // update payload state
    sendPayload('s', "r", 1);

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

    sendDebug();

#ifdef DEBUG
    delay(2000);

#endif
    // initialize communication with the barometer
    Wire.begin();
    initBarometer();

    // initialize communication with the payload
    Serial.begin(9600);

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

	bitSet(debug, 15);
    }

    sendDebug();

    // wait on CTRL press
    if (digitalRead(CTRL) == LOW) {
	// wait for unpress
	while (digitalRead(CTRL) != HIGH)
	    delay(100);

	// wait for debounce
	delay(500);
    }

    bitClear(debug, 15);

#ifdef DEBUG
    stream.begin(9600);
    stream.write('D');

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
