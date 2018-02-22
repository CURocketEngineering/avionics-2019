#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "datalog.h"
#include "ninedof.h"
#include "pins.h"

void datalog_setup() {
    SD.begin(BUILTIN_SDCARD);
}

void datalog_print() {
    File file = SD.open("box.json", FILE_WRITE);

    String str;

    ninedof_json.printTo(str);
    file.println(str);

    file.close();
}
