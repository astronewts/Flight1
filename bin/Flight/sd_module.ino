#include "Arduino.h"

// *****************************************************************************

// file system object
SdFat sd;

// text file for logging
ofstream logfile;
ArduinoOutStream cout(Serial);
// buffer to format data - makes it eaiser to echo to Serial
char buf[2400];

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
  char name[] = "LOGxxx.CSV";

  for (uint16_t i = 0; i < 1000; i++) {
    name[3] = (uint8_t) (i / 100) + '0';
    name[4] = (uint8_t) ((i % 100) / 10) + '0';
    name[5] = (uint8_t) (i % 10) + '0';
    if (sd.exists(name)) continue;
    logfile.open(name);
    cout << name << endl;
    break;
  }
  if (!logfile.is_open()) error("file.open");

  // format header in buffer
  obufstream bout(buf, sizeof(buf));

  bout << pstr("millis");

#if USE_DS1307
  bout << pstr(",date,time");
#endif  // USE_DS1307

  bout << pstr(",Vehicle Mode");                        //1
  bout << pstr(",Command Count");                       //2
  bout << pstr(",Normal Transmit Rate");                //3
  bout << pstr(",Loadshed Transmit Rate");              //4
  bout << pstr(",Transit Transmit Rate");               //5
  bout << pstr(",Emergency Transit Transmit Rate");     //6
  bout << pstr(",Spare Transmit Rate");                 //7
  bout << pstr(",SD Card Write Rate");                  //8
  bout << pstr(",Battery 1 Temp 1");                    //9
  bout << pstr(",Battery 1 Temp 2");                    //10
  bout << pstr(",Battery 2 Temp 1");                    //11
  bout << pstr(",Battery 2 Temp 2");                    //12
  bout << pstr(",Inner External Temp");                 //13
  bout << pstr(",Outer External Temp");                 //14
  bout << pstr(",Internal Temp");                       //15
  bout << pstr(",Air Pressure");                        //16
  bout << pstr(",Battery 1 Voltage Pri");               //17
  bout << pstr(",Battery 1 Voltage Red");               //18
  bout << pstr(",Battery 1 Charge Current 1");          //19
  bout << pstr(",Battery 1 Charge Current 2");          //20
  // TODO: I THINK WE'RE ACTUALLY OUTPUTTING TEMP CONSTANTS IN SPARE 1 & 2
  bout << pstr(",Spare 1");                             //21  
  bout << pstr(",Spare 2");                             //22
  bout << pstr(",Telemetry Processing Period");         //23
  bout << pstr(",Spare 3");                             //24
  bout << pstr(",GPS Latitude");                        //25
  bout << pstr(",GPS Longitude");                       //26
  bout << pstr(",GPS Altitude");                        //27
  bout << pstr(",GPS Location Age");                    //28
  bout << pstr(",GPS Altitude Age");                    //29
  bout << pstr(",GPS Course");                          //30
  bout << pstr(",GPS Speed");                           //31
  bout << pstr(",GPS # of Satellites");                 //32
  bout << pstr(",GPS Date");                            //33
  bout << pstr(",GPS Time");                            //34
  bout << pstr(",HDOP Value");                          //35
  bout << pstr(",GPS Chars Processed");                 //36
  bout << pstr(",GPS Sentences with Fix");              //37
  bout << pstr(",GPS Failed Checksum");                 //38
  bout << pstr(",GPS Isvalid Conglomerate");            //39
  bout << pstr(",Acceleration Min X");                  //40
  bout << pstr(",Acceleration Max X");                  //41
  bout << pstr(",Acceleration Min Y");                  //42
  bout << pstr(",Acceleration Max Y");                  //43
  bout << pstr(",Acceleration Min Z");                  //44
  bout << pstr(",Acceleration Max Z");                  //45
  bout << pstr(",Magnetometer Min X");                  //46
  bout << pstr(",Magnetometer Max X");                  //47
  bout << pstr(",Magnetometer Min Y");                  //48
  bout << pstr(",Magnetometer Max Y");                  //49
  bout << pstr(",Magnetometer Min Z");                  //50
  bout << pstr(",Magnetometer Max Z");                  //51
  bout << pstr(",GYRO Raw Quaternion");                 //52
  bout << pstr(",GYRO Euler Angle");                    //53
  bout << pstr(",GYRO Fused Euler Angle");              //54
  bout << pstr(",Accelerometer Temp");                  //55
  bout << pstr(",Voltage Sanity Check High");           //56
  bout << pstr(",Voltage Sanity Check Low");            //57  
  bout << pstr(",Charge Current Sanity High");          //58
  bout << pstr(",Charge Current Sanity Low");           //59
  bout << pstr(",Battery 1 Recharge Ratio");            //60
  bout << pstr(",Battery 1 Amp Hours Charging");        //61
  bout << pstr(",Battery 1 Amp Hours Discharging");     //62
  bout << pstr(",Battery 1 Capacity Limit High");       //63
  bout << pstr(",Battery 1 Capacity Limit Low");        //64
  bout << pstr(",Battery 1 Voltage Term. Charge Limit");//65
  bout << pstr(",Battery 1 Voltage Init. Charge Limit");//66
  bout << pstr(",Battery 2 Recharge Ratio");            //67
  bout << pstr(",Battery 2 Amp Hours Charging");        //68
  bout << pstr(",Battery 2 Amp Hours Discharging");     //69
  bout << pstr(",Battery 2 Capacity Limit High");       //70
  bout << pstr(",Battery 2 Capacity Limit Low");        //71
  bout << pstr(",Battery 2 Voltage Term. Charge Limit");//72
  bout << pstr(",Battery 2 Voltage Init. Charge Limit");//73
  bout << pstr(",Battery Active Temp Limit High");      //74
  bout << pstr(",Battery Active Temp Limit Low");       //75
  bout << pstr(",Battery Temp Sanity Check High");      //76
  bout << pstr(",Battery Normal Temp Limit High");      //77
  bout << pstr(",Battery Normal Temp Limit Low");       //78
  bout << pstr(",Battery Survival Temp Limit High");    //79
  bout << pstr(",Battery Survival Temp Limit Low");     //80
  bout << pstr(",Battery Temp Sanity Check Low");       //81  
  bout << pstr(",Loadshed Entry Voltage Limit");        //82
  bout << pstr(",Auto Cutdown Voltage Limit");          //83
  bout << pstr(",Low Voltage Time until Cut-down");     //84
  bout << pstr(",Altitude Limit Low");                  //85  
  bout << pstr(",Altiude Sanity Check Low");            //86
  bout << pstr(",Pyro Pulse Width");                    //87
  bout << pstr(",Camera Period");                       //88
  bout << pstr(",Camera On Time");                      //89
  bout << pstr(",Battery 1 Charging Status");           //90-1
  bout << pstr(",Battery 2 Charging Status");           //90-2
  bout << pstr(",Battery Bus Low Voltage Flag");        //90-3  
  bout << pstr(",Heater State 1");                      //90-4
  bout << pstr(",Heater State 2");                      //90-5
  bout << pstr(",Cut-down Enable");                     //90-6
  bout << pstr(",Cut-down 1 Fire Status");              //90-7
  bout << pstr(",Cut-down 2 Fire Status");              //90-8
  bout << pstr(",Altitude Valid Flag");                 //90-9
  bout << pstr(",Camera Enabled");                      //90-10
  bout << pstr(",Camera Status");                       //90-11
  bout << pstr(",Battery 1 Temp TLM Valid Flag");       //90-12  
  bout << pstr(",Battery 2 Temp TLM Valid Flag");       //90-13  
  bout << pstr(",Bus Voltage TLM Valid Flag");          //90-14   
  bout << pstr(",Battery 1 Current TLM Valid Flag");    //90-15
  bout << pstr(",Battery 2 Current TLM Valid Flag");    //90-16  
  bout << pstr(",Altitude (ft)");                       //91
  bout << pstr(",Alt Temp");                            //92
  bout << pstr(",Alt Pressure");                        //93 
  bout << pstr(",Spare 5");                             //94 
  logfile << buf << endl << flush;

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
  bout << setprecision(8);

  // start with time in millis
  bout << m;

