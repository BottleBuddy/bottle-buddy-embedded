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
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::connect(BLEDevice central) {

}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::disconnect(BLEDevice central) {
    
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::loop() {

}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::receive(BottleBuddy::Embedded::Pipeline::Package* package) {

}
