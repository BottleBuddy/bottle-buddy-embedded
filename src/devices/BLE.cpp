#include "devices/BLE.h"

int ble_device_setup() {
    if (!BLE.begin()) {
        return -1;
    }

    BLE.setLocalName("BBUDDY");
    int advertiseSuccess = BLE.advertise();
    return advertiseSuccess - 1;
}
