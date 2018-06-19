/*
 * Declaration file for state functions
 */

/* DEFINE */
#ifndef STATE_H_
#define STATE_H_

/* LIBRARIES */
#include <avr/sleep.h>

#include <Arduino.h>

#include "pins.h"
#include "ninedof.h"
#include "barometer.h"
#include "gps.h"

// Delay to rate limit idle function
#define IDLE_DELAY 1000

// Header bytes for EEPROM
#define EEPROM_HEADER "MainRev6"

// Time to hold pin high for parachute charge
#define PARACHUTE_DELAY 1000
// Accelerometer values to determine changes in state
#define MIN_ACCEL 2.0
#define THRUST_ACCEL 0.0
// Barometer values to determine changes in state
#define APOGEE_DPRES 1.0
#define MIN_DPRES 1.0

// Time to wait before deploying parachute
#define PARACHUTE_WAIT 0
// Altitude to deploy main parachute
#define MAIN_ALT 1500.0

// Define enum type variables
enum state_e {
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
};

extern const char ** states;

extern enum state_e state, state_prev;

extern String eeprom_header;
extern int eeprom_state;
extern int eeprom_debug;
extern int eeprom_ground;

/* DECLARE FUNCTIONS */
void idle();
void halt();
void test();
void arm();
void ignite();
void burn();
void coast();
void apogee();
void wait();
void eject();
void fall();
void recover();

void state_init();
void state_loop();

#endif
