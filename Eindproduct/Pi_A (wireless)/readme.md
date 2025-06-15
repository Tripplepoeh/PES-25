# Pi_A

## Toelichting mappen

Deze Pi heeft twee belangrijke mappen:
- **Socket code**: Voor de socketverbinding met de andere Pi
- **Wemos server code**: Voor de server waarmee Wemossen kunnen verbinden

Beide mappen draaien elk hun eigen proces en communiceren onderling via FIFO pipes.  
Voor het goed functioneren van het netwerk moeten eerst de processen van deze Pi gestart worden, daarna pas de processen van de andere Pi.

---

### 1. Map: `Socket code`

In deze map vind je de code waarmee de Pi een socketverbinding opent, zodat de andere Pi kan verbinden.

**Belangrijk om aan te passen voor de echte toepassing:**
- Vul het **IP-adres van deze Pi** in, in de `main`.
- Vul het **juiste poortnummer** in, in de `main`.

De rest van de code hoeft niet aangepast te worden en kan direct gebruikt worden.

**Compileren en starten via een terminal:**
```bash
g++ -o [outputnaam] PiSocket.cpp PiSocketMain.cpp socket.cpp
./[outputnaam]
```

---

### 2. Map: `Wemos server code`

Deze map bevat de servercode waarmee maximaal 5 Wemossen tegelijk kunnen verbinden met de Pi.  
De `statuscontrol` klasse verwerkt alle ontvangen data en bepaalt de juiste respons.  
Daarnaast houdt `statuscontrol` alle actuatorstatussen bij en stuurt enkel sensorupdates door naar het andere socket-proces, dat deze weer doorstuurt naar de andere Pi.  
Ook de andere Pi houdt zijn eigen actuatorstatussen bij; er worden dus alleen sensorwaardes tussen de Pi’s uitgewisseld.  
Het systeem is plug & play: een nieuwe Wemos kan eenvoudig toegevoegd worden en verbinding maken met de server.

**Geldige inputs voor de Wemos-client:**
- `set druksensor 19` &nbsp;_(zet een sensorwaarde)_
- `get ledstrip` &nbsp;_(opvragen van een actuatorstatus)_
- `set druksensor 19 get ledstrip` &nbsp;_(combineren op één regel is toegestaan)_

Alles op één regel is toegestaan; de juiste respons wordt teruggestuurd.

**Belangrijk om aan te passen voor de echte toepassing:**
- Vul het **IP-adres van de Pi** in de `main` in (waar de Wemossen op verbinden).
- Vul het **juiste poortnummer** in de `main` in.
- Voeg bij nieuwe sensoren/actuatoren deze toe aan de constructor van `statuscontrole.cpp` (dit moet ook bij de andere Pi aangepast worden).

De rest van de code hoeft niet aangepast te worden en kan direct gebruikt worden.

**Compileren en starten via een terminal:**
```bash
g++ -o [outputnaam] wemosServer.cpp wemosServerMain.cpp socket.cpp wemosStatuscontrole.cpp
./[outputnaam]
```
