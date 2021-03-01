/**
 * @file demoService.h
 */

#pragma once

#include <arduino-timer.h>
#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    /**
     * @brief This service tracks how much water a Bottle Buddy user drinks throughout the day.
     * 
     * Specifically, the service creates a set of timestamped values corresponding to when and how much water
     * a user drank during the day. Additionally, it streams this dataset to the Bottle Buddy App.
     */
    class DemoService: public Service {
    public:
        DemoService(const char* uid);
        DemoService(BLEService bleService, BLECharacteristic* bleCharacteristics);

        void loop();
        void receive(Package* package);
    private:
        Timer<> timer;

        BLECharacteristic* bleCharacteristics;
    };

}}}}
