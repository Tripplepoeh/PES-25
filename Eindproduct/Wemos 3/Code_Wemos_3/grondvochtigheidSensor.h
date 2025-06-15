#ifndef GRONDVOCHTIGHEIDSENSOR_H
#define GRONDVOCHTIGHEIDSENSOR_H

#include <Arduino.h>

#define grondPin A0

class grondvochtigheidSensor {
  public:
    grondvochtigheidSensor();
    ~grondvochtigheidSensor();
    void leesSensor(); //functie die de grondvochtigheidsensor meet en print
    int getWaarde(); //functie die teruggeeft of de grond droog of nat is. 0 is te nat, 1 is te droog en 2 is perfect

  private:
   int grondWaarde; //opslag grondvochtigheidsensor waarde   
};

#endif //GRONDVOCHTIGHEIDSENSOR_H
