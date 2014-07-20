#include "Arduino.h"
float raw_val;
float actual_val;

void colect_telemetry(struct telemetry_data_struct* telemetry_data)
{  
  // Change the resolution to 12 bits
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
  
  //Air Pressure Data
  raw_val = analogRead(PIN_PRESSURE_SENSOR);
  actual_val = raw_val * PRESSURE_CONSTANT;
  Serial.print("Air Pressure: ");
  Serial.println(actual_val);
  
  //Battery 1 Temp
  raw_val = analogRead(PIN_BATTERY1_1_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Battery 1-1 Temp: ");
  Serial.println(actual_val);

  //Battery 2 Temp
  raw_val = analogRead(PIN_BATTERY1_2_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Battery 1-2 Temp: ");
  Serial.println(actual_val);
  
  //Battery 3 Temp
  raw_val = analogRead(PIN_BATTERY2_1_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Battery 2-1 Temp: ");
  Serial.println(actual_val);
  
  //Battery 4 Temp
  raw_val = analogRead(PIN_BATTERY2_2_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Battery 2-2 Temp: ");
  Serial.println(actual_val);

  //External Temp 1
  raw_val = analogRead(PIN_EXTERNAL_OUTTER_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Outter External 1 Temp: ");
  Serial.println(actual_val);  
  
  //External Temp 2
  raw_val = analogRead(PIN_EXTERNAL_INNER_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Inner External 2 Temp: ");
  Serial.println(actual_val);  

  //Internal Temp
  raw_val = analogRead(PIN_INTERNAL_TEMP);
  actual_val = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.print("Internal Temp: ");
  Serial.println(actual_val);  
  
  //Battery Voltage 1
  raw_val = analogRead(PIN_BATTERY_VOLTAGE_1);
  actual_val = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  Serial.print("Battery Voltage 1: ");
  Serial.println(actual_val);    
  
  //Battery Voltage 2
  raw_val = analogRead(PIN_BATTERY_VOLTAGE_2);
  actual_val = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  Serial.print("Battery Voltage 2: ");
  Serial.println(actual_val);    
  
  Serial.println("");  
  Serial.println("");  
  Serial.println("");  
}

void process_telemetry(struct telemetry_data_struct* telemetry_data)
{
  
}
