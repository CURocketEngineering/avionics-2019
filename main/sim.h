#ifdef SIM

/*
 * Declaration file for simulation
 */

/* DEFINE */
#ifndef SIM_H_
#define SIM_H_

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

struct flight_s {
    long time;
    float bar_p, bar_alt, bar_temp, bar_hum;
    float gyro_x, gyro_y, gyro_z;
    float acc_x, acc_y, acc_z;
    float mag_x, mag_y, mag_z;
};

void sim_init();
void sim_updateTelemetry();

#endif

#endif
