#include "devices/BLE.h"

int ble_device_setup() {
    if (!BLE.begin()) {
        return -1;
    }

    BLE.setLocalName("BBUDDY");
    //int advertiseSuccess = BLE.advertise();
    return 0;
}

int advertise_ble() {
    return BLE.advertise() - 1;
}

String wait_for_ble_connection() {
    while (!BLE.central().connected()) {
        if (BLE.central()) {
            return BLE.central().address();
        }
    }
}
