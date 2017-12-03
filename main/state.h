/*
 * Declaration file for state functions
 */

/* DEFINE */
#ifndef STATE_H_
#define STATE_H_

/* LIBRARIES */
#include <Arduino.h>
#include <avr/sleep.h>
#include <EEPROM.h>
#include "global.h"
#include "communication.h"
#include "accelerometer.h"
#include "barometer.h"
#include "gps.h"

// Pins
#define CTRL 7
#define TERM_MAIN 5
#define TERM_DROGUE 6
#define TERM_IGNITE 10

// Delay to rate limit idle function
#define IDLE_DELAY 1000

// Header bytes for EEPROM
#define EEPROM_HEADER "MainRev4"

// Time to hold pin high for parachute charge
#define DELAY_PARACHUTE 1000
// Time before allowing the rocket to be controlled from ignite to halt
#define MAX_IGNITE 300
// Accelerometer values to determine changes in state
#define MIN_ACCEL 2.0
#define THRUST_ACCEL 0.0
// Barometer values to determine changes in state
#define APOGEE_DPRES 1.0
#define MIN_DPRES 1.0
// Altitude to deploy main parachute
#define MAIN_ALT 2000.0

// Define enum type variables
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
