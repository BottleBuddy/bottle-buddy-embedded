/**
 * @file pipe.h
 */

#pragma once

#include <stdexcept>
#include "Pipeline/router.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    class Pipe {
    public:
        Pipe();
        Pipe(Location location);

        template<typename T>
        void sendPayload(T payload);
    private:
        Location location;
    };

}}}
