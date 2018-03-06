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

enum command_e {
  NO_COMM,
  CMD_NONE,
  CMD_TEST,
  CMD_ARM,
  CMD_DISARM,
  CMD_IGNITE,
  CMD_ABORT,
  CMD_PASS,
  CMD_FAIL
};

static const char * commands[] = {
  "",
  "none",
  "test",
  "arm",
  "disarm",
  "ignite",
  "abort",
  "pass",
  "fail"
};

/* DEFINE FUNCTIONS */
void communication_sendResult(bool pass);
void communication_sendState(enum state_e state);
enum command_e communication_recvCommand();
void communication_updateTelemetry();
void communication_init();

#endif
