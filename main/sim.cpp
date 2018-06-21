#include "sim.h"

#ifdef SIM
#include "barometer.h"
#include "ninedof.h"

#include "flight.h"

unsigned long sim_start = 0;
const struct flight_s * sim_cur = &flight[0];

void sim_init() {
}

void sim_updateTelemetry() {
    if (sim_start == 0)
        return;

    while (sim_cur != &flight[sizeof(flight)/sizeof(flight[0]) - 1] && millis() - sim_start > (sim_cur + 1)->time) {
        sim_cur++;
    }
}

void sim_getBarometer() {
    bar.p = sim_cur->bar_p;
    bar.alt = sim_cur->bar_alt;
    bar.temp = sim_cur->bar_temp;
    bar.hum = sim_cur->bar_hum;
}

void sim_getGyro() {
    gyro.x = sim_cur->gyro_x;
    gyro.y = sim_cur->gyro_y;
    gyro.z = sim_cur->gyro_z;
}

void sim_getAccel() {
    acc.x = sim_cur->acc_x;
    acc.y = sim_cur->acc_y;
    acc.z = sim_cur->acc_z;
}

void sim_getMag() {
    mag.x = sim_cur->mag_x;
    mag.y = sim_cur->mag_y;
    mag.z = sim_cur->mag_z;
}

#endif
