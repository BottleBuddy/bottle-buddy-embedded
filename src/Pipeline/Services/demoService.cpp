/**
 * @file demoService.cpp
 */

#include "Pipeline/Services/demoService.h"

BottleBuddy::Embedded::Pipeline::Services::DemoService::DemoService(const char* uid) : Service(uid) {
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
}

BottleBuddy::Embedded::Pipeline::Services::DemoService::DemoService(BLEService bleService, BLECharacteristic* bleCharacteristics) : Service(bleService, bleCharacteristics) {
    BLE.setAdvertisedService(bleService);

    for (int i = 0; i < 3; i++) {
        bleService.addCharacteristic(bleCharacteristics[i]);
    }

    BLE.addService(bleService);
    this->bleCharacteristics = bleCharacteristics;

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
}

void BottleBuddy::Embedded::Pipeline::Services::DemoService::receive(BottleBuddy::Embedded::Pipeline::Package* package) {
    if (package->getOrigin() == BottleBuddy::Embedded::Pipeline::Location::ToF) {
        unsigned short waterLevel;
        if (package->getData(waterLevel)) {
            this->bleCharacteristics[0].writeValue(waterLevel);
        }
    }
}
