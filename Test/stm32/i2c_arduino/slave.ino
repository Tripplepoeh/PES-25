#include <Wire.h> // Include wire library voor I2C
#include <Servo.h> // Include servo library voor servo

#define SLAVE_ADDRESS 0x08 // Defineer slave adres
#define SDA PB7 // Defineer SDA pin
#define SCL PB6 // Defineer SCL pin

#define SERVO_PIN PB8 // Defineer servo pin
#define BUTTON_PIN PB9 // Defineer button pin
#define RFID_PIN PB10 // Defineer RFID pin

Servo deurServo; // Defineer deur servo 

int buttonState = 0; // Defineer buttonState
int lastButtonState = 0; // Defineer lastButtonState
int rfidState = 0; // Defineer rfidState
int lastRfidState = 0; // Defineer lastRfidState
bool deurOpenen = false; // Defineer deurOpenen
uint8_t rfidAdres = 0xAB // rfid adres simuleren (uitlezen via SPI)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void setup() {
Wire.setSDA(SDA); // Set SDA pin
Wire.setSCL(SCL); // Set SCL pin
Serial.begin(115200); // Start serial communicatie

Wire.begin(SLAVE_ADDRESS); // Start I2C communicatie met slave adres
Wire.onReceive(ActionRec); // Roep receiveEvent aan wanneer er data ontvangen wordt

pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin als input
pinMode(RFID_PIN, INPUT_PULLUP); // Set RFID pin als input

deurServo.attach(SERVO_PIN); // Attach servo aan pin
deurServo.write(0); // Zet servo op 0 graden (deur dicht)
}

void loop() {
buttonState = digitalRead(BUTTON_PIN); // Lees button pin
rfidState = digitalRead(RFID_PIN); // Lees RFID pin

if (buttonState != lastButtonState && buttonState == LOW) { 
    ActionSend(0x01, 0x01); // Stuur actie 0x01 (knop) naar master
}
lastButtonState = buttonState; // Update lastButtonState

if (rfidState != lastRfidState && rfidState == LOW) {
    if rfidAdres == 0xAB { // Als RFID adres gelijk is aan 0xAB
        ActionSend(0x02, 0x01); // Stuur actie 0x02 (rfid) naar master
    }
    else {
        printf("RFID adres niet correct"); // Print error
    }
}
lastRfidState = rfidState; // Update lastRfidState




}
void ActionRec(int numBytes) {
    if(Wire.available()) {
        uint8_t adres = Wire.read(); // Lees actie van master
        uint8_t actie = Wire.read(); // Lees data van master
    }
}

void ActionSend(uint8_t adres, uint8_t actie) {
Wire.beginTransmission(SLAVE_ADDRESS); // Begin transmissie vanaf slave
Wire.write(adres); // Schrijf actie naar slave
Wire.write(actie); // Schrijf data naar slave
Wire.endTransmission(); // Eindig transmissie
}

void openDeur() {
    Serial.println("Deur openen...");
    deurServo.write(90);
    delay(3000);
    Serial.println("Deur sluiten...");
    deurServo.write(0);
}