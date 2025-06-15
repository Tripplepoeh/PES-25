# STM32 2
## Componenten
Dit STM32 bordje bestuurt de volgende componenten:\
- Luchtvochtigheid en temperatuur sensor
- Buzzer
- Buzzerknop
- Lichtkrant

## Werking componenten
- Luchtvochtigheid en temperatuur sensor:
  - De Luchtvochtigheid en temperatuur sensor meet de temperatuur en luchtvochtigheid en stuurt dit door naar de Pi via I2C. 
- Buzzer:
  - De buzzer wordt bestuurd door een noodknop of een lokale buzzerknop. 
-  Buzzerknop:
  - De buzzerknop is een lokale knop die de buzzer bestuurd. Deze knop gaat dus niet via I2C, maar is als echte noodgeval wanneer het netwerk niet meer online is.
- Lichtkrant:
  - De lichtkrant print de menu, temperatuur, luchtvochtigheid en de CO2-waarde.
## Code
- Luchtvochtigheid en temperatuur sensor in C
- Buzzer in C++
- Buzzerknop via interrupt in C
- Lichtkrant in C
