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

In het ontwerp hieronder is al een overzicht te zien welke componenten aan welke microcontrollers zijn verbonden. Daaronder zijn nog de klassendiagrammen te vinden voor het eindproduct.

## Ontwerp
ontwerp \
![ontwerpPes drawio](https://github.com/user-attachments/assets/030f37c0-5e71-45e6-b7ee-da5db4e126d3)

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
