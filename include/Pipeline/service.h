/**
 * @file service.h
 */

#pragma once

#include <string>
#include "Pipeline/router.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {
    
    class Service {
    public:
        virtual void receive(Package package);
    protected:
        std::string uid;
    };

}}}
