/**
 * @file calibrationService.cpp
 */

#include "Pipeline/Services/calibrationService.h"

BottleBuddy::Embedded::Pipeline::Services::CalibrationService::CalibrationService(const char* uid) : Service(uid) {
    BLE.setAdvertisedService(*this->bleService);

    createCharacteristic(std::string("calibration_date"), BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("calibration_time"), BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("calibration_wrote_time"), BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);

    BLE.addService(*this->bleService);

    byte noTime = 0;
    BLECharacteristic* wroteTimeCharacteristic = getCharacteristic(std::string("calibration_wrote_time"));
    wroteTimeCharacteristic->writeValue(noTime);

    this->connected = false;
    this->calibrated = false;
}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::connect(BLEDevice central) {
    this->connected = true;
}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::disconnect(BLEDevice central) {
    this->connected = false;
}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::loop() {
    if (!connected) return;

    byte wroteTime = 0;
    BLECharacteristic* wroteTimeCharacteristic = getCharacteristic(std::string("calibration_wrote_time"));
    wroteTimeCharacteristic->readValue(wroteTime);
    if (wroteTime) {
        unsigned int date = 0;
        BLECharacteristic* dateCharacteristic = getCharacteristic(std::string("calibration_date"));
        dateCharacteristic->readValue((uint8_t*)&date, sizeof(date));

        unsigned int time = 0;
        BLECharacteristic* timeCharacteristic = getCharacteristic(std::string("calibration_time"));
        timeCharacteristic->readValue((uint8_t*)&time, sizeof(time));

        this->initDate = date;
        this->initTime = time;
    }
}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::receive(Package* package) {}

bool BottleBuddy::Embedded::Pipeline::Services::CalibrationService::calibratedBottleBuddy() {
    return this->calibrated;
}

unsigned int BottleBuddy::Embedded::Pipeline::Services::CalibrationService::getDate() {
    if (!calibrated) return 0;
    return this->initDate;
}

unsigned int BottleBuddy::Embedded::Pipeline::Services::CalibrationService::getTime() {
    if (!calibrated) return 0;
    return this->initTime;
}
