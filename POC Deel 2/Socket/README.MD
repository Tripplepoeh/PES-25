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
```


# Toevoegen nieuwe sensor namen

Als je nieuwe sensors wilt toevoegen, moet je dat in de bovenstaande array toevoegen. pas bovendien ook de enum aan in `statusControl.h`, in de array op regel 27:

```cpp
 enum Sensoren {
        drukWaarde = 0,   // Sensor 0
        noodknopWaarde,         // Sensor 1
        plantWaarde,      // Sensor 2
        bewegingsWaarde,   // Sensor 3
        aantalSensoren   // Houdt het aantal sensoren bij
    };
