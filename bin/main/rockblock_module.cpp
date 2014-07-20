//Receive any data from satellite
read_satellite_command(struct satellite_command_struct* satellite_commands);
 

 //Format received satellite data, realloc if need be
format_satellite_data_for_sd(&satellite_commands, string_buffer);  