#if USE_DS1307
  DateTime now = RTC.now();
  bout << ',' << now;
#endif  // USE_DS1307

//#################################################
//############### ASTRONEWTS SD TLM ###############
//#################################################

  bout << ',' << parameters.vehicle_mode;                              //1
  bout << ',' << parameters.command_count;                             //2
  bout << ',' << thresholds.normal_transmit_period;                    //3
  bout << ',' << thresholds.load_shed_transmit_period;                 //4
  bout << ',' << thresholds.transit_transmit_period;                   //5
  bout << ',' << thresholds.emergency_transit_transmit_period;         //6
  bout << ',' << thresholds.spare_transmit_period;                     //7
  bout << ',' << parameters.sd_card_write_period;                      //8
  bout << ',' << telemetry_data.battery_1_temp_1;                      //9
  bout << ',' << telemetry_data.battery_1_temp_2;                      //10
  bout << ',' << telemetry_data.battery_2_temp_1;                      //11
  bout << ',' << telemetry_data.battery_2_temp_2;                      //12
  bout << ',' << telemetry_data.inner_external_temp;                   //13
  bout << ',' << telemetry_data.outter_external_temp;                  //14 
  bout << ',' << telemetry_data.internal_temp;                         //15
  bout << ',' << telemetry_data.air_pressure;                          //16
  bout << ',' << telemetry_data.battery_1_voltage_1;                   //17
  bout << ',' << telemetry_data.battery_1_voltage_2;                   //18
  bout << ',' << telemetry_data.battery_1_charge_current_1;            //19
  bout << ',' << telemetry_data.battery_1_charge_current_2;            //20
  bout << ',' << "00000000000000000000000000000000";                   //21
  bout << ',' << "00000000000000000000000000000000";                   //22
  bout << ',' << parameters.tlm_processing_period;                     //23
  bout << ',' << "00000000000000000000000000000000";                   //24
  bout << ',' << gps.location.lat();                                   //25
  bout << ',' << gps.location.lng();                                   //26
  bout << ',' << gps.altitude.meters();                                //27
  bout << ',' << gps.location.age();                                   //28
  bout << ',' << gps.altitude.age();                                   //29
  bout << ',' << gps.course.deg();                                     //30
  bout << ',' << gps.speed.kmph();                                     //31
  bout << ',' << gps.satellites.value();                               //32
  
