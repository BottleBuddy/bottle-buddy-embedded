/**
 * @file cleaningService.h
 */

#pragma once

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

        void connect(BLEDevice central);
        void disconnect(BLEDevice central);

        void loop();
        void receive(Package* package);
    private:
    };

}}}}