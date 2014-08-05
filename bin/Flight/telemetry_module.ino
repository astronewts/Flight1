#include "Arduino.h"
float raw_val;
float actual_val;

void colect_telemetry(struct telemetry_data_struct* telemetry_data)
{ 
  Serial.print("Air Pressure: ");
  Serial.println(actual_val);
  Serial.print("Battery 1-1 Temp: ");
  Serial.println(actual_val);
  Serial.print("Battery 1-2 Temp: ");
  Serial.println(actual_val);
  Serial.print("Battery 2-1 Temp: ");
  Serial.println(actual_val);
  Serial.print("Battery 2-2 Temp: ");
  Serial.println(actual_val);
  Serial.print("Outter External 1 Temp: ");
  Serial.println(actual_val);  
  Serial.print("Inner External 2 Temp: ");
  Serial.println(actual_val);  
  Serial.print("Internal Temp: ");
  Serial.println(actual_val);  
  Serial.print("Battery Voltage 1: ");
  Serial.println(actual_val);    
  Serial.print("Battery Voltage 2: ");
  Serial.println(actual_val);    

  // Change the resolution to 12 bits
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
  
  //Air Pressure Data
  raw_val = analogRead(PIN_PRESSURE_SENSOR);
  telemetry_data->air_pressure = raw_val * PRESSURE_CONSTANT;
  
  //Battery 1-1 Temp
  raw_val = analogRead(PIN_BATTERY1_1_TEMP);
  telemetry_data->battery_temp_1_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Battery 1-2 Temp
  raw_val = analogRead(PIN_BATTERY1_2_TEMP);
  telemetry_data->battery_temp_1_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Battery 2-1 Temp
  raw_val = analogRead(PIN_BATTERY2_1_TEMP);
  telemetry_data->battery_temp_2_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  
  //Battery 2-2 Temp
  raw_val = analogRead(PIN_BATTERY2_2_TEMP);
  telemetry_data->battery_temp_2_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //External Temp 1
  raw_val = analogRead(PIN_EXTERNAL_INNER_TEMP);
  telemetry_data->inner_external_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //External Temp 2
  raw_val = analogRead(PIN_EXTERNAL_OUTTER_TEMP);
  telemetry_data->outter_external_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Internal Temp
  raw_val = analogRead(PIN_INTERNAL_TEMP);
  telemetry_data->internal_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  
  //Battery Voltage 1
  raw_val = analogRead(PIN_BATTERY_VOLTAGE_1);
  telemetry_data->battery_voltage_1 = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  
  //Battery Voltage 2
  raw_val = analogRead(PIN_BATTERY_VOLTAGE_2);
  telemetry_data->battery_voltage_2 = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;

  //GPS Data
  get_gps_data();
  
  //Gyro Data
  get_gyro_data();
  
  Serial.println("");  
  Serial.println("");  
  Serial.println("");  
}

void process_telemetry(struct telemetry_data_struct* telemetry_data)
{
  
}
