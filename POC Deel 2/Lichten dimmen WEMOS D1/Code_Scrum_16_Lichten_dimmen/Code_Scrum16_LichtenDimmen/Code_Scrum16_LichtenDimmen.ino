#include <stdio.h>
#include <string.h>
#include <FastLED.h>

#include "LEDstrip.h"
#include "Druksensor.h"
#include "WiFiManager.h"

// Netwerkgegevens voor WiFi-verbinding en server
const char* WIFI_SSID = "NSELab";
const char* WIFI_PASSWORD = "NSELabWiFi";
const char* SERVER_IP = "145.52.127.184";
const int SERVER_PORT = 12345;

// Hardwaredefinities
#define LED_PIN D0           // Pin waarop de LED-strip is aangesloten
#define DRUK_SENSOR A0       // Analoge pin voor de druksensor
#define NUM_LEDS 8           // Aantal LEDs op de strip

// Initialisatie van objecten voor WiFi, LEDstrip en Druksensor
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);
LEDstrip ledstrip(LED_PIN, NUM_LEDS);
Druksensor drukSensor(DRUK_SENSOR);

// Variabelen voor tijdsregistratie
unsigned long buttonPressTime = 0;
unsigned long lastSendTime = 0;

// Tijdinstellingen (in milliseconden)
const unsigned long sendInterval = 1000;           // Interval om data te versturen
const unsigned long autoLightOffDelay = 5000;      // Tijd voordat licht automatisch uitgaat (5 seconden)

// Statusvariabelen voor verlichting
bool lightIsOff = false;
bool pendingLightOff = false;

void setup() {
  Serial.begin(115200);        // Start seriële communicatie voor debugging
  ledstrip.init();             // Initialiseer de LED-strip
  wifiManager.wifiInit();      // Maak WiFi-verbinding
  ledstrip.lichtAan();         // Zet het licht aan bij opstarten
  lightIsOff = false;
  buttonPressTime = 0;
  pendingLightOff = false;
}

void loop() {
  // int rawValue = drukSensor.getValue();           // Lees waarde van de druksensor
  int rawValue = 512;
  Serial.print("Test druksensorwaarde: ");
  Serial.println(rawValue);
  unsigned long currentTime = millis();           // Huidige tijd (milliseconden sinds start)

  // === Data naar server sturen ===
  wifiManager.connectToServer();                  // Maak verbinding met de server
  char* recvBuffer = wifiManager.receiveData();   // Ontvang eventuele eerste data van server
  char buffer[128];
  snprintf(buffer, sizeof(buffer), "set druksensor %d get ledstrip", rawValue); // Maak bericht aan server
  Serial.println(buffer);
  wifiManager.sendData(buffer);                   // Verstuur data naar server
  recvBuffer = "ledstrip: speciaal";      // Ontvang reactie van server
  wifiManager.disconnectFromServer();             // Verbreek serververbinding
  Serial.println(recvBuffer);

  // === Serverrespons verwerken ===
  char* regel = strtok(recvBuffer, "\n");         // Splits serverrespons per regel
  while (regel != NULL) {
    Serial.print("Verwerkte regel: ");
    Serial.println(regel);

    // Server vraagt om "speciaal" licht: start timer en schakel na verloop uit
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
    // Server zegt dat het licht aan moet
    else if (strncmp(regel, "ledstrip: aan", 13) == 0) {
      if (lightIsOff) {
        ledstrip.lichtAan();
        lightIsOff = false;
        Serial.println("Server zegt: licht AAN.");
      }
      // Reset eventuele uit-timer
      buttonPressTime = 0;
      pendingLightOff = false;
    }
    // Server zegt dat het licht uit moet
    else if (strncmp(regel, "ledstrip: uit", 13) == 0) {
      if (!lightIsOff && !pendingLightOff) {
        buttonPressTime = currentTime;
        pendingLightOff = true;
        Serial.println("Server vraagt om licht speciaal. Start 10s timer...");
      }
      if (!lightIsOff) {
        ledstrip.LichtDimmen();
        lightIsOff = true;
        Serial.println("Server zegt: licht uit.");
      }
      else if (pendingLightOff && (currentTime - buttonPressTime >= autoLightOffDelay)) {
        ledstrip.lichtUit();
        lightIsOff = true;
        pendingLightOff = false;
        Serial.println("Licht UIT na 10 seconden.");
      }
    }

    regel = strtok(NULL, "\n");   // Volgende regel verwerken
  }

  ledstrip.update();              // Update de LED-strip status
  delay(100);                     // Wacht kort om server niet te vaak te benaderen
}
