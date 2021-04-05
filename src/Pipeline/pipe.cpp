/**
 * @file pipe.cpp
 */

#include "Pipeline/pipe.h"

BottleBuddy::Embedded::Pipeline::Pipe::Pipe() {
    //TODO: Handle Pipe construction with no location.
}

BottleBuddy::Embedded::Pipeline::Pipe::Pipe(Location location) {
    this->location = location;
}

template<typename T>
void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload(T data) {
    Package package(this->location, data);
    Router::route(&package);
}
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<int>(int);
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<float>(float);

template<typename T>
void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload(T dim1, T dim2) {
    Package package(this->location, dim1, dim2);
    Router::route(&package);
}
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<int>(int, int);
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<float>(float, float);

template<typename T>
void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload(T dim1, T dim2, T dim3) {
    Package package(this->location, dim1, dim2, dim3);
    Router::route(&package);
}
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<int>(int, int, int);
template void BottleBuddy::Embedded::Pipeline::Pipe::sendPayload<float>(float, float, float);
