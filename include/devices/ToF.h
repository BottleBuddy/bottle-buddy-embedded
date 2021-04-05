/**
 * @file ToF.h
 * 
 * @brief Functions related to time of flight sensor.
 */

#pragma once

#include <Adafruit_VL53L0X.h>

/**
 * @brief Baud rate. 
 */
const int BAUD_RATE = 115200;

/**
 * @brief ToF Camera Sensor object.
 * 
 * Used to interface with the physical sensor.
 */
extern Adafruit_VL53L0X lox;

/**
 * @brief ToF Camera Sensor setup.
 * 
 * Call once to setup the physical sensor.
 */
int tof_sensor_setup();

/**
 * @brief Grab ToF sensor measurement.
 * 
 * Can be called continuously to grab the current reading on the ToF sensor.
 */
int tof_sensor_distance();
