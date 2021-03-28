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
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::GYRO, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::MAGNETIC, this);

    this->timer = timer_create_default();
    this->timer.every(500, BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateOrientation, this);
    
    this->filter = new Mahony();

    this->updatedWaterLevel = false;
    this->enteredDrinkingPos = false;
    this->waitingToStopDrinking = false;
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::loop() {
    this->timer.tick();

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
        case BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER:
            float x, y, z;
            if (package->getData(x, y , z)) {
                this->accelX = x;
                this->accelY = y;
                this->accelZ = z;
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::GYRO:
            if (package->getData(x, y, z)) {
                this->gyroX = x;
                this->gyroY = y;
                this->gyroZ = z;
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::MAGNETIC:
            if (package->getData(x, y, z)) {
                this->magneticX = x;
                this->magneticY = y;
                this->magneticZ = z;
            }
            break;
    }
}

bool BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateOrientation(void *waterInstance) {
    BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService *myself = (BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService*)waterInstance;

    myself->filter->update(myself->gyroX, myself->gyroY, myself->gyroZ, myself->accelX, myself->accelY, myself->accelZ, myself->magneticX, myself->magneticY, myself->magneticZ);
    float pitch = myself->filter->getPitch();
    if ((pitch > 0) && (pitch < 1.57)) {
        myself->enteredDrinkingPos = true;
        digitalWrite(2, HIGH);
    } else {
        myself->enteredDrinkingPos = false;
        digitalWrite(2, LOW);
    }
    return true;
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
