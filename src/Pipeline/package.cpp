/**
 * @file package.cpp
 */

#include "Pipeline/package.h"

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, int data) {
    this->origin = origin;
}

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, float data) {
    this->origin = origin;
}

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, int dim1, int dim2, int dim3) {
    this->origin = origin;
}

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, float dim1, float dim2, float dim3) {
    this->origin = origin;
}

BottleBuddy::Embedded::Pipeline::Package::~Package() {
    
}

BottleBuddy::Embedded::Pipeline::Location BottleBuddy::Embedded::Pipeline::Package::getOrigin() {
    return origin;
}
