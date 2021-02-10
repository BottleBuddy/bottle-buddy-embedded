/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>
#include <Wire.h>
#include <ArduinoBLE.h>
#include <features/BottleBuddyBLE.h>
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
#include "Pipeline/pipeFactory.h"

BottleBuddy::Embedded::Pipeline::Pipe *waterLevelPipe;

void loop()
{
  ble_init();
  BLE.poll();
  send_test_data();
}
/**
 * @brief Serial speed
 */
constexpr int serialSpeed = 115200;

/**
 * @brief Delay time
 */
constexpr int delayTime = 1000;

constexpr int ledPin = 2;

/**
 * @brief Setup loop.
 * 
 * Makes necessary initializations for system to be able to run.
 */
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(serialSpeed, SERIAL_8N1);
  if (!BLE.begin())
  {
    Serial.println("Failed to initialize BLE!");
    while (1)
      ;
  }

  //Using pipeline api. First step is to use pipeline factory to create pipes around "locations" within the Bottle Buddy
  waterLevelPipe = BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location::WATER_LEVEL);
}

/** 
 * @brief Main loop.
 * 
 *  This loop blinks an LED for demonstration purposes.
 */
void loop()
{
  digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
  Serial.print(1);
  Serial.print("\t");
  delay(delayTime);          // wait for a second
  digitalWrite(ledPin, LOW); // turn the LED off by making the voltage LOW
  Serial.print(0);
  Serial.print("\t");
  delay(delayTime); // wait for a second

  //Using pipeline api. In loop, when you have gotten a new reading from the distance sensor, simply send the payload using the pipe you constructed in setup. In this case, we send a reading of "1" every time.
  int payload = 1;
  waterLevelPipe->sendPayload<int>(payload);
}
