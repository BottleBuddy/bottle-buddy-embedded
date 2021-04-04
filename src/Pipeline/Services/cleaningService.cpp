/**
 * @file cleaningService.cpp
 */

#include "Pipeline/Services/cleaningService.h"

BottleBuddy::Embedded::Pipeline::Services::CleaningService::CleaningService(const char* uid) : Service(uid) {
    createCharacteristic(std::string("clean"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);

    BLE.addService(*this->bleService);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::FSR1, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::FSR2, this);

    byte initialVal = 0x00;
    getCharacteristic(std::string("clean"))->writeValue(initialVal);

    this->needToClean = false;
    this->fsrReading1 = 0;
    this->fsrReading2 = 0;
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

    int fsrDiff = fsrReading2 - fsrReading1;
    if (std::abs(fsrDiff) < FSR_TOLERANCE) {
        this->fsrReading = (fsrReading1 + fsrReading2) / 2;
    } else {
        //Something's wrong
    }
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::receive(Package* package) {
    int fsrVal;
    switch (package->getOrigin()) {
        case BottleBuddy::Embedded::Pipeline::Location::FSR1:
            if (package->getData(fsrVal)) {
                this->fsrReading1 = fsrVal;
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::FSR2:
            if (package->getData(fsrVal)) {
                this->fsrReading2 = fsrVal;
            }
            break;
    }
}

bool BottleBuddy::Embedded::Pipeline::Services::CleaningService::capIsOn() {

}
