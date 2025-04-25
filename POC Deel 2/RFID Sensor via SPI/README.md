Om de RFID Sensor code te laten runnen moeten de header en c file van deze github in de cubeIDE zijn:
https://github.com/CircuitGatorHQ/RFID-RC522-with-STM32 

de pins gaan als volgt van RFID sensor naar het bordje:
SDA -> A3
SCK -> A4
MOSI -> A6
MISO -> A5
IRQ wordt niet gebruikt!!(het lukte mij niet om de interrupt te laten werken minstens)
GND -> GND
RST -> D3 
3.3v -> 3.3v
