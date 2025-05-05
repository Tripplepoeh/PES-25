#include "grondvochtigheidSensor.h"

grondvochtigheidSensor::grondvochtigheidSensor() {
}

grondvochtigheidSensor::~grondvochtigheidSensor() {
}

void grondvochtigheidSensor::leesSensor() {
  grondWaarde = analogRead(grondPin); //analoge read van de grondvochtigheidsensor 
  Serial.println(grondWaarde);
}

int grondvochtigheidSensor::natOfDroog() {
  Serial.println(grondWaarde);
  if (grondWaarde < nat) {
    return 0; // te nat
  } else if(grondWaarde >= nat && grondWaarde < droog ){
    return 2; // perfecte situatie
  }else{
    return 1; // is nu te droog en heeft water nodig
  }
}
