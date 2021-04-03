/**
 * @file package.cpp
 */

#include "Pipeline/package.h"

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, int data) {
    this->origin = origin;

    this->datatype = BBInt;

    int num_bytes = sizeof(int) * 1;
    dataptr = malloc(num_bytes);
    *(int*)dataptr = data;
}

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, float data) {
    this->origin = origin;

    this->datatype = BBFloat;

    int num_bytes = sizeof(float) * 1;
    dataptr = malloc(num_bytes);
    *(float*)dataptr = data;
}

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, int dim1, int dim2, int dim3) {
    this->origin = origin;

    this->datatype = BBInt;

    int num_bytes = sizeof(int) * 3;
    dataptr = malloc(num_bytes);
    int* tempptr = (int*)dataptr;
    
    *tempptr = dim1;
    tempptr++;
    *tempptr = dim2;
    tempptr++;
    *tempptr = dim3;
}

BottleBuddy::Embedded::Pipeline::Package::Package(Location origin, float dim1, float dim2, float dim3) {
    this->origin = origin;

    this->datatype = BBFloat;

    int num_bytes = sizeof(float) * 3;
    dataptr = malloc(num_bytes);
    float* tempptr = (float*)dataptr;

    *tempptr = dim1;
    tempptr++;
    *tempptr = dim2;
    tempptr++;
    *tempptr = dim3;
}

BottleBuddy::Embedded::Pipeline::Package::~Package() {
    free(dataptr);
}

BottleBuddy::Embedded::Pipeline::Location BottleBuddy::Embedded::Pipeline::Package::getOrigin() {
    return origin;
}

bool BottleBuddy::Embedded::Pipeline::Package::getData(int& data) {
    if (this->datatype != BBInt) {
        return false;
    }
    if (this->origin == ACCELEROMETER) {
        return false;
    }

    data = *(int*)dataptr;
    return true;
}

bool BottleBuddy::Embedded::Pipeline::Package::getData(int& dim1, int& dim2, int& dim3) {
    if (this->datatype != BBInt) {
        return false;
    }
    if (this->origin == ToF) {
        return false;
    }

    dim1 = *(int*)dataptr;
    dim2 = *(((int*)dataptr) + 1);
    dim3 = *(((int*)dataptr) + 2);
    return true;
}

bool BottleBuddy::Embedded::Pipeline::Package::getData(float& data) {
    if (this->datatype != BBFloat) {
        return false;
    }
    if (this->origin == ACCELEROMETER) {
        return false;
    }

    data = *(float*)dataptr;
    return true;
}

bool BottleBuddy::Embedded::Pipeline::Package::getData(float& dim1, float& dim2, float& dim3) {
    if (this->datatype != BBFloat) {
        return false;
    }
    if (this->origin == ToF) {
        return false;
    }

    dim1 = *(float*)dataptr;
    dim2 = *(((float*)dataptr) + 1);
    dim3 = *(((float*)dataptr) + 2);
    return true;
}
