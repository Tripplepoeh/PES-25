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
unsigned long lastActivityTime = 0;
unsigned long lastSendTime = 0;
unsigned long lightOffTime = 0;

const unsigned long sendInterval = 1000;
const unsigned long autoLightOnDelay = 5000;  // 5 seconden

bool lightIsOff = false;
bool pressedPreviously = false;

void setup() {
  Serial.begin(115200);
  ledstrip.init();
  wifiManager.wifiInit();
  ledstrip.lichtAan();
  lightIsOff = false;
  lastActivityTime = millis();
}

void loop() {
  int rawValue = drukSensor.getValue();
  //bool pressed = rawValue > 800;
  unsigned long currentTime = millis();
  // === 4. Data naar server sturen ===

  wifiManager.connectToServer();
  char* recvBuffer = wifiManager.receiveData();
  char buffer[128];  // Initialiseer LED-strip en sensoren
  snprintf(buffer, sizeof(buffer), "set druksensor %d get ledstrip", rawValue);
  Serial.println(buffer);
  wifiManager.sendData(buffer);
  recvBuffer = wifiManager.receiveData();
  Serial.println(recvBuffer);
  wifiManager.disconnectFromServer();
  delay(500);


  lastSendTime = currentTime;



  char* regel = strtok(recvBuffer, "\n");
  while (regel != NULL) {
    Serial.print("Verwerkte regel: ");
    Serial.println(regel);


    if (strncmp(regel, "ledstrip: uit", 13) == 0) {
      if (!lightIsOff) {
        if (buttonPressTime == 0) {
          buttonPressTime = currentTime;
          //     Serial.println("in de if statement");
        }

        if ((currentTime - buttonPressTime >= 5000) && !lightIsOff) {
          ledstrip.lichtUit();
          lightIsOff = true;
          lightOffTime = currentTime;
          Serial.println("Licht UIT na 5 seconden drukken.");
        }

        // ledstrip.lichtUit();
        // lightIsOff = true;
        // Serial.println("Server zegt: licht UIT.");
      }
    } else if (strncmp(regel, "ledstrip: aan", 13) == 0) {
      if (lightIsOff) {
        ledstrip.lichtAan();
        lightIsOff = false;
        Serial.println("Server zegt: licht AAN.");
      }
    }
    // pressedPreviously = pressed;
    regel = strtok(NULL, "\n");
  }



  ledstrip.update();
  //delay(1000);
}

// Serial.print("Raw sensor value: ");
// Serial.print(rawValue);
// Serial.print(" → Pressed? ");
// Serial.println(pressed ? "JA" : "NEE");

//== = 1. Licht uit na 5 sec drukken == =
// int pressed = memcmp("ledstrip: uit", recvBuffer, sizeof recvBuffer);
//   if (pressed == 0) {
//   if (buttonPressTime == 0) {
//     buttonPressTime = currentTime;
//     Serial.println("in de if statement");
//   }

//   if ((currentTime - buttonPressTime >= 5000) && !lightIsOff) {
//     ledstrip.lichtUit();
//     lightIsOff = true;
//     lightOffTime = currentTime;
//     Serial.println("Licht UIT na 5 seconden drukken.");
//   }

//   lastActivityTime = currentTime;
// }
// else {
//   buttonPressTime = 0;
// }

// // === 2. Licht uit na 10 sec inactiviteit ===
// if (pressed != 0 && !lightIsOff && (currentTime - lastActivityTime >= 10000)) {
//   ledstrip.lichtUit();
//   lightIsOff = true;
//   lightOffTime = currentTime;
//   Serial.println("Licht UIT na 10 seconden inactiviteit.");
// }

// // === 3. Altijd weer aan na 5 sec uit ===
// if (lightIsOff && (currentTime - lightOffTime >= autoLightOnDelay)) {
//   ledstrip.lichtAan();
//   lightIsOff = false;
//   lastActivityTime = currentTime;
//   Serial.println("Licht AAN na 5 seconden uit geweest te zijn.");
// }
