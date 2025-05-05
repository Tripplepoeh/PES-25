#ifndef GRONDVOCHTIGHEIDSENSOR_H
#define GRONDVOCHTIGHEIDSENSOR_H

#include <Arduino.h>

#define grondPin A0

class grondvochtigheidSensor {
  public:
    grondvochtigheidSensor();
    ~grondvochtigheidSensor();
    void leesSensor(); //functie die de grondvochtigheidsensor meet en print
    int natOfDroog(); //functie die teruggeeft of de grond droog of nat is. 0 is te nat, 1 is te droog en 2 is perfect

  private:
   int grondWaarde; //opslag grondvochtigheidsensor waarde
   int droog = 400; //drempel wanneer de grond te droog is. boven dit is de grond te droog
   int nat = 270; // //drempel wanneer de grond te nat is. onder dit is de grond te nat
   
};

#endif //GRONDVOCHTIGHEIDSENSOR_H
