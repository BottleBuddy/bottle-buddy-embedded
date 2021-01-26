/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>
#include "Pipeline/pipeFactory.h"

BottleBuddy::Embedded::Pipeline::Pipe* waterLevelPipe;

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
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(serialSpeed, SERIAL_8N1);

  //Using pipeline api. First step is to use pipeline factory to create pipes around "locations" within the Bottle Buddy
  waterLevelPipe = BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location::WATER_LEVEL);
}

/** 
 * @brief Main loop.
 * 
 *  This loop blinks an LED for demonstration purposes.
 */
void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.print(1);              
  Serial.print("\t");
  delay(delayTime);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  Serial.print(0);             
  Serial.print("\t");           
  delay(delayTime);                       // wait for a second

  //Using pipeline api. In loop, when you have gotten a new reading from the distance sensor, simply send the payload using the pipe you constructed in setup. In this case, we send a reading of "1" every time.
  int payload = 1;
  waterLevelPipe->sendPayload<int>(payload);
}
