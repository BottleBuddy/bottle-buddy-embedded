/**
 * @file service.cpp
 */

#include "Pipeline/service.h"

BottleBuddy::Embedded::Pipeline::Service::Service(const char* uid) : bleService(uid) {
    this->uid = uid;
    this->numCharacteristics = 0;

    BLE.addService(bleService);

    hexConversions.emplace('a', 10);
    hexConversions.emplace('A', 10);
    hexConversions.emplace('b', 11);
    hexConversions.emplace('B', 11);
    hexConversions.emplace('c', 12);
    hexConversions.emplace('C', 12);
    hexConversions.emplace('d', 13);
    hexConversions.emplace('D', 13);
    hexConversions.emplace('e', 14);
    hexConversions.emplace('E', 14);
    hexConversions.emplace('f', 15);
    hexConversions.emplace('F', 15);
}

void BottleBuddy::Embedded::Pipeline::Service::createCharacteristic(std::string name, uint8_t properties) {
    const char* characteristicUUID = makeCharacteristicUUID();
    BLEByteCharacteristic bleCharacteristic(characteristicUUID, properties);
    this->bleService.addCharacteristic(bleCharacteristic);
    characteristics.emplace(name, bleCharacteristic);
}

const char* BottleBuddy::Embedded::Pipeline::Service::makeCharacteristicUUID() {

}
