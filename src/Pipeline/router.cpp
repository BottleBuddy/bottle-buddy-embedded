/**
 * @file router.cpp
 */

#include "Pipeline/router.h"

std::unordered_map<BottleBuddy::Embedded::Pipeline::Location, std::vector<BottleBuddy::Embedded::Pipeline::Service*>, std::hash<int>> BottleBuddy::Embedded::Pipeline::Router::subscriptions;

void BottleBuddy::Embedded::Pipeline::Router::route(Package package) {
    Location origin = package.getOrigin();
    if (subscriptions.find(origin) == subscriptions.end()) {
        return;
    }

    std::vector<Service*> subscribedServices = subscriptions.at(origin);
    for(std::vector<Service*>::iterator servicesIter = subscribedServices.begin(); servicesIter != subscribedServices.end(); servicesIter++) {
        Service *service = *servicesIter;
        service->receive(package);
    }
}

void BottleBuddy::Embedded::Pipeline::Router::subscribe(Location location, Service *service) {
    subscriptions.emplace(location, std::vector<Service*>());
    subscriptions.at(location).push_back(service);
}
