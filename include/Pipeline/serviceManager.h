/**
 * @file serviceManager.h
 */

#pragma once

#include <vector>
#include <ArduinoBLE.h>
#include "Pipeline/Services/cleaningService.h"
#include "Pipeline/Services/waterIntakeService.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    enum ServiceType { WATER_INTAKE, CLEANING };

    struct pendingService {
        ServiceType serviceType;
        const char* uid;
    } typedef PendingService;

    /**
     * @brief Manages all instantiated services.
     * 
     * Executes important lifecycle events for services such as looping, connecting, and disconnecting.
     */
    class ServiceManager {
    public:
        /**
         * @brief Adds a service to the list of services the service manager handles.
         * 
         * The provided service will now be connected, disconnected, and looped.
         */
        static void addService(ServiceType serviceType, const char* uid);

        /**
         * @brief Calls the virtual connect function on all managed services.
         * 
         * Call this function when the BLE device connects to a central device.
         */
        static void connectedBLE(BLEDevice central);

        /**
         * @brief Calls the virtual disconnect function on all managed services.
         * 
         * Call this function when the BLE device disconnects from a central device.
         */
        static void disconnectedBLE(BLEDevice central);

        /**
         * @brief Calls the virtual loop function on all managed services.
         * 
         * Call this function once per main loop.
         */
        static void loopServices();
    private:
        static bool calibratedBottleBuddy;

        static std::vector<PendingService*> pendingServices;

        /**
         * @brief The list of managed services.
         */
        static std::vector<Service*> services;
    };

}}}
