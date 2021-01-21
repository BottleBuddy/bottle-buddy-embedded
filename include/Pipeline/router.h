/**
 * @file router.h
 */

#pragma once

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    enum Location { WATER_LEVEL, CLEANING, NOTIFICATION };

    class Router {
    public:
        template<typename T>
        static void route(T payload, Location location);
    private:
        Router();
    };

}}}
