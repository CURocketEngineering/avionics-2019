/*
 * Main v0.4
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
#include "global.h"
#include "accelerometer.h"
#include "state.h"
#include "barometer.h"
#include "communication.h"
#include "gps.h"

/* MAIN PROGRAM FUNCTIONS */

void setup() {
    // Initialize communication with the barometer
    barometer_init();

    // Initialize communication with GPS receiver
    gps_init();

    // Initialize communication with the base station
    communication_init();

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
        accelerometer_read(false);
        acc_x += acc.x;
        acc_y += acc.y;
        acc_z += acc.z;

        barometer_read(false);
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

    gps_read();

    state_init();
}

void loop() {
    state_loop();
}
