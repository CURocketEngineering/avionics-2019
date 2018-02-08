/*
 * Declaration file for communication functions
 */

/* DEFINE */
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

/* LIBRARIES */
#include <Arduino.h>

/* DEFINE FUNCTIONS */
void communication_send(char type, const char * data, unsigned int len);
char communication_recv();
void communication_updateTelemetry();
void communication_init();

#endif
