/**
 * @file cleaningService.h
 */

#pragma once

#include <arduino-timer.h>
#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    /**
     * @brief This class automatically cleans the users bottle.
     * 
     * When the Bottle Buddy app determines that the user's bottle is due for a cleaning session, this
     * class will receive that information then initiate cleaning once it has determined it is safe.
     */
    class CleaningService: public Service {
    public:
        CleaningService(const char* uid, bool connected = false);

        void connect(BLEDevice central);
        void disconnect(BLEDevice central);

        void loop();
        void receive(Package* package);

        /**
         * @brief Called when cleaning successfully completes.
         * 
         * Once the cap is on for at least 10 seconds after the app initiates
         * cleaning, this function turns the UVC light off and notifies the app.
         */
        bool static finishCleaning(void *cleaningInstance);
    private:
        Timer<> timer;

        /**
         * @brief Specifies if Bottle Buddy is connected to a central device.
         */
        bool connected;

        const int LIGHT_PIN = A0;
        const int LIGHT_WRITE = 255;

        /**
         * @brief Specifies if the app has told Bottle Buddy it needs to initiate cleaning.
         * 
         * Used to remember to clean if the cap is off the bottle.
         */
        bool needToClean;
        /**
         * @brief Specifies if we are currently cleaning the bottle.
         */
        bool cleaning;
        const int CLEANING_TIME = 10000;
        uintptr_t cleaningTask;

        int fsrReading1, fsrReading2;
        const int FSR_THRESHOLD = 250;

        /**
         * @brief Determines if the cap is on the bottle.
         */
        bool capIsOn();
        /**
         * @brief Restarts the cleaning process.
         * 
         * This function is called if we started the cleaning process but
         * the user took the cap off the bottle.
         */
        void restartCleaning();
    };

}}}}
