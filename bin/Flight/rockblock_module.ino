#include "Arduino.h"

void process_satellite_data()
{
  // THIS NEEDS TO BE TIED INTO THE ROCKBLOCK MODULE
  String CommandString = "PlaceholderStringCommand";
  
  // Allow the Commands through if it matches the intended vehicle and code version
  if (CommandString.substring(0,5) == "030133") {
    // INCREMENT VALID COMMAND RECIEVED
    
    // PUT LOGIC IN HERE TO SEE IF THE PARITY IS CORRECT
    
    if (CommandString.substring(538) == "7") {
      // Parity Check is correct! *** This is TEMP -> Fix her
      
      if (CommandString.substring(6,13) == "11330001") {
        // This is a command to change the Spacecraft Mode
         if (CommandString.substring(14,15) == "1") {
           // Set the Mode to Normal OPS 
         }
         if (CommandString.substring(14,15) == "2") {
           // Set the Mode to Loadshed 
         }
         if (CommandString.substring(14,15) == "3") {
           // Set the Mode to Descent Mode
         }
         if (CommandString.substring(14,15) == "5") {
           // Set the Mode to Test Mode
         } 
      }// This is a command to change the Spacecraft Mode
      
      if (CommandString.substring(6,13) == "22330002") {
        // This is a command to Change the EPS Charge State
        if (CommandString.substring(14,15) == "F") {
           // Disable Battery Charging 
         }
         if (CommandString.substring(14,15) == "0") {
           // Enable Battery Charding 
         }
         
       if (CommandString.substring(6,13) == "23330004") {
        // This is a command to Change the Pyro Enable Relay State
        if (CommandString.substring(14,15) == "F") {
           // Enable Pyro Relay
         }
         if (CommandString.substring(14,15) == "0") {
           // Disable Pyro Relay
         }
         
       if (CommandString.substring(6,13) == "23330008") {
        // This is a command to Fire the Pyro and Initiate Descent
        if (CommandString.substring(14,15) == "1") {
           // Fire Pyro #1
         }
         if (CommandString.substring(14,15) == "2") {
           // Fire Pyro #1
         }
       if (CommandString.substring(6,13) == "23330010") {
        // This is a command set Pyro Fire Pulse Width
        // Pulse Width = CommandString.substring(14,15)
        
       if (CommandString.substring(6,13) == "24330020") {
        // This is a command set the SD Card Write Period
        // SD Card Write Period = CommandString.substring(14,15)
        }
       if (CommandString.substring(6,13) == "45330040") {
        // This is a command set the Camera Write Period
        // Camera Write Period = CommandString.substring(14,16)
        }
       if (CommandString.substring(6,13) == "46330080") {
        // This is a command to set the Transmit Rates Permode
        
        // Normal Ops Transmit Rate = CommandString.substring(14,17)
        normal_transmit_rate = CommandString.substring(14,17);
        
        // Loadshed Transmit Rate = CommandString.substring(18,21)
        load_shed_transmit_rate = CommandString.substring(18,21);
        
        // Transit Transmit Rate = CommandString.substring(22,25)
        transit_transmit_rate = CommandString.substring(22,25);
        
        // Emergency Transit Transmit Rate = CommandString.substring(26,29)
        emergency_transit_transmit_rate =  CommandString.substring(26,29);
        
        // Test Transmit Rate = CommandString.substring(30,33)
        test_transmit_rate = CommandString.substring(30,33);
        
        }
       if (CommandString.substring(6,13) == "42330100") {
        // This is a command to set the TCS Thresholds
        
        // Sanity Check High Temp Threshold  = CommandString.substring(14,15)
        
        // Normal OPS High Temp Threshold  = CommandString.substring(16,17)
        normal_battery_temperature_limit_high = CommandString.substring(16,17);
        
        // Normal OPS Low Temp Threshold  = CommandString.substring(18,19)
        normal_battery_temperature_limit_low = CommandString.substring(18,19);
        
        // Loadshed High Temp Threshold = CommandString.substring(20,21)
        survival_battery_temperature_limit_high = CommandString.substring(20,21);
        
        // Loadshed High Temp Threshold = CommandString.substring(22,23)
        survival_battery_temperature_limit_low = CommandString.substring(22,23);
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(24,25)
        }
       if (CommandString.substring(6,13) == "42330200") {
        // This is a command to set the Voltage Setpoints
        
        // Sanity Check High Voltage Threshold  = CommandString.substring(14,15)
        // Charge Termination Voltage Threshold  = CommandString.substring(16,17)
        // Charge Inialization Voltage Threshold  = CommandString.substring(18,19)
        // Sanity Check Low Temp Threshold  = CommandString.substring(20,21)
        }
       if (CommandString.substring(6,13) == "42330400") {
        // This is a command to set the Amp-Hour Setpoints
        
        // Charge Termination Amp-Hour Threshold  = CommandString.substring(14,17)
        // Charge Inialization Amp-Hour  Threshold  = CommandString.substring(18,21)
        // Sanity Check High Current Threshold  = CommandString.substring(22,25)
        // Sanity Check Low Current Threshold  = CommandString.substring(26,29)
        }
       if (CommandString.substring(6,13) == "42330800") {
        // This is a command to set the Recharge Ratio
        
        // Recharge Ratio  = CommandString.substring(14,15)
        }
       if (CommandString.substring(6,13) == "48331000") {
        // This is a command to set the Altitude Descent Trigger
        
        // Altitude Descent Trigger  = CommandString.substring(14,17)
        // Low Sanity Check Altitude  = CommandString.substring(18,21)
        }
       if (CommandString.substring(6,13) == "48332000") {
        // This is a command to set the Voltage Descent Trigger
        
        // Voltage Descent Trigger  = CommandString.substring(14,15)
        }
       if (CommandString.substring(6,13) == "48334000") {
        // This is a command to set the Pressure  Descent Trigger
        
        // Sanity Check High Pressure Threshold  = CommandString.substring(14,17)
        // Pressure Descent Trigger  = CommandString.substring(18,21)
        
        }
       if (CommandString.substring(6,13) == "48338000") {
        // This is a command to set the Length of time in Loadshed Mode until we trigger Emergency Descent
        
        // Time in Loadshed Trigger  = CommandString.substring(14,15)
       }
    
    }// Parity Check is correct!
    else{
      // Increment an Error in some fashion regard to parity failure.  
    }
    
  } // INCREMENT VALID COMMAND RECIEVED
  else{
    // Increment an Error in regard to bad commands getting through.
  }

//Receive any data from satellite
int read_satellite_data()
{
  return NO_COMMANDS_TO_PROCESS;
}

