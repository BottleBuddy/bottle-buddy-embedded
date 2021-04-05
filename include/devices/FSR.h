/**
 * @file FSR.h
 * 
 * Functions related to the force sensitive resistor.
 */

#pragma once

#include <Arduino.h>

extern const int fsr_pin_1;
extern const int fsr_pin_2;

int read_fsr_1();

int read_fsr_2();
