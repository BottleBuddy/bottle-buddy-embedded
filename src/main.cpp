/*************************************************************
 This is the official test software for the bottle buddy hardware
 Written by Zane Freeman and Jason Siegel
 *************************************************************
  Note: This requires the following libraries:
          Adafruit_VL53L0X Library
          AdafruitBLE Library
          Arduino_LSM9DS1 Library
 *************************************************************
 To run the BLE functions, download LightBlue App on Google Play/AppStore
 *************************************************************/

/*HEADER FILES FOR SOFTWARE*/
#include <Arduino.h>
#include <Wire.h>
#include <ArduinoBLE.h>
/**************************/

/*BLE Service and Characteristic Definitions*/
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
//
BLEByteCharacteristic waterCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
/*******************************************/

void setup()
{

  /**
   * @brief Initialization of BLE Sensor
   * 
   * This is the basic setup for the BLE module
   */

  if (!BLE.begin())
  {
    Serial.println("Failed to initialize BLE!");
    while (1)
      ;
  }

  // set the local name peripheral advertises
  BLE.setLocalName("BBUDDY");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);
  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(waterCharacteristic);
  // add the service
  BLE.addService(ledService);
  //starting values for both charactersistics
  ledCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);
  waterCharacteristic.writeValue(0);
  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
  ble_init();
  BLE.poll();
  ble_data();
}
