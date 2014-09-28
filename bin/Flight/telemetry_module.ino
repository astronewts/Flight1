#include "Arduino.h"
#include "Time.h"

double raw_val;
double actual_val;

void colect_telemetry()
{ 
  // Change the resolution to 12 bits
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
  
  //Air Pressure Data
  raw_val = analogRead(PIN_PRESSURE_SENSOR);
  telemetry_data.air_pressure = raw_val * PRESSURE_CONSTANT;
  
  //Battery 1-1 Temp
  raw_val = analogRead(PIN_BATTERY1_1_TEMP);
  telemetry_data.battery_temp_1_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Battery 1-2 Temp
  raw_val = analogRead(PIN_BATTERY1_2_TEMP);
  telemetry_data.battery_temp_1_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Battery 2-1 Temp
  raw_val = analogRead(PIN_BATTERY2_1_TEMP);
  telemetry_data.battery_temp_2_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  
  //Battery 2-2 Temp
  raw_val = analogRead(PIN_BATTERY2_2_TEMP);
  telemetry_data.battery_temp_2_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //External Temp 1
  raw_val = analogRead(PIN_EXTERNAL_INNER_TEMP);
  telemetry_data.inner_external_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //External Temp 2
  raw_val = analogRead(PIN_EXTERNAL_OUTTER_TEMP);
  telemetry_data.outter_external_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Internal Temp
  raw_val = analogRead(PIN_INTERNAL_TEMP);
  telemetry_data.internal_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  
  //Battery Voltage 1
  raw_val = analogRead(PIN_BATTERY_VOLTAGE_1);
  telemetry_data.battery_voltage_1 = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  
  //Battery Voltage 2
  raw_val = analogRead(PIN_BATTERY_VOLTAGE_2);
  telemetry_data.battery_voltage_2 = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;

  //Charge Flag
  raw_val = digitalRead(PIN_CHARGE_FLAG);
  telemetry_data.charge_flag = raw_val;

  //Temperature Fault Flag
  raw_val = digitalRead(PIN_TEMP_FAULT_FLAG);
  telemetry_data.temp_fault_flag = raw_val;
}

void process_telemetry()
{

   double avg_temp = 0.0;
   // Check the battery temperatures
   // Battery 1
   avg_temp = (telemetry_data.battery_temp_1_1 + telemetry_data.battery_temp_1_2)/2.0;


   if(avg_temp < parameters.battery_temperature_limit_low)
   {
      //Turn heating element on
      digitalWrite(PIN_HEATER_CONTROL_1, HIGH);
      //LOG - "Heating element #1 turned on due to battery temperature of __ being below threshold of ___
   }
   else if(avg_temp > parameters.battery_temperature_limit_high)
   {
      //Turn heating element of
      digitalWrite(PIN_HEATER_CONTROL_1, LOW);
      //LOG - "Heating element #1 turned off due to battery temperature of __ being above threshold of ___
   }
  
   // Battery 2
   avg_temp = (telemetry_data.battery_temp_2_1 + telemetry_data.battery_temp_2_2)/2.0;


   if(avg_temp < parameters.battery_temperature_limit_low)
   {
      //Turn heating element on
      digitalWrite(PIN_HEATER_CONTROL_2, HIGH);
      //LOG - "Heating element #2 turned on due to battery temperature of __ being below threshold of ___
   }
   else if(avg_temp > parameters.battery_temperature_limit_high)
   {
      //Turn heating element of
      digitalWrite(PIN_HEATER_CONTROL_2, LOW);
      //LOG - "Heating element #2 turned off due to battery temperature of __ being above threshold of ___
   }

   //Check Battery Voltage
   //Battery 1
   if(telemetry_data.battery_voltage_1 < parameters.low_voltage_limit)
   {
      // Check if flag is already set
      if(parameters.battery_1_low_voltage_flag == true)
	  {
		  //Check if the timer has reached 
		  //the the low voltage time limit
		  if ((now() - parameters.battery_1_low_voltage_start_time) > parameters.low_voltage_time_limit)
		  {
             //Going into Drop Mode  
		     //LOG - ""
		  }
	  }
	  else
	  {
         //Battery voltage is low - set flag and mark time
	     parameters.battery_1_low_voltage_flag = true;
         parameters.battery_1_low_voltage_start_time = now();
         //LOG
      }
   }
   else
   {
      if(parameters.battery_1_low_voltage_flag == true)
      {
         parameters.battery_1_low_voltage_flag = false;
      }
   }
   
   //Check Battery Voltage
   //Battery 2
   if(telemetry_data.battery_voltage_2 < parameters.low_voltage_limit)
   {
      // Check if flag is already set
      if(parameters.battery_2_low_voltage_flag == true)
	  {
		  //Check if the timer has reached 
		  //the the low voltage time limit
		  if ((now() - parameters.battery_2_low_voltage_start_time) > parameters.low_voltage_time_limit)
		  {
             //Going into Drop Mode  
		     //LOG - ""
		  }
	  }
	  else
	  {
         //Battery voltage is low - set flag and mark time
	     parameters.battery_2_low_voltage_flag = true;
         parameters.battery_2_low_voltage_start_time = now();
         //LOG
      }
   }
   else
   {
      if(parameters.battery_2_low_voltage_flag == true)
      {
         parameters.battery_2_low_voltage_flag = false;
      }
   }
   
}

void print_telemetry()
{
  Serial.print("Air Pressure: ");
  Serial.println(telemetry_data.air_pressure);
  Serial.print("Battery 1-1 Temp: ");
  Serial.println(telemetry_data.battery_temp_1_1);
  Serial.print("Battery 1-2 Temp: ");
  Serial.println(telemetry_data.battery_temp_1_2);
  Serial.print("Battery 2-1 Temp: ");
  Serial.println(telemetry_data.battery_temp_2_1);
  Serial.print("Battery 2-2 Temp: ");
  Serial.println(telemetry_data.battery_temp_2_2);
  Serial.print("Outter External Temp: ");
  Serial.println(telemetry_data.outter_external_temp); 
  Serial.print("Inner External Temp: ");
  Serial.println(telemetry_data.inner_external_temp); 
  Serial.print("Internal Temp: ");
  Serial.println(telemetry_data.internal_temp); 
  Serial.print("Battery Voltage 1: ");
  Serial.println(telemetry_data.battery_voltage_1);   
  Serial.print("Battery Voltage 2: ");
  Serial.println(telemetry_data.battery_voltage_2);   
  Serial.print("Temperature Fault Flag: ");
  Serial.println(telemetry_data.temp_fault_flag);
  Serial.print("Charge Flag: ");
  Serial.println(telemetry_data.charge_flag);

    //GPS Data
  get_gps_data();
  
  //Gyro Data
  get_gyro_data();
}