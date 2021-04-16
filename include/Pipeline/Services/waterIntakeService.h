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
        unsigned char year;
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

        static Time* createTimestamp(unsigned int date, unsigned int time, Time* timestamp);
    private:
        Timer<> timer;

        bool connected;

        bool waitingForAck;
        unsigned short deliveredId;
        unsigned short nextId;

        Time* currTime;

        Mahony *filter;

        int WATER_LEVEL_TOLERANCE = 5;

        int tofReading;
        int currWaterLevel;
        bool initializedWaterLevel;
        bool updatedWaterLevel;
        std::vector<int> waterReadings;
        uintptr_t updateWaterTask;

        bool enteredDrinkingPos;
        bool waitingToStopDrinking;
        int waterLevelBeforeDrinking;
        Time* timeWhenDrank;

        float accelX, accelY, accelZ;
        float gyroX, gyroY, gyroZ;
        float magneticX, magneticY, magneticZ;

        std::vector<WaterPackage*> waterPackages;

        static bool updateTime(void *waterInstance);

        static bool updateOrientation(void *waterInstance);

        static bool updateWaterLevel(void* waterInstance);

        void cacheWaterPackage(int oldHeight, int newHeight);
        void sendWaterPackage();
        bool removeWaterPackage(unsigned short id);
    };

}}}}
