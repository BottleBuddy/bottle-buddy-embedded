/**
 * @file serviceManager.cpp
 */

#include "Pipeline/serviceManager.h"

bool BottleBuddy::Embedded::Pipeline::ServiceManager::calibratedBottleBuddy = false;
std::vector<BottleBuddy::Embedded::Pipeline::PendingService*> BottleBuddy::Embedded::Pipeline::ServiceManager::pendingServices;
std::vector<BottleBuddy::Embedded::Pipeline::Service*> BottleBuddy::Embedded::Pipeline::ServiceManager::services;

void BottleBuddy::Embedded::Pipeline::ServiceManager::addService(ServiceType serviceType, const char* uid) {
    PendingService* pendingService = new PendingService();
    pendingService->serviceType = serviceType;
    pendingService->uid = uid;
    pendingServices.push_back(pendingService);
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::connectedBLE(BLEDevice central) {
    if (calibratedBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->connect(central);
        }
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::disconnectedBLE(BLEDevice central) {
    if (calibratedBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->disconnect(central);
        }
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::loopServices() {
    if (calibratedBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->loop();
        }
    }
}
