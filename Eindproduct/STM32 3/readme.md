# STM32 3
## Componenten
Dit STM32 bordje bestuurt de volgende componenten:\
- Deurknop
- Deur

## Werking componenten
- Deurknop: 
  - De deurknop checkt steeds in de while loop of er gedrukt is op de knop. De status wordt dan gestuurd naar de Pi. 
- Deur:
  - De deur wordt bestuurd door een deurknop of een RFIDsensor. Door de Pi wordt er gestuurd of de deur open mag of niet. 

## Code
- Deurknop in C++
- Deur in C++
