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
        CleaningService(const char* uid);

        void connect();
        void disconnect();

        void loop();
        void receive(Package* package);

        bool static finishCleaning(void *cleaningInstance);
    private:
        Timer<> timer;

        const int LIGHT_PIN = A0;
        const int LIGHT_WRITE = 255;

        bool needToClean;
        bool cleaning;
        const int CLEANING_TIME = 10000;
        int cleaningTimeLeft;
        uintptr_t cleaningTask;

        int fsrReading1, fsrReading2;
        const int FSR_THRESHOLD = 250;

        bool capIsOn();
        void restartCleaning();
    };

}}}}
