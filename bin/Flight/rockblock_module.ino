#include "Arduino.h"
#include <string>
//#include <Streaming>

#define MSEC_IN_MIN (1000*60)
#define MSEC_IN_SEC 1000

elapsedMillis time_initialization_rb;

/* Periodic callback, do housekeeping here. */
bool ISBDCallback()
{
if (Flag_RB.loop_started==1)
  {
    if (parameters.vehicle_mode==FLIGHT_MODE_WITH_DEBUG)
    {
    Main_flight_loop();
    }
    if (parameters.vehicle_mode==FLIGHT_MODE)
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
}

void sendreceive_satellite_data()
{
  debug.mode = 1;
  //do-while loop to Send/Receive until Iridium queue is cleared
  do
  {
    // output telemetry to "output_dataword"
    write_output_telemetry_dataword();
    
    // determine length of concatenated dataword string
    size_t size_mssg = parameters.output_dataword.length();
    if(debug.mode == 1)
    {
       Serial.println("length of output dataword:");
       Serial.println(parameters.output_dataword.length());
    }

    // determine number of bytes in dataword
    size_t tx_bufferSize  = size_mssg / 8;
    tx_bufferSize += size_mssg % 8 ? 1 : 0;
    if(debug.mode == 1)
    {
       Serial.println("numb of bytes in mssg:");
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
      // TIC MARK!!!
      Serial.println(" ");
      Serial.print("TIC: ");
      // TODO: USE A BETTER TIMER HERE
      Serial.println(parameters.cutdown_initiation_elapsed_time);
      Serial.println(" ");
  
      Serial.println(" ");
      Serial.print("###########################  isbd.adjustATTimeout(");
      Serial.print(DEFAULT_RB_AT_BUS_TIMEOUT);
      Serial.println(") is commanded! ###########################");     
      Serial.println(" ");   
  
      Serial.println(" ");
      Serial.print("###########################  isbd.adjustSendReceiveTimeout(");
      Serial.print(DEFAULT_RB_SEND_RECIEVE_TIMEOUT);
      Serial.println(") is commanded! ###########################");     
      Serial.println(" ");    
      
      Serial.println(" ");
      Serial.print("###########################  isbd.setPowerProfile(");
      Serial.print(DEFAULT_RB_POWER_MODE);   
      Serial.println(") is commanded! ###########################");   
      Serial.println(" "); 
      
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
    int sig_qual_err = isbd.getSignalQuality(signalQuality);

    if(debug.mode == 1)
    {
      Serial.println(" ");
      Serial.print("########  signalQuality = ");
      Serial.print(signalQuality);
      Serial.println("##########");
      Serial.print("########  isbd.getSignalQuality(signalQuality) = ");
      Serial.print(sig_qual_err);
      Serial.println(" ##########");
      Serial.println(" ");
    }
    
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
      //if(sig_qual_err == 7)
      //{
      //   Serial.println("!!!!! ERROR: Signal Quality Error = 7 !!!!!");
         Serial.print("Error: ");
         Serial.println(sig_qual_err);
     
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
        return;
      }

      // If we are here, it means the send/receive was complete
      Flag_RB.try_send_reveive=1;
      write_telemetry_data_to_sd();
      gyro.count_between_RB=0;
      Flag_RB.try_send_reveive=0;

      Serial.print("ELAPSED TIME: ");
      Serial.println(parameters.transmit_elapsed_time);
      Serial.println("**Satellite transmit/receive complete!**");

      if(debug.mode == 1)
        {
            Serial.println("");
            Serial.println("**This is output.dataword**");
            Serial.println(parameters.output_dataword);

            // TOC MARK!!!
            Serial.println(" ");
            Serial.print("TOC: ");
            Serial.println(parameters.cutdown_initiation_elapsed_time);
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
         } // end debug section
         
      if (rx_bufferSize == 0)
        break; // all done with do-while loop to Send/Receive until Iridium queue is cleared
      
      // if a message has been received from Iridium, process the command 
      process_satellite_command();
  } while (isbd.getWaitingMessageCount() > 0);
  
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
    
    if (parameters.command_count < 255) { 
      parameters.num_rb_words_recieved = parameters.num_rb_words_recieved + 1;
    }
    else {  
      parameters.num_rb_words_recieved = 1;
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
         }
         if (CommandString.substring(14,15) == "5") {
           // Set the Mode to Spare Mode
           set_spare_mode();
         }

        Serial.print("Mode has been changed to: ");
        Serial.println(CommandString.substring(14,15)); 
      }// This ends the section to command to change the Spacecraft Mode

      
      if (CommandString.substring(6,14) == "22330002") {
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
         parameters.camera_status == true;
         }
         if (CommandString.substring(14,15) == "0") {
         // Disable the Camera
         parameters.camera_status == false;
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
        parameters.cutdown_pulse_width = hexstring_to_int(CommandString,14,18);
  
       }
       if (CommandString.substring(6,14) == "44330020") {
        // This is a command set the SD Card Write Period
        // Converted from sec to msec
        //parameters.sd_card_write_period = CommandString.substring(14,18).toInt() * 1000;
        parameters.sd_card_write_period = hexstring_to_int(CommandString,14,18) * 1000;
        }
        
       if (CommandString.substring(6,14) == "45330040") {
        // This is a command set the Camera Write Period & Camera On Time
        // Converted from sec to msec
        //parameters.camera_period = CommandString.substring(14,18).toInt() * 1000;
        //parameters.camera_on_time = CommandString.substring(18,22).toInt() * 1000;
        parameters.camera_period = hexstring_to_int(CommandString,14,18) * 1000;
        parameters.camera_on_time = hexstring_to_int(CommandString,18,22) * 1000;
        }
       if (CommandString.substring(6,14) == "47330080") {
        // This is a command to set the Transmit Rates Permode
        // Converted from min to msec
        
        // Normal Ops Transmit Rate = CommandString.substring(14,17)
        //thresholds.normal_transmit_period = CommandString.substring(14,18).toInt() * 3600 * 1000;
        thresholds.normal_transmit_period = hexstring_to_int(CommandString,14,18) * 60 * 1000;
        
        // Loadshed Transmit Rate = CommandString.substring(18,21)
        //thresholds.load_shed_transmit_period = CommandString.substring(18,22).toInt() * 3600 * 1000;
        thresholds.load_shed_transmit_period = hexstring_to_int(CommandString,18,22) * 60 * 1000;
        
        // Transit Transmit Rate = CommandString.substring(22,25)
        //thresholds.transit_transmit_period = CommandString.substring(22,26).toInt() * 3600 * 1000;
        thresholds.transit_transmit_period = hexstring_to_int(CommandString,22,26) * 60 * 1000;
        
        // Emergency Transit Transmit Rate = CommandString.substring(26,29)
        //thresholds.emergency_transit_transmit_period =  CommandString.substring(26,30).toInt() * 3600 * 1000;
        thresholds.emergency_transit_transmit_period =  hexstring_to_int(CommandString,26,30) * 60 * 1000;
        
        // Spare Transmit Rate = CommandString.substring(30,33)
        //thresholds.spare_transmit_period = CommandString.substring(30,34).toInt() * 3600 * 1000;
        thresholds.spare_transmit_period = hexstring_to_int(CommandString,30,34) * 60 * 1000;
        
        }
        
        
       if (CommandString.substring(6,14) == "46330100") {
        // This is a command to set the TCS Thresholds
        // Converted from Kelvin to Celsius
        
        // Sanity Check High Temp Threshold  = CommandString.substring(14,15)
        //parameters.battery_temperature_sanity_check_high = CommandString.substring(14,18).toInt() - 273;
        parameters.battery_temperature_sanity_check_high = hexstring_to_int(CommandString,14,18) - 273;
        
        // Normal OPS High Temp Threshold  = CommandString.substring(16,17)
        //thresholds.normal_battery_temperature_limit_high = CommandString.substring(18,22).toInt() - 273;
        thresholds.normal_battery_temperature_limit_high = hexstring_to_int(CommandString,18,22) - 273;
        
        // Normal OPS Low Temp Threshold  = CommandString.substring(18,19)
        //thresholds.normal_battery_temperature_limit_low = CommandString.substring(22,26).toInt() - 273;
        thresholds.normal_battery_temperature_limit_low = hexstring_to_int(CommandString,22,26) - 273;
        
        // Loadshed High Temp Threshold = CommandString.substring(20,21)
        //thresholds.survival_battery_temperature_limit_high = CommandString.substring(26,30).toInt() - 273;
        thresholds.survival_battery_temperature_limit_high = hexstring_to_int(CommandString,26,30) - 273;
        
        // Loadshed High Temp Threshold = CommandString.substring(22,23)
        //thresholds.survival_battery_temperature_limit_low = CommandString.substring(30,34).toInt() - 273;
        thresholds.survival_battery_temperature_limit_low = hexstring_to_int(CommandString,30,34) - 273;
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(24,25)
        //parameters.battery_temperature_sanity_check_low = CommandString.substring(34,38).toInt() - 273;
        parameters.battery_temperature_sanity_check_low = hexstring_to_int(CommandString,34,38) - 273;
        }
        
       if (CommandString.substring(6,14) == "42330200") {
        // This is a command to set the Voltage Setpoints
        // Converted from dVolts to Volts
        
        // Sanity Check High Voltage Threshold  = CommandString.substring(14,15)
        //parameters.voltage_sanity_check_high = CommandString.substring(14,16).toInt() / 10;
        parameters.voltage_sanity_check_high = hexstring_to_int(CommandString,14,16) / 10;
        
        // Charge Termination Voltage Threshold  = CommandString.substring(16,17)
        //parameters.battery_1_voltage_term_threshold = CommandString.substring(16,18).toInt() / 10;
        parameters.battery_1_voltage_term_threshold = hexstring_to_int(CommandString,16,18) / 10;
        
        // Charge Inialization Voltage Threshold  = CommandString.substring(18,19)
        //parameters.battery_1_voltage_init_threshold = CommandString.substring(18,20).toInt() / 10;
        parameters.battery_1_voltage_init_threshold = hexstring_to_int(CommandString,18,20) / 10;
        
        // Sanity Check Low Temp Threshold  = CommandString.substring(20,21)
        //parameters.voltage_sanity_check_low = CommandString.substring(20,22).toInt() / 10;
        parameters.voltage_sanity_check_low = hexstring_to_int(CommandString,20,22) / 10;
        }
        
       if (CommandString.substring(6,14) == "42330400") {
        // This is a command to set the Amp-Hour Setpoints
        // Conversion from mAmp-Hrs to Amp-Hrs
        
        // Charge Termination Amp-Hour Threshold  = CommandString.substring(14,17)
        //parameters.battery_1_amphrs_term_threshold = CommandString.substring(14,17).toInt() / 1000;
        parameters.battery_1_amphrs_term_threshold = hexstring_to_int(CommandString,14,17) / 1000;
        
        // Charge Inialization Amp-Hour Threshold  = CommandString.substring(18,21)
        //parameters.battery_1_amphrs_init_threshold = CommandString.substring(17,20).toInt() / 1000;
        parameters.battery_1_amphrs_init_threshold = hexstring_to_int(CommandString,17,20) / 1000;
        
        // Conversion from cAmp to Amp
        // Sanity Check High Current Threshold  = CommandString.substring(22,25)
        //parameters.charge_current_sanity_check_high = CommandString.substring(20,24).toInt() / 100;
        parameters.charge_current_sanity_check_high = hexstring_to_int(CommandString,20,24) / 100;
        
        // Sanity Check Low Current Threshold  = CommandString.substring(26,29)
        //parameters.charge_current_sanity_check_low = CommandString.substring(24,28).toInt() * -1 / 100;
        parameters.charge_current_sanity_check_low = hexstring_to_int(CommandString,24,28) * -1 / 100;
       }
       
       if (CommandString.substring(6,14) == "42330800") {
        // This is a command to set the Recharge Ratio
        // Convert percentage to scale multiplication factor
        // Recharge Ratio  = CommandString.substring(14,15)
        //parameters.battery_1_recharge_ratio = CommandString.substring(14,16).toInt() / 100;
        parameters.battery_1_recharge_ratio = hexstring_to_int(CommandString,14,16) / 100;
        }
        
       if (CommandString.substring(6,14) == "48331000") {
        // This is a command to set the Altitude Descent Trigger
        // Defined in meters
        
        // Altitude Descent Trigger  = CommandString.substring(14,17)
        //parameters.altitude_limit_low = CommandString.substring(14,18).toInt();
        parameters.altitude_limit_low = hexstring_to_int(CommandString,14,18);
        
        // Low Sanity Check Altitude  = CommandString.substring(18,21)
        //parameters.altitude_sanity_check_low = CommandString.substring(18,22).toInt();
        parameters.altitude_sanity_check_low = hexstring_to_int(CommandString,18,22);
        }
        
       if (CommandString.substring(6,14) == "48332000") {
        // This is a command to set the Voltage Descent Trigger
        // Voltage Descent Trigger  = CommandString.substring(14,15)
        // Converted from dVolts to Volts
        
        //parameters.low_voltage_limit_for_auto_cutdown = CommandString.substring(14,16).toInt() / 10;
        parameters.low_voltage_limit_for_auto_cutdown = hexstring_to_int(CommandString,14,16) / 10;
        }
     
       if (CommandString.substring(6,14) == "48338000") {
        // This is a command to set the Length of time in Loadshed Mode until we trigger Emergency Descent
        // Time in Loadshed Trigger  = CommandString.substring(14,21)
        // Convert seconds to milliseconds
         //parameters.low_voltage_time_limit = CommandString.substring(14,18).toInt() * 1000;
         parameters.low_voltage_time_limit = hexstring_to_int(CommandString,14,18) * 1000;
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
  Serial.println("Satellite command processing complete!");
  Serial.println("");
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
    
    Serial.println("");
    Serial.println(parameters.output_dataword);
    
//    for (int i=1; i<DB_SIZE+1; i++) {
//      Serial.print("Print line: ");
//      Serial.println(i);
//      if (db[i].tlm_type == "float") 
//      {
//        parameters.output_dataword = combine_float(db[i].bitsize, *db[i].float_pointer, parameters.output_dataword); 
//        Serial.print("Print float data title: ");
//        Serial.println(db[i].SD_Card_Title);
//        Serial.print("Print float data: ");
//        Serial.println(*db[i].float_pointer);
//      }
//      else if (db[i].tlm_type == "long")
//      {
//        parameters.output_dataword = combine(db[i].bitsize, *db[i].long_pointer, parameters.output_dataword); 
//        Serial.print("Print long data title: ");
//        Serial.println(db[i].SD_Card_Title);        
//        Serial.print("Print long data: ");
//        Serial.println(*db[i].long_pointer);
//      }
//      else if ((db[i].tlm_type == "int")) 
//      {
//        parameters.output_dataword = combine(db[i].bitsize, *db[i].int_pointer, parameters.output_dataword); 
//        Serial.print("Print int data title: ");
//        Serial.println(db[i].SD_Card_Title);        
//        Serial.print("Print int data: ");
//        Serial.println(*db[i].int_pointer);      
//      }
//      else if ((db[i].tlm_type == "null")) 
//      {
//        parameters.output_dataword = combine(db[i].bitsize, (long) 0, parameters.output_dataword); 
//        Serial.print("Print int data title: ");
//        Serial.println(db[i].SD_Card_Title);        
//        Serial.print("Print int data: None");
//      }
//    }
    Serial.println(parameters.output_dataword);
    
//    parameters.output_dataword = "10101010";                                                                                               // Balloon ID
//    parameters.output_dataword = combine(8, parameters.vehicle_mode, parameters.output_dataword);                                          //1
//    parameters.output_dataword = combine(8, parameters.command_count, parameters.output_dataword);                                         //2
//    parameters.output_dataword = combine(8, thresholds.normal_transmit_period/MSEC_IN_MIN, parameters.output_dataword);                    //3
//    parameters.output_dataword = combine(8, thresholds.load_shed_transmit_period/MSEC_IN_MIN, parameters.output_dataword);                 //4
//    parameters.output_dataword = combine(8, thresholds.transit_transmit_period/MSEC_IN_MIN, parameters.output_dataword);                   //5
//    parameters.output_dataword = combine(8, thresholds.emergency_transit_transmit_period/MSEC_IN_MIN, parameters.output_dataword);         //6
//    parameters.output_dataword = combine(8, thresholds.spare_transmit_period/MSEC_IN_MIN, parameters.output_dataword);                      //7
//    parameters.output_dataword = combine(8, parameters.sd_card_write_period/1000, parameters.output_dataword);                             //8
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_battery_1_temp_1, parameters.output_dataword);                      //9
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_battery_1_temp_2, parameters.output_dataword);                      //10
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_battery_2_temp_1, parameters.output_dataword);                      //11
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_battery_2_temp_2, parameters.output_dataword);                      //12
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_inner_external_temp, parameters.output_dataword);                   //13
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_outter_external_temp, parameters.output_dataword);                  //14
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_internal_temp, parameters.output_dataword);                         //15                             
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_air_pressure, parameters.output_dataword);                          //16
//    parameters.output_dataword = combine(12, raw_telemetry_data.raw_analog_VIN_voltage, parameters.output_dataword);                   //17
//    parameters.output_dataword = parameters.output_dataword + "000000000000";                                                          //18
//    // TODO: RELABEL ALL VERTICAL DATA ID'S ON THE LEFT
//    // Current Sensors Data
//    parameters.output_dataword = combine_float(32, telemetry_data.battery_1_charge_current, parameters.output_dataword);          //19
//    parameters.output_dataword = combine_float(32, telemetry_data.shuntvoltage_batt1, parameters.output_dataword);                //20
//    parameters.output_dataword = combine_float(32, telemetry_data.busvoltage_batt1, parameters.output_dataword);                  //21
//    parameters.output_dataword = combine_float(32, telemetry_data.loadvoltage_batt1, parameters.output_dataword);                 //22
//    parameters.output_dataword = combine_float(32, telemetry_data.battery_2_charge_current, parameters.output_dataword);          //23                            
//    parameters.output_dataword = combine_float(32, telemetry_data.shuntvoltage_batt2, parameters.output_dataword);                //24
//    parameters.output_dataword = combine_float(32, telemetry_data.busvoltage_batt2, parameters.output_dataword);                  //25
//    parameters.output_dataword = combine_float(32, telemetry_data.loadvoltage_batt2, parameters.output_dataword);                 //26 
//    parameters.output_dataword = combine_float(32, telemetry_data.sa_current, parameters.output_dataword);                        //27
//    parameters.output_dataword = combine_float(32, telemetry_data.shuntvoltage_sa, parameters.output_dataword);                   //28
//    parameters.output_dataword = combine_float(32, telemetry_data.busvoltage_sa, parameters.output_dataword);                     //29
//    parameters.output_dataword = combine_float(32, telemetry_data.loadvoltage_sa, parameters.output_dataword);                    //30
//    parameters.output_dataword = combine_float(32, telemetry_data.load_path_current, parameters.output_dataword);                 //31 
//    parameters.output_dataword = combine_float(32, telemetry_data.shuntvoltage_load_path, parameters.output_dataword);            //32
//    parameters.output_dataword = combine_float(32, telemetry_data.busvoltage_load_path, parameters.output_dataword);              //33
//    parameters.output_dataword = combine_float(32, telemetry_data.loadvoltage_load_path, parameters.output_dataword);             //34
//     
//    parameters.output_dataword = combine(8, parameters.tlm_processing_period/1000, parameters.output_dataword);              //35
//    parameters.output_dataword = parameters.output_dataword + "00000000000000000000000000000000000000000000000000000000";    //36
//    parameters.output_dataword = combine_float(32, gps.location.lat(), parameters.output_dataword);                          //37
//    parameters.output_dataword = combine_float(32, gps.location.lng(), parameters.output_dataword);                          //38
//    parameters.output_dataword = combine_float(32, gps.altitude.meters(), parameters.output_dataword);                       //39
//    parameters.output_dataword = combine_float(32, gps.location.age(), parameters.output_dataword);                          //40
//    parameters.output_dataword = combine_float(32, gps.altitude.age(), parameters.output_dataword);                          //41
//    parameters.output_dataword = combine_float(32, gps.course.deg(), parameters.output_dataword);                            //42
//    parameters.output_dataword = combine_float(32, gps.speed.kmph(), parameters.output_dataword);                            //43
//    parameters.output_dataword = combine(4, gps.satellites.value(), parameters.output_dataword);                             //44
//    parameters.output_dataword = combine(32, (long) gps.date.value(), parameters.output_dataword);                           //45
//    parameters.output_dataword = combine(32, (long) gps.time.value(), parameters.output_dataword);                           //46
//    parameters.output_dataword = combine(32, gps.hdop.value(), parameters.output_dataword);                                  //47
//    parameters.output_dataword = combine(32, gps.charsProcessed(), parameters.output_dataword);                              //48
//    parameters.output_dataword = combine(32, gps.sentencesWithFix(), parameters.output_dataword);                            //49
//    parameters.output_dataword = combine(32, gps.failedChecksum(), parameters.output_dataword);                              //50
//    
//    valid_str = "";
//    valid_str = valid_str + String(gps.location.isValid());                                                                  //51-1
//    valid_str = valid_str + String(gps.altitude.isValid());                                                                  //51-2
//    valid_str = valid_str + String(gps.course.isValid());                                                                    //51-3
//    valid_str = valid_str + String(gps.speed.isValid());                                                                     //51-4
//    valid_str = valid_str + String(gps.satellites.isValid());                                                                //51-5
//    valid_str = valid_str + String(gps.date.isValid());                                                                      //51-6
//    valid_str = valid_str + String(gps.time.isValid());                                                                      //51-7
//    valid_str = valid_str + String(gps.hdop.isValid());                                                                      //51-8
//  
//    parameters.output_dataword = parameters.output_dataword + valid_str;                                                     //51
//    
//    parameters.output_dataword = combine_float(32, gyro.ax, parameters.output_dataword);                                     //52
//    parameters.output_dataword = combine_float(32, dummy_value, parameters.output_dataword);                                 //53
//    parameters.output_dataword = combine_float(32, gyro.ay, parameters.output_dataword);                                     //54
//    parameters.output_dataword = combine_float(32, dummy_value, parameters.output_dataword);                                 //55
//    parameters.output_dataword = combine_float(32, gyro.az, parameters.output_dataword);                                     //56
//    parameters.output_dataword = combine_float(32, dummy_value, parameters.output_dataword);                                 //57
//    parameters.output_dataword = combine_float(32, gyro.mx, parameters.output_dataword);                                     //58
//    parameters.output_dataword = combine_float(32, dummy_value, parameters.output_dataword);                                 //59
//    parameters.output_dataword = combine_float(32, gyro.my, parameters.output_dataword);                                     //60
//    parameters.output_dataword = combine_float(32, dummy_value, parameters.output_dataword);                                 //61
//    parameters.output_dataword = combine_float(32, gyro.mz, parameters.output_dataword);                                     //62
//    parameters.output_dataword = combine_float(32, dummy_value, parameters.output_dataword);                                 //63
//    // Quaternion consists of 4 32-bit floats. Only including vector component zero is a known issue. Pivotal bug ID 95940810
//    parameters.output_dataword = combine_float(32, gyro.gx, parameters.output_dataword);                                     //64
//    // Euler pose is a 3-dimensional vector. Only including one vector component here is a known issue. Pivotal bug ID 95940810
//    parameters.output_dataword = combine_float(32, gyro.gy, parameters.output_dataword);                                     //65
//    parameters.output_dataword = combine_float(32, gyro.gz, parameters.output_dataword);                                     //66
//    parameters.output_dataword = combine(32, gyro.gyro_temp, parameters.output_dataword);                                    //67 
//    parameters.output_dataword = combine(8, parameters.voltage_sanity_check_high, parameters.output_dataword);               //68
//    parameters.output_dataword = combine(8, parameters.voltage_sanity_check_low, parameters.output_dataword);                //69
//    parameters.output_dataword = combine(8, parameters.charge_current_sanity_check_high, parameters.output_dataword);        //70
//    parameters.output_dataword = combine(8, parameters.charge_current_sanity_check_low, parameters.output_dataword);         //71
//    parameters.output_dataword = combine(32, parameters.battery_1_recharge_ratio, parameters.output_dataword);               //72
//    parameters.output_dataword = combine(32, parameters.battery_1_amphrs_charging, parameters.output_dataword);              //73
//    parameters.output_dataword = combine(32, parameters.battery_1_amphrs_discharging, parameters.output_dataword);           //74
//    parameters.output_dataword = combine(8, parameters.battery_1_amphrs_term_threshold, parameters.output_dataword);         //75
//    parameters.output_dataword = combine(8, parameters.battery_1_amphrs_init_threshold, parameters.output_dataword);         //76
//    parameters.output_dataword = combine(8, parameters.battery_1_voltage_term_threshold, parameters.output_dataword);        //77
//    parameters.output_dataword = combine(8, parameters.battery_1_voltage_init_threshold, parameters.output_dataword);        //78
//    parameters.output_dataword = combine(32, parameters.battery_2_recharge_ratio, parameters.output_dataword);               //79
//    parameters.output_dataword = combine(32, parameters.battery_2_amphrs_charging, parameters.output_dataword);              //80
//    parameters.output_dataword = combine(32, parameters.battery_2_amphrs_discharging, parameters.output_dataword);           //81
//    parameters.output_dataword = combine(8, parameters.battery_2_amphrs_term_threshold, parameters.output_dataword);         //82
//    parameters.output_dataword = combine(8, parameters.battery_2_amphrs_init_threshold, parameters.output_dataword);         //83
//    parameters.output_dataword = combine(8, parameters.battery_2_voltage_term_threshold, parameters.output_dataword);        //84
//    parameters.output_dataword = combine(8, parameters.battery_2_voltage_init_threshold, parameters.output_dataword);        //85
//    
//    parameters.output_dataword = combine(16, parameters.battery_temperature_limit_high, parameters.output_dataword);         //86
//    parameters.output_dataword = combine(16, parameters.battery_temperature_limit_low, parameters.output_dataword);          //87
//    parameters.output_dataword = combine(16, parameters.battery_temperature_sanity_check_high, parameters.output_dataword);  //88
//    parameters.output_dataword = combine(16, thresholds.normal_battery_temperature_limit_high, parameters.output_dataword);  //89
//    parameters.output_dataword = combine(16, thresholds.normal_battery_temperature_limit_low, parameters.output_dataword);   //90
//    parameters.output_dataword = combine(16, thresholds.survival_battery_temperature_limit_high, parameters.output_dataword);//91
//    parameters.output_dataword = combine(16, thresholds.survival_battery_temperature_limit_low, parameters.output_dataword); //92
//    parameters.output_dataword = combine(16, parameters.battery_temperature_sanity_check_low, parameters.output_dataword);   //93
//    parameters.output_dataword = combine(8, parameters.low_voltage_limit_for_loadshed_entry, parameters.output_dataword);    //94
//    parameters.output_dataword = combine(8, parameters.low_voltage_limit_for_auto_cutdown, parameters.output_dataword);      //95
//    parameters.output_dataword = combine(8, parameters.low_voltage_time_limit, parameters.output_dataword);                  //96
//    parameters.output_dataword = combine(16, parameters.altitude_limit_low, parameters.output_dataword);                     //97
//    parameters.output_dataword = combine(16, parameters.altitude_sanity_check_low, parameters.output_dataword);              //98
//    parameters.output_dataword = combine(8, parameters.cutdown_pulse_width, parameters.output_dataword);                     //99
//    parameters.output_dataword = combine(12, parameters.camera_period, parameters.output_dataword);                          //100
//    parameters.output_dataword = combine(12, parameters.camera_on_time, parameters.output_dataword);                         //101
//
//    // Start Bilevel Parent TLM 
//    parameters.output_dataword = combine(1, parameters.battery_1_charging_status, parameters.output_dataword);                //102-1
//    parameters.output_dataword = combine(1, parameters.battery_2_charging_status, parameters.output_dataword);                //102-2    
//    parameters.output_dataword = combine(1, parameters.battery_bus_low_voltage_flag, parameters.output_dataword);             //102-3
//    parameters.output_dataword = combine(1, parameters.heater_state_1, parameters.output_dataword);                           //102-4
//    parameters.output_dataword = combine(1, parameters.heater_state_2, parameters.output_dataword);                           //102-5
//    parameters.output_dataword = combine(1, parameters.cutdown_enable_state, parameters.output_dataword);                     //102-6
//    parameters.output_dataword = combine(1, parameters.cutdown_1_status, parameters.output_dataword);                         //102-7
//    parameters.output_dataword = combine(1, parameters.cutdown_2_status, parameters.output_dataword);                         //102-8
//    
//    parameters.output_dataword = combine(1, parameters.altitude_valid_flag, parameters.output_dataword);                      //102-9
//    parameters.output_dataword = combine(1, parameters.camera_enabled, parameters.output_dataword);                           //102-10
//    parameters.output_dataword = combine(1, parameters.camera_status, parameters.output_dataword);                            //102-11
//    parameters.output_dataword = combine(1, parameters.battery_1_temp_tlm_valid_flag, parameters.output_dataword);            //102-12 
//    parameters.output_dataword = combine(1, parameters.battery_2_temp_tlm_valid_flag, parameters.output_dataword);            //102-13
//    parameters.output_dataword = combine(1, parameters.battery_voltage_tlm_valid_flag, parameters.output_dataword);           //102-14
//    parameters.output_dataword = combine(1, parameters.battery_1_current_tlm_valid_flag, parameters.output_dataword);         //102-15 
//    parameters.output_dataword = combine(1, parameters.battery_2_current_tlm_valid_flag, parameters.output_dataword);         //102-16 
//      
//    parameters.output_dataword = combine_float(32,alt.altitude_in_feet, parameters.output_dataword);                          //103
//    parameters.output_dataword = combine_float(32,alt.temperature, parameters.output_dataword);                               //104
//    parameters.output_dataword = combine_float(32,alt.pressure, parameters.output_dataword);                                  //105 
//
//    parameters.output_dataword = combine(8, parameters.num_rb_words_recieved, parameters.output_dataword);                    //106
//    parameters.output_dataword = parameters.output_dataword + "000000000000000000000000";                                     //107
  //}
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
//  // Process the Spare Transmit Format
//  if (parameters.vehicle_mode == 5){  
//    
//  }
//}



