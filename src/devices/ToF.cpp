#include "devices/ToF.h"
#include <Arduino.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int tof_sensor_setup() {
    if (!lox.begin(BAUD_RATE)) {
        delay(100);
        if (lox.begin(BAUD_RATE)) {
            return 0;
        }
        return -1;
    }
    return 0;
}

uint16_t tof_sensor_distance() {
    VL53L0X_RangingMeasurementData_t value;
    lox.rangingTest(&value, false);
    if (value.RangeStatus != 4) {
        return value.RangeMilliMeter;
    }
    return 0;
}
