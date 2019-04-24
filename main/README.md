# On-board avionics
### Usage
#### Compiling and Running
1. `make` to compile and run
#### Debugging
1. *todo*
#### Configuration
Configuration for several parameters can be made via `config.h`.

### Components
#### Current Components
* General Hardware
  * Teensy 3.5 Arduino
  * ![Teensy](../pics/Teensy_3.5.jpg)
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
  3. GPS 

### Wishlist
1. XBEE Voice Messages
2. Live video feed
