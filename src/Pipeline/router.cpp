/**
 * @file router.cpp
 */

#include "Pipeline/router.h"

template<typename T>
void BottleBuddy::Embedded::Pipeline::Router::route(T payload, Location location) {
    switch(location) {
        case ToF:
            break;
        case CLEANING:
            break;
        case NOTIFICATION:
            break;
    }
}
template void BottleBuddy::Embedded::Pipeline::Router::route<int>(int, Location);
