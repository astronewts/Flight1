//Format telemetry data
void format_telemetry_data_for_sd(struct telemetry_data_struct* telemetry_data, string_buffer);

//Write telemetry data to SD card
void write_telemetry_data_to_sd(string_buffer);

//Format satellite data
void format_satellite_data_for_sd(struct satellite_command_struct* sattelite_data, string_buffer);

//Write satellie data to SD card
void write_satallite_data_to_sd(string_buffer);