//NOTE: THE NEXT TWO MIGHT NEEDS SOME MASSAGING POST PROCESS

  bout << ',' << gps.date.value();                                     //33
  bout << ',' << gps.time.value();                                     //34
  bout << ',' << gps.hdop.value();                                     //35
  bout << ',' << gps.charsProcessed();                                 //36
  bout << ',' << gps.sentencesWithFix();                               //37
  bout << ',' << gps.failedChecksum();                                 //38
  
// ISVALID STRINGS FOR GPS!!!
  gps_isvalid_str = String(gps.hdop.isValid());                        //39-1
  gps_isvalid_str = gps_isvalid_str + String(gps.location.isValid());  //39-2 
  gps_isvalid_str = gps_isvalid_str + String(gps.altitude.isValid());  //39-3 
  gps_isvalid_str = gps_isvalid_str + String(gps.course.isValid());    //39-4 
  gps_isvalid_str = gps_isvalid_str + String(gps.speed.isValid());     //39-5       
  gps_isvalid_str = gps_isvalid_str + String(gps.satellites.isValid());//39-6                    
  gps_isvalid_str = gps_isvalid_str + String(gps.date.isValid());      //39-7               
  gps_isvalid_str = gps_isvalid_str + String(gps.time.isValid());      //39-8
  gps_isvalid_str = gps_isvalid_str + String(gps.hdop.isValid());      //39-9
  
  bout << ',' << gps_isvalid_str;                                       //39
                                                                       
  bout << ',' << dueMPU.m_calAccel[VEC3_X];                            //40
  bout << ',' << std::max(calData.accelMaxX, (short)(-calData.accelMinX)); //41
  bout << ',' << dueMPU.m_calAccel[VEC3_Y];                            //42
  bout << ',' << std::max(calData.accelMaxY, (short)(-calData.accelMinY)); //43
  bout << ',' << dueMPU.m_calAccel[VEC3_Z];                            //44
  bout << ',' << std::max(calData.accelMaxZ, (short)(-calData.accelMinZ)); //45
  bout << ',' << dueMPU.m_calMag[VEC3_X];                               //46
  bout << ',' << std::max(calData.magMaxX, (short)(-calData.magMinX)); //47
  bout << ',' << dueMPU.m_calMag[VEC3_Y];                              //48
  bout << ',' << std::max(calData.magMaxY, (short)(-calData.magMinY)); //49
  bout << ',' << dueMPU.m_calMag[VEC3_Z];                              //50
  bout << ',' << std::max(calData.magMaxZ, (short)(-calData.magMinZ)); //51
  bout << ',' << dueMPU.m_rawQuaternion;                               //52
  bout << ',' << dueMPU.m_dmpEulerPose;                                //53
  bout << ',' << dueMPU.m_fusedEulerPose;                              //54
  bout << ',' << gyro_temp;                                            //55
  bout << ',' << parameters.voltage_sanity_check_high;                 //56
  bout << ',' << parameters.voltage_sanity_check_low;                  //57
  bout << ',' << parameters.charge_current_sanity_check_high;          //58
  bout << ',' << parameters.charge_current_sanity_check_low;           //59
  bout << ',' << parameters.battery_1_recharge_ratio;                  //60
  bout << ',' << parameters.battery_1_amphrs_charging;                 //61
  bout << ',' << parameters.battery_1_amphrs_discharging;              //62
  bout << ',' << parameters.battery_1_amphrs_term_threshold;           //63
  bout << ',' << parameters.battery_1_amphrs_init_threshold;           //64
  bout << ',' << parameters.battery_1_voltage_term_threshold;          //65
  bout << ',' << parameters.battery_1_voltage_init_threshold;          //66
  bout << ',' << parameters.battery_2_recharge_ratio;                  //67
  bout << ',' << parameters.battery_2_amphrs_charging;                 //68
  bout << ',' << parameters.battery_2_amphrs_discharging;              //69
  bout << ',' << parameters.battery_2_amphrs_term_threshold;           //70
  bout << ',' << parameters.battery_2_amphrs_init_threshold;           //71
  bout << ',' << parameters.battery_2_voltage_term_threshold;          //72
  bout << ',' << parameters.battery_2_voltage_init_threshold;          //73
  bout << ',' << parameters.battery_temperature_limit_high;            //74
  bout << ',' << parameters.battery_temperature_limit_low;             //75
  bout << ',' << parameters.battery_temperature_sanity_check_high;     //76
  bout << ',' << thresholds.normal_battery_temperature_limit_high;     //77
  bout << ',' << thresholds.normal_battery_temperature_limit_low;      //78
  bout << ',' << thresholds.survival_battery_temperature_limit_high;   //79 
  bout << ',' << thresholds.survival_battery_temperature_limit_low;    //80
  bout << ',' << parameters.battery_temperature_sanity_check_low;      //81
  bout << ',' << parameters.low_voltage_limit_for_loadshed_entry;      //82
  bout << ',' << parameters.low_voltage_limit_for_auto_cutdown;        //83
  bout << ',' << parameters.low_voltage_time_limit;                    //84
  bout << ',' << parameters.altitude_limit_low;                        //85
  bout << ',' << parameters.altitude_sanity_check_low;                 //86
  bout << ',' << parameters.cutdown_pulse_width;                       //87
  bout << ',' << parameters.camera_period;                             //88
  bout << ',' << parameters.camera_on_time;                            //89
  
