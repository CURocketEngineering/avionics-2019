#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "datalog.h"

#ifndef BUILTIN_SDCARD
#define BUILTIN_SDCARD 0
#endif

void datalog_init() {
    SD.begin(BUILTIN_SDCARD);

    datalog_write("---");
}

void datalog_write(const String & str) {
    static File file;
    file = SD.open("box.json", FILE_WRITE);
    file.println(str);
    file.close();
}
