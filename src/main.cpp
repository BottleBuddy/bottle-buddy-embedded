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
/**************************/

/*Global Variables*/
const int GREEN_LED_PIN = 4;
const int RED_LED_PIN = 3;
const int BLUE_LED_PIN = 2;
const int BUTTON_PIN = 5;
const int BAUD_RATE = 115200;
/******************/

void setup() {
  /*Pin Initializations*/
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  /*********************/

  /*Serial Initialization for Testing*/
  Serial.begin(BAUD_RATE);
  while (! Serial){delay(1);}
  Serial.println("Bottle Buddy Test Program");
  /**********************************/
}

void loop() {}
