/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>
#include <ArduinoBLE.h>
#include "Pipeline/Services/demoService.h"
#include "Pipeline/pipeFactory.h"
#include "devices/ToF.h"
#include "devices/IMU.h"
#include "devices/BLE.h"

BottleBuddy::Embedded::Pipeline::Pipe *waterLevelPipe;
BottleBuddy::Embedded::Pipeline::Pipe *accelerometerPipe;

BottleBuddy::Embedded::Pipeline::Services::DemoService *demoService;

const int GREEN_LED_PIN = 4;
const int RED_LED_PIN = 3;
const int BLUE_LED_PIN = 2;

/**
 * @brief Setup loop.
 * 
 * Makes necessary initializations for system to be able to run.
 */
void setup() {
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(RED_LED_PIN, HIGH);
  if(tof_sensor_setup() == -1) {
    while(1)
      ;
  }
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, HIGH);

  if(imu_sensor_setup() == -1) {
    while(1)
      ;
  }

  if(ble_device_setup() == -1) {
    while(1)
      ;
  }
  
  demoService = new BottleBuddy::Embedded::Pipeline::Services::DemoService("19B10010-E8F2-537E-4F6C-D104768A1214");
  int advertising_success = advertise_ble();

  waterLevelPipe = BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location::ToF);
  accelerometerPipe = BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER);
  
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

/** 
 * @brief Main loop.
 * 
 *  This loop currently grabs a ToF measurement value and sends it down the water level pipe, as well as a 3-dimensional accelerometer reading.
 */
void loop() {
  String central_address = wait_for_ble_connection();
  BLE.poll();

  demoService->loop();

  uint16_t tof_reading = tof_sensor_distance();
  waterLevelPipe->sendPayload<uint16_t>(tof_reading);

  float x, y, z;
  read_accelerometer(x, y, z);
  accelerometerPipe->sendPayload<float>(x, y, z);
  delay(100);
}
