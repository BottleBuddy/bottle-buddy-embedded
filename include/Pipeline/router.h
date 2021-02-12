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

        static void subscribe(Location location, Service *service);
    private:
        Router();

        static std::unordered_map<Location, std::vector<Service*>, std::hash<int>> subscriptions;
    };

}}}
