#include "Arduino.h"

void colect_telemetry(struct telemetry_data_struct* telemetry_data)
{  
  /************************************/
  /* Gather Air Pressure Data         */
  /* Change the resolution to 12 bit  */
  /************************************/
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
  int extPressure = analogRead(PIN_PRESSURE_SENSOR);
  
  Serial.println("Pressure " + extPressure);
}

void process_telemetry(struct telemetry_data_struct* telemetry_data)
{
  
}
