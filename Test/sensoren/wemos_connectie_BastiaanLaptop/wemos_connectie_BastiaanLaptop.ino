#include <ESP8266WiFi.h>
#include <Servo.h>

// Replace with your network credentials
const char* ssid = "LAPTOP-49B6IC9B 7763";     // Your WiFi SSID
const char* password = "42H0$o31"; // Your WiFi Password
Servo myservo; 



void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); // Print the assigned IP
}

void loop() {
    // Your main code here
}
