// pins
#define ACCEL_X 3
#define ACCEL_Y 2
#define ACCEL_Z 10

#define BARO_MOSI 9
#define BARO_MISO 7
#define BARO_SCK 8

#define CTRL 6

#define PANEL_CLOCK 15
#define PANEL_DATA 14
#define PANEL_LATCH 16

#define TERM_MAIN 4
#define TERM_DROGUE 5
#define TERM_IGNITE -1

unsigned int debug = 0b1111111111111111;

void sendDebug() {
	// send bits
	digitalWrite(PANEL_LATCH, LOW);
	shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, lowByte(debug));
	shiftOut(PANEL_DATA, PANEL_CLOCK, LSBFIRST, highByte(debug));
	digitalWrite(PANEL_LATCH, HIGH);
	delay(50);
}

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
}

void loop() {
	sendDebug();
	delay(2000);
	debug = 0b0000000000000000;
	sendDebug();
	delay(2000);
	debug = 0b1111111111111111;
}
