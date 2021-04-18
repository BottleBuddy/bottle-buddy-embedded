/**
 * @file waterIntakeService.cpp
 */

#include "Pipeline/Services/waterIntakeService.h"

BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::WaterIntakeService(const char* uid, Time* initTimestamp, bool connected) : Service(uid, connected) {
    //BLE.setAdvertisedService(*this->bleService);

    createCharacteristic(std::string("water_package_id"), BLERead | BLENotify, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedShort);
    createCharacteristic(std::string("water_package_timestamp_date"), BLERead, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("water_package_timestamp_time"), BLERead, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("water_package_heights"), BLERead, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("acknowledgment"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedShort);
    createCharacteristic(std::string("timestamp_date"), BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("timestamp_time"), BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::UnsignedInt);
    createCharacteristic(std::string("wrote_time"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);
    createCharacteristic(std::string("drink_water"), BLERead | BLEWrite, BottleBuddy::Embedded::Pipeline::BLEType::Boolean);

    BLE.addService(*this->bleService);

    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ToF, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::GYRO, this);
    BottleBuddy::Embedded::Pipeline::Router::subscribe(BottleBuddy::Embedded::Pipeline::Location::MAGNETIC, this);

    this->timer = timer_create_default();
    this->timer.every(100, BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateOrientation, this);
    
    this->connected = false;

    this->waitingForAck = false;
    this->deliveredId = 0;
    this->nextId = 1;
    getCharacteristic(std::string("acknowledgment"))->writeValue(this->deliveredId);

    this->currTime = initTimestamp;
    this->timer.every(1000, BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateTime, this);

    this->filter = new Mahony();
    this->filter->begin(10);

    this->initializedWaterLevel = false;
    this->updatedWaterLevel = false;
    this->enteredDrinkingPos = false;
    this->waitingToStopDrinking = false;
    this->timeWhenDrank = new Time();
    this->updateWaterTask = this->timer.every(1000, updateWaterLevel, this);
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::connect(BLEDevice central) {
    this->connected = true;
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::disconnect(BLEDevice central) {
    this->connected = false;
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::loop() {
    this->timer.tick();

    if (enteredDrinkingPos) {
        this->waterLevelBeforeDrinking = this->currWaterLevel;
        this->waterReadings.clear();
        this->updatedWaterLevel = false;
        this->waitingToStopDrinking = true;
        this->timer.cancel(this->updateWaterTask);
    }

    if (!enteredDrinkingPos && waitingToStopDrinking) {
        digitalWrite(23, HIGH);
        this->waitingToStopDrinking = false;

        this->timeWhenDrank->year = this->currTime->year;
        this->timeWhenDrank->month = this->currTime->month;
        this->timeWhenDrank->day = this->currTime->day;
        this->timeWhenDrank->hour = this->currTime->hour;
        this->timeWhenDrank->minute = this->currTime->minute;
        this->timeWhenDrank->second = this->currTime->second;

        this->updateWaterTask = this->timer.every(1000, updateWaterLevel, this);
    }

    if (updatedWaterLevel) {
        if ((currWaterLevel - waterLevelBeforeDrinking) > WATER_LEVEL_TOLERANCE) {
            cacheWaterPackage(waterLevelBeforeDrinking, currWaterLevel);
        }
        digitalWrite(23, LOW);
        this->waitingToStopDrinking = false;
        this->updatedWaterLevel = false;
    }

    if (waitingForAck) {
        BLECharacteristic* ackCharacteristic = getCharacteristic(std::string("acknowledgment"));
        unsigned short ackId = 0;
        ackCharacteristic->readValue(ackId);
        this->waitingForAck = ackId != this->deliveredId;
        if (ackId == this->deliveredId) removeWaterPackage(ackId);
    } else {
        sendWaterPackage();
    }

    byte wroteTime = 0;
    BLECharacteristic* wroteTimeCharacteristic = getCharacteristic(std::string("wrote_time"));
    wroteTimeCharacteristic->readValue(wroteTime);
    if (wroteTime) {
        unsigned int date = 0;
        BLECharacteristic* dateCharacteristic = getCharacteristic(std::string("timestamp_date"));
        dateCharacteristic->readValue((uint8_t*)&date, sizeof(date));

        unsigned int time = 0;
        BLECharacteristic* timeCharacteristic = getCharacteristic(std::string("timestamp_time"));
        timeCharacteristic->readValue((uint8_t*)&time, sizeof(time));

        WaterIntakeService::createTimestamp(date, time, this->currTime);
    }
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::receive(BottleBuddy::Embedded::Pipeline::Package* package) {
    switch (package->getOrigin()) {
        case BottleBuddy::Embedded::Pipeline::Location::ToF:
            int waterReading;
            if (package->getData(waterReading)) {
                if (waterReading < 20) waterReading = 20;
                this->tofReading = waterReading;
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::ACCELEROMETER:
            float x, y, z;
            if (package->getData(x, y , z)) {
                this->accelX = x;
                this->accelY = y;
                this->accelZ = z;
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::GYRO:
            if (package->getData(x, y, z)) {
                this->gyroX = x;
                this->gyroY = y;
                this->gyroZ = z;
            }
            break;
        case BottleBuddy::Embedded::Pipeline::Location::MAGNETIC:
            if (package->getData(x, y, z)) {
                this->magneticX = x;
                this->magneticY = y;
                this->magneticZ = z;
            }
            break;
    }
}

BottleBuddy::Embedded::Pipeline::Services::Time* BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::createTimestamp(unsigned int date, unsigned int time, Time* timestamp) {
    timestamp->year = (unsigned short)((date & 0xFFFF0000) >> 16);
    timestamp->month = (unsigned char)((date & 0x0000FF00) >> 8);
    timestamp->day = (unsigned char)(date & 0x000000FF);
    timestamp->hour = (unsigned char)((time & 0x00FF0000) >> 16);
    timestamp->minute = (unsigned char)((time & 0x0000FF00) >> 8);
    timestamp->second = (unsigned char)(time & 0x000000FF);

    return timestamp;
}

bool BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateTime(void *waterInstance) {
    BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService *myself = (BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService*)waterInstance;
    Time* theTime = myself->currTime;

    theTime->second = (theTime->second + 1) % 60;
    if (theTime->second == 0) theTime->minute = (theTime->minute + 1) % 60;
    if (theTime->minute == 0) theTime->hour = (theTime->hour + 1) % 24;
    if (theTime->hour == 0) {
        int numDays;
        if (theTime->month == 1) numDays = 28;
        else (theTime->month % 2) == 0 ? numDays = 31 : numDays = 30;
        theTime->day = (theTime->day + 1) % numDays;
    }
    if (theTime->day == 0) theTime->month = (theTime->month + 1) % 12;
    if (theTime->month == 0) theTime->year = theTime->year + 1;

    return true;
}

bool BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateOrientation(void *waterInstance) {
    BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService *myself = (BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService*)waterInstance;

    //myself->filter->update(myself->gyroX, myself->gyroY, myself->gyroZ, myself->accelX, myself->accelY, myself->accelZ, myself->magneticX, myself->magneticY, myself->magneticZ);
    myself->filter->updateIMU(myself->gyroX, myself->gyroY, myself->gyroZ, myself->accelX, myself->accelY, myself->accelZ);
    float pitch = myself->filter->getPitch();
    if ((pitch > 6.0)) {
        myself->enteredDrinkingPos = true;
        digitalWrite(22, HIGH);
    } else {
        myself->enteredDrinkingPos = false;
        digitalWrite(22, LOW);
    }
    return true;
}

bool BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::updateWaterLevel(void* waterInstance) {
    WaterIntakeService* myself = (WaterIntakeService*)waterInstance;

    if (myself->enteredDrinkingPos) {
        return true;
    }
    
    int magicNumber = 10;   //magic number
    if (myself->waterReadings.size() < magicNumber) {
        myself->waterReadings.push_back(myself->tofReading);
    } else {
        int averageWaterReading = 0;
        for (std::vector<int>::iterator it = myself->waterReadings.begin(); it != myself->waterReadings.end(); it++) {
            averageWaterReading += *it;
        }
        averageWaterReading = averageWaterReading / magicNumber;
        myself->currWaterLevel = averageWaterReading;
        myself->waterReadings.clear();
        if (!myself->initializedWaterLevel) {
            myself->initializedWaterLevel = true;
        } else {
            myself->updatedWaterLevel = true;
        }
        myself->timer.cancel(myself->updateWaterTask);
    }
    return true;
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::cacheWaterPackage(int oldHeight, int newHeight) {
    WaterPackage* waterPackage = new WaterPackage();
    waterPackage->timestamp = new Time();

    waterPackage->id = this->nextId;
    this->nextId = (this->nextId + 1) % SHRT_MAX;

    waterPackage->timestamp->year = this->timeWhenDrank->year;
    waterPackage->timestamp->month = this->timeWhenDrank->month;
    waterPackage->timestamp->day = this->timeWhenDrank->day;
    waterPackage->timestamp->hour = this->timeWhenDrank->hour;
    waterPackage->timestamp->minute = this->timeWhenDrank->minute;
    waterPackage->timestamp->second = this->timeWhenDrank->second;

    waterPackage->oldHeight = oldHeight;
    waterPackage->newHeight = newHeight;

    this->waterPackages.push_back(waterPackage);
}

void BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::sendWaterPackage() {
    if (!connected) {
        return;
    }
    if (this->waterPackages.size() == 0) {
        return;
    }

    WaterPackage* package = this->waterPackages.at(0);

    BLECharacteristic* dateCharacteristic = getCharacteristic(std::string("water_package_timestamp_date"));
    unsigned int yearChunk = (((unsigned int)package->timestamp->year) << 16) & 0xFFFF0000;
    unsigned int monthChunk = (((unsigned int)package->timestamp->month) << 8) & 0x0000FF00;
    unsigned int dayChunk = ((unsigned int)package->timestamp->day) & 0x000000FF;
    unsigned int date = 0x00000000 | yearChunk | monthChunk | dayChunk;
    dateCharacteristic->writeValue((uint8_t*)&date, sizeof(date));

    BLECharacteristic* timeCharacteristic = getCharacteristic(std::string("water_package_timestamp_time"));
    unsigned int hourChunk = (((unsigned int)package->timestamp->hour) << 16) & 0x00FF0000;
    unsigned int minuteChunk = (((unsigned int)package->timestamp->minute) << 8) & 0x0000FF00;
    unsigned int secondChunk = ((unsigned int)package->timestamp->second) & 0x000000FF;
    unsigned int time = 0x00000000 | hourChunk | minuteChunk | secondChunk;
    timeCharacteristic->writeValue((uint8_t*)&time, sizeof(time));

    BLECharacteristic* heightsCharacteristic = getCharacteristic(std::string("water_package_heights"));
    unsigned int oldHeightChunk = (((unsigned int)package->oldHeight) << 16) & 0xFFFF0000;
    unsigned int newHeightChunk = ((unsigned int)package->newHeight) & 0x0000FFFF;
    unsigned int heights = 0x00000000 | oldHeightChunk | newHeightChunk;
    heightsCharacteristic->writeValue((uint8_t*)&heights, sizeof(heights));

    BLECharacteristic* idCharacteristic = getCharacteristic(std::string("water_package_id"));
    idCharacteristic->writeValue((uint8_t*)&package->id, sizeof(package->id));

    this->waitingForAck = true;
    this->deliveredId = package->id;
}

bool BottleBuddy::Embedded::Pipeline::Services::WaterIntakeService::removeWaterPackage(unsigned short id) {
    for (std::vector<WaterPackage*>::iterator it = waterPackages.begin(); it != waterPackages.end(); it++) {
        WaterPackage* waterPackage = *it;
        if (waterPackage->id == id) {
            waterPackages.erase(it);
            delete(waterPackage->timestamp);
            delete(waterPackage);
            return true;
        }
    }
    return false;
}
