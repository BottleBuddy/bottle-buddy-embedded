#include "devices/ToF.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();;

int tof_sensor_setup() {
    if (!lox.begin(BAUD_RATE)) {
        return -1;
    }
    return 0;
}

int tof_sensor_distance() {
    VL53L0X_RangingMeasurementData_t value;
    lox.rangingTest(&value, false);
    if (value.RangeStatus != 4) {
        return value.RangeMilliMeter;
    }
    return 0;
}
