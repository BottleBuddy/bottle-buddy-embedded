/**
 * @file demoService.cpp
 */

#include "Pipeline/Services/demoService.h"

BottleBuddy::Embedded::Pipeline::Services::DemoService::DemoService(const char* uid) : Service(uid) {
    BLE.setAdvertisedService(*this->bleService);

    createCharacteristic(std::string("tof"), BLERead | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedShort);
    createCharacteristic(std::string("accelX"), BLERead | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::String);
    createCharacteristic(std::string("accelY"), BLERead | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::String);
    createCharacteristic(std::string("accelZ"), BLERead | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::String);
    createCharacteristic(std::string("notif"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);

    BLE.addService(*bleService);

    this->timer = timer_create_default();

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
}

void BottleBuddy::Embedded::Pipeline::Services::DemoService::loop() {
    this->timer.tick();

    byte startCleaning = 0;
    BLECharacteristic* notifCharacteristic = getCharacteristic(std::string("notif"));
    notifCharacteristic->readValue(startCleaning);
    if (startCleaning) {
        digitalWrite(2, HIGH);

        byte reset = 0x00;
        notifCharacteristic->writeValue(reset);
        
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
                BLECharacteristic* tofCharacteristic = getCharacteristic(std::string("tof"));
                tofCharacteristic->writeValue(waterLevel);
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER:
            float x, y, z;
            if (package->getData(x, y, z)) {
                BLEStringCharacteristic* accelXCharacteristic = getStringCharacteristic(std::string("accelX"));
                BLEStringCharacteristic* accelYCharacteristic = getStringCharacteristic(std::string("accelY"));
                BLEStringCharacteristic* accelZCharacteristic = getStringCharacteristic(std::string("accelZ"));

                accelXCharacteristic->writeValue(arduino::String(x));
                accelYCharacteristic->writeValue(arduino::String(y));
                accelZCharacteristic->writeValue(arduino::String(z));
            }
            break;
    }
}
