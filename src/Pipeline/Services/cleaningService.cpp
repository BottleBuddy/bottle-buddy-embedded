/**
 * @file cleaningService.cpp
 */

#include "Pipeline/Services/cleaningService.h"

BottleBuddy::Embedded::Pipeline::Services::CleaningService::CleaningService(const char* uid) : Service(uid) {
    createCharacteristic(std::string("clean"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);

    BLE.addService(*this->bleService);

    byte initialVal = 0x00;
    getCharacteristic(std::string("clean"))->writeValue(initialVal);

    this->needToClean = false;
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::loop() {
    if (needToClean) {
        if (capIsOn()) {
            //TODO: Initiate cleaning.
            this->needToClean = false;
        }
    } else {
        BLECharacteristic* characteristic = getCharacteristic(std::string("clean"));
        if (characteristic != NULL) {
            byte cleanNotif = 0;
            characteristic->readValue(cleanNotif);
            this->needToClean = cleanNotif;
        }
    }
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::receive(Package* package) {

}

bool BottleBuddy::Embedded::Pipeline::Services::CleaningService::capIsOn() {

}
