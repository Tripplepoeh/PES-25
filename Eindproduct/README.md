# Final Product

In deze folder staat het eindproduct van het Embedded Systems project voor de werking van een appartement.
## Toelichting
In dit eindproduct folder staan mapjes van alle componenten met hun code in C of C++. Alle code is voorzien van de juiste commentaar als toelichting.


In elk mapje geeft een toelichting hoe de microcontroller werkt met de componenten. Hierbij staan dus de pin verbindingen en werkwijze van de componenten toegelicht. De microcontrollers zijn als volgt geprogrammeerd:
- Wemos: via Arduino IDE
- STM32: via STM32CubeIDE
- Raspberry Pi: Geany

De verbindingswijze tussen de componenten gaan als volgt:
- Wemos: via WiFI naar Pi_A zijn serverapplicatie
- STM32: via I2C naar Pi_B
- Raspberry Pi: tussen de Pi's is er via een socket communicatie

Opstarten netwerk:
- Verzorg dat alle verbindingen van componenten kloppen en dat alle instellingen als IP-adres en poortnummer goed zijn ingesteld
- Start beide processen op van Pi_A en dan van Pi_B
- Hierna kunnen alle micorcontrollers worden gestart, waarbij het netwerk gelijk goed zal werken

In het geval dat het niet in een keer goed werkt kan er op de volgende wijze gedebugged worden:
- Check eerst of WiFi verbinding stabiel is en of alle IP-addressen en poortnummers goed zijn ingevuld bij de Pi's en de Wemossen\
- Check daarna of alle pins goed zijn verbonden\

### Korte samenvatting verbindingswijze
Eerder is kort benoemd op welke wijze de communicatie gebeurt. Hier is er een iets duidelijk toelichting tussen de communicatie van de Pi's naar de microcontrollers:
#### I2C
De I2C verbinding gebeurt door de master die de communicatie start. Bij de slave zit er speciaal klasse I2CBeheer die alle berichten verwerkt en een respons bericht klaar zet om naar de master te sturen. Hierbij stuurt dus de slave alleen als de master dat vraagt. 
#### Wemosserver
Bij de Wemossen wordt juist de verbinding gemaakt door de microcontrollers en maken zij verbinding met de Pi. Hierbij stuurt de Wemos zelf een bericht over zijn sensorwaardes en welke actuatorstatussen hij nodig heeft. \


**In het ontwerp hieronder is al een overzicht te zien welke componenten aan welke microcontrollers zijn verbonden. Daaronder zijn nog de klassendiagrammen te vinden voor het eindproduct.**

## Ontwerp
ontwerp \
![ontwerpPes drawio (1)](https://github.com/user-attachments/assets/55c98f5d-7a96-45fe-adb3-b6812ef53c6c)


Geheel klassendiagram \
![goedePES_klassendiagram](https://github.com/user-attachments/assets/0b8c93ba-060a-44a3-a641-7a96ea0392bb)


Simplistische klassendiagram \
![simpleKlassendiagram_projectPES](https://github.com/user-attachments/assets/30e41bd0-a7b7-4694-8fe7-283d5a7f2e84)



# Authors
Aashish Swami - 23136782 -- @aasepaas \
Bastiaan Wolters - 23052996 -- @KangarooAteMe \
Daniël van der Poel - 23135786 -- @tripplepoeh \
Ocker Repelaer van Driel - 23070889 -- @BitOcker\
Thomas Witvliet - 15042901 -- @thomaswitvliet
