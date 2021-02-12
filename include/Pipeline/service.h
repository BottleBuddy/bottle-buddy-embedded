/**
 * @file service.h
 */

#pragma once

#include <stdlib.h>
#include <string>
#include <cstring>
#include <math.h>
#include <unordered_map>
#include <ArduinoBLE.h>
#include "Pipeline/router.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    class Service {
    public:
        Service(const char* uid);

        ~Service();

        virtual void receive(Package package);
    protected:
        const char* uid;
        BLEService bleService;

        std::vector<const char*> uuids;
        int numCharacteristics;
        std::unordered_map<std::string, BLECharacteristic> characteristics;

        void createCharacteristic(std::string name, uint8_t properties);
    private:
        std::unordered_map<char, int> hexConversions;

        const char* makeCharacteristicUUID();
        int hexStringToInt(char* str, int endIdx);
        bool insertIncrementedUUID(char* str, int endIdx, int number);
    };

}}}
