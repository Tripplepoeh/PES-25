#include <Arduino.h>
#include "WiFiManager.h"
#include "BedLightController.h"
//#include "WiFiManager.cpp"
//#include "BedLightController.cpp"



// WiFi- en serverconfiguratie
const char* WIFI_SSID = "NSELab";
const char* WIFI_PASSWORD = "NSELabWiFi";
const char* SERVER_IP = "145.52.127.184"; // Raspberry Pi IP
const int SERVER_PORT = 9999;             // Raspberry Pi poort

// LED-strip en sensorconfiguratie
const int LED_PIN = D0;
const int NUM_LEDS = 9;
const int SENSOR_PIN = A0;
//const int BUTTON_PIN = D5;

// Objecten aanmaken
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);
//BedLightController bedLight(LED_PIN, NUM_LEDS, SENSOR_PIN, BUTTON_PIN);

void setup() {
    Serial.begin(115200);
    wifiManager.wifiInit();   // Verbind met WiFi en Raspberry Pi server
    //bedLight.begin();  
    char buffer[128];            // Initialiseer LED-strip en sensoren
     snprintf(buffer, sizeof(buffer), "set druksensor %d", 63);
    Serial.println(buffer);
    wifiManager.sendData(buffer);
    char* recvBuffer = wifiManager.receiveData();
    Serial.println(recvBuffer);

    char buffer1[128];            // Initialiseer LED-strip en sensoren
     snprintf(buffer1, sizeof(buffer1), "get druksensor");
    Serial.println(buffer1);
    wifiManager.sendData(buffer1);
    char* recvBuffer1 = wifiManager.receiveData();
    Serial.println(recvBuffer1);
}

void loop() {
    char buffer[128];

    // Druksensor uitlezen en naar server sturen
    //int sensorValue = bedLight.getSensorValue();
    //snprintf(buffer, sizeof(buffer), "Druksensor: %d", sensorValue);
    //Serial.println(buffer);
    //wifiManager.sendData(buffer);

    // LEDs bijwerken op basis van drukwaarde
    // bedLight.update();

    // // Indien knop ingedrukt, ook melden aan server
    // if (bedLight.isButtonPressed()) {
    //     snprintf(buffer, sizeof(buffer), "Knop: 1");
    //     Serial.println(buffer);
    //     wifiManager.sendData(buffer);
    // }

//   snprintf(buffer, sizeof(buffer), "set druksensor %d", 1);
//   Serial.println(buffer);
//     wifiManager.sendData(buffer);
//     char* recvBuffer = wifiManager.receiveData();
//     Serial.println(recvBuffer);

//     delay(500); // Eventuele debounce / performance delay
}
