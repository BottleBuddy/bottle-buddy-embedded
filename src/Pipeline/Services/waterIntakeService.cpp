/**
 * @file waterIntakeService.cpp
 */

#include "Pipeline/Services/waterIntakeService.h"

BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::WaterIntakeService(const char* uid) : Service(uid) {
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::receive(BottleBuddy::Embedded::Pipeline::Package package) {

}
