# Pi_A

## Toelichting mappen
Deze Pi heeft 2 mappen, namelijk een voor de socket verbinding met de andere pi en een server waar de Wemossen aan kunnen verbinden. Beide mappen runnen uiteindelijk hun eigen proces en communniceren met elkaar via FIFO pipes. Voor de werking van het netwerk moeten eerst deze processen gestart worden voordat de processen van de andere Pi gestart worden.
### Toelicht map 'Socket code'
In de Socket code map zit de code in waarmee de Pi een socket connectie openmaakt waarmee de andere Pi kan verbinden.\
Belangrijk om aan te passen voor de echte toepassing:\
-Verzorg dat het IP-adres van de Pi wordt ingevuld in de main\
-Verzorg dat het juiste poortnummer wordt ingevuld in de main

Overige code van de klassen hoeven niet aangepast te worden en kunnen zo gebruikt worden.

De compilatie en starten via een terminal:\
-g++ -o [outputnaam] PiSocket.cpp PiSocketMain.cpp socket.cpp\
-./[outputnaam]

### Toelicht map 'Wemos server code'
In de Wemos server code map zit de code in waarmee de Wemossen verbinding kunnen maken met de Pi. In deze code kunnen maximaal 5 clients tegelijk verbonden zijn met de server. De statuscontrol klasse parsed alle data en verewrkt het naar uiteindelijk een mogelijke respons. De statuscontrol houdt ook alle actuatorstates bij en stuurt alleen de sensorupdates door naar de andere proces van de socket en die stuurt het door naar de andere Pi. De andere Pi houdt zelf ook de actuatorstates bij, er worden dus alleen sensorwaardes gestuurd van Pi naar Pi. Het systeem is plug en play dus er kan gemakkelijk een nieuwe Wemos worden aageschafd en geprogrammeerd en die heeft dan de mogelijkheid om met de server contact te nemen. 

#### Geldige inputs

De volgende commando's zijn geldig:

- `set druksensor 19` //set van een sensorwaarde
- `get ledstrip` //get van een actuator
- `set druksensor 19 get ledstrip` 
  (Alles achter elkaar in één regel mag ook. De juiste respons wordt dan ook correct teruggestuurd.)

Belangrijk om aan te passen voor de echte toepassing:\
-Verzorg dat het IP-adres van de Pi wordt ingevuld in de main, waar de Wemossen aan gaan verbinden\
-Verzorg dat het juiste poortnummer wordt ingevuld in de main, waar de Wemossen aan gaan verbinden\
-Als er een nieuwe sensor of actuator wordt toegevoegd moet dat in de statuscontrole.cpp in de constructor worden toegevoegd. Dit moet dan ook bij de andere Pi aangepast worden./

Overige code van de klassen hoeven niet aangepast te worden en kunnen zo gebruikt worden.

De compilatie en starten via een terminal:\
-g++ -o [outputnaam] wemosServer.cpp wemosServerMain.cpp socket.cpp wemosStatuscontrole.cpp\
-./[outputnaam]
