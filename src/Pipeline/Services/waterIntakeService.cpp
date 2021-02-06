/**
 * @file waterIntakeService.cpp
 */

#include "Pipeline/Services/waterIntakeService.h"

BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::WaterIntakeService(const char* uid) : Service(uid) {
    createCharacteristic(std::string("waterIntake"), BLERead);
    createCharacteristic(std::string("time"), BLEWrite);
    createCharacteristic(std::string("received"), BLERead | BLEWrite);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::receive(BottleBuddy::Embedded::Pipeline::Package package) {

}
