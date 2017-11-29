/*
 * Definition file for barometer
 */

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>
#include "global.h"
#include "barometer.h"

void barometerWrite(byte address, byte data) {
     Wire.beginTransmission(BARO_ADDR);
     Wire.write(address);
     Wire.write(data);
     Wire.endTransmission();
}

void barometerRead(byte address, byte * buf, unsigned short len) {
     Wire.beginTransmission(BARO_ADDR);
     Wire.write(address);
     Wire.endTransmission();
     Wire.requestFrom(BARO_ADDR, len);
     for(byte idx = 0; idx < len; idx++) {
          buf[idx] = Wire.read();
     }
}

short barometerReadInt(byte address) {
     static byte buf[2];
     barometerRead(address, buf, 2);
     return ((short)buf[0] << 8) | buf[1];
}

unsigned short barometerReadUInt(byte address) {
     static byte buf[2];
     barometerRead(address, buf, 2);
     return ((unsigned short)buf[0] << 8) | buf[1];
}

void initBarometer() {
     // Datasheet magic
     bar_init.AC1 = barometerReadInt(0xAA);
     bar_init.AC2 = barometerReadInt(0xAC);
     bar_init.AC3 = barometerReadInt(0xAE);
     bar_init.AC4 = barometerReadUInt(0xB0);
     bar_init.AC5 = barometerReadUInt(0xB2);
     bar_init.AC6 = barometerReadUInt(0xB4);
     bar_init.VB1 = barometerReadInt(0xB6);
     bar_init.VB2 = barometerReadInt(0xB8);
     bar_init.MB = barometerReadInt(0xBA);
     bar_init.MC = barometerReadInt(0xBC);
     bar_init.MD = barometerReadInt(0xBE);

     bar_init.c3 = 160.0*pow(2, -15)*bar_init.AC3;
     bar_init.c4 = pow(10, -3) * pow(2, -15)*bar_init.AC4;
     bar_init.b1 = pow(160, 2)*pow(2, -30)*bar_init.VB1;
     bar_init.c5 = (pow(2, -15)/160)*bar_init.AC5;
     bar_init.c6 = bar_init.AC6;
     bar_init.mc = (pow(2, 11)/pow(160, 2))*bar_init.MC;
     bar_init.md = bar_init.MD/160.0;
     bar_init.x0 = bar_init.AC1;
     bar_init.x1 = 160.0*pow(2, -13)*bar_init.AC2;
     bar_init.x2 = pow(160, 2)*pow(2, -25)*bar_init.VB2;
     bar_init.y0 = bar_init.c4*pow(2, 15);
     bar_init.y1 = bar_init.c4*bar_init.c3;
     bar_init.y2 = bar_init.c4*bar_init.b1;
     bar_init.p0 = (3791.0 - 8.0)/1600.0;
     bar_init.p1 = 1.0 - 7357.0*pow(2, -20);
     bar_init.p2 = 3038.0*100.0*pow(2, -36);
}

void readBarometer(bool filter, bool debug, float p) {
     static byte buf[3];
     static float tu, a, pu, s, x, y, z;

     static float pressure, temperature;

     // Start temperature reading
     barometerWrite(0xF4, 0x2E);

     // Wait on reading
     delay(10);

     // Retrieve reading
     barometerRead(0xF6, buf, 2);

     // Calculate temperature
     tu = (buf[0]*256.0) + buf[1];
     a = bar_init.c5*(tu - bar_init.c6);
     temperature = a + (bar_init.mc/(a + bar_init.md));

     // Start pressure reading
     barometerWrite(0xF4, 0xF4);

     // Wait on reading
     delay(30);

     // Retrieve reading
     barometerRead(0xF6, buf, 3);

     // Calculate pressure
     pu = (buf[0]*256.0) + buf[1] + (buf[2]/256.0);
     s = temperature - 25.0;
     x = (bar_init.x2*pow(s, 2)) + (bar_init.x1*s) + bar_init.x0;
     y = (bar_init.y2*pow(s, 2)) + (bar_init.y1*s) + bar_init.y0;
     z = (pu - x) / y;
     pressure = (bar_init.p2*pow(z, 2)) + (bar_init.p1*z) + bar_init.p0;

     // Calculate compensated pressure
     bar_prev = bar;

     if (!debug) {
          bar.p = pressure;
     }
     else {
          bar.p = p;
     }

     if (filter) {
          bar.p = BARO_GAIN*bar.p + (1.0 - BARO_GAIN)*bar_prev.p;
     }

     bar.dp = bar.p - bar_prev.p;

     // Calculate altitude
     bar.alt = 145440.0*(1 - pow(pressure/NWS_ALTI, 1/5.255));
}
