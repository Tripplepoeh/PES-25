#include <FastLED.h>
#include "LEDstrip.h"
#include "Druksensor.h"
#include "WiFiManager.h"

// WiFi en serverinstellingen
const char* WIFI_SSID = "NSELab";
const char* WIFI_PASSWORD = "NSELabWiFi";
const char* SERVER_IP = "145.52.127.184";
const int SERVER_PORT = 9999;

// Hardwaredefinities
#define LED_PIN D0  
#define DRUK_SENSOR A0
#define PIR_PIN D6
#define NUM_LEDS 9
#define BUTTON_PIN D5

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);
LEDstrip ledstrip(LED_PIN, NUM_LEDS);  // LEDstrip-object
Druksensor drukSensor(DRUK_SENSOR);    // Druksensor-object


unsigned long buttonPressTime = 0;
unsigned long lastMotionTime = 0;
bool lightIsOff = true;
char buffer[100] = {0};

void setup() {
    Serial.begin(115200);
    ledstrip.init();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(PIR_PIN, INPUT);
    wifiManager.wifiInit();
}

void loop() {
    int val = drukSensor.getValue();
    int pirState = digitalRead(PIR_PIN);

    snprintf(buffer, sizeof(buffer), "Druksensor: %d, PIR: %d", val, pirState);
    Serial.println(buffer);

    // Logica druksensor
    if (drukSensor.isPressed()) {
        if (buttonPressTime == 0) {
            buttonPressTime = millis();
        }
        if (millis() - buttonPressTime >= 5000 && !lightIsOff) {
            ledstrip.lichtUit();
            lightIsOff = true;
            Serial.println("Licht uit na 5 seconden druk");
        }
    } else {
        buttonPressTime = 0; // Reset timer
    }

     if (millis() - lastSendTime > sendInterval) {
    char sendBuffer[50];
    snprintf(sendBuffer, sizeof(sendBuffer), "set druksensor %d\n", sensorValue);
    wifiManager.sendData(sendBuffer);

    char* recvBuffer = wifiManager.receiveData();
    Serial.print("Antwoord van server: ");
    Serial.println(recvBuffer);

    lastSendTime = millis();
    }
    
    // Bewegingssensor logica
    if (pirState == HIGH) {
        lastMotionTime = millis();  // Beweging gedetecteerd → reset timer

        if (lightIsOff) {
            ledstrip.setColor(1);  // Licht aan
            lightIsOff = false;
            Serial.println("Beweging gedetecteerd, licht aan");
        }
    } else {
        // Als er 10 seconden geen beweging is geweest
        if (!lightIsOff && millis() - lastMotionTime >= 10000) {
            ledstrip.lichtUit();
            lightIsOff = true;
            Serial.println("Geen beweging gedurende 10 seconden, licht uit");
        }
    }

    ledstrip.update();
    delay(100);
}
