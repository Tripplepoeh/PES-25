#include <FastLED.h>
#include <ESP8266WiFi.h>

#define LED_PIN D0  // Gebruik pin D0 (GPIO16)
#define Druk_Sensor A0

#define NUM_LEDS 11
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

const char* ssid = "YOUR_SSID";     // Your WiFi SSID
const char* password = "YOUR_PASSWORD"; // Your WiFi Password

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
static int waarde = 100;


void setup() {
  pinMode(LED_PIN, OUTPUT);  // Zet D0 als output
  pinMode(Druk_Sensor, INPUT);

  

  digitalWrite(LED_PIN, HIGH);
  Serial.begin(9600);
  Serial.println("hello");
  delay(3000);  // power-up safety delay

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {

  digitalWrite(LED_PIN, HIGH);  // Zet LED aan
  int val = analogRead(Druk_Sensor);
  Serial.println(val);
  if (!(val >= 30)) {
    pasLedAan(0);
  } else {
    pasLedAan(1);
  }
  FastLED.delay(1000 / UPDATES_PER_SECOND);

  delay(500);
}

void pasLedAan(int situatie) {

  if(situatie == waarde){
    return;
  }

  if (situatie == 1) {

    for (int colorStep = 255; colorStep >= 0; colorStep--) {
      int r = 255;
      int g = colorStep;
      int b = colorStep;

      // Now loop though each of the LEDs and set each one to the current color
      for (int x = 0; x < NUM_LEDS; x++) {
        leds[x] = CRGB(r, g, b);
      }

      // Display the colors we just set on the actual LEDs
      delay(10);
      FastLED.show();
    }
    waarde = 1;
    return;
  }
  if (situatie == 0) {
    for (int colorStep = 0; colorStep <= 255; colorStep++) {
      int r = 255;
      int g = colorStep;
      int b = colorStep;

      // Now loop though each of the LEDs and set each one to the current color
      for (int x = 0; x < NUM_LEDS; x++) {
        leds[x] = CRGB(r, g, b);
      }

      // Display the colors we just set on the actual LEDs
      delay(10);
      FastLED.show();
    }
    waarde = 0;
    return;
  }
}
