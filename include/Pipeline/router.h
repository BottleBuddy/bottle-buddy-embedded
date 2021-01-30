/**
 * @file router.h
 */

#pragma once

#include <unordered_map>
#include <vector>
#include "Pipeline/package.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    class Router {
    public:
        /**
         * @brief Routes data coming from a pipe.
         * 
         * Route's a pipe's data payload to every service subscribed to that location.
         */
        template<typename T>
        static void route(T payload, Location location);

        static void subscribe(Location location, void (*receive)(Package package));
    private:
        Router();

        static std::unordered_map<Location, std::vector<void (*)(Package package)>> subscriptions;
    };

}}}