// BEGINNING OF THE PARENT BILEVEL WORD                                //90
  bout << ',' << parameters.battery_1_charging_status;                 //90-1 
  bout << ',' << parameters.battery_2_charging_status;                 //90-2
  bout << ',' << parameters.battery_bus_low_voltage_flag;              //90-3   
  bout << ',' << parameters.heater_state_1;                            //90-4
  bout << ',' << parameters.heater_state_2;                            //90-5
  bout << ',' << parameters.cutdown_enable_state;                      //90-6
  bout << ',' << parameters.cutdown_1_status;                          //90-7
  bout << ',' << parameters.cutdown_2_status;                          //90-8
  bout << ',' << parameters.altitude_valid_flag;                       //90-9
  bout << ',' << parameters.camera_enabled;                            //90-10
  bout << ',' << parameters.camera_status;                             //90-11
  bout << ',' << parameters.battery_1_temp_tlm_valid_flag;             //90-12
  bout << ',' << parameters.battery_2_temp_tlm_valid_flag;             //90-13  
  bout << ',' << parameters.battery_voltage_tlm_valid_flag;            //90-14
  bout << ',' << parameters.battery_1_current_tlm_valid_flag;          //90-15
  bout << ',' << parameters.battery_2_current_tlm_valid_flag;          //90-16     
  bout << ',' << alt.altitude_in_feet;                                 //91
  bout << ',' << alt.temperature;                                      //92
  bout << ',' << alt.pressure;                                         //93
  bout << ',' << "00000000000000000000000000000000";                   //94

  bout << endl;
  
  // log data and flush to SD
  logfile << buf << flush;

  // check for error
  if (!logfile) error("SD card write data failed");

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
