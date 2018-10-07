# On-board avionics

### Making
#### Compiling and Running
1. `make ?` to compile and run
#### Debugging
1. Debug todo

### Components
#### Current Components
* General Hardware
  * Teensy 3.5 Arduino
* Sensors
  1. 9DOF - 3D gyroscope, magnetometer, and accelerometer
     * [sparkfun link](https://www.sparkfun.com/products/13944)
       * schematic and eagle files provided
     * I2C interface
  2. BME280 - altitude, pressure, and temperature
     * [sparkfun link](https://www.sparkfun.com/products/13676)
       * schematic and eagle files provided
     * 3.3V SPI interface and 5V tolerant I2C
     * 10 pins, < 6 used
  3. GPS - What gps did we use?? Are we still using it???
#### Past Components

### Order of Operations
* Setup():main.ino
  1. debug_init():
  2. ninedof_init():
  3. barometer_init():
  4. gps_init():
  5. datalog_init():
  6. communication_init():
  7. Sensor Setup
* Loop():main.ino
  * state_loop():state.cpp
    1. init():state.cpp
       * communication_sendstate(IDLE):?
       * 
    2. idle():state.cpp
    3. halt():state.cpp
    4. test():state.cpp
    5. arm():state.cpp
    6. ignite():state.cpp
    7. burn():state.cpp
    8. coast():state.cpp
    9. apogee():state.cpp
    10. wait():state.cpp
    11. eject():state.cpp
    12. fall():state.cpp
    13. recover():state.cpp
    14. EEPROM.put():?
