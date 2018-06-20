/*
 * Declaration file for communication functions
 */

/* DEFINE */
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

/* LIBRARIES */
#include <Arduino.h>

#include "state.h"

#define COMM_COUNT 64
#define COMM_DELAY 200

enum command_e {
  CMD_NONE,
  CMD_TEST,
  CMD_ARM,
  CMD_DISARM,
  CMD_IGNITE,
  CMD_ABORT,
  CMD_PASS,
  CMD_FAIL,
  NO_COMM
};

extern const char ** commands;

/* DEFINE FUNCTIONS */
void communication_sendResult(bool pass);
void communication_sendState(enum state_e state);
enum command_e communication_recvCommand();
void communication_updateTelemetry();
void communication_init();

#endif
