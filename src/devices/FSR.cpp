#include "devices/FSR.h"

const int fsr_pin_1 = A6;
const int fsr_pin_2 = A7;

int read_fsr_1() {
    int val = analogRead(fsr_pin_1);
    return val;
}

int read_fsr_2() {
    int val = analogRead(fsr_pin_2);
    return val;
}
