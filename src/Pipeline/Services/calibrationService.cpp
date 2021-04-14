/**
 * @file calibrationService.cpp
 */

#include "Pipeline/Services/calibrationService.h"

BottleBuddy::Embedded::Pipeline::Services::CalibrationService::CalibrationService(const char* uid) : Service(uid) {

}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::connect(BLEDevice central) {}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::disconnect(BLEDevice central) {}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::loop() {}

void BottleBuddy::Embedded::Pipeline::Services::CalibrationService::receive(Package* package) {}
