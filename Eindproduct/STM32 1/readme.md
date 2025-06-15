# STM32 1
## Componenten
Dit STM32 bordje bestuurt de volgende componenten:\
- CO2-sensor
- Bewegingsensor
- Speciaal beheer leds

## Werking componenten
- CO2-sensor:
  - De CO2-sensor meet de CO2 waarde in de omgeving en stuurt dat door via I2CBeheer klasse naar de master op verzoek van de master. De base waarde zal 400 ppm zijn.
- Bewegingsensor:
  - De bewegingsensor checkt of er beweging is en stuurt dat door via I2CBeheer klasse. Als er beweging is wordt er een 1 gestuurd anders een 0.
-  Speciaal beheer leds:
  - Groene led:
    - De groene led gaat aan als de grondvochtigheidssensor een waarde meet boven de 500. Dit geeft aan dat de grond droog is.
  - Rode led:
     - De rode led gaat aan als de CO2-waarde boven 500 is. Dit geeft aan dat de CO2-waarde in de omgeving te hoog is.
  - Gele led:
     - De gele led geeft aan als de noodknop gedrukt is. DIt geeft aan als er dus een noodsituatie plaatsvindt.
## Code
- CO2-sensor in C
- Bewegingsensor in C++
- Speciaal beheer leds in C++

