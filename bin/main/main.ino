void setup() 
{
   char* string_buffer = (char*)malloc(512);;
}

void loop() 
{
   //Process satellite commands from ROCKBLOCK, blocking unless it's time to make a loop cycle
   // 1 - Cord Snap
   // 2 - Turn on/off feed from solar panels
   // 3 - Set hi/low temperature thresholds 
   // 4 - Set sampling rate
   // 5 - Set camera rate
   // 6 - Set data format
   process_satellite_command(&satellite_commands);
   
   //Write satellie command to SD card, if any
   write_satallite_command_to_sd(string_buffer);   
   
   //Collect telemetry
   colect_telemetry(struct telemetry_data_struct* telemetry_data);
   
   //Is there anything to act on from tolemety read?
   // 1 - Turn heating element on/off if temperature breaks threshold()
   // 2 - Check voltage time() to check if cutdown is needed
   // 3 - Mode change (Low/Normal/High)
   process_telemetry(&telemetry_data);
   
   //Format telemetry, realloc if need be
   format_telemetry_for_sd(&telemetry_data, string_buffer);
   
   //Write telemetry data to SD card
   write_telemetry_to_sd(string_buffer);
   
   //Formate telemetry, realloc if need be
   format_telemetry_for_satellite(&telemetry_data, string_buffer);
   
   //Transmit telemetry data to satellite
   transmit_telemetry_to_satellite(&telemetry_data);
   
   //Receive any data from satellite
   receive_from_satellite(struct satellite_command_struct* satellite_commands);

   //Format received satellite data, realloc if need be
   format_satellite_data_for_sd(&satellite_commands, string_buffer);  
   
   //Is it time to turn camera on/off?
   process_camera_function();

   //Either sleep or continue looping, based on battery draw tests
}
