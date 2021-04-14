/**
 * @file serviceManager.cpp
 */

#include "Pipeline/serviceManager.h"

bool BottleBuddy::Embedded::Pipeline::ServiceManager::connected = false;
bool BottleBuddy::Embedded::Pipeline::ServiceManager::calibratedBottleBuddy = false;
BottleBuddy::Embedded::Pipeline::Services::Time* BottleBuddy::Embedded::Pipeline::ServiceManager::initTimestamp = new BottleBuddy::Embedded::Pipeline::Services::Time();
BottleBuddy::Embedded::Pipeline::Services::CalibrationService* BottleBuddy::Embedded::Pipeline::ServiceManager::calibrationService = new BottleBuddy::Embedded::Pipeline::Services::CalibrationService("19B10010-E8F2-537E-4F6C-D104768A1214");
std::vector<BottleBuddy::Embedded::Pipeline::PendingService*> BottleBuddy::Embedded::Pipeline::ServiceManager::pendingServices;
std::vector<BottleBuddy::Embedded::Pipeline::Service*> BottleBuddy::Embedded::Pipeline::ServiceManager::services;

void BottleBuddy::Embedded::Pipeline::ServiceManager::addService(ServiceType serviceType, const char* uid) {
    PendingService* pendingService = new PendingService();
    pendingService->serviceType = serviceType;
    pendingService->uid = uid;
    pendingServices.push_back(pendingService);
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::connectedBLE(BLEDevice central) {
    connected = true;
    if (calibratedBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->connect(central);
        }
    } else {
        calibrationService->connect(central);
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::disconnectedBLE(BLEDevice central) {
    connected = false;
    if (calibratedBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->disconnect(central);
        }
    } else {
        calibrationService->disconnect(central);
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::loopServices() {
    if (calibratedBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->loop();
        }
    } else {
        calibrationService->loop();
        if (calibrationService->calibratedBottleBuddy()) {
            calibratedBottleBuddy = true;
            unsigned int date = calibrationService->getDate();
            unsigned int time = calibrationService->getTime();
            BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::createTimestamp(date, time, initTimestamp);

            for (std::vector<PendingService*>::iterator it = pendingServices.begin(); it != pendingServices.end(); it++) {
                PendingService* pendingService = *it;
                BottleBuddy::Embedded::Pipeline::Service* service;
                const char* uid = pendingService->uid;
                switch (pendingService->serviceType) {
                    case WATER_INTAKE:
                        service = new BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService(uid, initTimestamp, connected);
                        break;
                    case CLEANING:
                        service = new BottleBuddy::Embedded::Pipeline::Services::CleaningService(uid, connected);
                        break;
                }
                services.push_back(service);
            }
        }
    }
}
