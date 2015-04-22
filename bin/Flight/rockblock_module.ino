#include "Arduino.h"
#include <string>
//#include <Streaming>

void process_satellite_data()
{
 
}

String combine(int bin_size, long input_data, String dataword)
{
    int zeros;
    String temp_str;
    temp_str = String(input_data,BIN);
    zeros = bin_size - temp_str.length();
 
    for (int i=0; i<zeros; i++) {
      temp_str = "0"+temp_str;
    }
    
    dataword = dataword + temp_str;
    
    return dataword;     
}

//Receive any data from satellite
int read_satellite_data()
{
   // THIS NEEDS TO BE TIED INTO THE ROCKBLOCK MODULE
  String CommandString = "PlaceholderStringCommand";
  
  
  // Allow the Commands through if it matches the intended vehicle and code version
  if (CommandString.substring(0,5) == "030133") {
    
    // INCREMENT VALID COMMAND RECIEVED COUNTER
    if (parameters.command_count < 255) { 
      parameters.command_count = parameters.command_count + 1;
    }
    else {
      parameters.command_count = 1;
    } 
    
    // ***************************************************
    // PUT LOGIC IN HERE TO SEE IF EXTRACT THE PARITY
    // ***************************************************
    
    //TODO: Fix parity to be 7 or 3
    
    if (CommandString.substring(538) == "7") {
      
      // ***************************************************
      // Parity Check is correct! *** This is TEMP -> Fix her
      // ***************************************************
      
      if (CommandString.substring(6,13) == "11330001") {
        // This is a command to change the Spacecraft Mode
         if (CommandString.substring(14,15) == "1") {
           // Set the Mode to Normal OPS
           set_normal_mode();
         }
         if (CommandString.substring(14,15) == "2") {
           // Set the Mode to Loadshed 
           set_load_shed_mode();
         }
         if (CommandString.substring(14,15) == "3") {
           // Set the Mode to Descent Mode
           set_transit_mode();
           //initiate pyro cutdown
         }
         if (CommandString.substring(14,15) == "4") {
           // Set the Mode to Descent Mode
           set_emergency_decent_mode();
         }
         if (CommandString.substring(14,15) == "5") {
           // Set the Mode to Test Mode
           set_test_mode();
         } 
      }// This ends the section to command to change the Spacecraft Mode
      
      if (CommandString.substring(6,13) == "22330002") {
        // This is a command to Change the EPS Charge State
        if (CommandString.substring(14,15) == "F") {
           // Disable Battery Charging 
           digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, HIGH);
         }
         if (CommandString.substring(14,15) == "0") {
           // Enable Battery Charding 
           digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
         }
      }
      
      //TODO: MAKE SURE WE ADD BATTERY 2 COMMANDS TO SHUT OFF Charging
      
      if (CommandString.substring(6,13) == "25330004") {
        // This is a command change the Camera Enable Status
         if (CommandString.substring(14,15) == "F") {
         // Enable the Camera
         parameters.camera_status == true;
         }
         if (CommandString.substring(14,15) == "0") {
         // Disable the Camera
         parameters.camera_status == false;
         }
      } 
      
       if (CommandString.substring(6,13) == "23330008") {
        // This is a command to Fire the Pyro and Initiate Descent
         if (CommandString.substring(14,15) == "F") {
            // Enable Pyro Relay, Fire Pyros, and disable Pyro Relay
            set_transit_mode(); 
            // This will also initiate transition to Transit Mode
            cutdown_fire();
          }
       }
       if (CommandString.substring(6,13) == "23330010") {
        // This is a command set Pyro Fire Pulse Width
        // Defined in msec
        parameters.cutdown_pulse_width = CommandString.substring(14,17).toInt();
  
       }
       if (CommandString.substring(6,13) == "24330020") {
        // This is a command set the SD Card Write Period
        // Converted from sec to msec
        parameters.sd_card_write_rate = CommandString.substring(14,17).toInt() * 1000;
        }
        
       if (CommandString.substring(6,13) == "45330040") {
        // This is a command set the Camera Write Period & Camera On Time
        // Converted from sec to msec
        parameters.camera_period = CommandString.substring(14,17).toInt() * 1000;
        parameters.camera_on_time = CommandString.substring(18,21).toInt() * 1000;
        }
       if (CommandString.substring(6,13) == "46330080") {
        // This is a command to set the Transmit Rates Permode
        // Converted from min to msec
        
        // Normal Ops Transmit Rate = CommandString.substring(14,17)
        thresholds.normal_transmit_rate = CommandString.substring(14,17).toInt() * 3600 * 1000;
        
        // Loadshed Transmit Rate = CommandString.substring(18,21)
        thresholds.load_shed_transmit_rate = CommandString.substring(18,21).toInt() * 3600 * 1000;
        
        // Transit Transmit Rate = CommandString.substring(22,25)
        thresholds.transit_transmit_rate = CommandString.substring(22,25).toInt() * 3600 * 1000;
        
        // Emergency Transit Transmit Rate = CommandString.substring(26,29)
        thresholds.emergency_transit_transmit_rate =  CommandString.substring(26,29).toInt() * 3600 * 1000;
        
        // Test Transmit Rate = CommandString.substring(30,33)
        thresholds.test_transmit_rate = CommandString.substring(30,33).toInt() * 3600 * 1000;
        
        }
       if (CommandString.substring(6,13) == "42330100") {
        // This is a command to set the TCS Thresholds
        // Converted from Kelvin to Celcius
        
        // Sanity Check High Temp Threshold  = CommandString.substring(14,15)
        parameters.battery_temperature_sanity_check_high = CommandString.substring(14,15).toInt() - 273;
        
        // Normal OPS High Temp Threshold  = CommandString.substring(16,17)
        thresholds.normal_battery_temperature_limit_high = CommandString.substring(16,17).toInt() - 273;
        
        // Normal OPS Low Temp Threshold  = CommandString.substring(18,19)
        thresholds.normal_battery_temperature_limit_low = CommandString.substring(18,19).toInt() - 273;
        
        // Loadshed High Temp Threshold = CommandString.substring(20,21)
        thresholds.survival_battery_temperature_limit_high = CommandString.substring(20,21).toInt() - 273;
        
        // Loadshed High Temp Threshold = CommandString.substring(22,23)
        thresholds.survival_battery_temperature_limit_low = CommandString.substring(22,23).toInt() - 273;
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(24,25)
        parameters.battery_temperature_sanity_check_low = CommandString.substring(24,25).toInt() - 273;
        }
        
       if (CommandString.substring(6,13) == "42330200") {
        // This is a command to set the Voltage Setpoints
        // Converted from dVolts to Volts
        
        // Sanity Check High Voltage Threshold  = CommandString.substring(14,15)
        parameters.voltage_sanity_check_high = CommandString.substring(14,15).toInt() / 10;
        
        // Battery 1 Charge Termination Voltage Threshold  = CommandString.substring(16,17)
        parameters.battery_1_voltage_term_threshold = CommandString.substring(16,17).toInt() / 10;
        
        // Battery 1 Charge Inialization Voltage Threshold  = CommandString.substring(18,19)
        parameters.battery_1_voltage_init_threshold = CommandString.substring(18,19).toInt() / 10;
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(20,21)
        parameters.voltage_sanity_check_low = CommandString.substring(20,21).toInt() / 10;
        }
       if (CommandString.substring(6,13) == "42330400") {
        // This is a command to set the Amp-Hour Setpoints
        // Conversion from mAmp-Hrs to Amp-Hrs
        
        // Battery 1 Charge Termination Amp-Hour Threshold  = CommandString.substring(14,17)
        parameters.battery_1_amphrs_term_threshold = CommandString.substring(14,17).toInt() / 1000;
        // Battery 1 Charge Inialization Amp-Hour Threshold  = CommandString.substring(18,21)
        parameters.battery_1_amphrs_init_threshold = CommandString.substring(18,21).toInt() / 1000;
        
        // Conversion from cAmp to Amp
        // Sanity Check High Current Threshold  = CommandString.substring(22,25)
        parameters.charge_current_sanity_check_high = CommandString.substring(22,25).toInt() / 100;
        // Sanity Check Low Current Threshold  = CommandString.substring(26,29)
        parameters.charge_current_sanity_check_low = CommandString.substring(26,29).toInt() * -1 / 100;
       }
       if (CommandString.substring(6,13) == "42330800") {
        // This is a command to set the Recharge Ratio
        // Convert percentage to scale multiplication factor
        // Recharge Ratio  = CommandString.substring(14,15)
        parameters.battery_1_recharge_ratio = CommandString.substring(14,15).toInt() / 100;
        }
        
        //TODO: ADD COMMAND PARAMETERS TO CHANGE BATTERY 2 RECHARGE RATIO!!!
        
       if (CommandString.substring(6,13) == "48331000") {
        // This is a command to set the Altitude Descent Trigger
        // Defined in meters
        
        // Altitude Descent Trigger  = CommandString.substring(14,17)
        parameters.altitude_limit_low = CommandString.substring(14,17).toInt();
        
        // Low Sanity Check Altitude  = CommandString.substring(18,21)
        parameters.altitude_sanity_check_low = CommandString.substring(18,21).toInt();
        }
        
       if (CommandString.substring(6,13) == "48332000") {
        // This is a command to set the Voltage Descent Trigger
        // Voltage Descent Trigger  = CommandString.substring(14,15)
        // Converted from dVolts to Volts
        
        parameters.low_voltage_limit_for_auto_cutdown = CommandString.substring(14,15).toInt() / 10;
        }

//TODO: ADD COMMAND FOR low_voltage_limit_for_loadshed_entry

       if (CommandString.substring(6,13) == "48338000") {
        // This is a command to set the Length of time in Loadshed Mode until we trigger Emergency Descent
        // Time in Loadshed Trigger  = CommandString.substring(14,21)
        // Convert seconds to milliseconds
         parameters.low_voltage_time_limit = CommandString.substring(14,17).toInt() * 1000;
       }
 
       // ****************************************************************
       // Put something in here if none of the if statement loops trip???
       // ****************************************************************

    } // End Parity Check conditional
    else{
      // Increment an Error in some fashion regard to parity failure.  
    }
    
  } // End check if we recieved a VALID COMMAND conditional
  
  else{
    // Increment an Error in regard to bad commands getting through.
  }
  
  // if something 
  //return NO_COMMANDS_TO_PROCESS;
  //
  
}

