/*
 * Main v0.3j
 *
 * Main computer logic handling: button inputs, debug outputs, sensor inputs,
 * ignition, parachute evjection, and telemetry. Telemetry and commands are
 * communicated with the base station over the radios on a best effort basis.
 * Everything else is a critical function.
 */

/* DEFINITIONS */

// Initialization of sensor data
#define SENSOR_INIT 20
#define SENSOR_DELAY 10

// Time to hold pin high for parachute charge
#define DELAY_PARACHUTE 1000

/* LIBRARIES */
#include <avr/sleep.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "global.h"
#include "accelerometer.h"
#include "statefunctions.h"
#include "barometer.h"
#include "communication.h"
#include "gps.h"

/* MAIN PROGRAM FUNCTIONS */

void setup() {
    // Set inputs and outputs
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

    // Initialize communication with the barometer
    Wire.begin();
    initBarometer();

    // Initialize communication with GPS receiver
    gpscomm.begin(9600);

    // Initialize communication with the base station
    Serial.begin(9600);

    // Define initial state
    state = INIT;
    debug = 0b0000000000000000;

    // Initialize sensor data
    acc.x = acc.y = acc.z = 512;
    bar.p = 808;
    bar.dp = 0;
    bar.alt = 0;
    bar.gnd = 0;

    // Sample filter seed data
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

    readGps();

    // Check for saved data in EEPROM
    bool stored = true;
    for (byte idx = 0; idx < eeprom_header.length(); idx++) {
         if (EEPROM[idx] != eeprom_header[idx]) {
              stored = false;
              EEPROM[idx] = eeprom_header[idx];
         }
    }

    // Set state from EEPROM if CTRL is not pressed
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

    // Wait on CTRL press
    if (digitalRead(CTRL) == LOW) {
         // Wait for unpress
         while (digitalRead(CTRL) != HIGH)
         delay(100);
         // Wait for debounce
         delay(500);
    }
    bitClear(debug, 15);
}

void loop() {
    state_prev = state;

    // Run appropriate state function
    switch (state) {
         // Go to idle from init state
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

         // Halt program in invalid state
         default:
         state = HALT;
    }

    if (state != state_prev) {
         EEPROM.put(eeprom_state, state);
         EEPROM.put(eeprom_debug, debug);
    }
}
