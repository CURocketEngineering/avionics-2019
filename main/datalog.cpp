/*
 * Definition file for datalogger
 */

/* LIBRARIES */
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "datalog.h"
#include "ninedof.h"
#include "pins.h"

void datalog_setup() {
    /* SETTING UP SD CARD */
    Serial.print("Initializing SD card");

    // Check if initialized
    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed or not present");
        return;
    }
    else {
        Serial.println("Card initialized");
    }
}

void datalog_print() {
    // Open file
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // Temporary string
    String jsonStr;

    // If file available, write to it
    if (dataFile) {
        ninedof_json.printTo(jsonStr);
        dataFile.println(jsonStr);
        // dataFile.close(); << HAVE TO INCLUDE SOMEWHERE but I don't know where

        // Print to serial port (remove maybe)
        ninedof_json.printTo(Serial);
    }
    else {
        Serial.println("Error opening datalog.txt.");
    }
}
