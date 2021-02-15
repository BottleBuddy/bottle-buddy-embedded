#include <Arduino.h>
#include <Wire.h>
#include <ArduinoBLE.h>
#include <features/BottleBuddyBLE.h>
#include <setup/Pins.h>
#include <features/CapSafety.h>

void ble_init()
{
    while (!BLE.central().connected())
    {
        if (BLE.central())
        {
            Serial.print("Connected to central: ");
            Serial.println(BLE.central().address());
        }
    }
}

void advertise()
{
    BLE.setLocalName("BBUDDY");
    // set the UUID for the service this peripheral advertises:
    BLE.setAdvertisedService(ledService);
    // add the characteristics to the service
    ledService.addCharacteristic(ledCharacteristic);
    ledService.addCharacteristic(buttonCharacteristic);
    ledService.addCharacteristic(waterCharacteristic);
    // add the service
    BLE.addService(ledService);
    //starting values for both charactersistics
    ledCharacteristic.writeValue(0);
    buttonCharacteristic.writeValue(0);
    waterCharacteristic.writeValue(0);
    // start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

int send_test_data()
{
    // read the current button pin state
    char buttonValue = digitalRead(BUTTON_PIN);
    char ledValue = digitalRead(BLUE_LED_PIN);
    // has the value changed since the last read
    boolean buttonChanged = (buttonCharacteristic.value() != buttonValue);
    if (buttonChanged)
    {
        // button state changed, update characteristics
        ledCharacteristic.writeValue(buttonValue);
        buttonCharacteristic.writeValue(buttonValue);
    }
    if (ledCharacteristic.written() || buttonChanged)
    {
        // update LED, either central has written to characteristic or button state has changed
        if (ledCharacteristic.value())
        {
            Serial.println("LED on");
            if (SAFE)
            {
                digitalWrite(BLUE_LED_PIN, HIGH);
            }
            else
            {
                digitalWrite(BLUE_LED_PIN, LOW);
            }
        }
        else
        {
            Serial.println("LED off");
            digitalWrite(BLUE_LED_PIN, LOW);
        }
    }

    return 0;
}