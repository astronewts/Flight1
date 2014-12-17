double raw_val;
double actual_val;
char buffer[128];

void colect_telemetry()
{ 
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
  
  //Charge Current 1
  raw_val = analogRead(PIN_CHARGE_CURRENT_1);
  telemetry_data.charge_current_1 = (raw_val * CHARGE_CONSTANT_1 * CHARGE_CONSTANT_2)/CHARGE_CONSTANT_3;
  
  //Charge Current 2
  raw_val = analogRead(PIN_CHARGE_CURRENT_2);
  telemetry_data.charge_current_2 = (raw_val * CHARGE_CONSTANT_1 * CHARGE_CONSTANT_2)/CHARGE_CONSTANT_3;  
}

void process_telemetry()
{
   int valid_data = true;
   double value = 0.0;
   
   // NOTE: WHAT ARE YOU FOR???
   double elapsed_time_factor = 0.0;
   
   //Check Pyros
   if(digitalRead(PIN_PYRO_1_FIRE) == HIGH)
   {
     if(parameters.pyro_initiation_elapsed_time >= parameters.pyro_pulse_width)
     {
        //Set Primary Pyro to Low
        digitalWrite(PIN_PYRO_1_FIRE, LOW);
        
        //Set Backup Pyro to High
        digitalWrite(PIN_PYRO_2_FIRE, HIGH);
        
        //Reset the initiation elapsed time
        parameters.pyro_initiation_elapsed_time = 0;
     }
   }
   
   if(digitalRead(PIN_PYRO_2_FIRE) == HIGH)
   {
     if(parameters.pyro_initiation_elapsed_time >= parameters.pyro_pulse_width)
     {
        //Set Backup Pyro to Low
        digitalWrite(PIN_PYRO_2_FIRE, LOW);
        
        //Disable the Pyro Pin
        digitalWrite(PIN_PYRO_ENABLE, LOW);
     }
   }
   
   // Check the battery temperatures
   // Battery 1
   
   //Sanity Check
   valid_data = true;
   if((telemetry_data.battery_temp_1_1 <= parameters.battery_temperature_sanity_check_high) &&
      (telemetry_data.battery_temp_1_1 >= parameters.battery_temperature_sanity_check_low))
   {
	   if((telemetry_data.battery_temp_1_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_temp_1_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
          value = (telemetry_data.battery_temp_1_1 + telemetry_data.battery_temp_1_2)/2.0;
       }
       else
       {
          value = telemetry_data.battery_temp_1_1;
       }
   }
   else
   {
       if((telemetry_data.battery_temp_1_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_temp_1_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
		  value = telemetry_data.battery_temp_1_2;
	   }
	   else
	   {
	      valid_data = false;
	   }
   }

   if(valid_data == true)
   {
	   sprintf(buffer, "Battery 1 avg. temp. = %f", value);
	   Serial.println(buffer);
	   if(value < parameters.battery_temperature_limit_low)
	   {
		  //Turn heating element on
		  digitalWrite(PIN_HEATER_CONTROL_1, HIGH);
                  parameters.heater_state_1 = true;
		  sprintf(buffer, "Heating element #1 turned on due to average battery temperature of %f going below threshhold of %f,",
						value, parameters.battery_temperature_limit_low);
		  Serial.println(buffer);
	   }
	   else if(value > parameters.battery_temperature_limit_high)
	   {
		  //Turn heating element off
		  digitalWrite(PIN_HEATER_CONTROL_1, LOW);
                  parameters.heater_state_1 = false;
		  sprintf(buffer, "Heating element #1 turned off due to average battery temperature of %f going above threshhold of %f,",
						value, parameters.battery_temperature_limit_high);
		  Serial.println(buffer);
	   }
   }
   
   // Battery 2
   //Sanity Check
   valid_data = true;
   if((telemetry_data.battery_temp_2_1 <= parameters.battery_temperature_sanity_check_high) &&
      (telemetry_data.battery_temp_2_1 >= parameters.battery_temperature_sanity_check_low))
   {
	   if((telemetry_data.battery_temp_2_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_temp_2_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
          value = (telemetry_data.battery_temp_2_1 + telemetry_data.battery_temp_2_2)/2.0;
       }
       else
       {
          value = telemetry_data.battery_temp_2_1;
       }
   }
   else
   {
       if((telemetry_data.battery_temp_2_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_temp_2_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
		  value = telemetry_data.battery_temp_2_2;
	   }
	   else
	   {
	      valid_data = false;
	   }
   }

   if(valid_data == true)
   {
	   sprintf(buffer, "Battery 2 avg. temp. = %f", value);
	   Serial.println(buffer);

	   if(value < parameters.battery_temperature_limit_low)
	   {
		  //Turn heating element on
		  digitalWrite(PIN_HEATER_CONTROL_2, HIGH);
                  parameters.heater_state_2 = true;
		  sprintf(buffer, "Heating element #2 turned on due to average battery temperature of %f going below threshhold of %f,",
						value, parameters.battery_temperature_limit_low);
		  Serial.println(buffer);
	   }
	   else if(value > parameters.battery_temperature_limit_high)
	   {
		  //Turn heating element off
		  digitalWrite(PIN_HEATER_CONTROL_2, LOW);
                  parameters.heater_state_2 = false;
		  sprintf(buffer, "Heating element #1 turned off due to average battery temperature of %f going above threshhold of %f,",
						value, parameters.battery_temperature_limit_high);
		  Serial.println(buffer);
	   }
   }

   //Check Battery Voltage
   //Get the highest voltage
   //Sanity Check
   valid_data = true;
   if((telemetry_data.battery_voltage_1 <= parameters.voltage_sanity_check_high) &&
      (telemetry_data.battery_voltage_1 >= parameters.voltage_sanity_check_low))
   {
      if((telemetry_data.battery_voltage_2 <= parameters.voltage_sanity_check_high) &&
         (telemetry_data.battery_voltage_2 >= parameters.voltage_sanity_check_low))
      {  
		   if(telemetry_data.battery_voltage_1 >= telemetry_data.battery_voltage_2)
		   {
			  value = telemetry_data.battery_voltage_1;
		   }
		   else
		   {
			  value = telemetry_data.battery_voltage_2;
		   }
		}
		else
		{
		   value = telemetry_data.battery_voltage_1;
		}
   }
   else
   {
      if((telemetry_data.battery_voltage_2 <= parameters.voltage_sanity_check_high) &&
         (telemetry_data.battery_voltage_2 >= parameters.voltage_sanity_check_low))
      {
         value = telemetry_data.battery_voltage_2;
      }
      else
      {
         valid_data = false;
      }
   }
   
   if(valid_data == true)
   {
      //Compare voltage to limit
      if(value < parameters.low_voltage_limit)
      {
	     if(parameters.battery_low_voltage_flag == false)
	     {
            //Battery voltage is low - set flag and mark time
	        parameters.battery_low_voltage_flag = true;
           parameters.battery_low_voltage_elapsed_time = 0;
           sprintf(buffer, "Battery voltage of %f is below threshhold of %f. Starting timer of maximum allowed low voltage time (%fs).  Goind into Load Shed Mode.",
                            value, parameters.low_voltage_limit, parameters.low_voltage_time_limit);
           Serial.println(buffer);
            
           //Enter Load Shed Mode
           set_load_shed_mode();
         }
      }
      else
      {
         if(parameters.battery_low_voltage_flag == true)
         {
            parameters.battery_low_voltage_flag = false;
         }
      }
      
      // Battery Voltage Charge Control
      if(value < parameters.voltage_power_limit_low)
      {
         //Turn the power on
         digitalWrite(PIN_POWER_SHUTDOWN, LOW);
         parameters.batttery_charge_shutdown = false;
      }
      
      if(value > parameters.voltage_power_limit_high)
      {
         //Turn the power off
         digitalWrite(PIN_POWER_SHUTDOWN, HIGH);
         parameters.batttery_charge_shutdown = true;
         
         //Reset the charge counts
         parameters.amphrs_charging = 0.0;
         parameters.amphrs_discharging = 0.0;
      }       
  }
   
   //Charge Current 
   //Sanity Check
   valid_data = true;
   if((telemetry_data.charge_current_1 <= parameters.charge_current_sanity_check_high) &&
      (telemetry_data.charge_current_1 >= parameters.charge_current_sanity_check_low))
   {
	   if((telemetry_data.charge_current_2 <= parameters.charge_current_sanity_check_high) &&
		   (telemetry_data.charge_current_2 >= parameters.charge_current_sanity_check_low))
	   { 
          value = (telemetry_data.charge_current_1 + telemetry_data.charge_current_2)/2.0;
       }
       else
       {
          value = telemetry_data.charge_current_1;
       }
   }
   else
   {
       if((telemetry_data.charge_current_2 <= parameters.charge_current_sanity_check_high) &&
		    (telemetry_data.charge_current_2 >= parameters.charge_current_sanity_check_low))
	   { 
		  value = telemetry_data.charge_current_2;
	   }
	   else
	   {
	      valid_data = false;
	   }
   }
   
   if(valid_data == true)
   {
      //Get the elapsed time factor
      elapsed_time_factor = MS_IN_SEC / parameters.charge_current_read_elapsed_time;
      
      //reset elapsed charge current time
      parameters.charge_current_read_elapsed_time = 0;
      
      if(value > 0)
      {
         parameters.amphrs_charging += ((value / (elapsed_time_factor * SECS_IN_HOUR)) / parameters.recharge_ratio);
      }
      else if (value < 0)
      {
         parameters.amphrs_discharging += (value / (elapsed_time_factor * SECS_IN_HOUR));
      }
      
      value = parameters.amphrs_charging - parameters.amphrs_discharging;
      
      if(value < parameters.capacity_limit_low)
      {
         //Turn the power ON
         digitalWrite(PIN_POWER_SHUTDOWN, LOW);
         parameters.batttery_charge_shutdown = false;
         // NOTE: Capacity limit is negative
      }
      
      if(value > parameters.capacity_limit_high)
      {
         //Turn the power Off
         digitalWrite(PIN_POWER_SHUTDOWN, HIGH);
         parameters.batttery_charge_shutdown = true;
         // NOTE: Capacity limit is negative
         
         //Reset the charge counts
         parameters.amphrs_charging = 0.0;
         parameters.amphrs_discharging = 0.0;
      }
   }
   
  // Battery Failure Checking 
  // Check if voltage flag is already set
  if(parameters.battery_low_voltage_flag == true)
  {
	  //Check if the timer has reached 
	  //the the low voltage time limit
	  if (parameters.battery_low_voltage_elapsed_time >= parameters.low_voltage_time_limit)
	  {
	     sprintf(buffer, "Battery voltage (currently %f) has been below threshhold (%f) for low voltage time limit of %fs.  Going into Emergency Descent Mode.",
                 value, parameters.low_voltage_limit, parameters.low_voltage_time_limit);
          Serial.println(buffer);
          
          //Enter Emergency Descent Mode
          set_emergency_decent_mode();
	  }
  }
  
  
  //Check Altitude.
  if(parameters.altitude_valid_flag == true)
  {
     if(gps.altitude.isValid())
     {
        if(gps.altitude.meters() >= parameters.altitude_sanity_check_low)
        {
           if(gps.altitude.meters() < parameters.altitude_limit_low)
           {
              //Enter Emergency Descent Mode
             set_emergency_decent_mode();
           }
        }
      }
   }
}

void print_telemetry()
{
  Serial.println("-----------Telemetry---------------");
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
  Serial.print("Charge Current 1: ");
  Serial.println(telemetry_data.charge_current_1);
  Serial.print("Charge Current 2: ");
  Serial.println(telemetry_data.charge_current_2);
  print_gps_data();
  get_gyro_data();
  Serial.println("");
  Serial.println("");
  Serial.println("");
}
