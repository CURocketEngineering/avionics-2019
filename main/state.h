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
#define IDLE_DELAY 100

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

// Qwiic
void toggleRelay(bool on, byte relayAddress);
const byte qwiicRelayAddressAp = 0x18; //Apogee
const byte qwiicRelayAddressMa = 0x19; //Main
#define RELAY_ADDR 0x6D
#define TOGGLE_RELAY_ONE 0x01
#define TOGGLE_RELAY_TWO 0x02
#define TOGGLE_RELAY_THREE 0x03
#define TOGGLE_RELAY_FOUR 0x04

#endif
