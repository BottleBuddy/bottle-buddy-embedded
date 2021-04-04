/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>
#include "Pipeline/serviceManager.h"
#include "Pipeline/pipe.h"
#include "devices/ToF.h"
#include "devices/IMU.h"
#include "devices/FSR.h"
#include "devices/BLE.h"

BottleBuddy::Embedded::Pipeline::Pipe *waterLevelPipe;
BottleBuddy::Embedded::Pipeline::Pipe *accelerometerPipe;
BottleBuddy::Embedded::Pipeline::Pipe *gyroscopePipe;
BottleBuddy::Embedded::Pipeline::Pipe *magnetometerPipe;
BottleBuddy::Embedded::Pipeline::Pipe *fsr1Pipe;
BottleBuddy::Embedded::Pipeline::Pipe *fsr2Pipe;

BottleBuddy::Embedded::Pipeline::ServiceManager *serviceManager;

/**
 * @brief Serial speed
 */
constexpr int serialSpeed = 115200;

/**
 * @brief Setup loop.
 * 
 * Makes necessary initializations for system to be able to run.
 */
void setup() {
  
  Serial.begin(serialSpeed, SERIAL_8N1);

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

  serviceManager = new BottleBuddy::Embedded::Pipeline::ServiceManager();
  serviceManager->addService(new BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService("19B10010-E8F2-537E-4F6C-D104768A1214"));
  serviceManager->addService(new BottleBuddy::Embedded::Pipeline::Services::CleaningService("19B10020-E8F2-537E-4F6C-D104768A1214"));

  int advertising_success = advertise_ble();

  waterLevelPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::ToF);
  accelerometerPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER);
  gyroscopePipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::GYRO);
  magnetometerPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::MAGNETIC);
  fsr1Pipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::FSR1);
  fsr2Pipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::FSR2);
}

/** 
 * @brief Main loop.
 * 
 *  This loop currently grabs a ToF measurement value and sends it down the water level pipe, as well as a 3-dimensional accelerometer reading.
 */
void loop() {
  int tofVal = tof_sensor_distance();
  waterLevelPipe->sendPayload<int>(tofVal);

  float x, y, z;
  read_accelerometer(x, y, z);
  accelerometerPipe->sendPayload<float>(x, y, z);
  read_gyroscope(x, y, z);
  gyroscopePipe->sendPayload<float>(x, y, z);
  read_magnetometer(x, y, z);
  magnetometerPipe->sendPayload<float>(x, y, z);

  int fsrVal = read_fsr_1();
  fsr1Pipe->sendPayload<int>(fsrVal);
  fsrVal = read_fsr_2();
  fsr2Pipe->sendPayload<int>(fsrVal);

  serviceManager->loopServices();
}
