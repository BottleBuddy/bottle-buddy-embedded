/**
 * @file FSR.h
 * 
 * Functions related to the force sensitive resistor.
 */

#pragma once

#include <Arduino.h>

int fsr_pin_1 = A6;
int fsr_pin_2 = A7;

int read_fsr_1();

int read_fsr_2();
