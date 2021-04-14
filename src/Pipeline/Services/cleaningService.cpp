/**
 * @file cleaningService.cpp
 */

#include "Pipeline/Services/cleaningService.h"

BottleBuddy::Embedded::Pipeline::Services::CleaningService::CleaningService(const char* uid, bool connected) : Service(uid, connected) {
    createCharacteristic(std::string("clean_notif"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::String);

    BLE.addService(*this->bleService);
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::connect(BLEDevice central) {
    
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::disconnect(BLEDevice central) {
    
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::loop() {
    
}

void BottleBuddy::Embedded::Pipeline::Services::CleaningService::receive(Package* package) {

}
