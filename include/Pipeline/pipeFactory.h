/**
 * @file pipeFactory.h
 */

#pragma once

#include <stdexcept>
#include "pipe.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    class PipeFactory {
    public:
        static Pipe* producePipe(Location location);
    private:
        PipeFactory();
    };

}}}
