#ifndef CONFIG_H_
#define CONFIG_H_

// Header bytes for EEPROM
#define EEPROM_HEADER "MainRev7"

// Time to hold pin high for parachute charge
#define PARACHUTE_DELAY 1000
// Accelerometer values to determine changes in state
#define MIN_ACCEL 2.0
#define THRUST_ACCEL 0.0
// Barometer values to determine changes in state
#define APOGEE_DPRES 1.0
#define MIN_DPRES 1.0

// Time to wait before deploying parachute
#define PARACHUTE_WAIT 0
// Altitude to deploy main parachute
#define MAIN_ALT 1500.0

// Direction of 9DoF up vector
#define NINEDOF_UP acc.z

#endif
