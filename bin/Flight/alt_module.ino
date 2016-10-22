void collect_alt_data()
{
   alt.altitude_in_meters = baro.getHeightCentiMeters() / 100.0;
   alt.temperature = baro.getTemperatureCentigrade() / 100.0;
   alt.pressure = baro.getAvgNormPressurePascals();
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
