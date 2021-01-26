/**
 * @file router.h
 */

#pragma once

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Location corresponding to a hardware subsystem.
     * 
     * Used to mark the location of a pipe so that the router class can know what to do with the data that comes down that pipe.
     */
    enum Location { WATER_LEVEL, CLEANING, NOTIFICATION };

    class Router {
    public:
        /**
         * @brief Routes data coming from a pipe.
         * 
         * Route's a pipe's data payload to every use case that requires that data.
         */
        template<typename T>
        static void route(T payload, Location location);
    private:
        Router();
    };

}}}
