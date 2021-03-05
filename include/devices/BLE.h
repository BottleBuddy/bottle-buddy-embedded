/**
 * @file BLE.h
 * 
 * Functions related to the bluetooth low energy device.
 */

#pragma once

#include <ArduinoBLE.h>
#include <arduino-timer.h>

extern Timer<> ble_timer;

/**
 * @brief Bluetooth Low Energy device setup.
 * 
 * Call once to set up BLE device.
 */
int ble_device_setup();

int advertise_ble();

String wait_for_ble_connection();
