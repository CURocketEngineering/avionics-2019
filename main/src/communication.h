/*
 * Declaration file for communication functions
 */

/* LIBRARIES */
#include <Arduino.h>
#include "global.h"

/* DEFINE */
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

     /* DEFINE FUNCTIONS */
     void sendBase(char type, const char * data, unsigned int len);
     char recvBase();
     void sendDebug();
     void updateTelemetry();

#endif
