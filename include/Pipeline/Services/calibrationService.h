/**
 * @file calibrationService.h
 */

#pragma once

#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    class CalibrationService : public Service {
    public:
        CalibrationService(const char* uid);

        void connect(BLEDevice central);
        void disconnect(BLEDevice central);

        void loop();
        void receive(Package* package);

        bool calibratedBottleBuddy();
        unsigned int getDate();
        unsigned int getTime();
    private:
        bool connected;

        bool calibrated;
        unsigned int initDate, initTime;
    };

}}}}
