/**
 * @file serviceManager.h
 */

#pragma once

#include <vector>
#include <ArduinoBLE.h>
#include "Pipeline/Services/calibrationService.h"
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
        /**
         * @brief Determines whether the system is connected to a central device or not.
         * 
         * Since services will have a delayed instantiation, it is very possible they will not receive
         * a connected notification, so this initialization is necessary.
         */
        static bool connected;
        static bool calibratedBottleBuddy;

        static Services::Time* initTimestamp;

        static BottleBuddy::Embedded::Pipeline::Services::CalibrationService* calibrationService;

        /**
         * @brief The list of pending services
         * 
         * These services are not instantiated until the calibration session is completed.
         */
        static std::vector<PendingService*> pendingServices;

        /**
         * @brief The list of managed services.
         */
        static std::vector<Service*> services;
    };

}}}
