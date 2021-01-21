/**
 * @file pipeFactory.h
 */

#pragma once

#include "pipe.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    class PipeFactory {
    public:
        PipeFactory();
        Pipe producePipe();
    private:
    };

}}}
