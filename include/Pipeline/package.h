/**
 * @file package.h
 */

#pragma once

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Location corresponding to a low level sensor.
     * 
     * Used to mark the location of a pipe so that the router class can know what to do with the data that comes down that pipe.
     */
    enum Location { ToF, ACCELEROMETER };

    /**
     * @brief Encapsulates low level sensor data traveling through the pipeline.
     * 
     * This class provides a clean interface both for low level drives to transport their sensor data as well as
     * high level services to read data from low level sensors.
     */
    class Package {
    public:
        Package(Location origin, int data);
        Package(Location origin, float data);

        Package(Location origin, int dim1, int dim2, int dim3);
        Package(Location origin, float dim1, float dim2, float dim3);

        ~Package();

        /**
         * @brief Returns the location corresponding to which sensor this package is from.
         */
        Location getOrigin();
    private:
        /**
         * @brief Represents the location from which this package is from.
         */
        Location origin;
    };

}}}
