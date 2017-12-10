#include "Arduino.h"

// *****************************************************************************

// file system object
SdFat sd;

int linecount = 0;
int max_num_lines = 500000;

// set up variables using the SD utility library functions:
//Sd2Card card;
//SdVolume volume;
//SdFile s_file;

// text file for logging
ofstream logfile;
ArduinoOutStream cout(Serial);
// buffer to format data - makes it eaiser to echo to Serial
char buf[2500];

//define parent monitor string for GPS Isvalid data
String gps_isvalid_str;

//------------------------------------------------------------------------------
//#if SENSOR_COUNT > 6
//#error SENSOR_COUNT too large
//#endif  // SENSOR_COUNT
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
//#define error(s) sd.errorHalt_P(PSTR(s))
#define error(s) sd.errorPrint(PSTR(s))
//------------------------------------------------------------------------------
#if USE_DS1307
// use RTClib from Adafruit
// https://github.com/adafruit/RTClib

// The Arduino IDE has a bug that causes Wire and RTClib to be loaded even
// if USE_DS1307 is false.


RTC_DS1307 RTC;  // define the Real Time Clock object
//------------------------------------------------------------------------------
// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {
  DateTime now = RTC.now();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year(), now.month(), now.day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}
//------------------------------------------------------------------------------
// format date/time
ostream& operator << (ostream& os, DateTime& dt) {
  os << dt.year() << '/' << int(dt.month()) << '/' << int(dt.day()) << ',';
  os << int(dt.hour()) << ':' << setfill('0') << setw(2) << int(dt.minute());
  os << ':' << setw(2) << int(dt.second()) << setfill(' ');
  return os;
}
#endif  // USE_DS1307

ostream& operator << (ostream& os, String& s) {
  for (int i=0;i<s.length();i++){
    os << s.charAt(i);
  }
  return os;
}

// ****************************************************************************

// Format telemetry data
// void format_telemetry_data_for_sd(struct telemetry_data_struct* telemetry_data, char* string_buffer)
// {
// }

void sd_setup() {

  while (!Serial){
    delay(400);
  }

  cout << endl << pstr("FreeRam: ") << FreeRam() << endl;

#if USE_DS1307
  // connect to RTC
  Wire.begin();
  if (!RTC.begin()) error("RTC failed");

  // set date time callback function
  SdFile::dateTimeCallback(dateTime);
  DateTime now = RTC.now();
  cout  << now << endl;
#endif  // USE_DS1307

  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  if (!sd.begin(SD_CHIP_SELECT, SPI_HALF_SPEED)) sd.initErrorPrint();

  // create a new file in root, the current working directory
  //char name[] = "LOGxxx.MODEx.CSV";
  char name[] = "LOGxxx.CSV";

 // name[11] = (uint8_t) parameters.vehicle_mode + '0';
  
  for (uint16_t i = 0; i < 1000; i++) 
  {
    name[3] = (uint8_t) (i / 100) + '0';
    name[4] = (uint8_t) ((i % 100) / 10) + '0';
    name[5] = (uint8_t) (i % 10) + '0';
    if (sd.exists(name)) continue;
    logfile.open(name);
    cout << name << endl;
    parameters.sd_card_num = (int)i;
    break;
  }
 
  //parameters.sd_card_num = ((int)temp).toInt();
  
  if (!logfile.is_open()) error("file.open");

  // format header in buffer
  obufstream bout(buf, sizeof(buf));

  bout << pstr("Elapsed Time [ms]");                                 

  initialize_database();
  
  for (int i=0; i<(DB_SIZE); i++) 
  { 
    bout << ','; 
    bout << db[i].SD_Card_Title;
  }
  
  logfile << buf << endl << flush;
}

//Write telemetry data to SD card
//void write_telemetry_data_to_sd(char* string_buffer)
void write_telemetry_data_to_sd()
{
  uint32_t m;
  double dummy_value;
  dummy_value=0;
  linecount = linecount + 1; 

  //if(sd.exists(name))
  //{
  //  Serial.println("The file exists!!!");
  //  Serial.println(sd.chvol());
  //}
  
  if(linecount > max_num_lines)
  {
    //  debug_println("FILE SIZE EXCEEDED! Closing FILE!");
    logfile.close();
    //  debug_println("OPENING A NEW FILE!");
    sd_setup();
    linecount = 0;
  }
  
  if (logfile.is_open())
  {
  // wait for time to be a multiple of interval
  //do {
  m = millis();
  //  } while (m % LOG_INTERVAL);

  // use buffer stream to format line
  obufstream bout(buf, sizeof(buf));
  bout << setprecision(8);
  if (debug.mode==1){
    Serial.print("=> DEBUG: WRITE SD_CARD AT TIME: ");
    Serial.println(m);
  }
  // start with time in millis
  bout << m;

#if USE_DS1307
  DateTime now = RTC.now();
  
//  bout << ',' << now;                                                  //0
    bout << ',' << parameters.time_since_start;   
#endif  // USE_DS1307

  //#################################################
  //############### ASTRONEWTS SD TLM ###############
  //#################################################

  initialize_database();
  
  for (int i=0; i<(DB_SIZE); i++) 
  { 
      if (db[i].tlm_type == "float") { bout << ',' << db[i].float_pointer; }
      else if (db[i].tlm_type == "long") { bout << ',' << db[i].long_pointer; }
      else if (db[i].tlm_type == "int") { bout << ',' << db[i].int_pointer; }
      else if (db[i].tlm_type == "header") { bout << ',' << "10101010"; }
  }
 
  bout << endl;

  // log data and flush to SD
  logfile << buf << flush;
  
  // check for error
  if (!logfile) error("SD card write data failed");

  // don't log two points in the same millis
  if (m == millis()) delay(1);

  //##########################################
  //##########################################
  //##########################################
  // GL and LC (july 06 2016) suspect that was the 
  // weird "DONE" issue. to work correctly the 
  // Serial must have been not available all the time ?????
  
//  if (!Serial.available())
//  {
//  logfile.close();
//  cout << pstr("Serial was not available, close SD file and end everything!");
//  // while (1);
//  return;
//  }
  
  } // for sthg above
  

}
