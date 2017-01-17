#include "Arduino.h"

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
static char sz[64];
    
// The TinyGPS++ object
TinyGPSPlus gps;

void collect_gps_data(struct gps_data_struct data)

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

  gps_data.gps_location_valid = gps.location.isValid();
  if (gps.location.isValid()) {
    unsigned int lat, lng;
    //encode_lat_long(gps.location.lat(), gps.location.lng(), &lat, &lng);
    //data.gps_processed_lat = lat;
    //data.gps_processed_long = lng;
    gps_data.gps_processed_lat = gps.location.lat();
    gps_data.gps_processed_long = gps.location.lng();
    //data.gps_location_age = constrain(gps.location.age() / 1000, 0, 255);
    gps_data.gps_location_age = gps.location.age();
    //data.gps_altitude = constrain(gps.altitude.meters(), 0, (1<<16)-1);
    gps_data.gps_altitude = gps.altitude.meters();

    gps_data.gps_altitude = gps.altitude.meters();
    
    gps_data.gps_altitude_age = gps.altitude.age();
    //data.gps_heading = constrain(gps.course.deg() * 360 / 256, 0, 255);
    gps_data.gps_heading = gps.course.deg();
    //data.gps_speed = constrain(gps.speed.kmph(), 0, 255);
    gps_data.gps_speed = gps.speed.kmph();
    
    gps_data.gps_date = gps.date.value();
    gps_data.gps_time = gps.time.value();
    gps_data.gps_hdop = gps.hdop.value();
    gps_data.gps_chars_processed = gps.charsProcessed();
    gps_data.gps_sentances_with_fix = gps.sentencesWithFix();
    gps_data.gps_failed_checksum = gps.failedChecksum();
    
    gps_data.gps_location_valid = gps.location.isValid();
    gps_data.gps_altitude_valid = gps.altitude.isValid();
    gps_data.gps_heading_valid = gps.course.isValid();
    gps_data.gps_speed_valid = gps.speed.isValid();
    gps_data.gps_num_satellites_valid = gps.satellites.isValid();
    gps_data.gps_date_valid = gps.date.isValid();
    gps_data.gps_time_valid = gps.time.isValid();
    gps_data.gps_hdop_valid = gps.hdop.isValid();
 
    gps_data.gps_num_satellites = gps.satellites.value(); 
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

