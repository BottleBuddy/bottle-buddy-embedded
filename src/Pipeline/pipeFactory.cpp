/**
 * @file pipeFactory.cpp
 */

#include "Pipeline/pipeFactory.h"

BottleBuddy::Embedded::Pipeline::Pipe* BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location location) {
    if (NULL == location) {
        throw std::runtime_error("Pipe construction must provide location.");
    }

    return new BottleBuddy::Embedded::Pipeline::Pipe(location);
}
