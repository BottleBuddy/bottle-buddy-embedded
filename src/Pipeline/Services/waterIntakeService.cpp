/**
 * @file waterIntakeService.cpp
 */

#include "Pipeline/Services/waterIntakeService.h"

BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::WaterIntakeService(const char* uid) : Service(uid) {
    BLE.setAdvertisedService(*this->bleService);

    createCharacteristic(std::string("water_intake"), BLERead | BLEIndicate, BottleBuddy::Embedded::Pipeline::BLEType::String);
    createCharacteristic(std::string("time"), BLEWrite | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::String);
    createCharacteristic(std::string("received_id"), BLEWrite | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::String);

    BLE.addService(*this->bleService);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);

    this->updatedWaterLevel = false;
    this->enteredDrinkingPos = false;
    this->waitingToStopDrinking = false;
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::loop() {
    if (enteredDrinkingPos && !waitingToStopDrinking) {
        this->waterLevelBeforeDrinking = this->currWaterLevel;
        this->waterReadings.clear();
        this->updatedWaterLevel = false;
        this->waitingToStopDrinking = true;
    }

    if (waitingToStopDrinking && updatedWaterLevel) {
        if (currWaterLevel < (waterLevelBeforeDrinking - WATER_LEVEL_TOLERANCE)) {
            int heightDrank = waterLevelBeforeDrinking - currWaterLevel;
            //TODO: Calculate volume drank from height.
            cacheWaterIntake(heightDrank);
        }
        this->waitingToStopDrinking = false;
    }
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::receive(BottleBuddy::Embedded::Pipeline::Package* package) {
    switch (package->getOrigin()) {
    case BottleBuddy::Embedded::Pipeline::Location::ToF:
        int waterReading;
        if (package->getData(waterReading)) {
            updateWaterLevel(waterReading);
        }
        break;
    }
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateWaterLevel(int waterReading) {
    if (enteredDrinkingPos) {
        return;
    }
    
    int magicNumber = 10;   //magic number
    if (this->waterReadings.size() < magicNumber) {
        this->waterReadings.push_back(waterReading);
    } else {
        int averageWaterReading = 0;
        for (std::vector<int>::iterator it = this->waterReadings.begin(); it != this->waterReadings.end(); it++) {
            averageWaterReading += *it;
        }
        averageWaterReading = averageWaterReading / magicNumber;
        this->currWaterLevel = averageWaterReading;
        this->waterReadings.clear();
        this->updatedWaterLevel = true;
    }
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::cacheWaterIntake(int volumeDrank) {
    WaterPackage* waterPackage = (WaterPackage*)malloc(sizeof(WaterPackage));
    waterPackage->volumeDrank = volumeDrank;
    //TODO: Figure out time.

    this->waterPackages.push_back(waterPackage);
}
