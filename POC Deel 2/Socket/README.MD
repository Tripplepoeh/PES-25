# Statuscontrole Systeem

Deze code maakt het mogelijk om sensorwaardes op te vragen en in te stellen via een FIFO-verbinding (named pipes).

## Geldige inputs

De volgende commando's zijn geldig:

- `set druksensor 19`
- `get druksensor`
- `set druksensor 19 get druksensor`  
  (Alles achter elkaar in één regel mag ook. De juiste respons wordt dan ook correct teruggestuurd.)

## Sensorconfiguratie

De toegestane sensornamen worden gedefinieerd in `statusControl.h`, in de array op regel 35:

```cpp
const char* sensorNamen[aantalSensoren] = {"druksensor", "noodknop", "plant", "beweging"};
