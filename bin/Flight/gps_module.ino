#include "Arduino.h"

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
static char sz[64];
    
// The TinyGPS++ object
TinyGPSPlus gps;

void collect_gps_data()
{
  if(Serial1.available())
  {
   // smartDelay ensures that the GPS is fed 
   smartDelay(10);
  }
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS data received: check wiring"));
  }
}

void print_gps_data()
{
  Serial.println("***INTERNAL DATA***");
  Serial.print("GPS Characters Processed: ");
  Serial.println(gps.charsProcessed());  
  Serial.print("GPS Sentences With Fix: ");
  Serial.println(gps.sentencesWithFix());
  Serial.print("GPS Failed CheckSum: ");
  Serial.println(gps.failedChecksum());
  Serial.print("GPS Passed CheckSum: ");
  Serial.println(gps.passedChecksum());
  
  Serial.println("***EXTERNAL DATA***");
  Serial.print("GPS Nb of satellites: ");
  Serial.print(gps.satellites.value()); 
  Serial.print("  Validity: ");
  Serial.println(gps.satellites.isValid());
  Serial.print("GPS HDOP Value: ");
  Serial.print(gps.hdop.value());
  Serial.print("  Validity: ");
  Serial.println(gps.hdop.isValid());
  Serial.print("GPS Latitude [deg]: ");
  Serial.println(gps.location.lat());
  Serial.print("GPS Longtitude [deg]: ");
  Serial.println(gps.location.lng());
  Serial.print("GPS Location Age: ");
  Serial.print(gps.location.age());
  Serial.print("  Validity: ");
  Serial.println(gps.location.isValid());
  Serial.print("GPS date: ");
  Serial.print(gps.date.value());
  Serial.print("  Validity: ");
  Serial.println(gps.date.isValid()); 
  Serial.print("GPS time: ");
  Serial.print(gps.time.value()); 
  Serial.print("  Validity: ");
  Serial.println(gps.time.isValid()); 
  Serial.print("GPS Altitude [m]: ");
  Serial.print(gps.altitude.meters());
  Serial.print("  Validity: ");
  Serial.println(gps.altitude.isValid()); 
  Serial.print("GPS Course Deg: ");
  Serial.print(gps.course.deg());
  Serial.print("  Validity: ");
  Serial.println(gps.course.isValid()); 
  Serial.print("GPS Speed [km/h]: ");
  Serial.print(gps.speed.kmph());
  Serial.print("  Validity: ");
  Serial.println(gps.speed.isValid()); 
  
  Serial.print("GPS Course Cardinal: ");
  Serial.println(TinyGPSPlus::cardinal(gps.course.value()));
  Serial.print("\n");
}

// This custom version of delay() ensures that the gps object is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {   
      while(Serial1.available())
        gps.encode(Serial1.read());
      
  } while (millis() - start < ms);
}

