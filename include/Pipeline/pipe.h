/**
 * @file pipe.h
 */

#pragma once

#include <stdexcept>
#include "Pipeline/router.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Transports sensor data.
     * 
     * This is a general class used to bi-directionally transport sensor data across the Bottle Buddy embedded system.
     */
    class Pipe {
    public:
        Pipe(Location location);

        /**
         * @brief Sends sensor data.
         * 
         * Use this function when a sensor reads new data that will be used across the Bottle Buddy embedded system.
         */
        template<typename T>
        void sendPayload(T payload);
    private:
        Pipe();

        /**
         * @brief Pipe's sensor location.
         * 
         * Used by the Router class to determine what to do with the sensor data that comes down this pipe.
         */
        Location location;
    };

}}}
