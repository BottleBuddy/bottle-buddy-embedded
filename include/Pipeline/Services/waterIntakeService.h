/**
 * @file waterIntakeService.h
 */

#pragma once

#include "Pipeline/service.h"
#include <vector>
#include <MahonyAHRS.h>

namespace BottleBuddy { namespace Embedded { namespace Pipeline { namespace Services {

    struct waterIntakePackage {
        int timestamp;
        int volumeDrank;
    } typedef WaterPackage;

    /**
     * @brief This service tracks how much water a Bottle Buddy user drinks throughout the day.
     * 
     * Specifically, the service creates a set of timestamped values corresponding to when and how much water
     * a user drank during the day. Additionally, it streams this dataset to the Bottle Buddy App.
     */
    class WaterIntakeService: public Service {
    public:
        WaterIntakeService(const char* uid);

        void loop();
        void receive(Package* package);
    private:
        int WATER_LEVEL_TOLERANCE = 5;

        int currWaterLevel;
        bool updatedWaterLevel;
        std::vector<int> waterReadings;

        bool enteredDrinkingPos;
        bool waitingToStopDrinking;
        int waterLevelBeforeDrinking;

        std::vector<WaterPackage*> waterPackages;

        void updateWaterLevel(int waterReading);

        void cacheWaterIntake(int volumeDrank);
    };

}}}}
