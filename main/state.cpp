/*
 * Definition file for state functions
 */

/* LIBRARIES */
#include <avr/sleep.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "state.h"
#include "communication.h"
#include "global.h"

String eeprom_header = EEPROM_HEADER;
int eeprom_state = eeprom_header.length();
int eeprom_debug = eeprom_header.length() + sizeof(state);
int eeprom_ground = eeprom_header.length() + sizeof(state) + sizeof(debug);

void idle() {
    while (state == IDLE) {
        // Update base station state
        communication_send('s', "h", 1);

        // Check on ctrl term
        if (digitalRead(CTRL) == LOW) {
            // Wait for debounce and intent
            delay(500);

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
        switch (communication_recv()) {
            // Do nothing if no command
            case 'h':
                // Debug 1 Green - idling
                bitClear(debug, 13);
                bitSet(debug, 14);
                break;
                // Run test
            case 't':
                state = TEST;
                break;
                // Arm rocket
            case 'a':
                state = ARM;
                break;
                // No communication with base station
            case ' ':
                // Debug 1 Red - no communication with base station
                bitSet(debug, 13);
                bitClear(debug, 14);
                break;
                // Halt program in invalid state
            default:
                state = HALT;
        }

        // Update debug lights
        communication_writeDebug();

        delay(IDLE_DELAY);
    }
}

void halt() {
    // Debug 1-4 Red - halt
    debug = 0b0010010010010000;
    communication_writeDebug();

    // Turn off all important lines
    digitalWrite(TERM_MAIN, LOW);
    digitalWrite(TERM_DROGUE, LOW);
    digitalWrite(TERM_IGNITE, LOW);

    // Clear interrupts and put processor to sleep
    cli();
    sleep_enable();
    sleep_cpu();
}

void test() {
    // Debug 1 Blue - run test
    bitSet(debug, 15);
    communication_writeDebug();

    // Update base station state
    communication_send('s', "t", 1);

    char cmd;

    while ((cmd = communication_recv()) == 'h') {
        communication_updateTelemetry();
    }

    if (cmd == 'e') {
        communication_send('t', "p", 1);
        // Debug 2 Green - pass test
        bitSet(debug, 11);
    }
    else {
        communication_send('t', "f", 1);
        // Debug 2 Red - fail test
        bitSet(debug, 10);
    }

    communication_writeDebug();

    // Wait for lights to be read
    delay(2000);

    // Turn off lights
    bitClear(debug, 10);
    bitClear(debug, 11);
    bitClear(debug, 15);
    communication_writeDebug();

    state = IDLE;
}

void arm() {
    // Arm Blue - armed
    bitSet(debug, 3);
    communication_writeDebug();

    while (state == ARM) {
        // Update base station state
        communication_send('s', "a", 1);
        communication_updateTelemetry();

        // Sample ground altitude
        bar.gnd = bar.alt;

        // Check on ctrl term
        if (digitalRead(CTRL) == LOW) {
            // Wait for debounce and intent
            delay(500);

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
        switch (communication_recv()) {
            // Do nothing if no command
            case 'h':
                break;
                // Disarm rocket
            case 'd':
                bitClear(debug, 3);
                state = IDLE;
                break;

                // Ignite rocket
            case 'i':
                state = IGNITE;
                break;

                // No communication with base station
            case ' ':
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
    communication_writeDebug();

    // Update base station state
    communication_send('s', "i", 1);

    // Store ground in EEPROM
    EEPROM.put(eeprom_ground, bar.gnd);

    // Send ignition signal
    digitalWrite(TERM_IGNITE, HIGH);

    // Ignition time for measuring
    unsigned long start = millis();

    // Wait for rocket to move up
    communication_updateTelemetry();
    while (acc.z < MIN_ACCEL) {
        // Halt if button pressed after still time
        if (millis() - start > MAX_IGNITE*1000U) {
            if (digitalRead(CTRL) == LOW) {
                // Wait for debounce and intent
                delay(500);

                // Cancel launch if button still pressed
                if (digitalRead(CTRL) == LOW) {
                    state = HALT;
                    return;
                }
            }

            if (communication_recv() == 'c') {
                state = HALT;
                return;
            }
        }

        communication_updateTelemetry();
    }

    // End ignition signal
    digitalWrite(TERM_IGNITE, LOW);

    // Change to burn
    state = BURN;
}

void burn() {
    // Debug 3 Yellow - burning
    bitSet(debug, 8);
    communication_writeDebug();

    // Update base station state
    communication_send('s', "b", 1);

    // Update telemetry during burn
    while (acc.z > THRUST_ACCEL) {
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
    communication_writeDebug();

    // Update base station state
    communication_send('s', "c", 1);

    // Update telemetry during coast
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
    communication_writeDebug();

    // Update base station state
    communication_send('s', "d", 1);

    // Send parachute signal
    digitalWrite(TERM_DROGUE, HIGH);
    delay(DELAY_PARACHUTE);
    digitalWrite(TERM_DROGUE, LOW);

    // Change to wait
    state = WAIT;
}

void wait() {
    // Debug 4 Blue - waiting
    bitSet(debug, 6);
    communication_writeDebug();

    // Update base station state
    communication_send('s', "w", 1);

    // Update telemetry during descent
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
    communication_writeDebug();

    // Update base station state
    communication_send('s', "e", 1);

    // Send parachute signal
    digitalWrite(TERM_MAIN, HIGH);
    delay(DELAY_PARACHUTE);
    digitalWrite(TERM_MAIN, LOW);

    // Change to fall
    state = FALL;
}

void fall() {
    // Debug 4 Green - falling
    bitClear(debug, 4);
    communication_writeDebug();

    // Update base station state
    communication_send('s', "l", 1);

    // Update telemetry during descent
    while (bar.dp > MIN_DPRES) {
        communication_updateTelemetry();
    }

    // Change to recover
    state = RECOVER;
}

void recover() {
    // Arm Green - recovery
    bitClear(debug, 3);
    communication_writeDebug();

    // Update base station state
    communication_send('s', "r", 1);

    // Clear interrupts and put processor to sleep
    cli();
    sleep_enable();
    sleep_cpu();
}

void state_init() {
    pinMode(CTRL, INPUT_PULLUP);

    pinMode(TERM_MAIN, OUTPUT);
    digitalWrite(TERM_MAIN, LOW);
    pinMode(TERM_DROGUE, OUTPUT);
    digitalWrite(TERM_DROGUE, LOW);
    pinMode(TERM_IGNITE, OUTPUT);
    digitalWrite(TERM_IGNITE, LOW);

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