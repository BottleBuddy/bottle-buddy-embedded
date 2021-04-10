/**
 * @file service.cpp
 */

#include "Pipeline/service.h"

BottleBuddy::Embedded::Pipeline::Service::Service(const char* uid) {
    this->uid = uid;
    this->numCharacteristics = 0;

    this->bleService = new BLEService(uid);

    hexConversions.emplace('a', 10);
    hexConversions.emplace('A', 10);
    hexConversions.emplace('b', 11);
    hexConversions.emplace('B', 11);
    hexConversions.emplace('c', 12);
    hexConversions.emplace('C', 12);
    hexConversions.emplace('d', 13);
    hexConversions.emplace('D', 13);
    hexConversions.emplace('e', 14);
    hexConversions.emplace('E', 14);
    hexConversions.emplace('f', 15);
    hexConversions.emplace('F', 15);
}

BottleBuddy::Embedded::Pipeline::Service::~Service() {
	for (std::vector<const char*>::iterator uuidIter = uuids.begin(); uuidIter != uuids.end(); uuidIter++) {
		const char* uuid = *uuidIter;
		free((void*)uuid);
	}
	for (auto it = characteristics.begin(); it != characteristics.end(); it++) {
		delete it->second;
	}
	for (auto it = stringCharacteristics.begin(); it != stringCharacteristics.end(); it++) {
		delete it->second;
	}
}

bool BottleBuddy::Embedded::Pipeline::Service::createCharacteristic(std::string name, uint8_t properties, BottleBuddy::Embedded::Pipeline::BLEType characteristicType) {
	if (numCharacteristics >= 15) {
		return false;
	}

	const char* characteristicUUID = makeCharacteristicUUID();
	if (characteristicUUID == NULL) {
		return false;
	}
	uuids.push_back(characteristicUUID);
	numCharacteristics++;

	if (characteristicType == BottleBuddy::Embedded::Pipeline::BLEType::String) {
		BLEStringCharacteristic* bleCharacteristic = new BLEStringCharacteristic(characteristicUUID, properties, 64);
		stringCharacteristics.emplace(name, bleCharacteristic);
		bleService->addCharacteristic(*bleCharacteristic);
	} else {
		BLECharacteristic* bleCharacteristic;
		switch (characteristicType) {
			case BottleBuddy::Embedded::Pipeline::BLEType::UnsignedLong:
				bleCharacteristic = new BLEUnsignedLongCharacteristic(characteristicUUID, properties);
				break;
			case BottleBuddy::Embedded::Pipeline::BLEType::UnsignedShort:
				bleCharacteristic = new BLEUnsignedShortCharacteristic(characteristicUUID, properties);
				break;
			case BottleBuddy::Embedded::Pipeline::BLEType::UnsignedChar:
				bleCharacteristic = new BLEUnsignedCharCharacteristic(characteristicUUID, properties);
				break;
			case BottleBuddy::Embedded::Pipeline::BLEType::Boolean:
				bleCharacteristic = new BLEBooleanCharacteristic(characteristicUUID, properties);
				break;
		}
		characteristics.emplace(name, bleCharacteristic);
		bleService->addCharacteristic(*bleCharacteristic);
	}

	return true;
}

const char* BottleBuddy::Embedded::Pipeline::Service::makeCharacteristicUUID() {
	int uid_length = strlen(uid);
	int num_bytes = sizeof(char) * (uid_length + 1);
	char* uuid_temp = (char*)malloc(num_bytes);
    std::strcpy(uuid_temp, uid);

	int i = 0;
	while (*(uuid_temp + i + 1) != '-') {
		i++;
	}
    
    int UUID_chunk = this->hexStringToInt(uuid_temp, i);
	if (UUID_chunk == -1) {
		return NULL;
	}
	UUID_chunk = UUID_chunk + numCharacteristics + 1;

	bool success = this->insertIncrementedUUID(uuid_temp, i, UUID_chunk);
	if (!success) {
		return NULL;
	}
	const char* uuid = uuid_temp;

	return uuid;
}

int BottleBuddy::Embedded::Pipeline::Service::hexStringToInt(char* str, int endIdx) {
	int ZERO_ASCII = 48;
	int NINE_ASCII = 57;
	int A_ASCII = 65;
	int F_ASCII = 70;
	int a_ASCII = 97;
	int f_ASCII = 102;

	int number = 0;
	int base = 16;
	int exponent = 0;
	while (endIdx >= 0) {
		int digit;
		if ((*(str + endIdx) >= ZERO_ASCII) && (*(str + endIdx) <= NINE_ASCII)) {
			digit = (int) * (str + endIdx) - ZERO_ASCII;
		} else if (((*(str + endIdx) >= A_ASCII) && (*(str + endIdx) <= F_ASCII)) || ((*(str + endIdx) >= a_ASCII) && (*(str + endIdx) <= f_ASCII))) {
			digit = hexConversions.at(*(str + endIdx));
		} else {
			//Invalid digit
			return -1;
		}
		number = number + (digit * (int)pow((double)base, (double)exponent));
		exponent++;
		endIdx--;
	}
	return number;
}

bool BottleBuddy::Embedded::Pipeline::Service::insertIncrementedUUID(char* str, int endIdx, int number) {
	std::vector<int> digits;
	int base = 16;
	while (number > 0) {
		int digit = number % base;
		digits.push_back(digit);
		if (digits.size() > endIdx + 1) {
			//not enough room for incrememnted UUID.
			return false;
		}
		number = number / base;
	}

	for (std::vector<int>::iterator digitsIter = digits.begin(); digitsIter != digits.end(); digitsIter++) {
		int digit = *digitsIter;
		int ascii_off;
		digit < 10 ? ascii_off = 48 : ascii_off = 55;
		*(str + endIdx) = (char)(digit + ascii_off);
		endIdx--;
	}
	return true;
}

BLECharacteristic* BottleBuddy::Embedded::Pipeline::Service::getCharacteristic(std::string name) {
	if (characteristics.find(name) != characteristics.end()) {
		return characteristics.at(name);
	}
	return NULL;
}

BLEStringCharacteristic* BottleBuddy::Embedded::Pipeline::Service::getStringCharacteristic(std::string name) {
	if (stringCharacteristics.find(name) != stringCharacteristics.end()) {
		return stringCharacteristics.at(name);
	}
	return NULL;
}
