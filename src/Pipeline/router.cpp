/**
 * @file router.cpp
 */

#include "Pipeline/router.h"

std::unordered_map<BottleBuddy::Embedded::Pipeline::Location, std::vector<BottleBuddy::Embedded::Pipeline::Service*>, std::hash<int>> BottleBuddy::Embedded::Pipeline::Router::subscriptions;

template<typename T>
void BottleBuddy::Embedded::Pipeline::Router::route(Location location, T payload) {
    if (subscriptions.find(location) == subscriptions.end()) {
        return;
    }

    Package package(location, payload);
    std::vector<Service*> subscribedServices = subscriptions.at(location);
    for(std::vector<Service*>::iterator servicesIter = subscribedServices.begin(); servicesIter != subscribedServices.end(); servicesIter++) {
        Service *service = *servicesIter;
        service->receive(package);
    }
}
template void BottleBuddy::Embedded::Pipeline::Router::route<int>(Location, int);

void BottleBuddy::Embedded::Pipeline::Router::subscribe(Location location, Service *service) {
    subscriptions.emplace(location, std::vector<Service*>());
    subscriptions.at(location).push_back(service);
}
