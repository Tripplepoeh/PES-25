#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 0x08  // Slave adres
#define SERVO_PIN PA8       // Servo motor pin
#define BUTTON_PIN PB5      // Knop pin
#define LED_PIN PB3         // LED status pin

static byte binnenkomendData = 0;
static byte dataSturen = 100;
Servo deurServo;

void setup() 
{
  Wire.setSCL(PB6);
  Wire.setSDA(PB7);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);  // Ontvangst van data van master
  Wire.onRequest(sendData);    // Stuur data naar master wanneer daarom wordt gevraagd

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  deurServo.attach(SERVO_PIN);
  deurServo.write(0); // Startpositie: deur gesloten
}

void loop() { 
  // Voer de knopcontrole en servo-acties uit
  if (digitalRead(BUTTON_PIN) == LOW || binnenkomendData == 1) {
    openDeur();  // Als de knop ingedrukt is, open de deur
    dataSturen = 1;
    binnenkomendData = 0;
  } else {
    digitalWrite(LED_PIN, LOW);
    // if (deurServo.read() == 0) {
      dataSturen = 0;  // Deur is gesloten, stuur 0 naar de master
      deurServo.write(deurServo.read());
    // }
  }

  // Zorg ervoor dat de servo geen onnodige bewegingen maakt
  // Hier geen servo bewegingen als de deur al gesloten is.
  delay(50);  // Kort delay om te voorkomen dat de loop te snel draait en de servo overbelast wordt
}

// Ontvang data van de master
void receiveData(int bytecount) {
  for (int i = 0; i < bytecount; i++) {
    binnenkomendData = Wire.read();
  } 
  
}

// Stuur data naar de master
void sendData() {
  Wire.write(dataSturen);  // Stuur data (1 als de deur geopend is, anders 0)
}

// Functie om de deur te openen
void openDeur() {
  // Beweeg servo van 0 naar 60 graden
  for (int pos = 0; pos <= 60; pos += 1) {
    deurServo.write(pos);
    delay(20);  // Verhoog de vertraging voor een soepeler beweging
  }

  delay(3000);  // Laat de deur even open (3 seconden)

  // Beweeg servo van 60 naar 0 graden
  for (int pos = 60; pos >= 0; pos -= 1) {
    deurServo.write(pos);
    delay(20);  // Verhoog de vertraging voor een soepeler beweging
  }

  // Zet dataSturen naar 1 om een signaal naar de master te sturen (deur is geopend)
 
}
