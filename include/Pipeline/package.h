/**
 * @file package.h
 */

#pragma once

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Location corresponding to a hardware subsystem.
     * 
     * Used to mark the location of a pipe so that the router class can know what to do with the data that comes down that pipe.
     */
    enum Location { ToF, CLEANING, NOTIFICATION };

    class Package {
    public:
        Package(Location origin, int payload);

        Location getOrigin();
    private:
        Location origin;
        int payloadInt;
    };

}}}
