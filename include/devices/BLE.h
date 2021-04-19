/**
 * @file BLE.h
 * 
 * Functions related to the bluetooth low energy device.
 */

#pragma once

#include <ArduinoBLE.h>

/**
 * @brief Bluetooth Low Energy device setup.
 * 
 * Call once to set up BLE device.
 */
int ble_device_setup();

/**
 * @brief Advertise the BLE device
 * 
 * Used when ready to advertise BLE.
 */
int advertise_ble();

String wait_for_ble_connection();
