#include "WiFiManager.h"


WiFiManager::WiFiManager(const char* ssid, const char* password, const char* serverIP, int serverPort)
    : ssid(ssid), password(password), serverIP(serverIP), serverPort(serverPort) {}

void WiFiManager::wifiInit() {
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    connectToServer();
}

void WiFiManager::disconnectFromServer() {
    if (client.connected()) {
      client.stop();
      Serial.println("Disconnected from server.");
    }
  }

void WiFiManager::connectToServer() {
    if (client.connect(serverIP, serverPort)) {
        Serial.println("Connected to server");
        client.println("hoi ik ben wemos");
    } else {
        Serial.println("Connection to server failed");
    }
}

void WiFiManager::sendData(const char* data) {
    if (client.connected()) {
        client.println(data);
    } else {
        Serial.println("Server disconnected, reconnecting...");
        connectToServer();
    }
}

char* WiFiManager::receiveData(){
     static char buffer[512];  // Zorg dat de buffer geldig blijft na de functie
    int index = 0;

    // Wacht tot er data beschikbaar is
    unsigned long startTime = millis();
    while (!client.available()) {
        if (millis() - startTime > 2000) {  // timeout van 2 seconden
            Serial.println("No response from server");
            buffer[0] = '\0';
            return buffer;
        }
        delay(10);
    }

    // Lees de data zolang die beschikbaar is
    while (client.available() && index < sizeof(buffer) - 1) {
        char c = client.read();
        buffer[index++] = c;
    }
    buffer[index] = '\0';  // Sluit de string af

    Serial.print("Received from server: ");
    Serial.println(buffer);
    return buffer;
}
