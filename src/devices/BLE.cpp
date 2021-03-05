#include "devices/BLE.h"

Timer<> ble_timer;

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
        digitalWrite(4, HIGH);
        if (BLE.central()) {
            digitalWrite(4, LOW);
            ble_timer.in(500, [](void *) -> bool {
                digitalWrite(4, HIGH);
                ble_timer.in(500, [](void *) -> bool {
                    digitalWrite(4, LOW);
                    ble_timer.in(500, [](void *) -> bool {
                        digitalWrite(4, HIGH);
                        ble_timer.in(500, [](void *) -> bool {
                            digitalWrite(4, LOW);
                            return true;
                        });
                        return true;
                    });
                    return true;
                });
                return true;
            });
            return BLE.central().address();
        }
    }
}
