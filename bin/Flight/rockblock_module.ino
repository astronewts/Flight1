#include "Arduino.h"
#include <string>
//#include <Streaming>

#define MSEC_IN_MIN (1000*60)
#define MSEC_IN_SEC 1000

elapsedMillis time_initialization_rb;

/* Periodic callback, do housekeeping here. */
bool ISBDCallback()
{
if(parameters.vehicle_mode != SIGNAL_TEST_MODE)
{
  if (Flag_RB.loop_started==1)
  {
    Main_flight_loop();
  }
}
return true;
}

void initialize_rb()
{
    Serial.println("If the RockBlock is not connected, this initialization will timeout in 4 min");
    // Set RB Timeout Variables
    
    isbd.adjustATTimeout(DEFAULT_RB_AT_BUS_TIMEOUT);                  // Default is 20 seconds. Value is set in definition.h
    
    isbd.adjustSendReceiveTimeout(DEFAULT_RB_SEND_RECIEVE_TIMEOUT);   // Default is 300 seconds. Value is set in definition.h
    
    if(debug.mode == 1)
    {
        // The following two lines are diagnostic routines for monitoring traffic and debug messages on a PC - comment these out for final flight code
        isbd.attachConsole(Serial); // see http://arduiniana.org/libraries/iridiumsbd/ for details 
        Serial.println(" ");
        Serial.println("###########################  isbd.attachConsole(Serial) was just commanded ###########################");   
      
        isbd.attachDiags(Serial);   // see http://arduiniana.org/libraries/iridiumsbd/ for details 
        Serial.println("###########################  isbd.attachDiags(Serial) was just commanded ###########################");   
        Serial.println(" ");
        Serial.println("Setting the RB Power Mode: isbd.setPowerProfile(DEFAULT_RB_POWER_MODE). 0 = High Power Mode, 1 = Low Power Mode");
    }
      
    isbd.setPowerProfile(DEFAULT_RB_POWER_MODE); 

    if(debug.mode == 1)
    {
       Serial.print("Power Mode is Set to: ");
       Serial.println(DEFAULT_RB_POWER_MODE);
       
       Serial.print("\nChecking: isbd.isAsleep() = ");
       Serial.println(isbd.isAsleep()); 
       Serial.println(" ");
       Serial.println("[Note: isbd.isAsleep() = 0 if RB is awake but 1 if asleep]");
    }
      
    if (isbd.isAsleep() == 1)
    {
      if(debug.mode == 1)
      {
        Serial.println("The Rockblock is Asleep.  We are initiating with isbd.begin()...");
        time_initialization_rb = 0;
      }
      Serial.println("Note: The initialization will get stuck here if the RB is not connected.  It will take XXXX seconds (This var is set in IridiumSBD.h)");
      
      parameters.rb_initialization_error_status = isbd.begin(); 
       
      if(debug.mode == 1)
      { 
         if(parameters.rb_initialization_error_status == 5)
         {
            Serial.println("!!!!! ERROR:  Init Error Status is equal to 5, which means the Rockblock is not physically connected. !!!!!");
         }
         Serial.println("Calling isbd.begin() has completed.");
         Serial.print("The initialization took:");
         Serial.print(time_initialization_rb);
         Serial.println("msec\n");
      }
    }
    if(debug.mode == 1)
    { 
      Serial.print("###########################  isbd.adjustATTimeout(");
      Serial.print(DEFAULT_RB_AT_BUS_TIMEOUT);
      Serial.println(") ###########################");     
      Serial.println(" ");   
      Serial.print("###########################  isbd.adjustSendReceiveTimeout(");
      Serial.print(DEFAULT_RB_SEND_RECIEVE_TIMEOUT);
      Serial.println(") ###########################");     
      Serial.println(" ");    
      Serial.print("###########################  isbd.setPowerProfile(");
      Serial.print(DEFAULT_RB_POWER_MODE);   
      Serial.println(")  ###########################");   
      Serial.println(" ");
    }
}

