#include <Wire.h> 
#include "IntersemaBaro.h"

Intersema::BaroPressure_MS5607 baro;
//Intersema::BaroPressure_MS5607B baro(true);

void setup() { 
    Serial.begin(9600);
    baro.init();
}

void loop() {
  int alt = baro.getHeightCentiMeters();
  Serial.print("Centimeters: ");
  Serial.print((float)(alt));
  Serial.print(", Feet: ");
  Serial.println((float)(alt) / 30.48);
  delay(400);
}
