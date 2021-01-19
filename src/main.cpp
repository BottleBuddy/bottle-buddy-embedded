/**
 * @file main.cpp
 * 
 * @brief Main file.
 * 
 * Entrance point to system.
 */

#include <Arduino.h>

constexpr int serialSpeed = 115200;
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
}