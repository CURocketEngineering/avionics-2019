/*
 * Definition file for state functions
 */

/* LIBRARIES */
#include <Arduino.h>
#include "statefunctions.h"
#include "global.h"

String eeprom_header = EEPROM_HEADER;
int eeprom_state = eeprom_header.length();
int eeprom_debug = eeprom_header.length() + sizeof(state);
int eeprom_ground = eeprom_header.length() + sizeof(state) + sizeof(debug);

void idle() {
     while (state == IDLE) {
          // Update base station state
          sendBase('s', "h", 1);

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
          switch (recvBase()) {
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
          sendDebug();

          delay(IDLE_DELAY);
     }
}

void halt() {
     // Debug 1-4 Red - halt
     debug = 0b0010010010010000;
     sendDebug();

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
     sendDebug();

     // Update base station state
     sendBase('s', "t", 1);

     char cmd;

     while ((cmd = recvBase()) == 'h') {
          updateTelemetry();
     }

     if (cmd == 'e') {
          sendBase('t', "p", 1);
          // Debug 2 Green - pass test
          bitSet(debug, 11);
    }
    else {
         sendBase('t', "f", 1);
         // Debug 2 Red - fail test
         bitSet(debug, 10);
    }

    sendDebug();

    // Wait for lights to be read
    delay(2000);

    // Turn off lights
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

    while (state == ARM) {
         // Update base station state
         sendBase('s', "a", 1);
         updateTelemetry();

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
        switch (recvBase()) {
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
     sendDebug();

     // Update base station state
     sendBase('s', "i", 1);

     // Store ground in EEPROM
     EEPROM.put(eeprom_ground, bar.gnd);

     // Send ignition signal
     digitalWrite(TERM_IGNITE, HIGH);

     // Ignition time for measuring
     unsigned long start = millis();

     // Wait for rocket to move up
     updateTelemetry();
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

               if (recvBase() == 'c') {
                    state = HALT;
                    return;
               }
          }

          updateTelemetry();
     }

     // End ignition signal
     digitalWrite(TERM_IGNITE, LOW);

     // Change to burn
     state = BURN;
}

void burn() {
     // Debug 3 Yellow - burning
     bitSet(debug, 8);
     sendDebug();

     // Update base station state
     sendBase('s', "b", 1);

     // Update telemetry during burn
     while (acc.z > THRUST_ACCEL) {
          updateTelemetry();
     }

     // Change to coast
     state = COAST;
}

void coast() {
     // Debug 3 Blue - coasting
     bitClear(debug, 7);
     bitClear(debug, 8);
     bitSet(debug, 9);
     sendDebug();

     // Update base station state
     sendBase('s', "c", 1);

     // Update telemetry during coast
     while (bar.dp < APOGEE_DPRES){
          updateTelemetry();
     }

     // Change to apogee
     state = APOGEE;
}

void apogee() {
     // Debug 3 Green - apogee
     bitClear(debug, 9);
     bitSet(debug, 8);
     sendDebug();

     // Update base station state
     sendBase('s', "d", 1);

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
     sendDebug();

     // Update base station state
     sendBase('s', "w", 1);

     // Update telemetry during descent
     while (bar.alt > MAIN_ALT + bar.gnd) {
          updateTelemetry();
     }

     // Change to eject
     state = EJECT;
}

void eject() {
     // Debug 4 Yellow - ejecting
     bitClear(debug, 6);
     bitSet(debug, 4);
     bitSet(debug, 5);
     sendDebug();

     // Update base station state
     sendBase('s', "e", 1);

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
     sendDebug();

     // Update base station state
     sendBase('s', "l", 1);

     // Update telemetry during descent
     while (bar.dp > MIN_DPRES) {
          updateTelemetry();
     }

     // Change to recover
     state = RECOVER;
}

void recover() {
     // Arm Green - recovery
     bitClear(debug, 3);
     sendDebug();

     // Update base station state
     sendBase('s', "r", 1);

     // Clear interrupts and put processor to sleep
     cli();
     sleep_enable();
     sleep_cpu();
}
