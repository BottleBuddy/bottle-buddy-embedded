/**
 * @file waterIntakeService.cpp
 */

#include "Pipeline/Services/waterIntakeService.h"

BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::WaterIntakeService(const char* uid) : Service(uid) {
    createCharacteristic(std::string("water_intake"), BLERead | BLEIndicate);
    createCharacteristic(std::string("time"), BLEWrite | BLENotify);
    createCharacteristic(std::string("received_id"), BLEWrite | BLENotify);

    BLE.setAdvertisedServiceUuid(uid);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::receive(BottleBuddy::Embedded::Pipeline::Package* package) {

}