void sendreceive_satellite_data()
{
  //debug.mode = 1;
  if (parameters.frame_counter > 254)
  {
    parameters.frame_counter = 1;
  }
  else
  {
    parameters.frame_counter = parameters.frame_counter + 1;
  }
  
  //do-while loop to Send/Receive until Iridium queue is cleared
  do
  {
    // output telemetry to "output_dataword"
    write_output_telemetry_dataword();
    
    // determine length of concatenated dataword string
    size_t size_mssg = parameters.output_dataword.length();
    if(debug.mode == 1)
    {
       Serial.println("About to Start Sending a Dataword:");
       Serial.print("Length of output dataword: ");
       Serial.println(parameters.output_dataword.length());
    }

    // determine number of bytes in dataword
    size_t tx_bufferSize  = size_mssg / 8;
    tx_bufferSize += size_mssg % 8 ? 1 : 0;
    if(debug.mode == 1)
    {
       Serial.print("Numb of bytes in message: ");
       Serial.println(tx_bufferSize);
    }
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

    ////////////////// Start of Iridium Transmit Code //////////////////////////

    if(debug.mode == 1)
    {
      Serial.println(" ");
      Serial.print("Time Since Start: ");
      Serial.println(parameters.time_since_start);
     
      Serial.println(" ");
      Serial.print("Sleep Status (#1):");
      Serial.println(isbd.isAsleep());   
    }
     
    // begin =  Starts (or wakes) the RockBLOCK modem and prepare it to communicate.
    
    if (isbd.isAsleep() == 1)
    {
      parameters.rb_initialization_error_status = isbd.begin();
      if(debug.mode == 1)
      {
        Serial.println(" ");
        Serial.print("parameters.rb_initialization_error_status = ");
        Serial.println(parameters.rb_initialization_error_status);
        Serial.println(" ");
        Serial.println("########################### isdb.begin() was just commanded ################################");
        Serial.println(" ");
      }
    }

    if(debug.mode == 1)
    {
      Serial.println(" ");
      Serial.print("Sleep Status (#2):");
      Serial.println(isbd.isAsleep());
    }
       
    // isbd.useMSSTMWorkaround(false);  // see http://arduiniana.org/libraries/iridiumsbd/ for details 
    
    // int getSignalQuality(int &quality);
    // Description:   Queries the signal strength and visibility of satellites
    // Returns:       ISBD_SUCCESS if successful, a non-zero code otherwise;
    // Parameter:     quality – Return value: the strength of the signal (0=nonexistent, 5=high)

    int signalQuality = -1;
    int sig_qual_err = -1; //DEBUG123
    sig_qual_err = isbd.getSignalQuality(signalQuality); //DEBUG123

    if(debug.mode == 1)
    {
      Serial.println(" ");
      Serial.print("########  signalQuality = ");
      Serial.print(signalQuality);
      Serial.println("##########");
      Serial.print("########  isbd.getSignalQuality(signalQuality) = ");
      Serial.print(sig_qual_err);
      Serial.println("##########");
      Serial.println(" ");
    }

    parameters.signal_quality_record = signalQuality;
    parameters.signal_quality_error_record = sig_qual_err; //DEBUS123
    
    if (sig_qual_err != 0)
    {
      // TODO: WRITE THIS TO THE ERROR BUFFER
      if(debug.mode == 1)
      {
         Serial.println("SignalQuality failed: error ");
         Serial.println(sig_qual_err);
         Serial.print("Time: ");
         Serial.println(parameters.transmit_elapsed_time);
      }

      //Serial.print("Error: ");
      //Serial.println(sig_qual_err);
     
      // TODO: WRITE THIS TO THE SD BUFFER
      //
      //      ERROR CODES!!!
      //      #define ISBD_SUCCESS             0
      //      #define ISBD_ALREADY_AWAKE       1
      //      #define ISBD_SERIAL_FAILURE      2
      //      #define ISBD_PROTOCOL_ERROR      3
      //      #define ISBD_CANCELLED           4
      //      #define ISBD_NO_MODEM_DETECTED   5
      //      #define ISBD_SBDIX_FATAL_ERROR   6
      //      #define ISBD_SENDRECEIVE_TIMEOUT 7
      //      #define ISBD_RX_OVERFLOW         8
      //      #define ISBD_REENTRANT           9
      //      #define ISBD_IS_ASLEEP           10
      //      #define ISBD_NO_SLEEP_PIN        11
    
      write_telemetry_data_to_sd();  //DEBUG456
      return;
    }

    if(debug.mode == 1)
    {
      Serial.println(" ");
      Serial.print("Signal quality (0=nonexistent, 5=high) is ");
      Serial.println(signalQuality);
      Serial.println(" ");
    }
    
    // Comment out above code after diagnostics are complete
    
    // int sendReceiveSBDBinary(const uint8_t *txData, size_t txDataSize, uint8_t *rxBuffer, size_t &rxBufferSize);
    // Description:   Transmits a binary message to the global satellite system and receives a message if one is available.
    // Returns:            ISBD_SUCCESS if successful, a non-zero code otherwise;
    // Parameter:      txData – The buffer containing the binary data to be transmitted.
    // Parameter:      txDataSize - The size of the outbound buffer in bytes.
    // Parameter:      rxBuffer – The buffer to receive the inbound message.
    // Parameter:      rxBufferSize - The size of the buffer in bytes.
    // NOTE: uint8_t is shorthand for: a type of unsigned integer of length 8 bits
    // NOTE: The maximum size of a transmitted packet (including header and checksum) is 340 bytes.
    // NOTE: The maximum size of a received packet is 270 bytes.
    //=========== real command =========================================== //
   
      size_t rx_bufferSize = sizeof(rx_buffer);

      int send_receive_err = isbd.sendReceiveSBDBinary(tx_buffer, tx_bufferSize, rx_buffer, rx_bufferSize);
      parameters.rb_send_receive_err = send_receive_err;
    //=========== end real command ======================================= //
    
      if (send_receive_err != 0)
      {
        // TODO: WRITE THIS TO THE ERROR BUFFER

         Serial.println("Send Recieve FAIL !!!!!!!!!!!!!!!!!!!!!!!!");
         Serial.print("Error: ");
         Serial.println(send_receive_err);
         Serial.print("Time: ");
         Serial.println(parameters.transmit_elapsed_time);
        
        if(debug.mode == 1)
        {
          Serial.print("########  ERROR: isbd.sendReceiveSBDBinary(a,b,c,d) = ");
          Serial.print(send_receive_err);
          Serial.println("##########");
          Serial.print("sendReceiveSBDBinary failed: error ");
          
          Serial.println(" ");
          //err = isbd.sleep();
        
          //if (err != 0)
          //{
          //  Serial.print("sleepfailed: error ");
          //  Serial.println(err);
          //}
        
          // TOC MARK!!!
          
          Serial.println(" ");
          Serial.print("TOC: ");
          Serial.println(parameters.cutdown_initiation_elapsed_time);
          Serial.println(" ");
          
          //Serial.println(send_receive_err);
        }
        write_telemetry_data_to_sd();   //DEBUG456
        parameters.rb_send_receive_err = -1;
        return;
      }

      // If we are here, it means the send/receive was complete
      Flag_RB.try_send_receive=1;
      write_telemetry_data_to_sd();
      gyro.count_between_RB=0;
      alt.count_between_RB=0;
      gps_data.count_between_RB=0;
      parameters.signal_quality_record = -1;
      parameters.signal_quality_error_record = -1;
      parameters.rb_send_receive_err = -1;

      Serial.print("ELAPSED TIME SINCE LAST TRANSMIT: ");
      Serial.println(parameters.transmit_elapsed_time);
      Serial.println("**Satellite transmit/receive complete!**");

      if(debug.mode == 1)  //DEBUG123
      {
            Serial.println("");
            Serial.println("**This is output.dataword**");
            Serial.println(parameters.output_dataword);

            // TOC MARK!!!
            Serial.println(" ");
            Serial.print("TIME SINCE START: ");
            Serial.println(parameters.time_since_start);
            Serial.println(" ");
        
            // ================ Print inbound message ================================= //
        
            Serial.print("Inbound buffer size is ");
            Serial.println(rx_bufferSize);

            Serial.println("**This is rx buffer in RAW*");
            for (int i=0; i<rx_bufferSize; ++i)
            {
              // Serial.write(rx_buffer[i]);
              Serial.print(" ");
              Serial.print(rx_buffer[i], HEX);
              //Serial.print(rx_buffer[i]);
             // Serial.print(") ");
            }

          Serial.println("**This is rx buffer in HEX*");
            for (int i=0; i<rx_bufferSize; ++i)
            {
              // Serial.write(rx_buffer[i]);
              Serial.print("(");
              Serial.print(rx_buffer[i], HEX);
              //Serial.print(rx_buffer[i]);
              Serial.print(") ");
            }
              // ================ END Print inbound message ============================== //
                Serial.println("");
                Serial.print("Number of remaining messages in Iridium queue: ");
              //  int getWaitingMessageCount();
              //  Description:   Returns the number of waiting messages on the Iridium servers.
              //  Returns:            The number of messages waiting.
                Serial.println(isbd.getWaitingMessageCount());
        
       } // end debug section //DEBUG123
         
      if (rx_bufferSize == 0)
        break; // all done with do-while loop to Send/Receive until Iridium queue is cleared
      
      // if a message has been received from Iridium, process the command 
      process_satellite_command();
  } while (isbd.getWaitingMessageCount() > 0);

  Flag_RB.try_send_receive=0;
  write_telemetry_data_to_sd();  //DEBUG456
  debug.mode = 0; 
  //////////End of Iridium Transmit Code/////////////////
}

