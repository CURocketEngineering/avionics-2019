/*
 * Definition file for 9 degrees of freedom sensor
 */

/* LIBRARIES */
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "ninedof.h"

void ninedof_setup() {
    // These take place after calling imu.begin()
    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;

    gyro_dev_prev.x, gyro_dev_prev.z = 0.0;

    if (!imu.begin()) {
        Serial.println("Failed to communicate with LSM9DS1.");
    }
}

void ninedof_readData() {
    // If each sensor is available, read them
    if (imu.gyroAvailable()) {
        gyro_prev.x = imu.gx;
        gyro_prev.z = imu.gz;
        imu.readGyro();

        // Check if change in rotational velocity is > 1 deg/s
        if ((gyro_prev.x >= (imu.gx + 1.0)) || (gyro_prev.x <= (imu.gx - 1.0))) {
            gyro_dev.x = gyro_prev.x - imu.gx;
        }
        if ((gyro_prev.z >= (imu.gz + 1.0)) || (gyro_prev.z <= (imu.gz - 1.0))) {
            gyro_dev.z = gyro_prev.z - imu.gz;
        }
    }
    if (imu.accelAvailable()) {
        imu.readAccel();
    }
    if (imu.magAvailable()) {
        imu.readMag();
    }
}

void ninedof_printData() {
    // Print gyroscope
    Serial.print("Gyro: ");
    Serial.print(imu.calcGyro(imu.gx), 5);
    Serial.print(", ");
    Serial.print(imu.calcGyro(imu.gy), 5);
    Serial.print(", ");
    Serial.print(imu.calcGyro(imu.gz), 5);
    Serial.println(" deg/s");
    Serial.print("Deviation: ");
    Serial.print("X: ");
    Serial.print(ninedof_calcDeviationX(gyro_dev.x), 5);
    Serial.print(" Z: ");
    Serial.print(ninedof_calcDeviationZ(gyro_dev.z), 5);
    Serial.print(" deg/s");

    // Print accelerometer
    Serial.print("Accel: ");
    Serial.print(imu.calcAccel(imu.ax), 5);
    Serial.print(", ");
    Serial.print(imu.calcAccel(imu.ay), 5);
    Serial.print(", ");
    Serial.print(imu.calcAccel(imu.az), 5);
    Serial.println(" g");

    // Print magnetometer
    Serial.print("Mag: ");
    Serial.print(imu.calcMag(imu.mx), 5);
    Serial.print(", ");
    Serial.print(imu.calcMag(imu.my), 5);
    Serial.print(", ");
    Serial.print(imu.calcMag(imu.mz), 5);
    Serial.println(" gauss");
}

void ninedof_calcAttitude(float ax, float ay, float az,
                          float mx, float my, float mz) {
    float roll = atan2(ay, az);
    float pitch = atan2(-ax, sqrt(ay * ay + az * az));

    float heading;
    if (my == 0)
      heading = (mx < 0) ? PI : 0;
    else
      heading = atan2(mx, my);

    heading -= DECLINATION * PI / 180;

    if (heading > PI) heading -= (2 * PI);
    else if (heading < -PI) heading += (2 * PI);
    else if (heading < 0) heading += 2 * PI;

    // Convert everything from radians to degrees:
    heading *= 180.0 / PI;
    pitch *= 180.0 / PI;
    roll  *= 180.0 / PI;
}

float ninedof_calcDeviationX(float gx) {
    // Check if the deviation has changed
    if (gyro_dev_prev.x != gx) {
        gyro_dev_prev.x = gx;
        return gx;
    }
    else {
        return 0.0; // If there is no change in deviation
    }
}

float ninedof_calcDeviationZ(float gz) {
    // Check if the deviation has changed
    if (gyro_dev_prev.z != gz) {
        gyro_dev_prev.z = gz;
        return gz;
    }
    else {
        return 0.0; // If there is no change in deviation
    }
}
