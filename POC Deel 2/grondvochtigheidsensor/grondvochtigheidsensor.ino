#include "grondvochtigheidSensor.h"

grondvochtigheidSensor grondSensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  grondSensor.leesSensor();
  int sensorWaarde = grondSensor.natOfDroog();
  Serial.println(sensorWaarde);
  Serial.println();
}
