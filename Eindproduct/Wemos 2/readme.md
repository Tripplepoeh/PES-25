# Wemos 2
## Componenten
Dit Wemos bordje bestuurt de volgende componenten:\
- RFIDsensor

## Werking componenten
- RFIDsensor:
  - De RFIDsensor stuurt de RFID waardes door naar de Pi. De Pi checkt of de waarde voorkomt in een database en zal dan bepalen of de deur open mag gaan. Als er geen kaart gescand wordt, wordt er een 0 gestuurd naar de server als RFIDsensor waarde

## Code
- RFIDsensor in C++
