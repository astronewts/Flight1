#include "Arduino.h"


// *****************************************************************************

// file system object
SdFat sd;

// text file for logging
ofstream logfile;
ArduinoOutStream cout(Serial);
// buffer to format data - makes it eaiser to echo to Serial
char buf[80];
//------------------------------------------------------------------------------
#if SENSOR_COUNT > 6
#error SENSOR_COUNT too large
#endif  // SENSOR_COUNT
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
#if USE_DS1307
// use RTClib from Adafruit
// https://github.com/adafruit/RTClib

// The Arduino IDE has a bug that causes Wire and RTClib to be loaded even
// if USE_DS1307 is false.

#error remove this line and uncomment the next two lines.
//#include <Wire.h>
//#include <RTClib.h>

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

// ****************************************************************************

// Format telemetry data
// void format_telemetry_data_for_sd(struct telemetry_data_struct* telemetry_data, char* string_buffer)
// {
// }

void sd_setup() {
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
  if (!sd.begin(SD_CHIP_SELECT, SPI_HALF_SPEED)) sd.initErrorHalt();

  // create a new file in root, the current working directory
  char name[] = "LOGGER00.CSV";

  for (uint8_t i = 0; i < 100; i++) {
    name[6] = i/10 + '0';
    name[7] = i%10 + '0';
    if (sd.exists(name)) continue;
    logfile.open(name);
    break;
  }
  if (!logfile.is_open()) error("file.open");

  // format header in buffer
  obufstream bout(buf, sizeof(buf));

  bout << pstr("millis");

#if USE_DS1307
  bout << pstr(",date,time");
#endif  // USE_DS1307

  bout << pstr(",Pressure Sensor");
  
  logfile << buf << endl;

#if ECHO_TO_SERIAL
  cout << buf << endl;
#endif  // ECHO_TO_SERIAL
  
}

