#include "RFIDSensor.hpp"
#include "WiFiManager.h"
#include <string.h>
#include <stdlib.h>
#define RFID_SS_PIN D8
#define RFID_RST_PIN D3

const char* WIFI_SSID = "NSELab";          // SSID van het WiFi-netwerk
const char* WIFI_PASSWORD = "NSELabWiFi";  // Wachtwoord voor het WiFi-netwerk
const char* SERVER_IP = "145.52.127.184";  // IP-adres van de server waarmee gecommuniceerd wordt
const int SERVER_PORT = 12345;             // Poortnummer van de server
unsigned long laatstescan = 0;


RFIDSensor rfid(RFID_SS_PIN, RFID_RST_PIN);

WiFiManager wifi(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);

void setup() {
  Serial.begin(9600);
  rfid.init(); //! begin de scanner
  wifi.wifiInit(); //! maak connectie met de wifi
  laatstescan = millis(); //! timer om sensor reset bij te houden
}

void loop() {
  char sender[64];
/*! kijk of er een kaar aangeboden wordt. zo ja, pak byte voor byte de uid en zet deze om naar hexadecimaal.
stuur daarna door naar de server. als er niks wordt gevonden stuur 0. */
  if (rfid.checkKaart()) {
    laatstescan = millis();
   
    uint8_t* uid = rfid.getUID();
    uint8_t grootte = 5;

    char uidStr[32];
    for (int i = 0; i < grootte; ++i) {
      sprintf(&uidStr[i * 2], "%02X", uid[i]);
    }


    uint64_t HtoI = strtoull(uidStr, nullptr, 16);
    snprintf(sender, sizeof(sender), "set rfidsensor %llu", (uint64_t)HtoI);
    Serial.println(sender);

    char* response = wifi.receiveData();
    Serial.println(response);
    

    delay(250);
  } else {
    snprintf(sender, sizeof(sender), "set rfidsensor %d", 0);
    
    delay(100);
  }
  if (millis() - lastScanTime > 10000) { //! als er 10 seconden niks wordt gezien, reinitialize de rfidsensor om lock-up te voorkomen.
        Serial.println("Reinitializing RFID");
      
      rfid.init(); 
      lastScanTime = millis();
    }
  wifi.connectToServer(); //! verbind met de zerver
  wifi.sendData(sender); //! stuur de data;
  wifi.disconnectFromServer(); //! verbreek de verbinding
}
