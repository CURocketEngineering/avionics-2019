/*
 * Declaration file for simulation
 */

/* DEFINE */
#ifndef SIM_H_
#define SIM_H_

#define SIM

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

extern unsigned long sim_start;

struct flight_s {
    unsigned long time;
    float bar_p, bar_alt, bar_temp, bar_hum;
    float gyro_x, gyro_y, gyro_z;
    float acc_x, acc_y, acc_z;
    float mag_x, mag_y, mag_z;
};

#ifdef SIM
void sim_init();
void sim_updateTelemetry();
void sim_getBarometer();
void sim_getGyro();
void sim_getAccel();
void sim_getMag();
#endif

#endif
