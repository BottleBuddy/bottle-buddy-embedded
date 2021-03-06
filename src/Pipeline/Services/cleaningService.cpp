/**
 * @file cleaningService.cpp
 */

#include "Pipeline/Services/cleaningService.h"

BottleBuddy::Embedded::Pipeline::Services::CleaningService::CleaningService(const char* uid, bool connected) : Service(uid) {
    createCharacteristic(std::string("clean"), BLEWrite, BLEType::Boolean);
    createCharacteristic(std::string("finished_cleaning"), BLERead | BLENotify, BLEType::Boolean);

    BLE.addService(*this->bleService);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::FSR, this);

    byte initialVal = 0x00;
    getCharacteristic(std::string("clean"))->writeValue(initialVal);

    this->timer = timer_create_default();

    this->connected = connected;

    this->needToClean = false;
    this->cleaning = false;

    this->fsrReading1 = 0;
    this->fsrReading2 = 0;
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::connect(BLEDevice central) {
    this->connected = true;
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::disconnect(BLEDevice central) {
    this->connected = false;
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::loop() {
    this->timer.tick();
    if (needToClean) {
        //digitalWrite(LEDB, LOW);
        if (capIsOn()) {
            this->needToClean = false;
            this->cleaning = true;

            analogWrite(this->LIGHT_PIN, this->LIGHT_WRITE);
            digitalWrite(LEDB, LOW);
            this->cleaningTask = this->timer.in(this->CLEANING_TIME, finishCleaning, this);
        }
    } else {
        BLECharacteristic* cleanCharacteristic = getCharacteristic(std::string("clean"));
        if (cleanCharacteristic != NULL) {
            byte cleanNotif = 0;
            cleanCharacteristic->readValue(cleanNotif);
            this->needToClean = cleanNotif;

            byte reset = 0x00;
            cleanCharacteristic->writeValue(reset);
        }
    }

    if (cleaning && !capIsOn()) restartCleaning();
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

bool BottleBuddy::Embedded::Pipeline::Services::CleaningService::finishCleaning(void *cleaningInstance) {
    BottleBuddy::Embedded::Pipeline::Services::CleaningService* myself = (BottleBuddy::Embedded::Pipeline::Services::CleaningService*)cleaningInstance;

    analogWrite(myself->LIGHT_PIN, 0);
    digitalWrite(LEDB, HIGH);
    myself->cleaning = false;
    myself->needToClean = false;

    BLECharacteristic* finishedCharacteristic = myself->getCharacteristic(std::string("finished_cleaning"));
    if (finishedCharacteristic != NULL) {
        byte finished = 0x01;
        finishedCharacteristic->writeValue(finished);
    }

    return true;
}

bool BottleBuddy::Embedded::Pipeline::Services::CleaningService::capIsOn() {
    return fsrReading1 > FSR_THRESHOLD || fsrReading2 > FSR_THRESHOLD;
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::restartCleaning() {
    if (!cleaning) return;

    analogWrite(LIGHT_PIN, 0);
    digitalWrite(LEDB, HIGH);
    this->cleaning = false;
    this->needToClean = true;
    this->timer.cancel(this->cleaningTask);
}
