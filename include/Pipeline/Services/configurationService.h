/**
 * @file configurationService.h
 */

#pragma once

#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    class ConfigurationService : public Service {
    public:
        ConfigurationService(const char* uid);

        void connect();
        void disconnect();

        void loop();
        void receive(Package* package);

        bool didConfigure();
        unsigned char getBottleType();
    private:
        bool configuredBottleBuddy;
        unsigned char bottleType;
    };

}}}}
