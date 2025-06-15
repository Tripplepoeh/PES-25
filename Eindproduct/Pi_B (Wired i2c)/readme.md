# Pi_B

## Toelichting mappen

Deze Pi heeft 2 mappen, namelijk:
- een voor de socket verbinding met de andere Pi
- een voor de I2CMaster code, waarmee communicatie plaatsvindt met de STM32-borden

Beide mappen runnen hun eigen proces en communiceren met elkaar via FIFO pipes.  
Voor de werking van het netwerk moeten eerst de processen van de andere Pi gestart worden voordat de processen van deze Pi gestart worden.

---

### Map: `Socket code`

In de map `Socket code` zit de code waarmee de Pi een socket-verbinding maakt met de andere Pi, die luistert naar connecties.

#### Belangrijk om aan te passen voor de echte toepassing:
- Vul het **IP-adres** van de andere Pi in, in de `main` van het programma.
- Vul het **juiste poortnummer** in, in de `main`.

De overige code van de klassen hoeft niet aangepast te worden en kan zo gebruikt worden.

#### Compilatie en starten via een terminal:
```bash
g++ -o [outputnaam] PiSocket.cpp PiSocketMain.cpp socket.cpp
./[outputnaam]
```

---

### Map: `I2C Master code`

In de map `I2C Master code` zit de code waarmee de Pi communiceert met de STM32-borden via I2C.

- De slave-adressen staan in de `main` gedefinieerd en worden doorgegeven aan het I2C-object.
- Het I2C-object gaat langs elke slave, start communicatie en luistert naar berichten van de slaves (sensorwaardes en gewenste actuatorstatussen).
- Het I2C-object stuurt de berichten door naar het `statuscontrol`-object, dat alles verwerkt en de juiste respons teruggeeft.
- `statuscontrol` logt via een database-object alles in een database.
- Menu's en RFID-tags kunnen in de database worden gezet, waar `statuscontrol` gebruik van maakt.
- De respons wordt uiteindelijk teruggestuurd naar de slave, die daarmee de actuatoren aanstuurt.
- `statuscontrol` houdt alle actuatorstatussen bij en stuurt alleen sensorupdates door naar het andere proces (`socket`), die deze weer doorstuurt naar de andere Pi. De andere Pi houdt eveneens de actuatorstatussen bij. Er worden dus alleen sensorwaardes van Pi naar Pi gestuurd.
- Het systeem is plug & play: componenten kunnen eenvoudig aan de slave-zijde worden toegevoegd of verwijderd.

#### Voorbeeld communicatie tussen master en slave

De communicatie verloopt via sensorID’s (2 bytes sensorwaardes, 5 bytes voor RFIDsensor).

Voorbeeld:
- `0x03 0x04` — Master start communicatie en vraagt welke sensoren/actuatoren er zijn
- `0x20 0x01 0x00 0x04 0x41` — Slave stuurt sensorwaarde en vraagt of een actuator aangestuurd mag worden
- `0x41 0x01` — Master stuurt terug dat de actuator mag worden aangestuurd

(Maximaal 64 karakters per bericht.)

Alle sensor- en actuator-ID’s staan in `alleDefines.h`.  
De verwerking van berichten gebeurt automatisch door de `I2CBeheer` klasse aan de slave-kant; in de `main` moeten alleen de juiste ID’s worden opgegeven.

---

### Databases

Er wordt gebruikgemaakt van MariaDB. Installeer MariaDB als volgt:
```bash
sudo apt install mariadb-server -y
sudo apt install libmariadb-dev -y  # Voor de mysql header file in C++
```

Een database en gebruiker zijn als volgt aangemaakt:
```sql
sudo mariadb
CREATE DATABASE sensordata;
USE sensordata;
CREATE USER 'rasp_b'@'localhost' IDENTIFIED BY 'rasp_b';
GRANT ALL PRIVILEGES ON sensordata.* TO 'rasp_b'@'localhost';
FLUSH PRIVILEGES;
exit
mysql -u rasp_b -p sensordata
```

De volgende drie tabellen zijn aangemaakt:

```sql
-- Loggen van sensordata
CREATE TABLE sensorlog (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    sensornaam VARCHAR(64),
    waarde BIGINT UNSIGNED
);

-- Dagmenu toevoegen
CREATE TABLE dagMenu (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    menu VARCHAR(64)
);

-- Toegestane RFID-codes
CREATE TABLE rfid (
    id INT AUTO_INCREMENT PRIMARY KEY,
    rfidcode BIGINT UNSIGNED NOT NULL,
    eigenaar VARCHAR(100) DEFAULT NULL,
    toegevoegd_op TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

Enkele handige databasecommando's:
```sql
SELECT * FROM sensorlog WHERE sensornaam = 'noodknop';
INSERT INTO rfid (rfidcode, eigenaar) VALUES (793615282944, 'Zorg-medewerker: 2313');
INSERT INTO dagMenu (menu) VALUES ('Pizza en lasagna');
```

---

#### Belangrijk om aan te passen voor de echte toepassing:
- Vul de juiste slave-adressen in, in de `main`.
- Voeg bij uitbreiding van sensoren of actuatoren deze toe aan de constructor van `statuscontrol.cpp` en geef een ID in `alleDefines.h` (ook bij de andere Pi updaten).

De overige code van de klassen hoeft niet aangepast te worden en kan zo gebruikt worden.

#### Compilatie en starten via een terminal:
```bash
g++ -o [outputnaam] I2C.cpp getfromdatabase.cpp i2cMain.cpp logtodatabase.cpp statuscontrol.cpp -lmariadb -lwiringPi
./[outputnaam]
```
