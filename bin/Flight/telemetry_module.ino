
double raw_val;
double actual_val;
char buffer[128];
int count_low_alt;

//Collect All Analog TLM
void collect_analog_telemetry()
{
  //Air Pressure Data
  raw_telemetry_data.raw_air_pressure = analogRead(PIN_PRESSURE_SENSOR);
  telemetry_data.air_pressure = raw_telemetry_data.raw_air_pressure * PRESSURE_CONSTANT;
  delay(100);

  //Battery 1-1 Temp
  raw_telemetry_data.raw_battery_1_temp_1 = analogRead(PIN_BATTERY1_1_TEMP);
  telemetry_data.battery_1_temp_1 = calculate_temp(raw_telemetry_data.raw_battery_1_temp_1);
  delay(100);

  //Battery 1-2 Temp
  raw_telemetry_data.raw_battery_1_temp_2 = analogRead(PIN_BATTERY1_2_TEMP);
  telemetry_data.battery_1_temp_2 = calculate_temp(raw_telemetry_data.raw_battery_1_temp_2);
  delay(100);

  //Battery 2-1 Temp
  raw_telemetry_data.raw_battery_2_temp_1 = analogRead(PIN_BATTERY2_1_TEMP);
  telemetry_data.battery_2_temp_1 = calculate_temp(raw_telemetry_data.raw_battery_2_temp_1);
  delay(100);

  //Battery 2-2 Temp
  raw_telemetry_data.raw_battery_2_temp_2 = analogRead(PIN_BATTERY2_2_TEMP);
  telemetry_data.battery_2_temp_2 = calculate_temp(raw_telemetry_data.raw_battery_2_temp_2);
  delay(100);

  //External Temp 1
  raw_telemetry_data.raw_inner_external_temp = analogRead(PIN_EXTERNAL_INNER_TEMP);
  telemetry_data.inner_external_temp = calculate_temp(raw_telemetry_data.raw_inner_external_temp);
  delay(100);

  //External Temp 2
  raw_telemetry_data.raw_outter_external_temp = analogRead(PIN_EXTERNAL_OUTTER_TEMP);
  telemetry_data.outter_external_temp = calculate_temp(raw_telemetry_data.raw_outter_external_temp);
  delay(100);

  //Internal Temp
  raw_telemetry_data.raw_internal_temp = analogRead(PIN_INTERNAL_TEMP);
  telemetry_data.internal_temp = calculate_temp(raw_telemetry_data.raw_internal_temp);
  delay(100);

  //ANALOG VIN VOLTAGE
  raw_telemetry_data.raw_analog_VIN_voltage = analogRead(PIN_ANALOG_VIN_VOLTAGE);
  telemetry_data.analog_VIN_voltage = ((raw_telemetry_data.raw_analog_VIN_voltage * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3 + VOLTAGE_CONSTANT_4;
  delay(100);
}

//void collect_analog_battery_current_telemetry()
//{
//  //Battery 1 Charge Current 1
//  raw_val = analogRead(PIN_BATTERY_1_CHARGE_CURRENT_1);
//  telemetry_data.battery_1_charge_current_1 = (((raw_val * CHARGE_CONSTANT_1)/CHARGE_CONSTANT_2) - CHARGE_CONSTANT_3) * CHARGE_CONSTANT_4;
//  delay(100);
//
//  //Battery 1 Charge Current 2
//  raw_val = analogRead(PIN_BATTERY_1_CHARGE_CURRENT_2);
//  telemetry_data.battery_1_charge_current_2 = (((raw_val * CHARGE_CONSTANT_1)/CHARGE_CONSTANT_2) - CHARGE_CONSTANT_3) * CHARGE_CONSTANT_4;
//  delay(100);
//
//  //TODO: ADD ALL BATTERY 2 TLM FLOWDOWNS
//}

double calculate_temp(int counts)
{
  double temp;
  if (counts > TEMP_BREAK_COUNT)
  {
    temp = TEMP_CONSTANT_1_1+TEMP_CONSTANT_1_2*(counts)+TEMP_CONSTANT_1_3*(pow(counts,2))+TEMP_CONSTANT_1_4*(pow(counts,3))+TEMP_CONSTANT_1_5*(pow(counts,4))+TEMP_CONSTANT_1_6*(pow(counts,5));
  }
  else
  {
    temp = TEMP_CONSTANT_2_1+TEMP_CONSTANT_2_2*(counts)+TEMP_CONSTANT_2_3*(pow(counts,2))+TEMP_CONSTANT_2_4*(pow(counts,3))+TEMP_CONSTANT_2_5*(pow(counts,4))+TEMP_CONSTANT_2_6*(pow(counts,5));
  }
  return temp;
}

void cutdown_check()
{
  //Check Cutdown Process
  if(parameters.cutdown_1_status == true)
  {
    if(parameters.cutdown_initiation_elapsed_time >= parameters.cutdown_pulse_width)
    {
      //Set Primary Cutdown to Low
      digitalWrite(PIN_CUTDOWN_1_FIRE, LOW);
      parameters.cutdown_1_status = false;

      //Set Backup Cutdown to High
      digitalWrite(PIN_CUTDOWN_2_FIRE, HIGH);
      parameters.cutdown_2_status = true;

      //Reset the initiation elapsed time
      parameters.cutdown_initiation_elapsed_time = 0;
    }
  }

  if(parameters.cutdown_2_status == true)
  {
    if(parameters.cutdown_initiation_elapsed_time >= parameters.cutdown_pulse_width)
    {
      //Set Backup Cutdown to Low
      digitalWrite(PIN_CUTDOWN_2_FIRE, LOW);
      parameters.cutdown_2_status = false;

      //Disable the Cutdown Pin
      //digitalWrite(PIN_CUTDOWN_ENABLE, LOW);
      //parameters.cutdown_enable_state = false;

      //Throw Flag to verify Cut-down sequence is complete
      parameters.cutdown_event_flag = true;
    }
  }
}

double sanity_processing(int TLM1, int TLM2, int TLM_ID)
{
  int tlm_out = 767;
  int high_thresh;
  int low_thresh;

  // Telemetry ID Decoder:
  // Battery Temp 1 = 1
  // Battery Temp 2 = 2
  // Battery Voltages = 3
  // Battery 1 Currents = 4
  // Battery 2 Currents = 5
  // GPS Altitude = 6
  // Alt Altitude = 7

  if(TLM_ID == 1)
  {
    parameters.battery_1_temp_tlm_valid_flag = true;
    high_thresh = parameters.battery_temperature_sanity_check_high;
    low_thresh = parameters.battery_temperature_sanity_check_low;
  }
  else if(TLM_ID == 2)
  {
    parameters.battery_2_temp_tlm_valid_flag = true;
    high_thresh = parameters.battery_temperature_sanity_check_high;
    low_thresh = parameters.battery_temperature_sanity_check_low;
  }
  else if(TLM_ID == 3)
  {
    parameters.battery_voltage_tlm_valid_flag = true;
    high_thresh = parameters.voltage_sanity_check_high;
    low_thresh = parameters.voltage_sanity_check_low;
  }
  else if(TLM_ID == 4)
  {
    parameters.battery_1_current_tlm_valid_flag = true;
    high_thresh = parameters.charge_current_sanity_check_high;
    low_thresh = parameters.charge_current_sanity_check_low;
  }
  else if(TLM_ID == 5)
  {
    parameters.battery_2_current_tlm_valid_flag = true;
    high_thresh = parameters.charge_current_sanity_check_high;
    low_thresh = parameters.charge_current_sanity_check_low;
  }
  else if(TLM_ID == 6)
  {
    parameters.gps_alt_valid_flag = true;
    high_thresh = parameters.altitude_sanity_check_high;
    low_thresh = parameters.altitude_sanity_check_low;
  }
  else if(TLM_ID == 7)
  {
    parameters.altitude_valid_flag = true;
    high_thresh = parameters.altitude_sanity_check_high;
    low_thresh = parameters.altitude_sanity_check_low;
  }

  //bool valid_data = true;

  if((TLM1 <= high_thresh) && (TLM1 >= low_thresh))
  {
    if((TLM2 <= high_thresh) && (TLM2 >= low_thresh))
    {
      if(TLM_ID == 3)
      {
        if(TLM1 > TLM2)
        {
          tlm_out = TLM1;
        }
        else
        {
          tlm_out = TLM2;
        }

      }
      else
      {
        tlm_out = (TLM1 + TLM2)/2.0;
      }
    }
    else
    {
      tlm_out = TLM1;
    }
  }
  else if((TLM2 <= high_thresh) && (TLM2 >= low_thresh))
  {
    tlm_out = TLM2;
  }
  else
  {   
    if(TLM_ID == 1)
    {
      parameters.battery_1_temp_tlm_valid_flag = false;
    }
    else if(TLM_ID == 2)
    {
      parameters.battery_2_temp_tlm_valid_flag = false;
    }
    else if(TLM_ID == 3)
    {
      parameters.battery_voltage_tlm_valid_flag = false;
    }
    else if(TLM_ID == 4)
    {
      parameters.battery_1_current_tlm_valid_flag = false;
    }
    else if(TLM_ID == 5)
    {
      parameters.battery_2_current_tlm_valid_flag = false;
    }
    else if(TLM_ID == 6)
    {
      parameters.gps_alt_valid_flag = false;
    }
    else if(TLM_ID == 7)
    {
      parameters.altitude_valid_flag = false;
    }
    
    write_telemetry_data_to_sd();
  }
  return tlm_out;
}

void evaluate_heater_state(double val, int batt_ID)
{
  if(val < parameters.battery_temperature_limit_low * 1.0)
  {
    //Turn heating element on
    //if((batt_ID == 1) || (parameters.heater_state_1 == false))
    if(batt_ID == 1)
    {
      digitalWrite(PIN_HEATER_CONTROL_1, HIGH);
      parameters.heater_state_1 = true;
      write_telemetry_data_to_sd();
      if(debug.mode == 1) {
        Serial.print("Heating element #");
        Serial.print(batt_ID);
        Serial.print("turned on due to average battery temperature of ");
        Serial.print(val);
        Serial.print("going below threshhold of ");
        Serial.println(parameters.battery_temperature_limit_low);
      }
    }
    //else if((batt_ID == 2) || (parameters.heater_state_2 == false))
    else if(batt_ID == 2) 
    {
      digitalWrite(PIN_HEATER_CONTROL_2, HIGH);
      parameters.heater_state_2 = true;
      write_telemetry_data_to_sd();
      if(debug.mode == 1) {
        Serial.print("Heating element #");
        Serial.print(batt_ID);
        Serial.print("turned on due to average battery temperature of ");
        Serial.print(val);
        Serial.print("going below threshhold of ");
        Serial.println(parameters.battery_temperature_limit_low);
       }
    }
    
  }
  else if(val > parameters.battery_temperature_limit_high * 1.0)
  {
    //Turn heating element off
    //if((batt_ID == 1) || (parameters.heater_state_1 == true))
    if(batt_ID == 1) 
    {
      digitalWrite(PIN_HEATER_CONTROL_1, LOW);
      parameters.heater_state_1 = false;
      write_telemetry_data_to_sd();
      if(debug.mode == 1) {
        Serial.print("Heating element #");
        Serial.print(batt_ID);
        Serial.print("turned OFF due to average battery temperature of ");
        Serial.print(val);
        Serial.print("going above threshhold of ");
        Serial.println(parameters.battery_temperature_limit_high);
      }
    }
    //else if((batt_ID == 2) || (parameters.heater_state_2 == true))
    else if(batt_ID == 2) 
    {
      digitalWrite(PIN_HEATER_CONTROL_2, LOW);
      parameters.heater_state_2 = false;
      write_telemetry_data_to_sd();
      if(debug.mode == 1) {
        Serial.print("Heating element #");
        Serial.print(batt_ID);
        Serial.print("turned OFF due to average battery temperature of ");
        Serial.print(val);
        Serial.print("going above threshhold of ");
        Serial.println(parameters.battery_temperature_limit_high);
       }
     }
  }
}

void execute_thermal_control_check()
{
  ////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////// THERMAL CONTROL OF BATTERY HEATERS ///////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////

  double tlm_value = 0.0;

  // Battery 1
  // Sanity Check: third input specifies Battery 1 TLM ID
  tlm_value = sanity_processing(telemetry_data.battery_1_temp_1, telemetry_data.battery_1_temp_2, 1);

  if(parameters.battery_1_temp_tlm_valid_flag == true)
  {
    // Second parameter specifies Battery 1
    evaluate_heater_state(tlm_value,1);
  }
  else
  {
    if(debug.mode == 1) {
        Serial.print("Battery 1 Temperatures are invalid! TLM1:");
        Serial.print(telemetry_data.battery_1_temp_1);
        Serial.print(", TLM2: ");
        Serial.println(telemetry_data.battery_1_temp_2); 
    }        
  }

  // Battery 2
  // Sanity Check: third input specifies Battery 2 TLM ID
  tlm_value = sanity_processing(telemetry_data.battery_2_temp_1, telemetry_data.battery_2_temp_2, 2);

  if(parameters.battery_2_temp_tlm_valid_flag == true)
  {
    // Second parameter specifies Battery 2
    evaluate_heater_state(tlm_value,2);
  }
  else
  {
    if(debug.mode == 1) {
        Serial.print("Battery 2 Temperatures are invalid! TLM1:");
        Serial.print(telemetry_data.battery_2_temp_1);
        Serial.print(", TLM2: ");
        Serial.println(telemetry_data.battery_2_temp_2); 
    }       
  }
}

void process_charge_current_tlm()
{ 
  double tlm_value = 0.0;
  double tlm_check = 0.0;
  float battery_1_elapsed_time_factor = 0.0;
  // NOTE: MIGHT WANT TO THINK ABOUT A ROLLING AVERAGE HERE IN THE FUTURE

  // Charge Current
  // Sanity Check
  tlm_value = sanity_processing(telemetry_data.battery_1_charge_current, telemetry_data.battery_1_charge_current, 4);

  if(parameters.battery_1_current_tlm_valid_flag == true)
  {
    // Get the elapsed time factor
    battery_1_elapsed_time_factor = MS_IN_SEC /  parameters.battery_1_charge_current_read_elapsed_time;

    // Reset elapsed charge current time
    parameters.battery_1_charge_current_read_elapsed_time = 0;

    if(tlm_value > 0)
    {
      parameters.battery_1_amphrs_charging += ((tlm_value / (battery_1_elapsed_time_factor * SECS_IN_HOUR)) / parameters.battery_1_recharge_ratio);
    }
    else if (tlm_value < 0)
    {
      parameters.battery_1_amphrs_discharging += (tlm_value / (battery_1_elapsed_time_factor * SECS_IN_HOUR));
    }

    tlm_check = parameters.battery_1_amphrs_charging - parameters.battery_1_amphrs_discharging;

    // NOTE: THIS FUNCTION IS EFFECTIVELY DISABLED
    // DISABLING THIS RESET DUE TO PRESENT HW DESIGN (LC: 9/8/16)
    if(tlm_check < parameters.battery_1_amphrs_init_threshold)
    {
      //Turn the power ON
      digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
      parameters.battery_1_charging_status = true;
    }

    // NOTE: THIS FUNCTION IS EFFECTIVELY DISABLED
    // DISABLING THIS RESET DUE TO PRESENT HW DESIGN (LC: 9/8/16)
    if(tlm_check > parameters.battery_1_amphrs_term_threshold)
    {
      //Turn the power Off
      digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, HIGH);
      parameters.battery_1_charging_status = false;

      //Reset the charge counts
      //parameters.battery_1_amphrs_charging = 0.0;
      //parameters.battery_1_amphrs_discharging = 0.0;
    }
  }
  
  // BATTERY 2 SECTION
  tlm_value = 0.0;
  tlm_check = 0.0;
  float battery_2_elapsed_time_factor = 0.0;

  // Charge Current
  // Sanity Check
  tlm_value = sanity_processing(telemetry_data.battery_2_charge_current, telemetry_data.battery_2_charge_current, 4);

  if(parameters.battery_2_current_tlm_valid_flag == true)
  {
    // Get the elapsed time factor
    battery_2_elapsed_time_factor = MS_IN_SEC /  parameters.battery_2_charge_current_read_elapsed_time;

    // Reset elapsed charge current time
    parameters.battery_2_charge_current_read_elapsed_time = 0;

    if(tlm_value > 0)
    {
      parameters.battery_2_amphrs_charging += ((tlm_value / (battery_2_elapsed_time_factor * SECS_IN_HOUR)) / parameters.battery_2_recharge_ratio);
    }
    else if (tlm_value < 0)
    {
      parameters.battery_2_amphrs_discharging += (tlm_value / (battery_2_elapsed_time_factor * SECS_IN_HOUR));
    }

    tlm_check = parameters.battery_2_amphrs_charging - parameters.battery_2_amphrs_discharging;

    // NOTE: THIS FUNCTION IS EFFECTIVELY DISABLED
    // DISABLING THIS RESET DUE TO PRESENT HW DESIGN (LC: 9/8/16)
    if(tlm_check < parameters.battery_2_amphrs_init_threshold)
    {
      //Turn the power ON
      digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, LOW);
      parameters.battery_2_charging_status = true;
    }

    // NOTE: THIS FUNCTION IS EFFECTIVELY DISABLED
    // DISABLING THIS RESET DUE TO PRESENT HW DESIGN (LC: 9/8/16)
    if(tlm_check > parameters.battery_2_amphrs_term_threshold)
    {
      //Turn the power Off
      digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, HIGH);
      parameters.battery_2_charging_status = false;

      //Reset the charge counts
      //parameters.battery_2_amphrs_charging = 0.0;
      //parameters.battery_2_amphrs_discharging = 0.0;
    }
  }
  
  // SA Current AMP HR Calculation
  double sa_elapsed_time_factor = 0.0;
  sa_elapsed_time_factor = MS_IN_SEC /  parameters.sa_current_read_elapsed_time;
  parameters.sa_current_read_elapsed_time = 0;
  parameters.sa_amphrs += (parameters.sa_amphrs / (sa_elapsed_time_factor * SECS_IN_HOUR));

  // LOAD Current AMP HR Calculation
  double load_elapsed_time_factor = 0.0;
  load_elapsed_time_factor = MS_IN_SEC /  parameters.load_current_read_elapsed_time;
  parameters.load_current_read_elapsed_time = 0;
  parameters.load_amphrs += (parameters.load_amphrs / (load_elapsed_time_factor * SECS_IN_HOUR));
}

