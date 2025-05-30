#include <ESP8266WiFi.h>
#include <Servo.h>
#include <FastLED.h>

// WiFi-gegevens
const char* ssid = "NSELab";     
const char* password = "NSELabWiFi"; 
const char* serverIP = "145.52.127.184"; 
const int serverPort = 12348; 
WiFiClient client;

// Hardwaredefinities
#define LED_PIN D0  
#define Druk_Sensor A0
#define NUM_LEDS 11
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

#define BUTTON_PIN D5

CRGBPalette16 currentPalette;
TBlendType currentBlending;

static int waarde = 100;
char buffer[100] = {0};

void setup() {
    Serial.begin(115200);
    wifiinit();
    ledsinit();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    int val = analogRead(Druk_Sensor);

    // Correcte string opbouw
    snprintf(buffer, sizeof(buffer), "Druksensor: %d", val);
    Serial.println(buffer);

    // Controleer of client verbonden is en stuur bericht
    if (client.connected()) {
        client.print(buffer); // GEEN println(), voorkomt extra \r\n
    } else {
        Serial.println("Verbinding met server verbroken, opnieuw verbinden...");
        if (client.connect(serverIP, serverPort)) {
            Serial.println("Opnieuw verbonden!");
        } else {
            Serial.println("Herverbinden mislukt.");
        }
    }

    if (val >= 30) {
        pasLedAan(0);
    } else {
        pasLedAan(1);
    }
    

    if (digitalRead(BUTTON_PIN) == LOW) {
      snprintf(buffer, sizeof(buffer), "Drukknop: %d", 1);
      Serial.println(buffer);
      client.print(buffer);
    }

    FastLED.delay(1000 / UPDATES_PER_SECOND);
    delay(500);
}

void pasLedAan(int situatie) {
    if (situatie == waarde) return;

    for (int colorStep = 0; colorStep <= 255; colorStep++) {
        int r = 255;
        int g = (situatie == 1) ? colorStep : 255 - colorStep;
        int b = (situatie == 1) ? colorStep : 255 - colorStep;

        for (int x = 0; x < NUM_LEDS; x++) {
            leds[x] = CRGB(r, g, b);
        }

        delay(10);
        FastLED.show();
    }
    waarde = situatie;
}

void ledsinit() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(Druk_Sensor, INPUT);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("hello");

    delay(3000);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

void wifiinit() {
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (client.connect(serverIP, serverPort)) {
        Serial.println("Connected to server");
    } else {
        Serial.println("Connection to server failed");
    }

    client.print("hoi ik ben wemos");
}
