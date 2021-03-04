/**
 * @file demoService.cpp
 */

#include "Pipeline/Services/demoService.h"

BottleBuddy::Embedded::Pipeline::Services::DemoService::DemoService(const char* uid) : Service(uid) {
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
}

BottleBuddy::Embedded::Pipeline::Services::DemoService::DemoService(BLEService bleService, BLEUnsignedShortCharacteristic tof, std::vector<BLEStringCharacteristic> accel, BLEBooleanCharacteristic notif) : Service(bleService) {
    BLE.setAdvertisedService(bleService);

    bleService.addCharacteristic(tof);
    bleService.addCharacteristic(accel.at(0));
    bleService.addCharacteristic(accel.at(1));
    bleService.addCharacteristic(accel.at(2));
    bleService.addCharacteristic(notif);

    BLE.addService(bleService);

    this->tofCharacteristic = tof;
    this->accelerometerCharacteristics = accel;
    this->notificationCharacteristic = notif;

    this->timer = timer_create_default();

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
}

void BottleBuddy::Embedded::Pipeline::Services::DemoService::loop() {
    this->timer.tick();

    byte startCleaning = 0;
    this->notificationCharacteristic.readValue(startCleaning);
    if (startCleaning) {
        digitalWrite(2, HIGH);

        byte reset = 0x00;
        this->notificationCharacteristic.writeValue(reset);
        
        this->timer.in(5000, [](void *) -> bool {
            digitalWrite(2, LOW);
            return true;
        });
    }
}

void BottleBuddy::Embedded::Pipeline::Services::DemoService::receive(BottleBuddy::Embedded::Pipeline::Package* package) {
    switch (package->getOrigin()) {
        case BottleBuddy::Embedded::Pipeline::Location::ToF:
            unsigned short waterLevel;
            if (package->getData(waterLevel)) {
                this->tofCharacteristic.writeValue(waterLevel);
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER:
            float x, y, z;
            if (package->getData(x, y, z)) {
                this->accelerometerCharacteristics.at(0).writeValue(String(x));
                this->accelerometerCharacteristics.at(1).writeValue(String(y));
                this->accelerometerCharacteristics.at(2).writeValue(String(z));
            }
            break;
    }
}
