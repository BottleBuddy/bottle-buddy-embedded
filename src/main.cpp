#include <Arduino.h>

int ledpin = 2;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledpin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(ledpin, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.print(1);
  Serial.print("\t");
  delay(1000);                       // wait for a second
  digitalWrite(ledpin, LOW);    // turn the LED off by making the voltage LOW
  Serial.print(0);
  Serial.print("\t");
  delay(1000);                       // wait for a second
}