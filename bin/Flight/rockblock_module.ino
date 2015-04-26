#include "Arduino.h"
#include <string>
//#include <Streaming>

#define MSEC_IN_MIN (1000*60)

void sendrecieve_satellite_data()
{
    
    // determine length of concatenated dataword string
    size_t size_mssg = parameters.output_dataword.length(); 

    // determine number of bytes in dataword
    size_t tx_bufferSize  = size_mssg / 8;
    float tx_bufferSize_rest =  size_mssg % 8;
    if ( tx_bufferSize_rest != 0) { tx_bufferSize = tx_bufferSize + 1;
    } 
    Serial.println("numb of bytes in mssg:");
    Serial.println(tx_bufferSize);

    // create unsigned integer array for input to Iridium send/receive function
    uint8_t tx_buffer[tx_bufferSize];

    //convert concatenated dataword characters into 8-bit chunks and add each to unsigned integer array
    
    for(int i=0; i<tx_bufferSize; i++){
        tx_buffer[i] = 0;
        for(int j=0; j<8; j++){
            int k = i*8 + j;
           char myChar = parameters.output_dataword.charAt(k); 
           int  bit_extr = std::max(myChar - '0', 0);  
           
           tx_buffer[i] = tx_buffer[i] + bit_extr * pow(2,7-j);
                  
       }
    }        
    
     //////////Start of Iridium Transmit Code/////////////////
    
    // The following two lines are diagnostic routines for monitoring traffic and debug messages on a PC - comment these out for final flight code
    isbd.attachConsole(Serial); // see http://arduiniana.org/libraries/iridiumsbd/ for details 
    isbd.attachDiags(Serial);   // see http://arduiniana.org/libraries/iridiumsbd/ for details 
    
    isbd.setPowerProfile(1); // This is a low power application
    
    // begin =  Starts (or wakes) the RockBLOCK modem and prepare it to communicate.
    isbd.begin();
    
    //int getSignalQuality(int &quality);
    //Description:   Queries the signal strength and visibility of satellites
    //Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
    //Parameter:      quality – Return value: the strength of the signal (0=nonexistent, 5=high)
    int signalQuality = -1;
    int err = isbd.getSignalQuality(signalQuality);
    if (err != 0)
    {
      Serial.print("SignalQuality failed: error ");
      Serial.println(err);
      return;
    }
   
    Serial.print("Signal quality (0=nonexistent, 5=high) is ");
    Serial.println(signalQuality);
    //Comment out above code after diagnostics are complete
    
    // int sendReceiveSBDBinary(const uint8_t *txData, size_t txDataSize, uint8_t *rxBuffer, size_t &rxBufferSize);
//Description:   Transmits a binary message to the global satellite system and receives a message if one is available.
//Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
//Parameter:      txData – The buffer containing the binary data to be transmitted.
//Parameter:      txDataSize - The size of the outbound buffer in bytes.
//Parameter:      rxBuffer – The buffer to receive the inbound message.
//Parameter:      rxBufferSize - The size of the buffer in bytes.
// NOTE: uint8_t is shorthand for: a type of unsigned integer of length 8 bits
// NOTE: The maximum size of a transmitted packet (including header and checksum) is 340 bytes.
// NOTE: The maximum size of a received packet is 270 bytes.
//=========== real command =========================================== //
   
  size_t rx_bufferSize = sizeof(rx_buffer);
  
//  err = isbd.sendReceiveSBDBinary(tx_buffer, tx_bufferSize, rx_buffer, rx_bufferSize);
 
//=========== end real command ======================================= //

  Serial.print("message sent");
  if (err != 0)
  {
    Serial.print("sendReceiveSBDText failed: error ");
    Serial.println(err);
    return;
  }
// ================ Print inbound message ================================= //
  Serial.print("Inbound buffer size is ");
  Serial.println(rx_bufferSize);
  for (int i=0; i<rx_bufferSize; ++i)
  {
 //   Serial.write(rx_buffer[i]);
    Serial.print("(");
    //Serial.print(rx_buffer[i], HEX);
    Serial.print(rx_buffer[i]);
    Serial.print(") ");
  }
// ================ END Print inbound message ============================== //

  
  Serial.print("Number messages left: ");
//  int getWaitingMessageCount();
//  Description:   Returns the number of waiting messages on the Iridium servers.
//  Returns:            The number of messages waiting.
  Serial.println(isbd.getWaitingMessageCount());
   
   //////////End of Iridium Transmit Code/////////////////
}

