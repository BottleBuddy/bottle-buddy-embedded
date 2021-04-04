/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>
#include "Pipeline/Services/waterIntakeService.h"
#include "Pipeline/Services/cleaningService.h"
#include "Pipeline/pipeFactory.h"
#include "devices/ToF.h"
#include "devices/IMU.h"
#include "devices/BLE.h"

BottleBuddy::Embedded::Pipeline::Pipe *waterLevelPipe;
BottleBuddy::Embedded::Pipeline::Pipe *accelerometerPipe;

BottleBuddy::Embedded::Pipeline::Service *waterIntakeService;
BottleBuddy::Embedded::Pipeline::Service *cleaningService;

/**
 * @brief Setup loop.
 * 
 * Makes necessary initializations for system to be able to run.
 */
void setup() {

  if(tof_sensor_setup() == -1) {
    Serial.println("Failed to initialize VL53L0X!");
    while(1)
      ;
  }

  if(imu_sensor_setup() == -1) {
    Serial.println("Failed to initialize IMU!");
    while(1)
      ;
  }

  if(ble_device_setup() == -1) {
    Serial.println("Failed to initialize BLE!");
    while(1)
      ;
  }

  waterIntakeService = new BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService("19B10010-E8F2-537E-4F6C-D104768A1214");
  cleaningService = new BottleBuddy::Embedded::Pipeline::Services::CleaningService("19B10020-E8F2-537E-4F6C-D104768A1214");
  int advertising_success = advertise_ble();

  waterLevelPipe = BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location::ToF);
  accelerometerPipe = BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER);
}

/** 
 * @brief Main loop.
 * 
 * This loop initiates BLE advertisement and reads all sensor data and sends it down their respective pipe.
 * Additionally, it uses the service manager to keep all active services up to date.
 */
void loop() {
  int payload = tof_sensor_distance();
  waterLevelPipe->sendPayload<int>(payload);

  float x, y, z;
  read_accelerometer(x, y, z);
  accelerometerPipe->sendPayload<float>(x, y, z);
}
