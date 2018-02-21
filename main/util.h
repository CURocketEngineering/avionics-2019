/*
 * Declaration file for barometer
 */

/* DEFINE */
#ifndef UTIL_H_
#define UTIL_H_

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>

/* DEFINE FUNCTIONS */
const char * util_getField(const char * sentence, char * buf, unsigned short len);
void util_readLine(Stream & stream, char * buf, unsigned short len, bool crlf = true);

#endif
