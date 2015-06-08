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
  int temp = baro.getTemperatureCentiC();
  Serial.print("Centimeters: ");
  Serial.print((float)(alt));
  Serial.print(", Feet: ");
  Serial.print((float)(alt) / 30.48);
  Serial.print(", Centigrade: ");
  Serial.println((float)(temp) / 100);
  delay(400);
}
