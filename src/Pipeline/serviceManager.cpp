/**
 * @file serviceManager.cpp
 */

#include "Pipeline/serviceManager.h"

BottleBuddy::Embedded::Pipeline::ServiceManager::ServiceManager() {}

BottleBuddy::Embedded::Pipeline::ServiceManager::~ServiceManager() {}

void BottleBuddy::Embedded::Pipeline::ServiceManager::addService(Service* service) {
    this->services.push_back(service);
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::connectedBLE() {
    for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
        Service* service = *it;
        service->connect();
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::disconnectedBLE() {
    for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
        Service* service = *it;
        service->disconnect();
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::loopServices() {
    for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
        Service* service = *it;
        service->loop();
    }
}
