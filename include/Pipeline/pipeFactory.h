/**
 * @file pipeFactory.h
 */

#pragma once

#include <stdexcept>
#include "pipe.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Class used to produce pipes to place around the system.
     * 
     * When initializing the Bottle Buddy embedded system, use this class to construct pipes.
     * A location must be provided when constructing a pipe.
     */
    class PipeFactory {
    public:
        /**
         * @brief Function used to create a new pipe.
         * 
         * Call this function when you wish to construct a new pipe head for sensor data transfer.
         * A Bottle Buddy location must be provided, which can be found in BottleBuddy::Embedded::Pipeline::Router.
         */
        static Pipe* producePipe(Location location);
    private:
        PipeFactory();
    };

}}}
