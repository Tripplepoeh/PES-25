# Wemos 3
## Componenten
Dit Wemos bordje bestuurt de volgende componenten:\
- Noodknop
- Grondvochtigheidssensor

## Werking componenten
- Grondvochtigheidssensor:
  - De grondvochtigheidssensor geeft een waarde terug wat de vochtigheid is in de omgeving. Wanneer de sensor een waarde boven 500 teruggeeft, is de omgeving droog
  - In de echte toepassing moet er voorzien worden van een goede behuizing, zodat het bordje en een deel van de sensor niet nat worden.
- Noodknop:
  - De noodknop checkt of de knop gedrukt is. Als de knop gedrukt is wordt er een noodprocedure gestart. Hierbij gaan de ledstrip, buzzer en gele led aan.
  - In de echte toepassing moet er voorzien worden van een goede behuizing, zodat de kabels tussen de knop en het bordje niet makkelijk te verbreken is.
- In realiteit moeten deze componenten op verschillende borden staan. Het team had dit niet kunnen realiseren door een tekort aan middelen. 

## Code
- Noodknop in C++
- Grondvochtigheidssensor in C++