// procedure :
//1) combine all telemetry: output="dataword" which is a string of binaries: 010101011100110 
//2) convert the string into a binary and send it through RockBlock (done in the example SendRecieve_Test1)


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

String combine_float(int bin_size, float input_fdata, String dataword)
{
    String temp_str;

    for (int i=0; i<32; i++) {
      if ((1 << i) & *((int *)&input_fdata)) {
        temp_str = "1" + temp_str;
      } else {
        temp_str = "0" + temp_str;
      }
    }
    
    dataword = dataword + temp_str;
    
    return dataword;     
}

String combine_int(int bin_size, int input_idata, String dataword)
{
    int zeros;
    String temp_str;
    temp_str = String(input_idata,BIN);
    zeros = bin_size - temp_str.length();
 
    for (int i=0; i<zeros; i++) {
      temp_str = "0"+temp_str;
    }
    
    dataword = dataword + temp_str;
    
    return dataword;     
}

String combine_uint8_t(int bin_size, uint8_t input_uintdata, String dataword)
{
    int zeros;
    String temp_str;
    temp_str = String(input_uintdata,BIN);
    zeros = bin_size - temp_str.length();
 
    for (int i=0; i<zeros; i++) {
      temp_str = "0"+temp_str;
    }
    
    dataword = dataword + temp_str;
    
    return dataword;     
}

