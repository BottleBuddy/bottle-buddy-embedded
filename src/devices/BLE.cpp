#include "devices/BLE.h"

Timer<> ble_timer;
bool not_connected = true;

int ble_device_setup() {
    if (!BLE.begin()) {
        return -1;
    }

    BLE.setLocalName("BBUDDY");
    ble_timer = timer_create_default();
    //int advertiseSuccess = BLE.advertise();
    return 0;
}

int advertise_ble() {
    return BLE.advertise() - 1;
}

String wait_for_ble_connection() {
    while (!BLE.central().connected()) {
        if (not_connected) {
            digitalWrite(4, HIGH);
        }
        if (BLE.central()) {
            if (not_connected) {
                digitalWrite(4, LOW);
                ble_timer.in(500, [](void *) -> bool {
                    digitalWrite(4, HIGH);
                    return true;
                });
                ble_timer.in(1000, [](void *) -> bool {
                    digitalWrite(4, LOW);
                    return true;
                });
                ble_timer.in(1500, [](void *) -> bool {
                    digitalWrite(4, HIGH);
                    return true;
                });
                ble_timer.in(2000, [](void *) -> bool {
                    digitalWrite(4, LOW);
                    return false;
                });
                not_connected = false;
            }
            return BLE.central().address();
        }
    }
}
