# Wemos 1
## Componenten
Dit Wemos bordje bestuurt de volgende componenten:\
- Ledstrip
- Druksensor

## Werking componenten
- Ledstrip: 
  - De ledstrip wordt bestuurd door de Pi op basis van de noodknop, bewgingsensor en druksensor. Het ledstrip heeft 3 standen:
    - Aan: Het ledstrip is altijd aan als er de noodknop gedrukt is of er beweging is
    - Dimmen: Het ledstrip zal dimmend op rood gaan wanneer er geslapen wordt. Dit is alleen als er dus geen beweging is of de noodknop niet gedrukt is
    - Uit: Als er geen beweging of het noodknop niet ingedrukt is, zal het ledstrip vanzelf na 5 seconden ongeveer uitgaan. Dit gebeurt ook nadat er 5 seconden lang op dimmend stand is geweest.
  - Voor de echte toepassing moet de tijd aangepast worden, zodat de ledstrip uitvalt na 30 minuten
- Druksensor:
  - De druksensor geeft terug hoeveel druk er op de sensor is. Dit geeft dus terug of er geslapen wordt of niet. 

## Code
- Ledstrip in C++
- Druksensor in C++
