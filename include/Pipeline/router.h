/**
 * @file router.h
 */

#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include "Pipeline/package.h"
#include "Pipeline/service.h"

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    class Service;

    class Router {
    public:
        /**
         * @brief Routes packages coming from a pipe.
         * 
         * Route's a pipe's package to every service subscribed to that location.
         */
        static void route(Package package);

        /**
         * @brief Subscribes a service to a sensor location.
         * 
         * If a service needs a specific sensor's data to provide its service, then calling this function
         * will allow the service specified to begin receiving data from the location specified.
         */
        static void subscribe(Location location, Service *service);
    private:
        Router();

        /**
         * @brief Used to route a package to subscribed services.
         * 
         * Hash keys are locations that map to a vector of subscribed services that are subscribed
         * to the location that was used as the key.
         */
        static std::unordered_map<Location, std::vector<Service*>, std::hash<int>> subscriptions;
    };

}}}
