de gegeven code kunnen er sensor waardes gekregen en ingevuld worden

de volgende inputs zijn geldig:
set druksensor 19
get druksensor
set druksensor 19 get druksensor //alles achter elkaar kan ook hij geeft dan ook een respons op de juiste wijze terug

de inputs worden bepaald in de statusControl.h in de array met sensornamen:
regel 35:    const char* sensorNamen[aantalSensoren] = {"druksensor", "noodknop", "plant", "beweging"};
wil je dat er een nieuwe sensor wordt toegevoegd voeg hem hier toe en pas dan ook de enum aan met
regel 27: enum Sensoren {
        drukWaarde = 0,   // Sensor 0
        noodknopWaarde,         // Sensor 1
        plantWaarde,      // Sensor 2
        bewegingsWaarde,   // Sensor 3
        aantalSensoren   // Houdt het aantal sensoren bij
    };

output is te verwachten als dit:
Message received: get druksensor
input is get druksensor
result is Huidige waarde druksensor: 0

hij stuurt dan alleen dit "Huidige waarde druksensor: 0" terug naar de client
