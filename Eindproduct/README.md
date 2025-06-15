# Final Product

In deze folder staat het eindproduct van het Embedded Systems project voor de werking van een appartement.
## Toelichting
In dit eindproduct folder staan mapjes van alle componenten met hun code in C of C++. Alle code is voorzien van de juiste commentaar als toelichting.


In elk mapje geeft een toelichting hoe de microcontroller werkt met de componenten. Hierbij staan dus de pin verbindingen en werkwijze van de componenten toegelicht. De microcontrollers zijn als volgt geprogrammeerd:\
-Wemos: via Arduino IDE\
-STM32: via STM32CubeIDE\
-Raspberry Pi: Geany

De verbindingswijze tussen de componenten gaan als volgt:\
-Wemos: via WiFI naar Pi_A zijn serverapplicatie\
-STM32: via I2C naar Pi_B\
-Raspberry Pi: tussen de Pi's is er via een socket communicatie

Opstarten netwerk:\
-Verzorg dat alle verbindingen van componenten kloppen en dat alle instellingen als IP-adres en poortnummer goed zijn ingesteld\
-Start beide processen op van Pi_A en dan van Pi_B\
-Hierna kunnen alle micorcontrollers worden gestart, waarbij het netwerk gelijk goed zal werken

In het geval dat het niet in een keer goed werkt kan er op de volgende wijze gedebugged worden:\
-Check eerst of WiFi verbinding stabiel is en of alle IP-addressen en poortnummers goed zijn ingevuld bij de Pi's en de Wemossen\
-Check daarna of alle pins goed zijn verbonden\


In het ontwerp hieronder is al een overzicht te zien welke componenten aan welke microcontrollers zijn verbonden. Daaronder zijn nog de klassendiagrammen te vinden voor het eindproduct.

## Ontwerp
ontwerp \
![ontwerpPes drawio (1)](https://github.com/user-attachments/assets/55c98f5d-7a96-45fe-adb3-b6812ef53c6c)


Geheel klassendiagram \
![image](https://github.com/user-attachments/assets/6a0dc9fe-b980-4308-b641-ec8e1042027e)

Simplistische klassendiagram \
![image](https://github.com/user-attachments/assets/a0dd0d7b-f333-4c67-96d1-ffc5b0f66a43)


# Authors
Aashish Swami - 23136782 -- @aasepaas \
Bastiaan Wolters - 23052996 -- @KangarooAteMe \
Daniël van der Poel - 23135786 -- @tripplepoeh \
Ocker Repelaer van Driel - 23070889 -- @BitOcker\
Thomas Witvliet - 15042901 -- @thomaswitvliet