//Write telemetry data to SD card
//void write_telemetry_data_to_sd(char* string_buffer)
void write_telemetry_data_to_sd()
{
  uint32_t m;

  // wait for time to be a multiple of interval
  do {
    m = millis();
  } while (m % LOG_INTERVAL);

  // use buffer stream to format line
  obufstream bout(buf, sizeof(buf));

  // start with time in millis
  bout << m;

#if USE_DS1307
  DateTime now = RTC.now();
  bout << ',' << now;
#endif  // USE_DS1307


//#################################################
//############### ASTRONEWTS SD TLM ###############
//#################################################

  bout << ',' << parameters.vehicle_mode;
  bout << ',' << parameters.command_count;
  bout << ',' << thresholds.normal_transmit_rate;
  bout << ',' << thresholds.load_shed_transmit_rate;
  bout << ',' << thresholds.transit_transmit_rate; 
  bout << ',' << thresholds.emergency_transit_transmit_rate;
  bout << ',' << thresholds.test_transmit_rate;
  bout << ',' << parameters.sd_card_write_rate;
  bout << ',' << telemetry_data.battery_temp_1_1;
  bout << ',' << telemetry_data.battery_temp_1_2;
  bout << ',' << telemetry_data.battery_temp_2_1;
  bout << ',' << telemetry_data.battery_temp_2_2;
  bout << ',' << telemetry_data.inner_external_temp;
  bout << ',' << telemetry_data.outter_external_temp;
  bout << ',' << telemetry_data.internal_temp;
  bout << ',' << telemetry_data.air_pressure;
  bout << ',' << telemetry_data.battery_voltage_1;
  bout << ',' << telemetry_data.battery_voltage_2;
  bout << ',' << telemetry_data.charge_current_1;
  bout << ',' << telemetry_data.charge_current_2;
  bout << ',' << gps.location.lat();  
  bout << ',' << gps.location.lng(); 
  bout << ',' << gps.altitude.meters(); 
  bout << ',' << gps.location.age(); 
  bout << ',' << gps.altitude.age(); 
  bout << ',' << gps.course.deg(); 
  bout << ',' << gps.speed.kmph();  
  bout << ',' << gps.satellites.value();
//  bout << ',' << gps.date;
//  bout << ',' << gps.time;
  bout << ',' << gps.hdop.value();
  bout << ',' << gps.charsProcessed();
  bout << ',' << gps.sentencesWithFix();
  bout << ',' << gps.failedChecksum(); 
// ADD VALID STRINGS FOR GPS!!!
  bout << ',' << calData.accelMinX;
  bout << ',' << calData.accelMaxX;
  bout << ',' << calData.accelMinY;
  bout << ',' << calData.accelMaxY;
  bout << ',' << calData.accelMinZ;
  bout << ',' << calData.accelMaxZ;
  bout << ',' << calData.magMinX;
  bout << ',' << calData.magMaxX;  
  bout << ',' << calData.magMinY;
  bout << ',' << calData.magMaxY;  
  bout << ',' << calData.magMinZ;
  bout << ',' << calData.magMaxZ;    
  bout << ',' << dueMPU.m_rawQuaternion; 
  bout << ',' << dueMPU.m_dmpEulerPose; 
  bout << ',' << dueMPU.m_fusedEulerPose;   
// TODO: ADD GYRO TEMP TELEMETRY
  bout << ',' << telemetry_data.temp_fault_flag; 
  bout << ',' << telemetry_data.charge_flag; 
  bout << ',' << parameters.batttery_charge_shutdown; 
  bout << ',' << parameters.recharge_ratio; 
  bout << ',' << parameters.charge_current_sanity_check_high; 
  bout << ',' << parameters.charge_current_sanity_check_low;   
  bout << ',' << parameters.amphrs_charging; 
  bout << ',' << parameters.amphrs_discharging; 
  bout << ',' << parameters.capacity_limit_high; 
  bout << ',' << parameters.capacity_limit_low; 
  bout << ',' << parameters.voltage_sanity_check_high; 
  bout << ',' << parameters.voltage_power_limit_high; 
  bout << ',' << parameters.voltage_power_limit_low;   
  bout << ',' << parameters.low_voltage_limit;    
  bout << ',' << parameters.voltage_sanity_check_low;  
  bout << ',' << parameters.low_voltage_time_limit;  
  bout << ',' << parameters.battery_low_voltage_flag;  
  bout << ',' << parameters.heater_state_1;  
  bout << ',' << parameters.heater_state_2;  
  bout << ',' << parameters.battery_temperature_limit_high;  
  bout << ',' << parameters.battery_temperature_limit_low;  
  bout << ',' << thresholds.normal_battery_temperature_limit_high;  
  bout << ',' << thresholds.normal_battery_temperature_limit_low;  
  bout << ',' << thresholds.survival_battery_temperature_limit_high;  
  bout << ',' << thresholds.survival_battery_temperature_limit_low;  
  bout << ',' << parameters.battery_temperature_sanity_check_low; 
  bout << ',' << parameters.pyro_enable; 
  bout << ',' << parameters.pyro_1_status; 
  bout << ',' << parameters.pyro_2_status; 
  bout << ',' << parameters.pyro_pulse_width; 
  bout << ',' << parameters.camera_flag; 
  bout << ',' << parameters.camera_period; 
  bout << ',' << parameters.camera_on_time;   
  bout << ',' << parameters.altitude_valid_flag; 
  bout << ',' << parameters.altitude_limit_low; 
  bout << ',' << parameters.altitude_sanity_check_low;   
 
  bout << endl;
  
  // log data and flush to SD
  logfile << buf << flush;

  // check for error
  if (!logfile) error("write data failed");

#if ECHO_TO_SERIAL
  cout << buf;
#endif  // ECHO_TO_SERIAL

  // don't log two points in the same millis
  if (m == millis()) delay(1);
  
//##########################################
//##########################################
//##########################################
  if (!Serial.available()) return;
  logfile.close();
  cout << pstr("Done!");
  while (1);
  
}
