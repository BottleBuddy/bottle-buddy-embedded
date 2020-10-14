#include <Arduino.h>

constexpr int serialSpeed = 115200;
constexpr int delayTime = 1000;
constexpr int ledPin = 2;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  Serial.begin(serialSpeed, SERIAL_8N1); // NOLINT(fuchsia-default-arguments-calls)
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.print(1);              // NOLINT(fuchsia-default-arguments-calls)
  Serial.print("\t");
  delay(delayTime);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  Serial.print(0);              // NOLINT(fuchsia-default-arguments-calls)
  Serial.print("\t");           
  delay(delayTime);                       // wait for a second
}
