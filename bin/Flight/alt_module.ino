void collect_alt_data()
{
   if (bme.begin()) {  
     alt.altitude_in_meters = bme.readAltitude(ALTIMETER_CALIBRATION_CONSTANT); // ADJUST THE INPUT HERE!!!!
     alt.temperature = bme.readTemperature();
     alt.pressure = bme.readPressure(); 
   }
}

void print_alt_data()
{ 
  Serial.println("-----------ALTIMITER Telemetry---------------"); 
  if (!bme.begin()) { 
     Serial.println("");
     Serial.println("ALTIMETER IS NOT WORKING!!!! NOOOOOOOOOOO!!!!!!!!");
  }
  else {
    Serial.print("Alt: Altitude in Meters: ");
    Serial.println(alt.altitude_in_meters);
    Serial.print("Alt: Temp:");
    Serial.println(alt.temperature);
    Serial.print("Alt: Pressure:");
    Serial.println(alt.pressure);
    Serial.println(" ");
  }
}