//Receive any data from satellite
int process_satellite_command()
{
   // Converts uint8_t array "rx_buffer" to string "CommandString"
  String CommandString = "";  
  // Convert rx_buffer from uint8_t array to string
      for (int i=0; i<sizeof(rx_buffer); i++) {
      CommandString = combine_uint8_t(8, rx_buffer[i], CommandString);
    }
  
  // Allow the Commands through if it matches the intended vehicle and code version
  if (CommandString.substring(0,5) == "030133") {
    
    // INCREMENT VALID COMMAND RECIEVED COUNTER
    if (parameters.command_count < 255) { 
      parameters.command_count = parameters.command_count + 1;
    }
    else {
      parameters.command_count = 1;
    } 
    
    // TODO
    // ***************************************************
    // PUT LOGIC IN HERE TO SEE IF EXTRACT THE PARITY
    // ***************************************************
    //TODO: Fix parity to be 7 or 3
    //if (CommandString.substring(538) == "7") {
      
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
        
        // Charge Termination Voltage Threshold  = CommandString.substring(16,17)
        parameters.battery_1_voltage_term_threshold = CommandString.substring(16,17).toInt() / 10;
        
        // Charge Inialization Voltage Threshold  = CommandString.substring(18,19)
        parameters.battery_1_voltage_init_threshold = CommandString.substring(18,19).toInt() / 10;
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(20,21)
        parameters.voltage_sanity_check_low = CommandString.substring(20,21).toInt() / 10;
        }
       if (CommandString.substring(6,13) == "42330400") {
        // This is a command to set the Amp-Hour Setpoints
        // Conversion from mAmp-Hrs to Amp-Hrs
        
        // Charge Termination Amp-Hour Threshold  = CommandString.substring(14,17)
        parameters.battery_1_amphrs_term_threshold = CommandString.substring(14,17).toInt() / 1000;
        // Charge Inialization Amp-Hour Threshold  = CommandString.substring(18,21)
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
     
       if (CommandString.substring(6,13) == "48338000") {
        // This is a command to set the Length of time in Loadshed Mode until we trigger Emergency Descent
        // Time in Loadshed Trigger  = CommandString.substring(14,21)
        // Convert seconds to milliseconds
         parameters.low_voltage_time_limit = CommandString.substring(14,17).toInt() * 1000;
       }
 
       // ****************************************************************
       // Put something in here if none of the if statement loops trip???
       // ****************************************************************

    // TODO: ADD BACK IN WHEN THE PARITY CHECK CODE IS WRITTEN
    // } // End Parity Check conditional
    
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

void write_output_telemetry_dataword()
{
  // Process the Normal OPS Transmit Format
  if (parameters.vehicle_mode < 6) {
    
    // Create intial word (+ header) to send to the ground
    String valid_str;
    valid_str = "0";
    
    // TODO: FIX THIS HEADER FROM HEX TO DECIMAL!!!
    //parameters.output_dataword = combine(3, parameters.vehicle_mode, "A30133");                                              //0
    parameters.output_dataword = combine(8, parameters.vehicle_mode, "10101010");  //TEMP 
    
    parameters.output_dataword = combine(8, parameters.vehicle_mode, parameters.output_dataword);                            //1                                                                                
    parameters.output_dataword = combine(8, parameters.command_count, parameters.output_dataword);                           //2
    parameters.output_dataword = combine(8, thresholds.normal_transmit_rate/MSEC_IN_MIN, parameters.output_dataword);                    //3
    parameters.output_dataword = combine(8, thresholds.load_shed_transmit_rate/MSEC_IN_MIN, parameters.output_dataword);                 //4
    parameters.output_dataword = combine(8, thresholds.transit_transmit_rate/MSEC_IN_MIN, parameters.output_dataword);                   //5
    parameters.output_dataword = combine(8, thresholds.emergency_transit_transmit_rate/MSEC_IN_MIN, parameters.output_dataword);         //6
    parameters.output_dataword = combine(8, thresholds.test_transmit_rate/MSEC_IN_MIN, parameters.output_dataword);                      //7
    parameters.output_dataword = combine(8, parameters.sd_card_write_rate/MSEC_IN_MIN, parameters.output_dataword);                      //8
    parameters.output_dataword = combine(12, telemetry_data.battery_1_temp_1, parameters.output_dataword);                   //9
    parameters.output_dataword = combine(12, telemetry_data.battery_1_temp_2, parameters.output_dataword);                   //10
    parameters.output_dataword = combine(12, telemetry_data.battery_2_temp_1, parameters.output_dataword);                   //11
    parameters.output_dataword = combine(12, telemetry_data.battery_2_temp_2, parameters.output_dataword);                   //12
    parameters.output_dataword = combine(12, telemetry_data.inner_external_temp, parameters.output_dataword);                //13
    parameters.output_dataword = combine(12, telemetry_data.outter_external_temp, parameters.output_dataword);               //14
    parameters.output_dataword = combine(12, telemetry_data.internal_temp, parameters.output_dataword);                      //15
    parameters.output_dataword = combine(12, telemetry_data.air_pressure, parameters.output_dataword);                       //16
    parameters.output_dataword = combine(12, telemetry_data.battery_1_voltage_1, parameters.output_dataword);                //17
    parameters.output_dataword = combine(12, telemetry_data.battery_1_voltage_2, parameters.output_dataword);                //18
    parameters.output_dataword = combine(13, telemetry_data.battery_1_charge_current_1, parameters.output_dataword);         //19
    parameters.output_dataword = combine(13, telemetry_data.battery_1_charge_current_2, parameters.output_dataword);         //20
   
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //21
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //22
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //23
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //24
    parameters.output_dataword = combine_float(32, gps.location.lat(), parameters.output_dataword);                          //25
    parameters.output_dataword = combine_float(32, gps.location.lng(), parameters.output_dataword);                          //26
    parameters.output_dataword = combine_float(32, gps.altitude.meters(), parameters.output_dataword);                       //27
    parameters.output_dataword = combine_float(32, gps.location.age(), parameters.output_dataword);                          //28
    parameters.output_dataword = combine_float(32, gps.altitude.age(), parameters.output_dataword);                          //29
    parameters.output_dataword = combine_float(32, gps.course.deg(), parameters.output_dataword);                            //30
    parameters.output_dataword = combine_float(32, gps.speed.kmph(), parameters.output_dataword);                            //31
    parameters.output_dataword = combine(4, gps.satellites.value(), parameters.output_dataword);                             //32
    parameters.output_dataword = combine(32, (long) gps.date.value(), parameters.output_dataword);                           //33
    parameters.output_dataword = combine(32, (long) gps.time.value(), parameters.output_dataword);                           //34
    parameters.output_dataword = combine(32, gps.hdop.value(), parameters.output_dataword);                                  //35
    parameters.output_dataword = combine(32, gps.charsProcessed(), parameters.output_dataword);                              //36
    parameters.output_dataword = combine(32, gps.sentencesWithFix(), parameters.output_dataword);                            //37
    parameters.output_dataword = combine(32, gps.failedChecksum(), parameters.output_dataword);                              //38
    
    valid_str = String(gps.hdop.isValid());   
    valid_str = valid_str + String(gps.location.isValid());                                                                  //39-1
    valid_str = valid_str + String(gps.altitude.isValid());                                                                  //39-2
    valid_str = valid_str + String(gps.course.isValid());                                                                    //39-3
    valid_str = valid_str + String(gps.speed.isValid());                                                                     //39-4
    valid_str = valid_str + String(gps.satellites.isValid());                                                                //39-5
    valid_str = valid_str + String(gps.date.isValid());                                                                      //39-6
    valid_str = valid_str + String(gps.time.isValid());                                                                      //39-7
    valid_str = valid_str + String(gps.hdop.isValid());                                                                      //39-8
  
    parameters.output_dataword = parameters.output_dataword + valid_str;                                                     //39
    
    parameters.output_dataword = combine_float(32, calData.accelMinX, parameters.output_dataword);                           //40
    parameters.output_dataword = combine_float(32, calData.accelMaxX, parameters.output_dataword);                           //41
    parameters.output_dataword = combine_float(32, calData.accelMinY, parameters.output_dataword);                           //42
    parameters.output_dataword = combine_float(32, calData.accelMaxY, parameters.output_dataword);                           //43
    parameters.output_dataword = combine_float(32, calData.accelMinZ, parameters.output_dataword);                           //44
    parameters.output_dataword = combine_float(32, calData.accelMaxZ, parameters.output_dataword);                           //45
    parameters.output_dataword = combine_float(32, calData.magMinX, parameters.output_dataword);                             //46
    parameters.output_dataword = combine_float(32, calData.magMaxX, parameters.output_dataword);                             //47
    parameters.output_dataword = combine_float(32, calData.magMinY, parameters.output_dataword);                             //48
    parameters.output_dataword = combine_float(32, calData.magMaxY, parameters.output_dataword);                             //49
    parameters.output_dataword = combine_float(32, calData.magMinZ, parameters.output_dataword);                             //50
    parameters.output_dataword = combine_float(32, calData.magMaxZ, parameters.output_dataword);                             //51
    parameters.output_dataword = combine(32, (long) dueMPU.m_rawQuaternion, parameters.output_dataword);                     //52
    parameters.output_dataword = combine_float(32, (long) dueMPU.m_dmpEulerPose, parameters.output_dataword);                //53
    parameters.output_dataword = combine_float(32, (long) dueMPU.m_fusedEulerPose, parameters.output_dataword);              //54
    parameters.output_dataword = combine(32, gyro_temp, parameters.output_dataword);                                         //55 
    parameters.output_dataword = combine(8, parameters.voltage_sanity_check_high, parameters.output_dataword);               //56
    parameters.output_dataword = combine(8, parameters.voltage_sanity_check_low, parameters.output_dataword);                //57
    parameters.output_dataword = combine(8, parameters.charge_current_sanity_check_high, parameters.output_dataword);        //58
    parameters.output_dataword = combine(8, parameters.charge_current_sanity_check_low, parameters.output_dataword);         //59
    parameters.output_dataword = combine(32, parameters.battery_1_recharge_ratio, parameters.output_dataword);               //60
    parameters.output_dataword = combine(32, parameters.battery_1_amphrs_charging, parameters.output_dataword);              //61
    parameters.output_dataword = combine(32, parameters.battery_1_amphrs_discharging, parameters.output_dataword);           //62
    parameters.output_dataword = combine(8, parameters.battery_1_amphrs_term_threshold, parameters.output_dataword);         //63
    parameters.output_dataword = combine(8, parameters.battery_1_amphrs_init_threshold, parameters.output_dataword);         //64
    parameters.output_dataword = combine(8, parameters.battery_1_voltage_term_threshold, parameters.output_dataword);        //65
    parameters.output_dataword = combine(8, parameters.battery_1_voltage_init_threshold, parameters.output_dataword);        //66
    parameters.output_dataword = combine(32, parameters.battery_2_recharge_ratio, parameters.output_dataword);               //67
    parameters.output_dataword = combine(32, parameters.battery_2_amphrs_charging, parameters.output_dataword);              //68
    parameters.output_dataword = combine(32, parameters.battery_2_amphrs_discharging, parameters.output_dataword);           //69
    parameters.output_dataword = combine(8, parameters.battery_2_amphrs_term_threshold, parameters.output_dataword);         //70
    parameters.output_dataword = combine(8, parameters.battery_2_amphrs_init_threshold, parameters.output_dataword);         //71
    parameters.output_dataword = combine(8, parameters.battery_2_voltage_term_threshold, parameters.output_dataword);        //72
    parameters.output_dataword = combine(8, parameters.battery_2_voltage_init_threshold, parameters.output_dataword);        //73
    
    parameters.output_dataword = combine(16, parameters.battery_temperature_limit_high, parameters.output_dataword);         //74
    parameters.output_dataword = combine(16, parameters.battery_temperature_limit_low, parameters.output_dataword);          //75
    parameters.output_dataword = combine(16, parameters.battery_temperature_sanity_check_high, parameters.output_dataword);  //76
    parameters.output_dataword = combine(16, thresholds.normal_battery_temperature_limit_high, parameters.output_dataword);  //77
    parameters.output_dataword = combine(16, thresholds.normal_battery_temperature_limit_low, parameters.output_dataword);   //78
    parameters.output_dataword = combine(16, thresholds.survival_battery_temperature_limit_high, parameters.output_dataword);//79
    parameters.output_dataword = combine(16, thresholds.survival_battery_temperature_limit_low, parameters.output_dataword); //80
    parameters.output_dataword = combine(16, parameters.battery_temperature_sanity_check_low, parameters.output_dataword);   //81
    parameters.output_dataword = combine(8, parameters.low_voltage_limit_for_loadshed_entry, parameters.output_dataword);    //82
    parameters.output_dataword = combine(8, parameters.low_voltage_limit_for_auto_cutdown, parameters.output_dataword);      //83
    parameters.output_dataword = combine(8, parameters.low_voltage_time_limit, parameters.output_dataword);                  //84
    parameters.output_dataword = combine(16, parameters.altitude_limit_low, parameters.output_dataword);                     //85
    parameters.output_dataword = combine(16, parameters.altitude_sanity_check_low, parameters.output_dataword);              //86
    parameters.output_dataword = combine(8, parameters.cutdown_pulse_width, parameters.output_dataword);                     //87
    parameters.output_dataword = combine(12, parameters.camera_period, parameters.output_dataword);                          //88
    parameters.output_dataword = combine(12, parameters.camera_on_time, parameters.output_dataword);                         //89

    // Start Bilevel Parent TLM 
    parameters.output_dataword = combine(1, parameters.battery_1_charging_status, parameters.output_dataword);               //90-1
    parameters.output_dataword = combine(1, parameters.battery_2_charging_status, parameters.output_dataword);               //90-2    
    parameters.output_dataword = combine(1, parameters.battery_bus_low_voltage_flag, parameters.output_dataword);            //90-3
    parameters.output_dataword = combine(1, parameters.heater_state_1, parameters.output_dataword);                          //90-4
    parameters.output_dataword = combine(1, parameters.heater_state_2, parameters.output_dataword);                          //90-5
    parameters.output_dataword = combine(1, parameters.cutdown_enable_state, parameters.output_dataword);                    //90-6
    parameters.output_dataword = combine(1, parameters.cutdown_1_status, parameters.output_dataword);                        //90-7
    parameters.output_dataword = combine(1, parameters.cutdown_2_status, parameters.output_dataword);                        //90-8
    parameters.output_dataword = combine(1, parameters.altitude_valid_flag, parameters.output_dataword);                     //90-9
    parameters.output_dataword = combine(1, parameters.camera_status, parameters.output_dataword);                           //90-10
    parameters.output_dataword = parameters.output_dataword + "000000";                                                      //90-[11-16]
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //91
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //92 
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //93 
    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000";                            //94
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



