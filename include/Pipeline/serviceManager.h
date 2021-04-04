/**
 * @file serviceManager.h
 * 
 * @brief Manages all instantiated services.
 * 
 * Executes important lifecycle events for services such as looping, connecting, and disconnecting.
 */

#pragma once

#include <vector>
#include "Pipeline/Services/cleaningService.h"
#include "Pipeline/Services/waterIntakeService.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { 

    class ServiceManager {
    public:
        ServiceManager();

        ~ServiceManager();

        void addService(Service* service);

        void connectedBLE();
        void disconnectedBLE();

        void loopServices();
    private:
        std::vector<Service*> services;
    };

}}}
