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
  int temp = baro.getTemperatureCentigrade();
  int pressure = baro.getAvgNormPressurePascals();
  Serial.print("Alt (cm): ");
  Serial.print((float)(alt));
  Serial.print(", Alt (ft): ");
  Serial.print((float)(alt) / 30.48);
  Serial.print(", Temp (C): ");
  Serial.print((float)(temp) / 100);
  Serial.print(", Press (Pa): ");
  Serial.println((float)(pressure));
  delay(400);
}
