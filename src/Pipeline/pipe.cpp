/**
 * @file pipe.cpp
 */

#include "Pipeline/pipe.h"

BottleBuddy::Embedded::Pipeline::Pipe::Pipe() {
    throw std::runtime_error("Pipe construction must provide a location.");
}

BottleBuddy::Embedded::Pipeline::Pipe::Pipe(Location location) {
    this->location = location;
}

template<typename T>
void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload(T payload) {
    Router::route(payload, this->location);
}
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<int>(int);
