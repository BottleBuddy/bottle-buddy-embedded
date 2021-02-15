/**
 * @file BottleBuddyBLE.h
 * 
 * @brief Functions related to the BLE Module.
 */
#include <ArduinoBLE.h>

/*BLE Service and Characteristic Definitions*/
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
//
BLEByteCharacteristic waterCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void ble_init();
int send_test_data();
void advertise();