#include "Arduino.h"

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
//static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
static char sz[64];
    
// The TinyGPS++ object
TinyGPSPlus gps;


void collect_gps_data()
{
  if(Serial1.available())
  {
   // smartDelay ensures that the GPS is fed 
   smartDelay(0);
  }
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS data received: check wiring"));
  }
}


void print_gps_data()
{
  Serial.print("GPS Characters Processed: ");
  Serial.println(gps.charsProcessed());
  Serial.print("GPS Sentences With Fix: ");
  Serial.println(gps.sentencesWithFix());
  Serial.print("GPS Failed CheckSum: ");
  Serial.println(gps.failedChecksum());
  Serial.print("GPS Nb of satellites: ");
  Serial.println(gps.satellites.value()); 
  Serial.print("GPS Latitude [deg]: ");
  Serial.println(gps.location.lat());
  Serial.print("GPS Longtitude [deg]: ");
  Serial.println(gps.location.lng());
  Serial.print("GPS Location Age: ");
  Serial.println(gps.location.age());
  Serial.print("GPS date: ");
  Serial.println(gps.date.value());
  Serial.print("GPS time: ");
  Serial.println(gps.time.value()); 
  Serial.print("GPS Altitude [m]: ");
  Serial.println(gps.altitude.meters());
  Serial.print("GPS Course Deg: ");
  Serial.println(gps.course.deg());
  Serial.print("GPS Speed [km/h]: ");
  Serial.println(gps.speed.kmph());
  Serial.print("GPS Course Cardinal: ");
  Serial.println(TinyGPSPlus::cardinal(gps.course.value()));
}

// This custom version of delay() ensures that the gps object is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    /*while (ss.available())
      gps.encode(ss.read());*/
      
      while(Serial1.available())
        gps.encode(Serial1.read());
      
      
  } while (millis() - start < ms);
}

