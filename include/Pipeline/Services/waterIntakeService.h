/**
 * @file waterIntakeService.h
 */

#pragma once

#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    class WaterIntakeService: public Service {
    public:
        WaterIntakeService(std::string uid);

        void receive(Package package);
    private:
    };

}}}}
