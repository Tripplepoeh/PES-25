#include "RFIDSensor.hpp"
#include "WiFiManager.h"
#include <string.h>
#include <stdlib.h>
#define RFID_SS_PIN D8
#define RFID_RST_PIN D3

const char* WIFI_SSID = "NSELab";  // SSID van het WiFi-netwerk
const char* WIFI_PASSWORD = "NSELabWiFi";  // Wachtwoord voor het WiFi-netwerk
const char* SERVER_IP = "145.52.127.184";  // IP-adres van de server waarmee gecommuniceerd wordt
const int SERVER_PORT = 12345;  // Poortnummer van de server


RFIDSensor rfid(RFID_SS_PIN, RFID_RST_PIN);

WiFiManager wifi(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);

void setup() {
    Serial.begin(9600);
    rfid.init();
    wifi.wifiInit();
}

void loop() {
  wifi.connectToServer();
    if (rfid.checkCard()) {
        uint8_t* uid = rfid.getUID();
        uint8_t size = 5;

        // Convert UID to string format (hex)
        char uidStr[32];
        char* uidsend;
        for (int i = 0; i < size; ++i) {
            sprintf(&uidStr[i * 2], "%02X", uid[i]);
        }

        char sender[64];
        uint64_t HtoI = strtoull(uidStr, nullptr, 16);
        snprintf(sender, sizeof(sender), "set rfidsensor %llu", (uint64_t)HtoI);
        Serial.println(sender);
        

        // Send to server
        wifi.sendData(sender);

        // Wait for response from server
        char* response = wifi.receiveData();
        Serial.println(response);

        delay(2000); 
    }
    else {
      char sender2[64];
      snprintf(sender2, sizeof(sender2), "set rfidsensor %d", 0);
      wifi.sendData(sender2);
      delay(100);
    }
    wifi.disconnectFromServer();
}