// procedure :
//1) combine all telemetry: output="dataword" which is a string of binaries: 010101011100110 
//2) convert the string into a binary and send it through RockBlock (done in the example SendReceive_Test1)


// Appends the lowest bin_size bits from input_data in binary string representation to dataword
String combine(int bin_size, long input_data, String dataword)
{
    String temp_str;
    temp_str = "";

    for (int i=0; i<bin_size; i++) {
      temp_str = (((input_data >> i) & 1) ? "1" :"0") + temp_str;
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


int hexstring_to_int(String input_hexstring, int index_begin, int index_end)
{           
    // hexstring_to_int function
    // Inputs: (1) string of hexadecimal values, (2) string index to start reading from, (3) string index to stop reading from
    // Output: Base 10 integer converted from hexadecimal input string
    
    String temp_hexstring = input_hexstring.substring(index_begin,index_end);
    const char * c = temp_hexstring.c_str();
    int output_int = strtol(c,NULL,16);
              
    return output_int;
}


//Receive any data from satellite
int process_satellite_command()
{
   // Convert uint8_t array "rx_buffer" to hexadecimal string "CommandString"
  String CommandString = "";
  String temp_str; 
      for (int i=0; i<sizeof(rx_buffer); i++) {
        temp_str = String(rx_buffer[i],HEX);
        if (temp_str.length() < 2)
        {
          temp_str = "0"+temp_str;
        }
      CommandString = CommandString + temp_str;
    }

    if(debug.mode == 1)
    {
        Serial.println("Satellite message received! Processing command...");
        Serial.println("CommandString in hexadecimal format:");
        Serial.println(CommandString);
    }
  
    if(debug.mode == 1)
    {
      Serial.println("");
      Serial.println("COMMAND STRING OUTPUT: ");
      Serial.println(CommandString);
    }
  
  // Allow the Commands through if it matches the intended vehicle and code version
  if (CommandString.substring(0,6) == "030133") {
    
    if(debug.mode == 1)
    {
      Serial.println("Vehicle ID and code version in command string OK");
    }
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
      
      if (CommandString.substring(6,14) == "11330001") {
        // This is a command to change the Spacecraft Mode
        Serial.println("Received command to change the spacecraft mode");
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
           parameters.edm_flag_type = 5;
         }
         //GL//if (CommandString.substring(14,15) == "5") {
         //GL//  // Set the Mode to Spare Mode
         //GL//  set_spare_mode();
         //GL//}

        Serial.print("Mode has been changed to: ");
        Serial.println(CommandString.substring(14,15)); 
      }// This ends the section to command to change the Spacecraft Mode

      
      if (CommandString.substring(6,14) == "47330002") {
        // This is a command to Change the Flight Telemetry Format
         if (CommandString.substring(15,16) == "1") {
           // Update the Flight Format to be Format #1
           parameters.telemetry_format = FORMAT_1;
         }
         if (CommandString.substring(15,16) == "2") {
           // Update the Flight Format to be Format #1
           parameters.telemetry_format = FORMAT_2;
         }
      }
      
      //TODO: MAKE SURE WE ADD BATTERY 2 COMMANDS TO SHUT OFF Charging
      
      if (CommandString.substring(6,14) == "25330004") {
        // This is a command change the Camera Enable Status
         if (CommandString.substring(14,15) == "f") {
         // Enable the Camera
         parameters.camera_status = true;
         }
         if (CommandString.substring(14,15) == "0") {
         // Disable the Camera
         parameters.camera_status = false;
         }
      }     
       if (CommandString.substring(6,14) == "23330008") {
        // This is a command to Fire the Pyro and Initiate Descent
         if (CommandString.substring(14,15) == "f") {
            // Enable Pyro Relay, Fire Pyros, and disable Pyro Relay
            set_transit_mode(); 
            // This will also initiate transition to Transit Mode
            cutdown_fire();
            Serial.println("Pyro fire command initiated!");
          }
       }
       
       if (CommandString.substring(6,14) == "23330010") {
        // This is a command set Pyro Fire Pulse Width
        // Defined in msec
        //parameters.cutdown_pulse_width = CommandString.substring(14,18).toInt();
        parameters.cutdown_pulse_width = hexstring_to_int(CommandString,14,18) * 1000.0;
  
       }
       if (CommandString.substring(6,14) == "44330020") {
        // This is a command set the SD Card Write Period
        // Converted from sec to msec
        //parameters.sd_card_write_period = CommandString.substring(14,18).toInt() * 1000;
        parameters.sd_card_write_period = hexstring_to_int(CommandString,14,18) * 1000.0;
        }
        
       if (CommandString.substring(6,14) == "45330040") {
        // This is a command set the Camera Write Period & Camera On Time
        // Converted from sec to msec
        parameters.camera_period = hexstring_to_int(CommandString,14,18) * 1000.0;
        parameters.camera_on_time = hexstring_to_int(CommandString,18,22) * 1000.0;
        }
       if (CommandString.substring(6,14) == "47330080") {
        // This is a command to set the Transmit Rates Permode
        // Converted from min to msec
        
        // Normal Ops Transmit Rate = CommandString.substring(14,17)
        thresholds.normal_transmit_period = hexstring_to_int(CommandString,14,18) * 60 * 1000.0;
        
        // Loadshed Transmit Rate = CommandString.substring(18,21)
        thresholds.load_shed_transmit_period = hexstring_to_int(CommandString,18,22) * 60 * 1000.0;
        
        // Transit Transmit Rate = CommandString.substring(22,25)
        thresholds.transit_transmit_period = hexstring_to_int(CommandString,22,26) * 60 * 1000.0;
        
        // Emergency Transit Transmit Rate = CommandString.substring(26,29)
        thresholds.emergency_transit_transmit_period =  hexstring_to_int(CommandString,26,30) * 60 * 1000.0;
        
        // Spare Transmit Rate = CommandString.substring(30,33)
        thresholds.spare_transmit_period = hexstring_to_int(CommandString,30,34) * 60 * 1000.0;
        
        }
        
        
       if (CommandString.substring(6,14) == "46330100") {
        // This is a command to set the TCS Thresholds
        // Converted from Kelvin to Celsius
        
        // Sanity Check High Temp Threshold  = CommandString.substring(14,15)
        parameters.battery_temperature_sanity_check_high = hexstring_to_int(CommandString,14,18) - 273;
        
        // Normal OPS High Temp Threshold  = CommandString.substring(16,17)
        thresholds.normal_battery_temperature_limit_high = hexstring_to_int(CommandString,18,22) - 273;
        
        // Normal OPS Low Temp Threshold  = CommandString.substring(18,19)
        thresholds.normal_battery_temperature_limit_low = hexstring_to_int(CommandString,22,26) - 273;
        
        // Loadshed High Temp Threshold = CommandString.substring(20,21)
        thresholds.survival_battery_temperature_limit_high = hexstring_to_int(CommandString,26,30) - 273;
        
        // Loadshed High Temp Threshold = CommandString.substring(22,23)
        thresholds.survival_battery_temperature_limit_low = hexstring_to_int(CommandString,30,34) - 273;
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(24,25)
        parameters.battery_temperature_sanity_check_low = hexstring_to_int(CommandString,34,38) - 273;
        }
        
       if (CommandString.substring(6,14) == "42330200") {
        // This is a command to set the Voltage Sanity Check Setpoints
        // Converted from millivolts to Volts
        
        // Sanity Check High Voltage Threshold  = CommandString.substring(14,18)
        parameters.voltage_sanity_check_high = hexstring_to_int(CommandString,14,18) / 1000.0;
        
        // Sanity Check Low Voltage Threshold  = CommandString.substring(18,22)
        parameters.voltage_sanity_check_low = hexstring_to_int(CommandString,18,22) / 1000.0;
        
        }
        
       if (CommandString.substring(6,14) == "42330400") {
        // This is a command to set the Amp-Hour Sanity Check Setpoints
        // Conversion from mAmp-Hrs to Amp-Hrs

        // Sanity Check High Current Threshold  = CommandString.substring(14,18)
        parameters.charge_current_sanity_check_high = hexstring_to_int(CommandString,14,18) / 1000.0;
        
        // Sanity Check Low Current Threshold  = CommandString.substring(18,22)
        parameters.charge_current_sanity_check_low = hexstring_to_int(CommandString,18,22) * -1 / 1000.0;
       }
       
       if (CommandString.substring(6,14) == "42330800") {
        // This is a command to set the Recharge Ratio
        // Convert percentage to scale multiplication factor
        // Recharge Ratio  = CommandString.substring(14,15)
        parameters.battery_1_recharge_ratio = hexstring_to_int(CommandString,14,16) / 100.0;
        parameters.battery_2_recharge_ratio = hexstring_to_int(CommandString,16,18) / 100.0;
        }
        
       if (CommandString.substring(6,14) == "48331000") {
        // This is a command to set the Altitude Descent Trigger
        // Defined in meters
        
        // GPS Altitude Descent Trigger  = CommandString.substring()
        parameters.gps_altitude_limit_low = hexstring_to_int(CommandString,14,18);
        
        // GPS Low Sanity Check Altitude  = CommandString.substring()
        parameters.gps_altitude_sanity_check_low = hexstring_to_int(CommandString,18,22);

        // GPS HIGH Sanity Check Altitude  = CommandString.substring()
        parameters.gps_altitude_sanity_check_high = hexstring_to_int(CommandString,22,26);

        // ALT Altitude Descent Trigger  = CommandString.substring()
        parameters.alt_altitude_limit_low = hexstring_to_int(CommandString,26,30);
        
        // ALT Low Sanity Check Altitude  = CommandString.substring()
        parameters.alt_altitude_sanity_check_low = hexstring_to_int(CommandString,30,34);

        // ALT HIGH Sanity Check Altitude  = CommandString.substring()
        parameters.alt_altitude_sanity_check_high = hexstring_to_int(CommandString,34,38);
        }
        
       if (CommandString.substring(6,14) == "48332000") {
        // This is a command to set the Voltage Descent Trigger
        // Voltage Descent Trigger  = CommandString.substring(14,15)
        // Converted from milivolts to Volts
        
        parameters.low_voltage_limit_for_auto_cutdown = hexstring_to_int(CommandString,14,18) / 1000.0;
        }
     
       if (CommandString.substring(6,14) == "48338000") {
        // This is a command to set the Length of time in Loadshed Mode until we trigger Emergency Descent
        // Time in Loadshed Trigger  = CommandString.substring(14,21)
        // Convert seconds to milliseconds
         parameters.low_voltage_time_limit = hexstring_to_int(CommandString,14,18) * 1000.0;
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
    parameters.invalid_command_recieved_count = parameters.invalid_command_recieved_count + 1;
  }
  
  // if something 
  //return NO_COMMANDS_TO_PROCESS;
  //
  
  if(debug.mode == 1) {
    Serial.println("Satellite command processing complete!");
    Serial.println("");
  }
}

void write_output_telemetry_dataword()
{
  // Process the Normal OPS Transmit Format
  //if (parameters.vehicle_mode < 6) {

     // Create intial word (+ header) to send to the ground
    String valid_str;
    valid_str = "0";

    //parameters.output_dataword = "10101010";
    parameters.output_dataword = "";

    initialize_database();

    for (int i=0; i<(DB_SIZE+1); i++) {
      if (((db[i].format_1 == 1) && (parameters.telemetry_format == FORMAT_1)) || ((db[i].format_2 == 1) && (parameters.telemetry_format == FORMAT_2))) {
        //Serial.print("Print line: ");
        //Serial.print(i);
        //Serial.print("  DATA TYPE: ");
        if (db[i].tlm_type == "float") 
        {
          parameters.output_dataword = combine_float(db[i].bitsize, db[i].float_pointer, parameters.output_dataword);
          //Serial.print(db[i].tlm_type);
          //Serial.print("  DATA: ");
          //Serial.println(db[i].float_pointer);
        }
        else if (db[i].tlm_type == "long")
        {
          parameters.output_dataword = combine(db[i].bitsize, db[i].long_pointer, parameters.output_dataword);
          //Serial.print(db[i].tlm_type);
          //Serial.print("  DATA: ");
          //Serial.println(db[i].long_pointer);
        }
        else if ((db[i].tlm_type == "int")) 
        {
          parameters.output_dataword = combine(db[i].bitsize, db[i].int_pointer, parameters.output_dataword);
          //Serial.print(db[i].tlm_type);
          //Serial.print("  DATA: ");
          //Serial.println(db[i].int_pointer);     
        }
        else if ((db[i].tlm_type == "header")) 
        {
          parameters.output_dataword = "10101010";
          //Serial.print(db[i].tlm_type);
          //Serial.print("  DATA: ");
          //Serial.println("10101010");
        }
      }
    }
    
   //if(debug.mode == 1) 
   //{
   //   Serial.println("The following is the Output TLM word: ");
   //   Serial.println(parameters.output_dataword);
   //   Serial.println(" ");
   // }  
}

void get_signal_quality()
{
  int signalQuality = -1;
  int sig_qual_err = -1;
  sig_qual_err = isbd.getSignalQuality(signalQuality);
  parameters.signal_quality_record = signalQuality;
  parameters.signal_quality_error_record = sig_qual_err;
}

