/*
 * Main v0.5
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

/* LIBRARIES */
#include "debug.h"
#include "ninedof.h"
#include "state.h"
#include "barometer.h"
#include "communication.h"
#include "gps.h"

/* MAIN PROGRAM FUNCTIONS */

void setup() {
    // Initialize debug leds
    debug_init();

    // Initialize communication with the ninedof sensor
    ninedof_init();

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
    gyro.x = gyro.y = gyro.z = 0;
    acc.x = acc.y = acc.z = 0;
    mag.x = mag.y = mag.z = 0;

    bar.p = 808;
    bar.dp = 0;

    bar.alt = 0;
    bar.gnd = 0;

    bar.temp = 0;
    bar.hum = 0;

    // Sample filter seed data
    unsigned long gyro_x = 0, gyro_y = 0, gyro_z = 0;
    unsigned long acc_x = 0, acc_y = 0, acc_z = 0;
    unsigned long mag_x = 0, mag_y = 0, mag_z = 0;
    unsigned long bar_p = 0, bar_dp = 0;
    unsigned long bar_alt = 0;
    unsigned long bar_temp = 0, bar_hum = 0;

    for (byte cnt = 0; cnt < SENSOR_INIT; cnt++) {
        ninedof_read(false);

        gyro_x += gyro.x;
        gyro_y += gyro.y;
        gyro_z += gyro.z;

        acc_x += acc.x;
        acc_y += acc.y;
        acc_z += acc.z;

        mag_x += mag.x;
        mag_y += mag.y;
        mag_z += mag.z;

        barometer_read(false);

        bar_p += bar.p;
        bar_dp += bar.dp;

        bar_alt += bar.alt;

        bar_temp += bar.temp;
        bar_hum += bar.hum;

        delay(SENSOR_DELAY);
    }

    gyro.x = gyro_x/SENSOR_INIT;
    gyro.y = gyro_y/SENSOR_INIT;
    gyro.z = gyro_z/SENSOR_INIT;

    acc.x = acc_x/SENSOR_INIT;
    acc.y = acc_y/SENSOR_INIT;
    acc.z = acc_z/SENSOR_INIT;

    mag.x = mag_x/SENSOR_INIT;
    mag.y = mag_y/SENSOR_INIT;
    mag.z = mag_z/SENSOR_INIT;

    bar.p = bar_p/SENSOR_INIT;
    bar.dp = bar_dp/SENSOR_INIT;

    bar.alt = bar_alt/SENSOR_INIT;

    bar.temp = bar_temp/SENSOR_INIT;
    bar.hum = bar_hum/SENSOR_INIT;

    gps_read();

    state_init();
}

void loop() {
    state_loop();
}
