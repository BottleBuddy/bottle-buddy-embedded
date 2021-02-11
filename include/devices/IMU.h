/**
 * @file IMU.h
 * 
 * @brief Functions related to the Inertial Measurement Unit.
 */

#pragma once

#include <Arduino_LSM9DS1.h>

/**
 * @brief Inertial Measurement Unit setup.
 * 
 * Call once to set up all three facets of the IMU.
 */
int imu_sensor_setup();

/**
 * @brief Grab x, y, and z accelerometer values.
 * 
 * Spins while measurements are not available, then writes accelerometer
 * values to the memory locations specified by parameters.
 */
void read_accelerometer(float& x, float& y, float& z);
