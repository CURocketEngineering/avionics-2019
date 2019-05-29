/*
 * Definition file for state functions
 */

/* LIBRARIES */
#include <avr/sleep.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "Wire.h"
#include "state.h"
#include "communication.h"
#include "sim.h"
#include "debug.h"
#include "pins.h"
#include "config.h"

const char * states_arr[] = {
     "init",
     "idle",
     "halt",
     "test",
     "arm",
     "ignite",
     "burn",
     "coast",
     "apogee",
     "wait",
     "eject",
     "fall",
     "recover"
};

const char ** states = states_arr;

enum state_e state, state_prev;

String eeprom_header = EEPROM_HEADER;
int eeprom_state = eeprom_header.length();
int eeprom_debug = eeprom_header.length() + sizeof(state);
int eeprom_ground = eeprom_header.length() + sizeof(state) + sizeof(debug);

void idle() {
    while (state == IDLE) {
        // Update base station state
        communication_sendState(IDLE);

        // Check on ctrl term
        if (digitalRead(CTRL) == LOW) {
            // Wait for debounce and intent
            delay(5000);

            // If button still held
            if (digitalRead(CTRL) == LOW) {
                state = ARM;

                // Wait for unpress
                while (digitalRead(CTRL) != HIGH)
                    delay(100);

                // Wait for debounce
                delay(500);

                // Skip remainder of processing
                break;
            }
        }

        // Receive base command
        switch (communication_recvCommand()) {
            // Do nothing if no command
            case CMD_NONE:
                // Debug 1 Green - idling
                bitClear(debug, 13);
                bitSet(debug, 14);
                break;
                // Run test
            case CMD_TEST:
                state = TEST;
                break;
                // Arm rocket
            case CMD_ARM:
                state = ARM;
                break;
                // No communication with base station
            case NO_COMM:
                // Debug 1 Red - no communication with base station
                bitSet(debug, 13);
                bitClear(debug, 14);
                break;
                // Halt program in invalid state
            default:
                state = HALT;
        }

        // Update debug lights
        debug_write();

        delay(IDLE_DELAY);
    }
}

void halt() {
    // Debug 1-4 Red - halt
    debug = 0b0010010010010000;
    debug_write();

    // Turn off all important lines
    digitalWrite(TERM_MAIN, LOW);
    digitalWrite(TERM_DROGUE, LOW);

    // Clear interrupts and put processor to sleep
    //cli();
    //sleep_enable();
    //sleep_cpu();

    state = IDLE;
}

void test() {
    // Debug 1 Blue - run test
    bitSet(debug, 15);
    debug_write();

    // Update base station state
    communication_sendState(TEST);

    enum command_e cmd;

    while ((cmd = communication_recvCommand()) == CMD_NONE) {
        communication_updateTelemetry();
    }

    if (cmd == CMD_PASS) {
        communication_sendResult(true);
        // Debug 2 Green - pass test
        bitSet(debug, 11);
    }
    else {
        communication_sendResult(false);
        // Debug 2 Red - fail test
        bitSet(debug, 10);
    }

    debug_write();

    // Wait for lights to be read
    delay(2000);

    // Turn off lights
    bitClear(debug, 10);
    bitClear(debug, 11);
    bitClear(debug, 15);
    debug_write();

    state = IDLE;
}

void arm() {
    // Arm Blue - armed
    bitSet(debug, 3);
    debug_write();

    communication_sendState(ARM);

    while (state == ARM) {
        // Update base station state
        communication_updateTelemetry();

        // Sample ground altitude
        bar.gnd = bar.alt;

        // Check on ctrl term
        if (digitalRead(CTRL) == LOW) {
            // Wait for debounce and intent
            delay(5000);

            // If button still held
            if (digitalRead(CTRL) == LOW) {
                state = IGNITE;

                // Wait for unpress
                while (digitalRead(CTRL) != HIGH) {
                    delay(100);
                }

                // Wait for debounce
                delay(500);

                // Skip remainder of processing
                break;
            }
        }

        // Receive base station command
        switch (communication_recvCommand()) {
            // Do nothing if no command
            case CMD_NONE:
            case CMD_ARM:
                break;

            // Disarm rocket
            case CMD_DISARM:
                bitClear(debug, 3);
                state = IDLE;
                break;

            // Ignite rocket
            case CMD_IGNITE:
                state = IGNITE;
                break;

            // No communication with base station
            case NO_COMM:
                // Revert to idle state
                state = IDLE;
                break;

            // Halt program in invalid state
            default:
                state = HALT;
        }
    }
}

void ignite() {
    // Debug 3 Red - igniting
    bitSet(debug, 7);
    debug_write();

    // Update base station state
    communication_sendState(IGNITE);

#ifdef SIM
    sim_start = millis();
#endif

    // Store ground in EEPROM
    EEPROM.put(eeprom_ground, bar.gnd);

    // Wait for rocket to move up
    communication_updateTelemetry();
    while (NINEDOF_UP < MIN_ACCEL) {
        communication_updateTelemetry();
    }

    // Change to burn
    state = BURN;
}

