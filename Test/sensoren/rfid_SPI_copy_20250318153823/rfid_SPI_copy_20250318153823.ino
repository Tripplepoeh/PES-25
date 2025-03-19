#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>


#define SS_PIN D8   // SDA naar D8 (GPIO15)
#define RST_PIN D3  // RST naar D3 (GPIO0)
#define Knop D2

Servo myservo;
int pos = 0;
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID Scanner klaar...");

  pinMode(Knop, INPUT);

  myservo.attach(D1);
}

void loop() {
  // Wacht tot een tag wordt gedetecteerd
  int teller = 0;
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Selecteer een RFID-tag
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    teller++;
    if (teller == 1) {
      for (pos = 0; pos <= 60; pos += 1) {  // goes from 0 degrees to 180 degrees

        myservo.write(pos);  // tell servo to go to position in variable 'pos'
        delay(15);
        Serial.println(pos);
      }
      for (int pos1 = 61; pos1 <= 120; pos1 += 1) {  // goes from 0 degrees to 180 degrees

        myservo.write(pos1);  // tell servo to go to position in variable 'pos'
        delay(15);
        Serial.println(pos1);
      }
      for (int pos2 = 121; pos2 <= 180; pos2 += 1) {  // goes from 0 degrees to 180 degrees

        myservo.write(pos2);  // tell servo to go to position in variable 'pos'
        delay(15);
        Serial.println(pos2);
      }
      for (int pos3 = 181; pos3 <= 240; pos3 += 1) {  // goes from 0 degrees to 180 degrees
        delay(15);
        myservo.write(pos3);  // tell servo to go to position in variable 'pos'
        Serial.println(pos3);
      }
    }
  }

  if(digitalRead(Knop) == HIGH){
    for (int pos3 = 181; pos3 <= 240; pos3 += 1) {  // goes from 0 degrees to 180 degrees
        delay(15);
        myservo.write(pos3);  // tell servo to go to position in variable 'pos'
        Serial.println(pos3);
      }
  }

  Serial.println();

  rfid.PICC_HaltA();  // Stop communicatie met tag
}
