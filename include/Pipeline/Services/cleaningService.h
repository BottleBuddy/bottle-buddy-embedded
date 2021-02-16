/**
 * @file cleaningService.h
 */

#pragma once

#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    class CleaningService : public Service {
    public:
        CleaningService(const char* uid);

        void receive(Package package);
    private:
    };

}}}}
