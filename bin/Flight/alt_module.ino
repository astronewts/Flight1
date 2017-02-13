void collect_alt_data()
{
   // alt.altitude_in_meters = (baro.getHeightCentiMeters() / 100.0)+60; // NOTE: +60 is a TEST OFFSET ONLY REMOVE FOR FLIGHT (maybe)
   // alt.temperature = baro.getTemperatureCentigrade() / 100.0;
   // alt.pressure = baro.getAvgNormPressurePascals();
   if (bme.begin()) {  
     alt.altitude_in_meters = bme.readAltitude(ALTIMETER_CALIBRATION_CONSTANT); // ADJUST THE INPUT HERE!!!!
     alt.temperature = bme.readTemperature();
     alt.pressure = bme.readPressure(); 
   }
}

void print_alt_data()
{ 
  Serial.println("-----------ALTIMITER Telemetry---------------"); 
  Serial.print("Alt: Altitude in Feet: ");
  Serial.println(alt.altitude_in_meters);
  Serial.print("Alt: Temp:");
  Serial.println(alt.temperature);
  Serial.print("Alt: Pressure:");
  Serial.println(alt.pressure);
  Serial.println(" ");
}
