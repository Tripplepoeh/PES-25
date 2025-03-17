#include <ESP8266WiFi.h>

#define BUTTON_PIN D5
const char* ssid = "SSID"; // Name of the WiFi network
const char* password = "PASSWORD"; // Password of the WiFi network
const char* serverIP = "0.0.0.0"; // IP of the server
const int serverPort = 8080; // Port of the server

WiFiClient client;

void setup() {
    pinMode(BUTTON_PIN, INPUT); // Set the button pin as input
    Serial.begin(115200);

    WiFi.begin(ssid, password); // Connect to the WiFi network
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");


    if (client.connect(serverIP, serverPort)) {
        Serial.println("Connected to server");
    } else {
        Serial.println("Connection to server failed");
    }
}


// data sturen in de loop functie
    // stuur data met client.println("data");
    // dit kan sensor data zijn of een button press
    // ontvang data met client.readStringUntil('\n'); 

void loop() {
    if (digitalRead(BUTTON_PIN) == HIGH) {
        client.println("Button pressed");
        Serial.println("sent: Button pressed");
    }
    delay(1000); // debounce 1 second

    if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(response);
    }
}