void burn() {
    // Debug 3 Yellow - burning
    bitSet(debug, 8);
    debug_write();

    // Update base station state
    communication_sendState(BURN);

    // Update telemetry during burn
    communication_updateTelemetry();
    while (NINEDOF_UP > THRUST_ACCEL) {
        communication_updateTelemetry();
    }

    // Change to coast
    state = COAST;
}

void coast() {
    // Debug 3 Blue - coasting
    bitClear(debug, 7);
    bitClear(debug, 8);
    bitSet(debug, 9);
    debug_write();

    // Update base station state
    communication_sendState(COAST);

    // Update telemetry during coast
    communication_updateTelemetry();
    while (bar.dp < APOGEE_DPRES){
        communication_updateTelemetry();
    }

    // Change to apogee
    state = APOGEE;
}

void apogee() {
    // Debug 3 Green - apogee
    bitClear(debug, 9);
    bitSet(debug, 8);
    debug_write();

    // Update base station state
    communication_sendState(APOGEE);

    // Wait for ejection delay
    if (PARACHUTE_WAIT) delay(PARACHUTE_WAIT);

    // Send parachute signal
    /* OLD
    digitalWrite(TERM_DROGUE, HIGH);
    delay(PARACHUTE_DELAY);
    digitalWrite(TERM_DROGUE, LOW);
    */
    // Qwiic
    toggleRelay(true,qwiicRelayAddressAp);
    delay(PARACHUTE_DELAY);
    toggleRelay(false,qwiicRelayAddressAp);

#ifdef SIM
    unsigned long sim_last = sim_start;
    unsigned long sim_cur = millis();

    sim_start = 0;

    while (communication_recvCommand() != CMD_ARM) {
        communication_updateTelemetry();
    }

    sim_start = sim_last + millis() - sim_cur;
#endif

    // Change to wait
    state = WAIT;
}

void wait() {
    // Debug 4 Blue - waiting
    bitSet(debug, 6);
    debug_write();

    // Update base station state
    communication_sendState(WAIT);

    // Update telemetry during descent
    communication_updateTelemetry();
    while (bar.alt > MAIN_ALT + bar.gnd) {
        communication_updateTelemetry();
    }

    // Change to eject
    state = EJECT;
}

void eject() {
    // Debug 4 Yellow - ejecting
    bitClear(debug, 6);
    bitSet(debug, 4);
    bitSet(debug, 5);
    debug_write();

    // Update base station state
    communication_sendState(EJECT);

    // Wait for ejection delay
    if (PARACHUTE_WAIT)
        delay(PARACHUTE_WAIT);

    // Send parachute signal
    /* old
    digitalWrite(TERM_MAIN, HIGH);
    delay(PARACHUTE_DELAY);
    digitalWrite(TERM_MAIN, LOW);
    */
    // Qwiic
    toggleRelay(true,qwiicRelayAddressMa);
    delay(PARACHUTE_DELAY);
    toggleRelay(false,qwiicRelayAddressMa);

    // Change to fall
    state = FALL;
}

void fall() {
    // Debug 4 Green - falling
    bitClear(debug, 4);
    debug_write();

    // Update base station state
    communication_sendState(FALL);

    // Update telemetry during descent
    communication_updateTelemetry();
    while (bar.dp > MIN_DPRES) {
        communication_updateTelemetry();
    }

    // Change to recover
    state = RECOVER;
}

void recover() {
    // Arm Green - recovery
    bitClear(debug, 3);
    debug_write();

    // Update base station state
    communication_sendState(RECOVER);

    state = IDLE;

    EEPROM.put(eeprom_state, state);
    EEPROM.put(eeprom_debug, debug);

    while (true) {
        communication_updateTelemetry();
    }
}

void state_init() {
    pinMode(CTRL, INPUT_PULLUP);

    pinMode(TERM_MAIN, OUTPUT);
    digitalWrite(TERM_MAIN, LOW);
    pinMode(TERM_DROGUE, OUTPUT);
    digitalWrite(TERM_DROGUE, LOW);

    // Check for saved data in EEPROM
    bool stored = true;
    for (byte idx = 0; idx < eeprom_header.length(); idx++) {
        if (EEPROM[idx] != eeprom_header[idx]) {
            stored = false;
            EEPROM[idx] = eeprom_header[idx];
        }
    }

#ifndef SIM
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
#else
    state = IDLE;
#endif

    communication_sendState(state);

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

void toggleRelay(bool on, byte relayAddress){
  if (on) {
    Wire.beginTransmission(relayAddress);
    Wire.write(COMMAND_RELAY_ON);
  } else {
    Wire.write(COMMAND_RELAY_OFF);
    Wire.endTransmission();
  }
}

void state_loop() {
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
