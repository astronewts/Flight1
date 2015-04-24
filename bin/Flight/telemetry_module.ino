
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
  telemetry_data.battery_1_temp_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Battery 1-2 Temp
  raw_val = analogRead(PIN_BATTERY1_2_TEMP);
  telemetry_data.battery_1_temp_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

  //Battery 2-1 Temp
  raw_val = analogRead(PIN_BATTERY2_1_TEMP);
  telemetry_data.battery_2_temp_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  
  //Battery 2-2 Temp
  raw_val = analogRead(PIN_BATTERY2_2_TEMP);
  telemetry_data.battery_2_temp_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);

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
  raw_val = analogRead(PIN_BATTERY_1_VOLTAGE_1);
  telemetry_data.battery_1_voltage_1 = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  
  //Battery Voltage 2
  raw_val = analogRead(PIN_BATTERY_1_VOLTAGE_2);
  telemetry_data.battery_1_voltage_2 = ((raw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;

  //Charge Flag
  //raw_val = digitalRead(PIN_CHARGE_FLAG);
  //telemetry_data.charge_flag = raw_val;

  //Temperature Fault Flag
  //raw_val = digitalRead(PIN_TEMP_FAULT_FLAG);
  //telemetry_data.temp_fault_flag = raw_val;
  
  //Charge Current 1
  raw_val = analogRead(PIN_BATTERY_1_CHARGE_CURRENT_1);
  telemetry_data.battery_1_charge_current_1 = (raw_val * CHARGE_CONSTANT_1 * CHARGE_CONSTANT_2)/CHARGE_CONSTANT_3;
  
  //Charge Current 2
  raw_val = analogRead(PIN_BATTERY_1_CHARGE_CURRENT_2);
  telemetry_data.battery_1_charge_current_2 = (raw_val * CHARGE_CONSTANT_1 * CHARGE_CONSTANT_2)/CHARGE_CONSTANT_3;  
}

void process_telemetry()
{
   int valid_data = true;
   double value = 0.0;
   
   // NOTE: WHAT ARE YOU FOR???!!!!
   double battery_1_elapsed_time_factor = 0.0;
   
   //Check Cutdown Process
   if(digitalRead(PIN_CUTDOWN_1_FIRE) == HIGH)
   {
     if(parameters.cutdown_initiation_elapsed_time >= parameters.cutdown_pulse_width)
     {
        //Set Primary Cutdown to Low
        digitalWrite(PIN_CUTDOWN_1_FIRE, LOW);
        
        //Set Backup Cutdown to High
        digitalWrite(PIN_CUTDOWN_2_FIRE, HIGH);
        
        //Reset the initiation elapsed time
        parameters.cutdown_initiation_elapsed_time = 0;
     }
   }
   
   if(digitalRead(PIN_CUTDOWN_2_FIRE) == HIGH)
   {
     if(parameters.cutdown_initiation_elapsed_time >= parameters.cutdown_pulse_width)
     {
        //Set Backup Cutdown to Low
        digitalWrite(PIN_CUTDOWN_2_FIRE, LOW);
        
        //Disable the Cutdown Pin
        digitalWrite(PIN_CUTDOWN_ENABLE, LOW);
     }
   }
   
   // Check the battery temperatures
   // Battery 1
   
   //Sanity Check
   valid_data = true;
   if((telemetry_data.battery_1_temp_1 <= parameters.battery_temperature_sanity_check_high) &&
      (telemetry_data.battery_1_temp_1 >= parameters.battery_temperature_sanity_check_low))
   {
	   if((telemetry_data.battery_1_temp_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_1_temp_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
          value = (telemetry_data.battery_1_temp_1 + telemetry_data.battery_1_temp_2)/2.0;
       }
       else
       {
          value = telemetry_data.battery_1_temp_1;
       }
   }
   else
   {
       if((telemetry_data.battery_1_temp_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_1_temp_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
		  value = telemetry_data.battery_1_temp_2;
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
   if((telemetry_data.battery_2_temp_1 <= parameters.battery_temperature_sanity_check_high) &&
      (telemetry_data.battery_2_temp_1 >= parameters.battery_temperature_sanity_check_low))
   {
	   if((telemetry_data.battery_2_temp_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_2_temp_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
          value = (telemetry_data.battery_2_temp_1 + telemetry_data.battery_2_temp_2)/2.0;
       }
       else
       {
          value = telemetry_data.battery_2_temp_1;
       }
   }
   else
   {
       if((telemetry_data.battery_2_temp_2 <= parameters.battery_temperature_sanity_check_high) &&
		  (telemetry_data.battery_2_temp_2 >= parameters.battery_temperature_sanity_check_low))
	   { 
		  value = telemetry_data.battery_2_temp_2;
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
   if((telemetry_data.battery_1_voltage_1 <= parameters.voltage_sanity_check_high) &&
      (telemetry_data.battery_1_voltage_1 >= parameters.voltage_sanity_check_low))
   {
      if((telemetry_data.battery_1_voltage_2 <= parameters.voltage_sanity_check_high) &&
         (telemetry_data.battery_1_voltage_2 >= parameters.voltage_sanity_check_low))
      {  
		   if(telemetry_data.battery_1_voltage_1 >= telemetry_data.battery_1_voltage_2)
		   {
			  value = telemetry_data.battery_1_voltage_1;
		   }
		   else
		   {
			  value = telemetry_data.battery_1_voltage_2;
		   }
		}
		else
		{
		   value = telemetry_data.battery_1_voltage_1;
		}
   }
   else
   {
      if((telemetry_data.battery_1_voltage_2 <= parameters.voltage_sanity_check_high) &&
         (telemetry_data.battery_1_voltage_2 >= parameters.voltage_sanity_check_low))
      {
         value = telemetry_data.battery_1_voltage_2;
      }
      else
      {
         valid_data = false;
      }
   }
   
   if(valid_data == true)
   {
      //Compare voltage to loadshed entry threshold
      if(value < parameters.low_voltage_limit_for_loadshed_entry)
      {
	     if(parameters.battery_bus_low_voltage_flag == false)
	     {
            //Battery voltage is low - set flag and mark time
	        parameters.battery_bus_low_voltage_flag = true;
           parameters.battery_low_voltage_elapsed_time = 0;
           sprintf(buffer, "Battery voltage of %f is below threshhold of %f. Starting timer of maximum allowed low voltage time (%fs).  Goind into Load Shed Mode.",
                            value, parameters.low_voltage_limit_for_loadshed_entry, parameters.low_voltage_time_limit);
           Serial.println(buffer);
            
           //Enter Load Shed Mode
           set_load_shed_mode();
         }
      }
      else
      {
         if(parameters.battery_bus_low_voltage_flag == true)
         {
            parameters.battery_bus_low_voltage_flag = false;
         }
      }
      
      // Battery Voltage Charge Control
      if(value < parameters.battery_1_voltage_init_threshold)
      {
         //Turn the power on
         digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
         parameters.battery_1_charging_status = true;
      }
      
      if(value > parameters.battery_1_voltage_term_threshold)
      {
         //Turn the power off
         digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, HIGH);
         parameters.battery_1_charging_status = false;
         
         //Reset the charge counts
         parameters.battery_1_amphrs_charging = 0.0;
         parameters.battery_1_amphrs_discharging = 0.0;
      }       
      //TODO: DUPLICATE TO ADD BATTERY 2 CODE!!!
  }
   
   //Charge Current 
   //Sanity Check
   valid_data = true;
   if((telemetry_data.battery_1_charge_current_1 <= parameters.charge_current_sanity_check_high) &&
      (telemetry_data.battery_1_charge_current_1 >= parameters.charge_current_sanity_check_low))
   {
	   if((telemetry_data.battery_1_charge_current_2 <= parameters.charge_current_sanity_check_high) &&
		   (telemetry_data.battery_1_charge_current_2 >= parameters.charge_current_sanity_check_low))
	   { 
          value = (telemetry_data.battery_1_charge_current_1 + telemetry_data.battery_1_charge_current_2)/2.0;
       }
       else
       {
          value = telemetry_data.battery_1_charge_current_1;
       }
   }
   else
   {
       if((telemetry_data.battery_1_charge_current_2 <= parameters.charge_current_sanity_check_high) &&
		    (telemetry_data.battery_1_charge_current_2 >= parameters.charge_current_sanity_check_low))
	   { 
		  value = telemetry_data.battery_1_charge_current_2;
	   }
	   else
	   {
	      valid_data = false;
	   }
   }
   
   if(valid_data == true)
   {
      //Get the elapsed time factor
      battery_1_elapsed_time_factor = MS_IN_SEC /  parameters.battery_1_charge_current_read_elapsed_time;
      
      //reset elapsed charge current time
       parameters.battery_1_charge_current_read_elapsed_time = 0;
      
      if(value > 0)
      {
         parameters.battery_1_amphrs_charging += ((value / (battery_1_elapsed_time_factor * SECS_IN_HOUR)) / parameters.battery_1_recharge_ratio);
      }
      else if (value < 0)
      {
         parameters.battery_1_amphrs_discharging += (value / (battery_1_elapsed_time_factor * SECS_IN_HOUR));
      }
      
      value = parameters.battery_1_amphrs_charging - parameters.battery_1_amphrs_discharging;
      
      if(value < parameters.battery_1_amphrs_init_threshold)
      {
         //Turn the power ON
         digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
         parameters.battery_1_charging_status = true;
      }
      
      if(value > parameters.battery_1_amphrs_term_threshold)
      {
         //Turn the power Off
         digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, HIGH);
         parameters.battery_1_charging_status = false;
         
         //Reset the charge counts
         parameters.battery_1_amphrs_charging = 0.0;
         parameters.battery_1_amphrs_discharging = 0.0;
      }
   }
   //TODO: DUPLICATE TO ADD BATTERY 2 CODE !!!!
   
//TODO: NEEDS TO BE CODE THAT SEPARATES LOADSHED ENTRY FROM AUTO-CUTDOWN

  // Battery Failure Checking 
  // Check if voltage flag is already set
  if(parameters.battery_bus_low_voltage_flag == true)
  {
	  //Check if the timer has reached 
	  //the the low voltage time limit
	  if (parameters.battery_low_voltage_elapsed_time >= parameters.low_voltage_time_limit)
	  {
	     sprintf(buffer, "Battery voltage (currently %f) has been below threshhold (%f) for low voltage time limit of %fs.  Going into Emergency Descent Mode.",
                 value, parameters.low_voltage_limit_for_loadshed_entry, parameters.low_voltage_time_limit);
          Serial.println(buffer);
          
          //Enter Emergency Descent Mode
          set_emergency_decent_mode();
	  }
  }
  
  
  // Check Altitude.
  // TODO: ADD CODE THAT IMPLEMENTS THE AUTOMATIC CUTDOWN IF THE BATTERY IS TOO LOW !
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
  Serial.println(telemetry_data.battery_1_temp_1);
  Serial.print("Battery 1-2 Temp: ");
  Serial.println(telemetry_data.battery_1_temp_2);
  Serial.print("Battery 2-1 Temp: ");
  Serial.println(telemetry_data.battery_2_temp_1);
  Serial.print("Battery 2-2 Temp: ");
  Serial.println(telemetry_data.battery_2_temp_2);
  Serial.print("Outter External Temp: ");
  Serial.println(telemetry_data.outter_external_temp); 
  Serial.print("Inner External Temp: ");
  Serial.println(telemetry_data.inner_external_temp); 
  Serial.print("Internal Temp: ");
  Serial.println(telemetry_data.internal_temp); 
  Serial.print("Battery Voltage 1: ");
  Serial.println(telemetry_data.battery_1_voltage_1);   
  Serial.print("Battery Voltage 2: ");
  Serial.println(telemetry_data.battery_1_voltage_2);   
  Serial.print("Charge Current 1: ");
  Serial.println(telemetry_data.battery_1_charge_current_1);
  Serial.print("Charge Current 2: ");
  Serial.println(telemetry_data.battery_1_charge_current_2);
  print_gps_data();
  get_gyro_data();
  Serial.println("");
  Serial.println("");
  Serial.println("");
}
