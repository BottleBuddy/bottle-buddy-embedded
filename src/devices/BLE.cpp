#include "devices/BLE.h"

int ble_device_setup() {
    if (!BLE.begin()) {
        return -1;
    }

    BLE.setLocalName("BBUDDY");
    //TODO: Set advertised service.
    BLE.advertise();
}
