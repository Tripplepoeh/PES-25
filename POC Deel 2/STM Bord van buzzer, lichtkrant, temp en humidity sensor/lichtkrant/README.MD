Dit is heel het project voor de werking van het stmbord met de volgende sensors en actuators:

- Buzzer
- Buzzerknop
- Temperatuur- en luchtvochtigheidsensor
- Lichtkrant

Verbinding gaan als volgt van sensor/actuator naar het stm32 bord:  
Buzzer: GND -> GND  buzzerpin -> D9  
Buzzerknop: GND -> buzzerknoppin -> D10  
Temperatuur- en luchtvochtigheidsensor: GND -> GND VCC -> 3.3V SCL -> A6 SDA -> D12
Lichtkrant: GND -> GND VCC -> 3.3V DIN -> D2 CS -> A3 CLK -> A1  

i2c van stm naar pi:
SDA: D4 pi SDA
SCL: D5 pi SCL
GND: GND pi GND
