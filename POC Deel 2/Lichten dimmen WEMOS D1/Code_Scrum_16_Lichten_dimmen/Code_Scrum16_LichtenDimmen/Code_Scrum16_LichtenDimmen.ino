#include <stdio.h>
#include <string.h>
#include <FastLED.h>

#include "LEDstrip.h"
#include "Druksensor.h"
#include "WiFiManager.h"

const char* WIFI_SSID = "NSELab";
const char* WIFI_PASSWORD = "NSELabWiFi";
const char* SERVER_IP = "145.52.127.184";
const int SERVER_PORT = 12345;

#define LED_PIN D0
#define DRUK_SENSOR A0
#define NUM_LEDS 8

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);
LEDstrip ledstrip(LED_PIN, NUM_LEDS);
Druksensor drukSensor(DRUK_SENSOR);

unsigned long buttonPressTime = 0;
unsigned long lastSendTime = 0;

const unsigned long sendInterval = 1000;
const unsigned long autoLightOffDelay = 5000;  // 5 seconden

bool lightIsOff = false;
bool pendingLightOff = false;

void setup() {
  Serial.begin(115200);
  ledstrip.init();
  wifiManager.wifiInit();
  ledstrip.lichtAan();
  lightIsOff = false;
  buttonPressTime = 0;
  pendingLightOff = false;
}

void loop() {
  int rawValue = drukSensor.getValue();
  unsigned long currentTime = millis();

  // === Data naar server sturen ===
  wifiManager.connectToServer();
  char* recvBuffer = wifiManager.receiveData();
  char buffer[128];
  snprintf(buffer, sizeof(buffer), "set druksensor %d get ledstrip", rawValue);
  Serial.println(buffer);
  wifiManager.sendData(buffer);
  recvBuffer = wifiManager.receiveData();
  wifiManager.disconnectFromServer();
  Serial.println(recvBuffer);

  // === Serverrespons verwerken ===
  char* regel = strtok(recvBuffer, "\n");
  while (regel != NULL) {
    Serial.print("Verwerkte regel: ");
    Serial.println(regel);

    if (strncmp(regel, "ledstrip: speciaal", 18) == 0) {
      if (!lightIsOff && !pendingLightOff) {
        buttonPressTime = currentTime;
        pendingLightOff = true;
        Serial.println("Server vraagt om licht speciaal. Start 10s timer...");
      }

      if (pendingLightOff && (currentTime - buttonPressTime >= autoLightOffDelay)) {
        ledstrip.lichtUit();
        lightIsOff = true;
        pendingLightOff = false;
        Serial.println("Licht UIT na 10 seconden.");
      }
    }
    else if (strncmp(regel, "ledstrip: aan", 13) == 0) {
      if (lightIsOff) {
        ledstrip.lichtAan();
        lightIsOff = false;
        Serial.println("Server zegt: licht AAN.");
      }
      // Reset eventuele uit-aftelling
      buttonPressTime = 0;
      pendingLightOff = false;
    }else if (strncmp(regel, "ledstrip: uit", 13) == 0) {
      if (!lightIsOff && !pendingLightOff) {
        buttonPressTime = currentTime;
        pendingLightOff = true;
        Serial.println("Server vraagt om licht speciaal. Start 10s timer...");
      }
      if (!lightIsOff) {
        ledstrip.LichtDimmen();
        lightIsOff = true;
        Serial.println("Server zegt: licht uit.");
      }else if (pendingLightOff && (currentTime - buttonPressTime >= autoLightOffDelay)) {
        ledstrip.lichtUit();
        lightIsOff = true;
        pendingLightOff = false;
        Serial.println("Licht UIT na 10 seconden.");
      }
      
    }

    regel = strtok(NULL, "\n");
  }

  ledstrip.update();
  delay(100);  // Niet te vaak naar server
}
