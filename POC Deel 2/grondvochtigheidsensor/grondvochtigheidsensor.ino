#include "grondvochtigheidSensor.h"
#include "WiFiManager.h"

const char* WIFI_SSID = "NSELab";
const char* WIFI_PASSWORD = "NSELabWiFi";
const char* SERVER_IP = "145.52.127.184"; // Raspberry Pi IP
const int SERVER_PORT = 12345;             // Raspberry Pi poort

grondvochtigheidSensor grondSensor;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifiManager.wifiInit(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  grondSensor.leesSensor();
  int sensorWaarde = grondSensor.getWaarde();
  Serial.println(sensorWaarde);
  Serial.println();
  wifiManager.connectToServer();
  char buffer[128];            // Initialiseer LED-strip en sensoren
  snprintf(buffer, sizeof(buffer), "set grondsensor %d", sensorWaarde);
  Serial.println(buffer);
  wifiManager.sendData(buffer);
  char* recvBuffer = wifiManager.receiveData();
  Serial.println(recvBuffer);
  wifiManager.disconnectFromServer();
  delay(500);
}
