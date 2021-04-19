/**
 * @file waterIntakeService.h
 */

#pragma once

#include "Pipeline/service.h"
#include <vector>
#include <arduino-timer.h>
#include <MahonyAHRS.h>

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    struct time {
        unsigned short year;
        unsigned char month;
        unsigned char day;
        unsigned char hour;
        unsigned char minute;
        unsigned char second;
    } typedef Time;

    struct waterIntakePackage {
        unsigned short id;
        Time* timestamp;
        unsigned short oldHeight;
        unsigned short newHeight;
    } typedef WaterPackage;

    /**
     * @brief This service tracks how much water a Bottle Buddy user drinks throughout the day.
     * 
     * Specifically, the service creates a set of timestamped values corresponding to when and how much water
     * a user drank during the day. Additionally, it streams this dataset to the Bottle Buddy App.
     */
    class WaterIntakeService: public Service {
    public:
        WaterIntakeService(const char* uid, Time* initTimestamp, bool connected = false);

        void connect(BLEDevice central);
        void disconnect(BLEDevice central);

        void loop();
        void receive(Package* package);

        /**
         * @brief Creates a timestamp object.
         * 
         * Given a date and time value as specified during the GATT phase, populates the given
         * timestamp pointer accordingly.
         */
        static Time* createTimestamp(unsigned int date, unsigned int time, Time* timestamp);
    private:
        Timer<> timer;

        /**
         * @brief Specifies if we are connected to a central device.
         */
        bool connected;

        const int BLUE_LED = LEDB;
        const int RED_LED = LEDR;
        const int GREEN_LED = LEDG;

        /**
         * @brief Specifies if we are waiting for an acknowledgment from the app.
         * 
         * Once we send a water package, the app must acknowledge its reception before
         * we can send any more water packages.
         */
        bool waitingForAck;
        unsigned short deliveredId;
        unsigned short nextId;

        Time* currTime;

        Mahony *filter;

        /**
         * @brief Tolerance for creating a new water package.
         * 
         * If the average tof reading is greater than this tolerance after leaving,
         * then Bottle Buddy creates a new water package.
         */
        int WATER_LEVEL_TOLERANCE = 5;

        int tofReading;
        int currWaterLevel;
        bool initializedWaterLevel;
        bool updatedWaterLevel;
        std::vector<int> waterReadings;
        uintptr_t updateWaterTask;

        /**
         * @brief Specifies if Bottle Buddy is in the drinking position
         * 
         * Once this position is entered, it is highly possible the user is taking
         * a drink of water, so we should check the water level once this position is left.
         */
        bool enteredDrinkingPos;
        /**
         * @brief Specifies if we are waiting for the user to stop taking a drink of water.
         * 
         * If this condition becomes true, then once it becomes false we need to check the
         * water level to see if the user drank water.
         */
        bool waitingToStopDrinking;
        int waterLevelBeforeDrinking;
        Time* timeWhenDrank;

        float accelX, accelY, accelZ;
        float gyroX, gyroY, gyroZ;
        float magneticX, magneticY, magneticZ;

        int fsrReading1, fsrReading2;
        const int FSR_THRESHOLD = 250;
        bool capIsOn();

        std::vector<WaterPackage*> waterPackages;

        /**
         * @brief Updates the Bottle Buddy's idea of the current time.
         */
        static bool updateTime(void *waterInstance);

        /**
         * @brief Updates the Bottle Buddy's orientation using the Mahony algorithm.
         */
        static bool updateOrientation(void *waterInstance);

        /**
         * @brief Updates the water level.
         * 
         * Takes [magic number] number of readings and averages them together, with
         * each reading being one second apart.
         */
        static bool updateWaterLevel(void* waterInstance);

        /**
         * @brief Caches a water package to be sent over BLE to the app.
         */
        void cacheWaterPackage(int oldHeight, int newHeight);
        /**
         * @brief Sends a water package over BLE to the app.
         */
        void sendWaterPackage();
        bool removeWaterPackage(unsigned short id);

        static bool turnLEDOff(void* waterInstance);
    };

}}}}
