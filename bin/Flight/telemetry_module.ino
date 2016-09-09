
double raw_val;
double actual_val;
float gyro_sum_ax;
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
      digitalWrite(PIN_CUTDOWN_ENABLE, LOW);
      parameters.cutdown_enable_state = false;
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
  else
  {
    if((TLM2 <= high_thresh) &&
       (TLM2 >= low_thresh))
    {
      tlm_out = TLM2;
    }
    else
    {
      write_telemetry_data_to_sd();
      
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
    }
  }
  return tlm_out;
}

void evaluate_heater_state(double val, int batt_ID)
{
  if(val < parameters.battery_temperature_limit_low * 1.0)
  {
    //Turn heating element on
    if((batt_ID == 1) || (parameters.heater_state_1 == false))
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
    else if((batt_ID == 2) || (parameters.heater_state_2 == false))
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
    if((batt_ID == 1) || (parameters.heater_state_1 == true))
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
    else if((batt_ID == 2) || (parameters.heater_state_2 == true))
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
  double battery_1_elapsed_time_factor = 0.0;
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

    if(tlm_check < parameters.battery_1_amphrs_init_threshold)
    {
      //Turn the power ON
      digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
      parameters.battery_1_charging_status = true;
    }

    if(tlm_check > parameters.battery_1_amphrs_term_threshold)
    {
      //Turn the power Off
      digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, HIGH);
      parameters.battery_1_charging_status = false;

      //Reset the charge counts
      //NOTE: DISABLING THIS RESET DUE TO PRESENT HW DESIGN (LC: 9/8/16)
      //parameters.battery_1_amphrs_charging = 0.0;
      //parameters.battery_1_amphrs_discharging = 0.0;
    }
  }
  // BATTERY 2 SECTION
  tlm_value = 0.0;
  tlm_check = 0.0;
  double battery_2_elapsed_time_factor = 0.0;

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

    if(tlm_check < parameters.battery_2_amphrs_init_threshold)
    {
      //Turn the power ON
      digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, LOW);
      parameters.battery_2_charging_status = true;
    }

    if(tlm_check > parameters.battery_2_amphrs_term_threshold)
    {
      //Turn the power Off
      digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, HIGH);
      parameters.battery_2_charging_status = false;

      //Reset the charge counts
      //NOTE: DISABLING THIS RESET DUE TO PRESENT HW DESIGN (LC: 9/8/16)
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

  //Check Battery Voltage
  //Get the highest voltage
  //Sanity Check

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
         parameters.battery_low_voltage_elapsed_time = 0;
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

    //TODO: THE FOLLOWING CODE WILL CHANGE WHEN WE HAVE ONLY ONE MONITOR PER BATTERY

    // Battery Voltage Charge Control
    if(tlm_value < parameters.battery_1_voltage_init_threshold)
    {
      //Turn the power on
      digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
      parameters.battery_1_charging_status = true;
    }

    if(tlm_value > parameters.battery_1_voltage_term_threshold)
    {
      //Turn the power off
      digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, HIGH);
      parameters.battery_1_charging_status = false;

      //Reset the charge counts
      parameters.battery_1_amphrs_charging = 0.0;
      parameters.battery_1_amphrs_discharging = 0.0;
    }
    //TODO: DUPLICATE TO ADD BATTERY 2 CODE!!!!
  }

  //TODO: NEEDS TO BE CODE THAT SEPARATES LOADSHED ENTRY FROM AUTO-CUTDOWN

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

  // Check Altitude.
  // TODO: ADD CODE THAT IMPLEMENTS THE AUTOMATIC CUTDOWN IF THE BATTERY IS TOO LOW !

  // other strategy: 1) we enable the test if the paypload pass a resonnable altitude thershold A1
  //                 2) cut down if the payload goes below a low alt A2   A1>A2
  //                 A1 = altitude_sanity_check_low and A2 = altitude_limit_low

  if(alt.altitude_in_feet >= parameters.altitude_sanity_check_low)
  {
    parameters.altitude_valid_flag = true;
    count_low_alt = 0 ; // counter: if counts 3 times below altitude thershold then cutdow
  }

  if(parameters.altitude_valid_flag == true)  // we are now flying
  {
    if(debug.mode == 1) {
      Serial.println("WE are floating !!! (test for altitude cutdow, line 419 in telemetry_module)");
    }
    if(alt.altitude_in_feet < parameters.altitude_limit_low) // if we are too low then start counting ... to 3
    {
      count_low_alt = count_low_alt + 1;
      write_telemetry_data_to_sd();
    }

    if(count_low_alt == 3 ) // we have been too low for a few counts => initiate cut down
    {
      //Enter Emergency Descent Mode
      parameters.altitude_valid_flag = false;
      write_telemetry_data_to_sd();
      if(debug.mode == 1) {
      Serial.println("SHIT we are too low: initiate cutdown (test for altitude cutdown, line 423 in telemetry_module)");
      }
      set_emergency_decent_mode();
      
    }

    // //The next section is commented as we will only rely on the altimeter for the critical altitude test.
    //     if(gps.altitude.isValid())
    //     {
    //        if(gps.altitude.meters() >= parameters.altitude_sanity_check_low)
    //        {
    //           if(gps.altitude.meters() < parameters.altitude_limit_low)
    //           {
    //              //Enter Emergency Descent Mode
    //             set_emergency_decent_mode();
    //             parameters.altitude_valid_flag = false;
    //           }
    //        }
    //      }
    //      if(alt.altitude_in_feet >= parameters.altitude_sanity_check_low)
    //       {
    //         if(alt.altitude_in_feet < parameters.altitude_limit_low)
    //           {
    //             //Enter Emergency Descent Mode
    //             set_emergency_decent_mode();
    //             parameters.altitude_valid_flag = false;
    //           }
    //       }
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
  Serial.print("cutdown_enable_state: ");
  Serial.println(parameters.cutdown_enable_state);
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




void Post_process_gyro()
{
if (gyro.count_between_RB==0)
  {
  gyro_sum_ax=0;  
  }  
  gyro_sum_ax=gyro_sum_ax+gyro.ax;
  gyro.count_between_RB=gyro.count_between_RB+1;
  gyro.mean_ax=gyro_sum_ax/gyro.count_between_RB;
}
