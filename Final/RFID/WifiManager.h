#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

class WiFiManager {
private:
    const char* ssid;
    const char* password;
    const char* serverIP;
    int serverPort;
    WiFiClient client;

public:
    WiFiManager(const char* ssid, const char* password, const char* serverIP, int serverPort);
    void wifiInit();
    void connectToServer();
    void disconnectFromServer();
    void sendData(const char* data);
    char* receiveData();
};

#endif
