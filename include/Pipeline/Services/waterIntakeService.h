/**
 * @file waterIntakeService.h
 */

#pragma once

#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    class waterIntakeService: public Service {
    public:
        waterIntakeService(std::string uid);

        void receive(Package package);
    private:
    };

}}}}
