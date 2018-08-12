#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "datalog.h"

void datalog_init() {
    SD.begin(BUILTIN_SDCARD);

    datalog_write("---");
}

void datalog_write(const String & str) {
    File file = SD.open("datalog.box", FILE_WRITE);
    file.println(str);
    file.close();
}
