#include "Arduino.h"
#include <string>

void process_satellite_data()
{
  // THIS NEEDS TO BE TIED INTO THE ROCKBLOCK MODULE
  String CommandString = "PlaceholderStringCommand";
  
  // Allow the Commands through if it matches the intended vehicle and code version
  if (CommandString.substring(0,5) == "030133") {
    // INCREMENT VALID COMMAND RECIEVED
    // ***************************************************
    // PUT LOGIC IN HERE TO SEE IF THE PARITY IS CORRECT
    // ***************************************************
    
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
           digitalWrite(PIN_POWER_SHUTDOWN, HIGH);
         }
         if (CommandString.substring(14,15) == "0") {
           // Enable Battery Charding 
           digitalWrite(PIN_POWER_SHUTDOWN, LOW);
         }
      }
      
      if (CommandString.substring(6,13) == "25330004") {
        // This is a command change the Camera Enable Status
         if (CommandString.substring(14,15) == "F") {
         // Enable the Camera
         parameters.camera_flag == true;
         }
         if (CommandString.substring(14,15) == "0") {
         // Disable the Camera
         parameters.camera_flag == false;
         }
      } 
      
       if (CommandString.substring(6,13) == "23330008") {
        // This is a command to Fire the Pyro and Initiate Descent
         if (CommandString.substring(14,15) == "F") {
            // Enable Pyro Relay, Fire Pyros, and disable Pyro Relay
            set_transit_mode(); 
            // This will also initiate transition to Transit Mode
            pyro_fire();
          }
       }
       if (CommandString.substring(6,13) == "23330010") {
        // This is a command set Pyro Fire Pulse Width
        parameters.pyro_pulse_width = CommandString.substring(14,15);
       }
       if (CommandString.substring(6,13) == "24330020") {
        // This is a command set the SD Card Write Period
        sd_card_write_rate = CommandString.substring(14,15);
        }
       if (CommandString.substring(6,13) == "45330040") {
        // This is a command set the Camera Write Period & Camera On Time
        parameters.camera_period = CommandString.substring(14,17);
        parameters.camera_on_time = CommandString.substring(18,21);
        }
       if (CommandString.substring(6,13) == "46330080") {
        // This is a command to set the Transmit Rates Permode
        
        // Normal Ops Transmit Rate = CommandString.substring(14,17)
        thresholds.normal_transmit_rate = CommandString.substring(14,17).toInt();
        
        // Loadshed Transmit Rate = CommandString.substring(18,21)
        thresholds.load_shed_transmit_rate = CommandString.substring(18,21).toInt();
        
        // Transit Transmit Rate = CommandString.substring(22,25)
        thresholds.transit_transmit_rate = CommandString.substring(22,25).toInt();
        
        // Emergency Transit Transmit Rate = CommandString.substring(26,29)
        thresholds.emergency_transit_transmit_rate =  CommandString.substring(26,29).toInt();
        
        // Test Transmit Rate = CommandString.substring(30,33)
        thresholds.test_transmit_rate = CommandString.substring(30,33).toInt();
        
        }
       if (CommandString.substring(6,13) == "42330100") {
        // This is a command to set the TCS Thresholds
        
        // Sanity Check High Temp Threshold  = CommandString.substring(14,15)
        parameters.battery_temperature_sanity_check_high = CommandString.substring(14,15).toInt();
        
        // Normal OPS High Temp Threshold  = CommandString.substring(16,17)
        thresholds.normal_battery_temperature_limit_high = CommandString.substring(16,17).toInt();
        
        // Normal OPS Low Temp Threshold  = CommandString.substring(18,19)
        thresholds.normal_battery_temperature_limit_low = CommandString.substring(18,19).toInt();
        
        // Loadshed High Temp Threshold = CommandString.substring(20,21)
        thresholds.survival_battery_temperature_limit_high = CommandString.substring(20,21).toInt();
        
        // Loadshed High Temp Threshold = CommandString.substring(22,23)
        thresholds.survival_battery_temperature_limit_low = CommandString.substring(22,23).toInt();
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(24,25)
        parameters.battery_temperature_sanity_check_low = CommandString.substring(24,25).toInt();
        }
       if (CommandString.substring(6,13) == "42330200") {
        // This is a command to set the Voltage Setpoints
        
        // Sanity Check High Voltage Threshold  = CommandString.substring(14,15)
        voltage_sanity_check_high = CommandString.substring(14,15);
        
        // Charge Termination Voltage Threshold  = CommandString.substring(16,17)
        voltage_power_limit_high = CommandString.substring(16,17);
        
        // Charge Inialization Voltage Threshold  = CommandString.substring(18,19)
        voltage_power_limit_low = CommandString.substring(18,19);
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(20,21)
        voltage_sanity_check_low = CommandString.substring(20,21);
        }
       if (CommandString.substring(6,13) == "42330400") {
        // This is a command to set the Amp-Hour Setpoints
        
        // Charge Termination Amp-Hour Threshold  = CommandString.substring(14,17)
        capacity_limit_high = CommandString.substring(14,17);
        // Charge Inialization Amp-Hour Threshold  = CommandString.substring(18,21)
        capacity_limit_low = CommandString.substring(18,21);
        // Sanity Check High Current Threshold  = CommandString.substring(22,25)
        charge_current_sanity_check_high = CommandString.substring(22,25);
        // Sanity Check Low Current Threshold  = CommandString.substring(26,29)
        charge_current_sanity_check_low = CommandString.substring(26,29);
       }
       if (CommandString.substring(6,13) == "42330800") {
        // This is a command to set the Recharge Ratio
        
        // Recharge Ratio  = CommandString.substring(14,15)
        recharge_ratio = CommandString.substring(14,15);
        }
       if (CommandString.substring(6,13) == "48331000") {
        // This is a command to set the Altitude Descent Trigger
        
        // Altitude Descent Trigger  = CommandString.substring(14,17)
        altitude_limit_low = CommandString.substring(14,17);
        
        // Low Sanity Check Altitude  = CommandString.substring(18,21)
        altitude_sanity_check_low = CommandString.substring(18,21);
        }
       if (CommandString.substring(6,13) == "48332000") {
        // This is a command to set the Voltage Descent Trigger
        // Voltage Descent Trigger  = CommandString.substring(14,15)
        low_voltage_limit =  = CommandString.substring(14,15);
        }
     
       if (CommandString.substring(6,13) == "48338000") {
        // This is a command to set the Length of time in Loadshed Mode until we trigger Emergency Descent
        // Time in Loadshed Trigger  = CommandString.substring(14,21)
         low_voltage_time_limit = CommandString.substring(14,21);
       }
 
       // ****************************************************************
       // Put something in here if none of the if statement loops trip???
       // ****************************************************************

    }// Parity Check is correct!
    else{
      // Increment an Error in some fashion regard to parity failure.  
    }
    
  } // INCREMENT VALID COMMAND RECIEVED
  else{
    // Increment an Error in regard to bad commands getting through.
  }
}

//Receive any data from satellite
int read_satellite_data()
{
  return NO_COMMANDS_TO_PROCESS;
}

void write_satellite_data()
{

}

