/**
 * @file cleaningService.cpp
 */

#include "Pipeline/Services/cleaningService.h"

BottleBuddy::Embedded::Pipeline::Services::CleaningService::CleaningService(const char* uid) : Service(uid) {
    createCharacteristic(std::string("clean"), BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);
    createCharacteristic(std::string("finished_cleaning"), BLERead | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);

    BLE.addService(*this->bleService);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::FSR, this);

    byte initialVal = 0x00;
    getCharacteristic(std::string("clean"))->writeValue(initialVal);

    this->timer = timer_create_default();

    this->needToClean = false;
    this->cleaning = false;
    this->cleaningTimeLeft = 0;

    this->fsrReading1 = 0;
    this->fsrReading2 = 0;
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::connect() {

}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::disconnect() {
    
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::loop() {
    this->timer.tick();
    if (needToClean) {
        if (capIsOn()) {
            this->needToClean = false;
            this->cleaning = true;

            analogWrite(this->LIGHT_PIN, this->LIGHT_WRITE);
            this->timer.in(this->CLEANING_TIME, BottleBuddy::Embedded::Pipeline::Services::CleaningService::stopCleaning, this);
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
    int fsrVal1, fsrVal2;
    switch (package->getOrigin()) {
        case BottleBuddy::Embedded::Pipeline::Location::FSR:
            if (package->getData(fsrVal1, fsrVal2)) {
                this->fsrReading1 = fsrVal1;
                this->fsrReading2 = fsrVal2;
            }
            break;
    }
}

bool BottleBuddy::Embedded::Pipeline::Services::CleaningService::stopCleaning(void *cleaningInstance) {
    BottleBuddy::Embedded::Pipeline::Services::CleaningService* myself = (BottleBuddy::Embedded::Pipeline::Services::CleaningService*)cleaningInstance;

    analogWrite(myself->LIGHT_PIN, 0);
    myself->cleaning = false;
    myself->needToClean = false;

    return true;
}

bool BottleBuddy::Embedded::Pipeline::Services::CleaningService::capIsOn() {
    return fsrReading1 > FSR_THRESHOLD || fsrReading2 > FSR_THRESHOLD;
}
