#include "RFIDSensor.h"
#include "RC522.h" //libray die nodig is om de rfidsensor uit te lezen
#include <string.h>     // voor memcpy
#include <stdio.h>      // voor snprintf, printf

//initialisatie van de rfidsensor
void RFIDSensorInit(){
	MFRC522_Init();
}
//functie die checkt of er een kaart is
int CheckKaart(){
	//dit retourneert een MI_OK als de rfidsensor een kaart/tag heeft gelezen
	status = MFRC522_Request(PICC_REQIDL, opslagRauweWaardes);
	if(status == MI_OK){ //check of de status ok is en ga dan verder anders niet
		//dit retourneert of de sensor de waardes van de tag/kaart heeft kunnen halen
		status = MFRC522_Anticoll(opslagRauweWaardes);
		if (status == MI_OK){//als de rfid sensor alle waardes heeft kunnen ophalen en sturen geeft hij een true door als return
			return 1;
		}
	}
	return 0;
}
//functie die de rfidsensor waardes omzet in 5 getallen in een string/char array
uint8_t* RFIDSensorWaarde(){
	//dit zet de juiste waardes van opslagRauweWaardes naar RFIDWaarde, wat de waarde van de tag/kaart is
	memcpy(RFIDWaarde, opslagRauweWaardes, sizeof(uint8_t) * 5);
	return RFIDWaarde;

}

//deze functie vergelijkt of er toegang verlenen mag worden aan een kaart, waar de eerste 2 gescande kaarten toegang mogen hebben en de rest niet
int CheckToegang(){
	//ga langs alle kaarten en check of de gescande kaart overeenkomt met een kaart die toegang mag hebben
	for(int i = 0;i < aantalKaarten; i++){
		if (memcmp(RFIDWaarde, toegangsKaarten[i], 5) == 0) return 1;
	}

	//check of er al 2 kaarten gescand zijn, zo niet voeg de kaart toe als toegangskaart
	if(aantalKaarten < 2){
		memcpy(toegangsKaarten[aantalKaarten], RFIDWaarde, 5);
		aantalKaarten++;
		return 1;
	}
	return 0;
}

