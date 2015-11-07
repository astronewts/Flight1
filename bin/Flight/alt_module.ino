
void collect_alt_data()
{
   alt.altitude_in_feet = baro.getHeightCentiMeters() / 30.48;
   alt.temperature = baro.getTemperatureCentigrade() / 100.0;
   alt.pressure = baro.getAvgNormPressurePascals();
}

void print_alt_data()
{ 
  Serial.println("-----------ALTIMITER Telemetry---------------"); 
  Serial.print("Alt: Altitude in Feet: ");
  Serial.println(alt.altitude_in_feet);
  Serial.print("Alt: Temp:");
  Serial.println(alt.temperature);
  Serial.print("Alt: Pressure:");
  Serial.println(alt.pressure);
  Serial.println(" ");
}
