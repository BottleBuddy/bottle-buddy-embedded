/**
 * @file package.h
 */

#pragma once

#include <stdlib.h>

namespace BottleBuddy { namespace Embedded { namespace Pipeline {

    /**
     * @brief Location corresponding to a low level sensor.
     * 
     * Used to mark the location of a pipe so that the router class can know what to do with the data that comes down that pipe.
     */
    enum Location { ToF, ACCELEROMETER, GYRO, MAGNETIC };

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

        bool getData(int& data);
        bool getData(int& dim1, int& dim2, int& dim3);
        bool getData(float& data);
        bool getData(float& dim1, float& dim2, float& dim3);
    private:
        enum BBDataType { BBInt, BBFloat };

        /**
         * @brief Represents the location from which this package is from.
         */
        Location origin;

        BBDataType datatype;

        void* dataptr;
    };

}}}
