/**
 * @file serviceManager.cpp
 */

#include "Pipeline/serviceManager.h"

BottleBuddy::Embedded::Pipeline::ServiceManager::ServiceManager() {
    this->configuredBottleBuddy = false;

    this->configurationService = new BottleBuddy::Embedded::Pipeline::Services::ConfigurationService("19B10010-E8F2-537E-4F6C-D104768A1214");
}

BottleBuddy::Embedded::Pipeline::ServiceManager::~ServiceManager() {

}

void BottleBuddy::Embedded::Pipeline::ServiceManager::addService(ServiceType serviceType, const char* uid) {
    PendingService* pendingService = new PendingService();
    pendingService->serviceType = serviceType;
    pendingService->uid = uid;
    this->pendingServices.push_back(pendingService);
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::connectedBLE() {
    if (configuredBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->connect();
        }
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::disconnectedBLE() {
    if (configuredBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->disconnect();
        }
    }
}

void BottleBuddy::Embedded::Pipeline::ServiceManager::loopServices() {
    if (configuredBottleBuddy) {
        for (std::vector<Service*>::iterator it = services.begin(); it != services.end(); it++) {
            Service* service = *it;
            service->loop();
        }
    } else {
        this->configurationService->loop();
        if (this->configurationService->didConfigure()) {
            this->configuredBottleBuddy = true;
            unsigned char configuration = this->configurationService->getBottleType();
            switch (configuration) {
                case 1:
                    this->bottleType = BottleBuddy::Embedded::Pipeline::BottleType::ZANE_YETI;
                    break;
            }

            for (std::vector<PendingService*>::iterator it = pendingServices.begin(); it != pendingServices.end(); it++) {
                PendingService* pendingService = *it;
                BottleBuddy::Embedded::Pipeline::Service* service;
                const char* uid = pendingService->uid;
                switch (pendingService->serviceType) {
                    case WATER_INTAKE:
                        service = new BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService(uid);
                        break;
                    case CLEANING:
                        service = new BottleBuddy::Embedded::Pipeline::Services::CleaningService(uid);
                        break;
                }
                this->services.push_back(service);
            }
        }
    }
}
