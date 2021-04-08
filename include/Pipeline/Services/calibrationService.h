/**
 * @file calibrationService.h
 */

#pragma once

#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    /**
     * @brief Handles configuration of the bottle buddy.
     * 
     * This service is the only advertised service until the Bottle Buddy App configures the embedded system
     * by sending a bottle type. Then, the rest of the services will be added to the BLE device.
     */
    class CalibrationService : public Service {
    public:
        CalibrationService(const char* uid);

        void connect();
        void disconnect();

        void loop();
        void receive(Package* package);

        /**
         * @brief Asks whether the embedded system has been configured.
         */
        bool didConfigure();

        /**
         * @brief Returns the bottle type sent by the app.
         * 
         * A return value of 0 also means the embedded system has not been configured yet.
         */
        unsigned char getBottleType();
    private:
        /**
         * @brief Keeps track of whether or not the embedded system has been configured.
         */
        bool configuredBottleBuddy;

        /**
         * @brief Stores the bottle type as sent by the app.
         */
        unsigned char bottleType;
    };

}}}}
