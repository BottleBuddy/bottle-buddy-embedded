/**
 * @file package.cpp
 */

#include "Pipeline/package.h"

BottleBuddy::Embedded::Pipeline::Package::Package(BottleBuddy::Embedded::Pipeline::Location o, int p) {
    origin = o;
    payloadInt = p;
}

BottleBuddy::Embedded::Pipeline::Location BottleBuddy::Embedded::Pipeline::Package::getOrigin() {
    return origin;
}
