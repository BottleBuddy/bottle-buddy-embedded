#include "devices/BLE.h"

int ble_device_setup() {
    if (!BLE.begin()) {
        return -1;
    }

    BLE.setLocalName("BBUDDY");
    return 0;
}

int advertise_ble() {
    return BLE.advertise() - 1;
}
