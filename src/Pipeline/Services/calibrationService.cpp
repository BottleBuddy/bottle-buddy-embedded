/**
 * @file configurationService.cpp
 */

#include "Pipeline/Services/configurationService.h"

BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::ConfigurationService(const char* uid) : Service(uid) {
    BLE.setAdvertisedService(*this->bleService);

    createCharacteristic(std::string("bottle_type"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedChar);
    createCharacteristic(std::string("change_bottle"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);

    BLE.addService(*this->bleService);

    this->configuredBottleBuddy = false;
    this->configuredBottleBuddy = 0;

    unsigned char noBottle = 0;
    BLECharacteristic* bottleTypeCharacteristic = getCharacteristic(std::string("bottle_type"));
    bottleTypeCharacteristic->writeValue(noBottle);

    byte noChange = 0x00;
    BLECharacteristic* changeBottleCharacteristic = getCharacteristic(std::string("change_bottle"));
    changeBottleCharacteristic->writeValue(noChange);
}

void BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::connect() {}

void BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::disconnect() {}

void BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::loop() {
    if (!configuredBottleBuddy) {
        unsigned char bottleType = 0;
        BLECharacteristic* bottleTypeCharacteristic = getCharacteristic(std::string("bottle_type"));
        bottleTypeCharacteristic->readValue(bottleType);
        if (bottleType != 0) {
            this->configuredBottleBuddy = true;
            this->bottleType = bottleType;
        }
    }
}

void BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::receive(Package* package) {}

bool BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::didConfigure() {
    return this->configuredBottleBuddy;
}

unsigned char BottleBuddy::Embedded::Pipeline::Services::ConfigurationService::getBottleType() {
    return this->bottleType;
}
