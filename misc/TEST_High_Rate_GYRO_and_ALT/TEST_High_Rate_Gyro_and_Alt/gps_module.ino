#include "Arduino.h"

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
//static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
static char sz[64];
    

// The TinyGPS++ object
TinyGPSPlus gps;

void get_gps_data()
{
  if(Serial1.available())
  {
     gps.encode(Serial1.read());
  }
  
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS data received: check wiring"));
  }
}

void print_gps_data()
{
  get_gps_data();
  
  Serial.println(F("Sats HDOP Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card   Chars Sentences Checksum"));
  Serial.println(F("          (deg)      (deg)       Age                      Age  (m)    --- from GPS ----    RX    RX        Fail"));
  Serial.println(F("--------------------------------------------------------------------------------------------------------------------"));

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printInt(gps.hdop.value(), gps.hdop.isValid(), 5);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);
  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  Serial.println(" ");
  
//  Serial.print("GPS Characters Processed: ");
//  Serial.println(gps.charsProcessed());
//  Serial.print("GPS Sentences With Fix: ");
//  Serial.println(gps.sentencesWithFix());
//  Serial.print("GPS Failed CheckSum: ");
//  Serial.println(gps.failedChecksum()); 
//  Serial.print("GPS Latitude: ");
//  Serial.println(gps.location.lat());
//  Serial.print("GPS Longtitude: ");
//  Serial.println(gps.location.lng());
//  Serial.print("GPS Location Age: ");
//  Serial.println(gps.location.age());
//  Serial.print("GPS Timestamp: ");
//  sprintf(sz, "%02d/%02d/%02dT%02d:%02d:%02d ", gps.date.month(), gps.date.day(), gps.date.year(),
//                                                gps.time.hour(), gps.time.minute(), gps.time.second());
//  Serial.println(sz);
//  Serial.print("GPS Altitude: ");
//  Serial.println(gps.altitude.meters());
//  Serial.print("GPS Course Deg: ");
//  Serial.println(gps.course.deg());
//  Serial.print("GPS Speed: ");
//  Serial.println(gps.speed.kmph());
//  Serial.print("GPS Course Cardinal: ");
//  Serial.println(TinyGPSPlus::cardinal(gps.course.value()));

/*
  Serial.println(F("Sats HDOP Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F("          (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail"));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------------------------"));
  
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printInt(gps.hdop.value(), gps.hdop.isValid(), 5);
  printdouble(gps.location.lat(), gps.location.isValid(), 11, 6);
  printdouble(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printdouble(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printdouble(gps.course.deg(), gps.course.isValid(), 7, 2);
  printdouble(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);
  
  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);

  unsigned long distanceKmToLondon = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(),
                                                                                 gps.location.lng(),
                                                                                 LONDON_LAT, 
                                                                                 LONDON_LON) / 1000;
  printInt(distanceKmToLondon, gps.location.isValid(), 9);

  double courseToLondon = TinyGPSPlus::courseTo(gps.location.lat(),
                                                gps.location.lng(),
                                                LONDON_LAT, 
                                                LONDON_LON);

  printdouble(courseToLondon, gps.location.isValid(), 7, 2);

  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);
  smartDelay(1000);
*/
}

// This custom version of delay() ensures that the gps object
// is being "fed".
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

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printdouble(double val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
}
