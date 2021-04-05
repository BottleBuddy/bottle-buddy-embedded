#include "devices/IMU.h"

int imu_sensor_setup() {
    if(!IMU.begin()) {
        return -1;
    }
    return 0;
}

void read_accelerometer(float& x, float& y, float& z) {
    float x_dim, y_dim, z_dim;
    while(!IMU.accelerationAvailable())
        ;
    IMU.readAcceleration(x_dim, y_dim, z_dim);
    x = x_dim;
    y = y_dim;
    z = z_dim;
}

void read_gyroscope(float& x, float& y, float& z) {
    float x_dim, y_dim, z_dim;
    while(!IMU.gyroscopeAvailable())
        ;
    IMU.readGyroscope(x_dim, y_dim, z_dim);
    x = x_dim;
    y = y_dim;
    z = z_dim; 
}

void read_magnetometer(float& x, float& y, float& z) {
    float x_dim, y_dim, z_dim;
    while(!IMU.magneticFieldAvailable())
        ;
    IMU.readMagneticField(x_dim, y_dim, z_dim);
    x = x_dim;
    y = y_dim;
    z = z_dim; 
}
