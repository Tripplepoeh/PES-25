#ifndef __RFIDSENSOR_H
#define __RFIDSENSOR_H

#include <stdio.h>
#include <stdint.h>


//variabelen:
//variabel om de statussen van de rfidsensor op te slaan
static uint8_t status;
//variabel om de rfidsensor waardes op te slaan
static uint8_t opslagRauweWaardes[16];
//variabel om de rfidsensor waardes om te zetten naar 5 getallen
static uint8_t RFIDWaarde[5];
//variabel om rfidsensor waardes op te slaan die toegang mogen hebben
static uint8_t toegangsKaarten[2][5];
//teller van aantal kaarten die toegang mogen hebben
static uint8_t aantalKaarten = 0;


//functies:
//initialisatie van de rfidsensor
void RFIDSensorInit();
//functie die checkt of er een kaart is
int CheckKaart();
//functie die de rfidsensor waardes omzet in 5 getallen in een string/char array
uint8_t* RFIDSensorWaarde();
//deze functie kijkt of de kaart/tag toegang mag hebben tot het appertement
int CheckToegang();


#endif //__RFIDSENSOR_H