void execute_electrical_control_check()
{
  int valid_data = true;
  double tlm_value = 0.0;

  // Check if the Cut-Down Process has been initiated, and continue if so.
  cutdown_check();

  /////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// BATTERY BUS VOLTAGE CHECK ////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  // Check Battery Voltage
  // Get the highest voltage
  // Sanity Check

  tlm_value = sanity_processing(telemetry_data.busvoltage_batt1, telemetry_data.busvoltage_batt2, 3);

  if(parameters.battery_voltage_tlm_valid_flag == true)
  {
    //Compare voltage to loadshed entry threshold
    if(tlm_value < parameters.low_voltage_limit_for_loadshed_entry)
    {
	     if(parameters.battery_bus_low_voltage_flag == false)
       {
         //Battery voltage is low - set flag and mark time
	       parameters.battery_bus_low_voltage_flag = true;
         parameters.battery_low_voltage_elapsed_time = 0.0;
         if(debug.mode == 1) {
          Serial.print("Battery voltage of ");
          Serial.print(tlm_value);
          Serial.print("is below threshhold of ");
          Serial.print(parameters.low_voltage_limit_for_loadshed_entry);
          Serial.print(". Starting timer of maximum allowed low voltage time: ");
          Serial.print(parameters.low_voltage_time_limit);
          Serial.println(" sec");
         }
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
  }

  // Battery Failure Checking
  // Check if voltage flag is already set
  if(parameters.battery_bus_low_voltage_flag == true)
  {
    //Check if the timer has reached
    //the the low voltage time limit
    if (parameters.battery_low_voltage_elapsed_time >= parameters.low_voltage_time_limit)
    {
	     write_telemetry_data_to_sd();
	     if(debug.mode == 1) {
	      Serial.print("Battery voltage (currently ");
	      Serial.print(tlm_value);
        Serial.print(") has been below threshhold (");
        Serial.print(parameters.low_voltage_limit_for_loadshed_entry);
        Serial.print(") for low voltage time limit of ");
        Serial.print(parameters.low_voltage_time_limit);
        Serial.println(" sec.  Going into Emergency Descent Mode.");
	     }

      //Enter Emergency Descent Mode
      set_emergency_decent_mode();
    }
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////// ALTITUDE CHECK //////////////??????//////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  // Check Altitude and Cutdown if we are too low

  int tlm_value_1 = sanity_processing(gps_data.gps_altitude, gps_data.gps_altitude, 6);
  int tlm_value_2 = sanity_processing(alt.altitude_in_meters, alt.altitude_in_meters, 7);
  
  if(!(parameters.vehicle_mode == TRANSIT_MODE) && !(parameters.vehicle_mode == EMERGENCY_DESCENT_MODE))
  {
    if((parameters.gps_alt_valid_flag == true) && (gps_data.gps_altitude_valid == 1))
    {
      if(debug.mode == 1) {
        Serial.println("WE are floating !!! Also - GPS Alt is good.  (test for altitude cutdown, line 419 in telemetry_module)");
      }
      if(gps_data.gps_altitude < parameters.altitude_limit_low)
      {
        count_low_alt = count_low_alt + 1; // counter: if counts 4 times below altitude thershold then cutdown
        write_telemetry_data_to_sd();
        
        if(count_low_alt > 3) // we have been too low for a few counts => initiate cut down
        {
          if(debug.mode == 1) {
            Serial.println("SHIT we are too low (GPS): initiate cutdown (test for altitude cutdown, line 423 in telemetry_module)");
          }
          //Enter Emergency Descent Mode
          set_emergency_decent_mode();
        }
      }
      else
      {
        count_low_alt = 0;
      }
    }  
    else if (parameters.altitude_valid_flag == true)
    {
      if(debug.mode == 1) {
        Serial.println("WE are floating !!! Also - GPS Alt is NOT good, but Altimeter is okay. (test for altitude cutdown, line 419 in telemetry_module)");
      }
      if(alt.altitude_in_meters < parameters.altitude_limit_low)
      {
        count_low_alt = count_low_alt + 1; // counter: if counts 4 times below altitude thershold then cutdown
        write_telemetry_data_to_sd();
        
        if(count_low_alt > 3) // we have been too low for a few counts => initiate cut down
        {
          if(debug.mode == 1) {
            Serial.println("SHIT we are too low (Alt): Initiate cutdown (test for altitude cutdown, line 423 in telemetry_module)");
          }
          //Enter Emergency Descent Mode
          set_emergency_decent_mode();
        }
      }
      else
      {
        count_low_alt = 0;
      }
    }
  } // end of test on altitude
} // end of process telemetry

/////////////////////////////////////////////////////////////////////////
////  This section includes code which prints to the Terminal output ////
/////////////////////////////////////////////////////////////////////////

void print_analog_data()
{
  Serial.println("-----------ANALOG Telemetry---------------");

  Serial.println("--------------------------------");
  Serial.print("Air Pressure: ");
  Serial.println(telemetry_data.air_pressure);
  Serial.print("Battery 1-1 Temp: ");
  Serial.println(telemetry_data.battery_1_temp_1);
  Serial.print("Battery 1-1 Temp RAW: ");
  Serial.println(raw_telemetry_data.raw_battery_1_temp_1);
  Serial.print("Battery 1-2 Temp: ");
  Serial.println(telemetry_data.battery_1_temp_2);
  Serial.print("Battery 1-2 Temp RAW: ");
  Serial.println(raw_telemetry_data.raw_battery_1_temp_2);
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
  Serial.print("Analog VIN Voltage: ");
  Serial.println(telemetry_data.analog_VIN_voltage);
  Serial.print("Analog VIN Voltage RAW: ");
  Serial.println(raw_telemetry_data.raw_analog_VIN_voltage);
}

void print_telemetry()
{
  Serial.println("----------------------------------------------------------");
  Serial.println("------------- PRINT ALL TERMINAL TELEMETRY ---------------");
  Serial.println("----------------------------------------------------------");
  Serial.println(" ");

  print_analog_data();
  print_battery_charge_current_data();
  print_low_rate_current_data();
  print_alt_data();
  print_gps_data();
  print_gyro_data();

  // TODO: ADD CODE FOR CALCULATED SOFTWARE VARIABLE THAT WE WANT TO OUTPUT !!!!

  Serial.println("");
  Serial.println("");
  Serial.println("");
}


void print_cutdown_telemetry()
{
  Serial.println(" ");
  Serial.println(" ");
  Serial.print("test_count: ");
  Serial.println(parameters.test_count);
  Serial.print("cutdown_event_flag: ");
  Serial.println(parameters.cutdown_event_flag);
  Serial.print("cutdown_1_status: ");
  Serial.println(parameters.cutdown_1_status);
  Serial.print("cutdown_2_status: ");
  Serial.println(parameters.cutdown_2_status);
  Serial.print("cutdown_initiation_elapsed_time: ");
  Serial.println(parameters.cutdown_initiation_elapsed_time);
  Serial.println(" ");
  Serial.println(" ");
}


// Convert latitude and longitude to two (16 bit) unsigned integers.
// This encoding is accurate within 2m at the equator while reducing
// the position encoding to 4 bytes from 16 bytes on the Due
// errors
// 0    Success
// -1   Input out of bounds
// -2   NULL pointer passed for outparameters
int encode_lat_long(double lat, double lng, unsigned int *newLat, unsigned int *newLong)
{
  if (lat < -90 || lat > 90 || lng < -180 || lng > 180) return -1;
  if (!newLat || !newLong) return -2;

  unsigned int maxEncodedInt = (1 << 16) - 1;

  *newLat = (lat + 90) / 180 * maxEncodedInt;
  *newLat = constrain(*newLat, 0, maxEncodedInt);

  *newLong = (lng + 180) / 360 * maxEncodedInt;
  *newLong = constrain(*newLong, 0, maxEncodedInt);

  return 0;
}

// Convert latitude and longitude from the compressed 2 16-bit unsigned int format into the
// familiar decimal form.
// Error:
// 0    Success
// -1   Encoded input out of bounds
// -2   NULL pointer passed for outparams
int decode_lat_long(double *lat, double *lng, unsigned int encLat, unsigned int encLong)
{
  int maxEncodedInt = (1 << 16) - 1;
  if (encLat > maxEncodedInt || encLong > maxEncodedInt) return -1;
  if (!lat || !lng) return -2;

  *lat = ((encLat * 180.0) / maxEncodedInt) - 90;
  *lat = constrain(*lat, -90, 90);

  *lng = ((encLong * 360.0) / maxEncodedInt) - 180;
  *lng = constrain(*lng, -180, 180);

  return 0;
}

void post_process_gyro()
{
  if (gyro.count_between_RB==0) { 
    gyro.sum_ax=0;
    gyro.mean_ax=0;
    gyro.max_ax=0;
    gyro.min_ax=0;
    gyro.sum_ay=0;
    gyro.mean_ay=0;
    gyro.max_ay=0;
    gyro.min_ay=0;
    gyro.sum_az=0;
    gyro.mean_az=0;
    gyro.max_az=0;
    gyro.min_az=0;  
    gyro.sum_gx=0;
    gyro.mean_gx=0;
    gyro.max_gx=0;
    gyro.min_gx=0;  
    gyro.sum_gy=0;
    gyro.mean_gy=0;
    gyro.max_gy=0;
    gyro.min_gy=0;  
    gyro.sum_gz=0;
    gyro.mean_gz=0;
    gyro.max_gz=0;
    gyro.min_gz=0; 
  }  
  gyro.count_between_RB=gyro.count_between_RB+1;
  
  gyro.sum_ax=gyro.sum_ax+gyro.ax;
  gyro.sum_ay=gyro.sum_ay+gyro.ay;
  gyro.sum_az=gyro.sum_az+gyro.az;
  gyro.sum_gx=gyro.sum_gx+gyro.gx;
  gyro.sum_gy=gyro.sum_gy+gyro.gy;
  gyro.sum_gz=gyro.sum_gz+gyro.gz;
  
  gyro.mean_ax=gyro.sum_ax/gyro.count_between_RB;
  gyro.mean_ay=gyro.sum_ay/gyro.count_between_RB;
  gyro.mean_az=gyro.sum_az/gyro.count_between_RB;
  gyro.mean_gx=gyro.sum_gx/gyro.count_between_RB;
  gyro.mean_gy=gyro.sum_gy/gyro.count_between_RB;
  gyro.mean_gz=gyro.sum_gz/gyro.count_between_RB;

  if(gyro.ax > gyro.max_ax) { gyro.max_ax = gyro.ax; } 
  if(gyro.ay > gyro.max_ay) { gyro.max_ay = gyro.ay; } 
  if(gyro.az > gyro.max_az) { gyro.max_az = gyro.az; } 
  if(gyro.gx > gyro.max_gx) { gyro.max_gx = gyro.gx; } 
  if(gyro.gy > gyro.max_gy) { gyro.max_gy = gyro.gy; } 
  if(gyro.gz > gyro.max_gz) { gyro.max_gz = gyro.gz; } 
  
  if(gyro.ax < gyro.min_ax) { gyro.min_ax = gyro.ax; } 
  if(gyro.ay < gyro.min_ay) { gyro.min_ay = gyro.ay; } 
  if(gyro.az < gyro.min_az) { gyro.min_az = gyro.az; } 
  if(gyro.gx < gyro.min_gx) { gyro.min_gx = gyro.gx; } 
  if(gyro.gy < gyro.min_gy) { gyro.min_gy = gyro.gy; } 
  if(gyro.gz < gyro.min_gz) { gyro.min_gz = gyro.gz; } 
 
}

void post_process_gps_alt()
{
  if (gps_data.count_between_RB==0) { 
    gps_data.max_gps_altitude=0;
    gps_data.min_gps_altitude=0;
  }  
  gps_data.count_between_RB=gps_data.count_between_RB+1;

  if(gps_data.gps_altitude > gps_data.max_gps_altitude) { gps_data.max_gps_altitude = gps_data.gps_altitude; }
  if(gps_data.gps_altitude < gps_data.min_gps_altitude) { gps_data.min_gps_altitude = gps_data.gps_altitude; }
  gps_data.avg_gps_altitude = (gps_data.avg_gps_altitude*(gps_data.count_between_RB-1)+gps_data.avg_gps_altitude)/gps_data.count_between_RB;
}

void post_process_alt_data()
{
  if (alt.count_between_RB==0) { 
    alt.max_altitude_in_meters=0;
    alt.min_altitude_in_meters=0;
    alt.max_pressure=0;
    alt.min_pressure=0;
  }  
  alt.count_between_RB=alt.count_between_RB+1;

  if(alt.altitude_in_meters > alt.max_altitude_in_meters) { alt.max_altitude_in_meters = alt.altitude_in_meters; } 
  if(alt.altitude_in_meters < alt.min_altitude_in_meters) { alt.min_altitude_in_meters = alt.altitude_in_meters; } 
  if(alt.pressure > alt.max_pressure) { alt.max_pressure = alt.pressure; }
  if(alt.pressure < alt.min_pressure) { alt.min_pressure = alt.pressure; }
}

