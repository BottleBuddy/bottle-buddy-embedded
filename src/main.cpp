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
BottleBuddy::Embedded::Pipeline::Pipe *fsrPipe;

BottleBuddy::Embedded::Pipeline::ServiceManager *serviceManager;
BottleBuddy::Embedded::Pipeline::Service* waterIntakeService;

const int GREEN_LED_PIN = LEDG;
const int RED_LED_PIN = LEDR;
const int BLUE_LED_PIN = LEDB;

/**
 * @brief Setup loop.
 * 
 * Makes necessary initializations for system to be able to run.
 */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);

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

  BottleBuddy::Embedded::Pipeline::ServiceManager::setup();
  BottleBuddy::Embedded::Pipeline::ServiceManager::addService(BottleBuddy::Embedded::Pipeline::ServiceType::WATER_INTAKE, "19B10020-E8F2-537E-4F6C-D104768A1214");
  BottleBuddy::Embedded::Pipeline::ServiceManager::addService(BottleBuddy::Embedded::Pipeline::ServiceType::CLEANING, "19B10030-E8F2-537E-4F6C-D104768A1214");

  int advertising_success = advertise_ble();
  BLE.setEventHandler(BLEConnected, BottleBuddy::Embedded::Pipeline::ServiceManager::connectedBLE);
  BLE.setEventHandler(BLEDisconnected, BottleBuddy::Embedded::Pipeline::ServiceManager::disconnectedBLE);

  waterLevelPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::ToF);
  accelerometerPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER);
  gyroscopePipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::GYRO);
  magnetometerPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::MAGNETIC);
  fsrPipe = new BottleBuddy::Embedded::Pipeline::Pipe(BottleBuddy::Embedded::Pipeline::Location::FSR);

  //digitalWrite(BLUE_LED_PIN, HIGH);
}

/** 
 * @brief Main loop.
 * 
 * This loop initiates BLE advertisement and reads all sensor data and sends it down their respective pipe.
 * Additionally, it uses the service manager to keep all active services up to date.
 */
void loop() {
  BLE.poll();

  int tofVal = tof_sensor_distance();
  waterLevelPipe->sendPayload<int>(tofVal);

  float x, y, z;
  read_accelerometer(x, y, z);
  accelerometerPipe->sendPayload<float>(x, y, z);
  read_gyroscope(x, y, z);
  gyroscopePipe->sendPayload<float>(x, y, z);
  read_magnetometer(x, y, z);
  magnetometerPipe->sendPayload<float>(x, y, z);

  int fsr1Val = read_fsr_1();
  int fsr2Val = read_fsr_2();
  fsrPipe->sendPayload<int>(fsr1Val, fsr2Val);

  BottleBuddy::Embedded::Pipeline::ServiceManager::loopServices();
}
