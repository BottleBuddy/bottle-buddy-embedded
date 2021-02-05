/**
 * @file pipeFactory.cpp
 */

#include "Pipeline/pipeFactory.h"

BottleBuddy::Embedded::Pipeline::Pipe* BottleBuddy::Embedded::Pipeline::PipeFactory::producePipe(BottleBuddy::Embedded::Pipeline::Location location) {
    if (NULL == location) {
        //TODO: Handle invalid pipe construction.
    }

    return new BottleBuddy::Embedded::Pipeline::Pipe(location);
}
