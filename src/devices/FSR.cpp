#include "devices/FSR.h"

int read_fsr_1() {
    int val = analogRead(fsr_pin_1);
    return val;
}

int read_fsr_2() {
    int val = analogRead(fsr_pin_2);
    return val;
}
