#include "WiFiManager.h"  // Include de WiFiManager voor WiFi-configuratie en servercommunicatie
#include "EmergencyButton.h"  // Include de EmergencyButton voor de noodknopfunctionaliteit
#include "grondvochtigheidSensor.h"

// WiFi- en serverconfiguratie
const char* WIFI_SSID = "NSELab";  // SSID van het WiFi-netwerk "NSELab"
const char* WIFI_PASSWORD = "NSELabWiFi";  // Wachtwoord voor het WiFi-netwerk "NSELabWiFi"
const char* SERVER_IP = "145.52.127.184";  // IP-adres van de server waarmee gecommuniceerd wordt "145.52.127.184"
const int SERVER_PORT = 12345;  // Poortnummer van de server "12345"

// LED Pin en Pushbutton configuratie
const int LED_PIN = D4;  // Pin voor de LED (gebruikt om de status van de noodknop aan te geven)
const int BUTTON_PIN = D5;  // Pin voor de drukknop (de noodknop)
const int BUZZER = D1; //Pin voor de buzzer

// Initialiseren van de WiFiManager en de EmergencyButton singleton
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, SERVER_IP, SERVER_PORT);  // WiFiManager instantie
EmergencyButton& knop = EmergencyButton::getInstance();  // EmergencyButton instantie via Singleton patroon
grondvochtigheidSensor grondSensor;

bool helpRequired = false;

void setup() {
    // Start de seriële communicatie voor debugging en logging
    Serial.begin(115200);  
    wifiManager.wifiInit();  // Initialiseer de WiFi verbinding en verbind met de server

    pinMode(LED_PIN, OUTPUT);  // Zet de LED-pin als uitvoer (om de status van de knop aan te geven)
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Zet de knop-pin als invoer met interne pull-up weerstand
    pinMode(5, OUTPUT); //Zet de buzzer als output
}

void loop() {
    static bool lastButtonState = HIGH;  // Bewaart de laatste knopstatus (voor de debounce logica)
    int currentState = digitalRead(BUTTON_PIN);  // Lees de huidige staat van de drukknop

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
    wifiManager.connectToServer();

    if (helpRequired){
        Serial.println("HELLUP!");
        // tone(5, 1000,500);
        // delay(500);
        // noTone(5);
        //delay(500);
    }
    // Controleer of de knop ingedrukt is (drukflank van HIGH naar LOW)
    if (lastButtonState == HIGH && currentState == LOW) {
        // Als de knop ingedrukt is en de vorige staat was niet-ingedrukt (HIGH -> LOW)

        if (!knop.isPressed()) {
            // Als de knop nog niet ingedrukt was, registreer de druk
            knop.press();
            digitalWrite(LED_PIN, LOW);  // Zet de LED uit om aan te geven dat de noodknop ingedrukt is
            wifiManager.sendData("set noodknop 1\n");  // Verstuur het commando naar de server om de noodknop in te schakelen
            helpRequired = true;
            delay(100);  // Wacht een beetje zodat de server tijd heeft om het commando te verwerken
            char* recvBuffer = wifiManager.receiveData();  // Ontvang de reactie van de server

            // Log het serverantwoord naar de seriële monitor
            Serial.print("Antwoord van server: ");
            Serial.println(recvBuffer);

        } else {
            // Als de knop al ingedrukt was (herstel van noodknop)
            knop.reset();  // Zet de knopstatus terug naar "niet ingedrukt"
            digitalWrite(LED_PIN, HIGH);  // Zet de LED aan om aan te geven dat de noodknop gereset is
            wifiManager.sendData("set noodknop 0");  // Verstuur het commando naar de server om de noodknop uit te schakelen
            helpRequired = false;
            delay(100);  // Wacht een beetje zodat de server tijd heeft om het commando te verwerken
            char* recvBuffer = wifiManager.receiveData();  // Ontvang de reactie van de server

            // Log het serverantwoord naar de seriële monitor
            Serial.print("Antwoord van server: ");
            Serial.println(recvBuffer);
        }
    }
    Serial.println(helpRequired);
    lastButtonState = currentState;  // Sla de huidige knopstatus op voor de volgende iteratie (voor debounce)
    wifiManager.disconnectFromServer();
    delay(50);  // Debounce vertraging van 50 ms om onbedoelde meerdere detecties van de knop te voorkomen
}