void write_satellite_data()
{
  // Process the Normal OPS Transmit Format
  if (parameters.vehicle_mode < 6) {
    
    // Create intial word (+ header) to send to the ground
    String valid_str;
    valid_str = "0";
    
    parameters.output_dataword = combine(3, parameters.vehicle_mode, "A30133");
    parameters.output_dataword = combine(8, parameters.command_count, parameters.output_dataword);
    parameters.output_dataword = combine(8, thresholds.normal_transmit_rate, parameters.output_dataword);
    parameters.output_dataword = combine(8, thresholds.load_shed_transmit_rate, parameters.output_dataword);
    parameters.output_dataword = combine(8, thresholds.transit_transmit_rate, parameters.output_dataword);
    parameters.output_dataword = combine(8, thresholds.emergency_transit_transmit_rate, parameters.output_dataword);
    parameters.output_dataword = combine(8, thresholds.test_transmit_rate, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.sd_card_write_rate, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.battery_temp_1_1, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.battery_temp_1_2, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.battery_temp_2_1, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.battery_temp_2_2, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.inner_external_temp, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.outter_external_temp, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.internal_temp, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.air_pressure, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.battery_voltage_1, parameters.output_dataword);
    parameters.output_dataword = combine(12, telemetry_data.battery_voltage_2, parameters.output_dataword);
    parameters.output_dataword = combine(13, telemetry_data.charge_current_1, parameters.output_dataword);
    parameters.output_dataword = combine(13, telemetry_data.charge_current_2, parameters.output_dataword);
    //parameters.output_dataword = combine(64, gps.location.lat(), parameters.output_dataword);
    //parameters.output_dataword = combine(64, gps.location.lng(), parameters.output_dataword);
    //parameters.output_dataword = combine(64, gps.altitude.meters(), parameters.output_dataword);
    //parameters.output_dataword = combine(32, gps.location.age(), parameters.output_dataword);
    //parameters.output_dataword = combine(32, gps.altitude.age(), parameters.output_dataword);
    //parameters.output_dataword = combine(64, gps.course.deg(), parameters.output_dataword);
    //parameters.output_dataword = combine(64, gps.speed.kmph(), parameters.output_dataword);
    parameters.output_dataword = combine(4, gps.satellites.value(), parameters.output_dataword);
    //parameters.output_dataword = combine(32, gps.date, parameters.output_dataword);
    //parameters.output_dataword = combine(32, gps.time, parameters.output_dataword);
    //parameters.output_dataword = combine(32, gps.hdop.value(), parameters.output_dataword);
    parameters.output_dataword = combine(32, gps.charsProcessed(), parameters.output_dataword);
    parameters.output_dataword = combine(32, gps.sentencesWithFix(), parameters.output_dataword);
    parameters.output_dataword = combine(32, gps.failedChecksum(), parameters.output_dataword);
   
    valid_str = String(gps.hdop.isValid());   
    valid_str = valid_str + String(gps.location.isValid());
    valid_str = valid_str + String(gps.altitude.isValid());
    valid_str = valid_str + String(gps.course.isValid());
    valid_str = valid_str + String(gps.speed.isValid());
    valid_str = valid_str + String(gps.satellites.isValid());
    valid_str = valid_str + String(gps.date.isValid());
    valid_str = valid_str + String(gps.time.isValid());
    valid_str = valid_str + String(gps.hdop.isValid());
    //valid_str = valid_str + String(gps.charsProcessed.isValid());
    //valid_str = valid_str + String(gps.sentencesWithFix.isValid());
    //valid_str = valid_str + String(gps.failedChecksum.isValid());
   
    //parameters.output_dataword = combine(16, parameters.valid_str, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.accelMinX, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.accelMaxX, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.accelMinY, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.accelMaxY;, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.accelMinZ, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.accelMaxZ, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.magMinX, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.magMaxX, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.magMinY, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.magMaxY, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.magMinZ, parameters.output_dataword);
    //parameters.output_dataword = combine(32, calData.magMaxZ, parameters.output_dataword);
    //parameters.output_dataword = combine(32, dueMPU.m_rawQuaternion, parameters.output_dataword);
    //parameters.output_dataword = combine(32, dueMPU.m_dmpEulerPose, parameters.output_dataword);
    //parameters.output_dataword = combine(32, dueMPU.m_fusedEulerPose, parameters.output_dataword);
    
    // TODO: ADD GYRO TEMP TELEMETRY
    
    parameters.output_dataword = combine(1, parameters.battery_1_charging_status, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.battery_2_charging_status, parameters.output_dataword);
    
    parameters.output_dataword = combine(32, parameters.battery_1_recharge_ratio, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.charge_current_sanity_check_high, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.charge_current_sanity_check_low, parameters.output_dataword);
    parameters.output_dataword = combine(32, parameters.battery_1_amphrs_charging, parameters.output_dataword);
    parameters.output_dataword = combine(32, parameters.battery_1_amphrs_discharging, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.battery_1_amphrs_term_threshold, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.battery_1_amphrs_init_threshold, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.battery_1_voltage_term_threshold, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.battery_1_voltage_init_threshold, parameters.output_dataword);
    
    parameters.output_dataword = combine(8, parameters.low_voltage_limit_for_auto_cutdown, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.voltage_sanity_check_low, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.low_voltage_time_limit, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.battery_bus_low_voltage_flag, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.heater_state_1, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.heater_state_2, parameters.output_dataword);
    parameters.output_dataword = combine(16, parameters.battery_temperature_limit_high, parameters.output_dataword);
    parameters.output_dataword = combine(16, parameters.battery_temperature_limit_low, parameters.output_dataword);
    parameters.output_dataword = combine(16, parameters.battery_temperature_sanity_check_high, parameters.output_dataword);
    parameters.output_dataword = combine(16, thresholds.normal_battery_temperature_limit_high, parameters.output_dataword);
    parameters.output_dataword = combine(16, thresholds.normal_battery_temperature_limit_low, parameters.output_dataword);
    parameters.output_dataword = combine(16, thresholds.survival_battery_temperature_limit_high, parameters.output_dataword);
    parameters.output_dataword = combine(16, thresholds.survival_battery_temperature_limit_low, parameters.output_dataword);
    parameters.output_dataword = combine(16, parameters.battery_temperature_sanity_check_low, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.cutdown_enable_state, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.cutdown_1_status, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.cutdown_2_status, parameters.output_dataword);
    parameters.output_dataword = combine(8, parameters.cutdown_pulse_width, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.camera_status, parameters.output_dataword);
    parameters.output_dataword = combine(12, parameters.camera_period, parameters.output_dataword);
    parameters.output_dataword = combine(12, parameters.camera_on_time, parameters.output_dataword);
    parameters.output_dataword = combine(1, parameters.altitude_valid_flag, parameters.output_dataword);
    parameters.output_dataword = combine(16, parameters.altitude_limit_low, parameters.output_dataword);
    parameters.output_dataword = combine(16, parameters.altitude_sanity_check_low, parameters.output_dataword);
 
 
 
  }
}

//  // Process the Loadshed Transmit Format
//  if (parameters.vehicle_mode == 2){
//       
//  }
//  // Process the Transit Transmit Format
//  if (parameters.vehicle_mode == 3){   
//    
//  }
//  // Process the Emergency Transit Transmit Format
//  if (parameters.vehicle_mode == 4){     
//  }
//  // Process the Test Transmit Format
//  if (parameters.vehicle_mode == 5){  
//    
//  }
//}

//public String combine(int bin_size, int input_data, String dataword)


