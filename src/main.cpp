/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>
#include "Pipeline/pipeFactory.h"
#include <Adafruit_VL53L0X.h>

/**
 * @brief variable declaration for the ToF Camera Sensor 
 */

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
/**
 * @brief variable for the Baud Rate 
 */

const int BAUD_RATE = 115200;

BottleBuddy::Embedded::Pipeline::Pipe *waterLevelPipe;

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

  if (!lox.begin(BAUD_RATE))
  {
    Serial.println("Failed to initialize VL53L0X!");
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
  int payload = tof_sensor_distance();

  waterLevelPipe->sendPayload<int>(payload);
}

int tof_sensor_distance()
{
  VL53L0X_RangingMeasurementData_t value;
  lox.rangingTest(&value, false);
  if (value.RangeStatus != 4)
  {
    return value.RangeMilliMeter;
  }
  return 0;
}