/**
 * @file serviceManager.cpp
 */

#include "Pipeline/serviceManager.h"

std::vector<BottleBuddy::Embedded::Pipeline::Service*> BottleBuddy::Embedded::Pipeline::ServiceManager::services;

void BottleBuddy::Embedded::Pipeline::ServiceManager::addService(Service* service) {
    services.push_back(service);
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::connectedBLE(BLEDevice central) {
    for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
        Service* service = *it;
        service->connect(central);
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::disconnectedBLE(BLEDevice central) {
    for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
        Service* service = *it;
        service->disconnect(central);
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::loopServices() {
    for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
        Service* service = *it;
        service->loop();
    }
}
