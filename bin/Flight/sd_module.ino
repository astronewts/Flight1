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

  bout << pstr(",Vehicle Mode [-]");                         //1
  bout << pstr(",Command Count [-]");                        //2
  bout << pstr(",Normal Transmit Period [ms]");              //3
  bout << pstr(",Loadshed Transmit Period [ms]");            //4
  bout << pstr(",Transit Transmit Period [ms]");             //5
  bout << pstr(",Emergency Transit Transmit Period [ms]");   //6
  bout << pstr(",Spare Transmit Period [ms]");               //7
  bout << pstr(",SD Card Write Period [ms]");                //8
  bout << pstr(",Battery 1 Temp 1 [C]");                     //9
  bout << pstr(",Battery 1 Temp 2 [C]");                     //10
  bout << pstr(",Battery 2 Temp 1 [C]");                     //11
  bout << pstr(",Battery 2 Temp 2 [C]");                     //12
  bout << pstr(",Inner External Temp [C]");                  //13
  bout << pstr(",Outer External Temp [C]");                  //14
  bout << pstr(",Internal Temp [C]");                        //15
  bout << pstr(",Air Pressure [Pa]");                        //16
  bout << pstr(",Battery 1 Voltage Pri [V]");                //17
  bout << pstr(",Battery 1 Voltage Red [V]");                //18
  bout << pstr(",Battery 1 Charge Current 1 [A]");           //19
  bout << pstr(",Battery 1 Charge Current 2 [A]");           //20
  // TODO: I THINK WE'RE ACTUALLY OUTPUTTING TEMP CONSTANTS IN SPARE 1 & 2
  bout << pstr(",Spare 1");                                  //21  
  bout << pstr(",Spare 2");                                  //22
  bout << pstr(",Telemetry Processing Period [ms]");         //23
  bout << pstr(",Spare 3");                                  //24
  bout << pstr(",GPS Latitude [deg]");                       //25
  bout << pstr(",GPS Longitude [deg]");                      //26
  bout << pstr(",GPS Altitude [m]");                         //27
  bout << pstr(",GPS Location Age [s]");                     //28
  bout << pstr(",GPS Altitude Age [s]");                     //29
  bout << pstr(",GPS Course [deg]");                         //30
  bout << pstr(",GPS Speed [km/h]");                         //31
  bout << pstr(",GPS # of Satellites [-]");                  //32
  bout << pstr(",GPS Date [-]");                             //33
  bout << pstr(",GPS Time [-]");                             //34
  bout << pstr(",HDOP Value [?]");                           //35
  bout << pstr(",GPS Chars Processed [-]");                  //36
  bout << pstr(",GPS Sentences with Fix [-]");               //37
  bout << pstr(",GPS Failed Checksum [-]");                  //38
  bout << pstr(",GPS Isvalid Conglomerate [-]");             //39
  bout << pstr(",Acc X [mg]");                               //40
  bout << pstr(",Acc Y [mg]");                               //41
  bout << pstr(",Acc Z [mg]");                               //42
  bout << pstr(",Gyro mag X [mG]");                          //43
  bout << pstr(",Gyro mag Y [mG]");                          //44
  bout << pstr(",Gyro mag Z [mG]");                          //45
  bout << pstr(",Gyro X [deg/s]");                           //46
  bout << pstr(",Gyro Y [deg/s]");                           //47
  bout << pstr(",Gyro Z [deg/s]");                           //48
  bout << pstr(",Gyro Temp [C]");                            //49
  bout << pstr(",Voltage Sanity Check High [V]");            //50
  bout << pstr(",Voltage Sanity Check Low [V]");             //51  
  bout << pstr(",Charge Current Sanity High [A]");           //52
  bout << pstr(",Charge Current Sanity Low [A]");            //53
  bout << pstr(",Battery 1 Recharge Ratio [-]");             //54
  bout << pstr(",Battery 1 Amp Hours Charging [A]");         //55
  bout << pstr(",Battery 1 Amp Hours Discharging [A]");      //56
  bout << pstr(",Battery 1 Capacity Limit High [A.h]");      //57
  bout << pstr(",Battery 1 Capacity Limit Low [A.h]");       //58
  bout << pstr(",Battery 1 Voltage Term. Charge Limit [V]"); //59
  bout << pstr(",Battery 1 Voltage Init. Charge Limit [V]"); //60
  bout << pstr(",Battery 2 Recharge Ratio [-]");             //61
  bout << pstr(",Battery 2 Amp Hours Charging [A]");         //62
  bout << pstr(",Battery 2 Amp Hours Discharging [A]");      //63
  bout << pstr(",Battery 2 Capacity Limit High [A.h]");      //64
  bout << pstr(",Battery 2 Capacity Limit Low [A.h]");       //65
  bout << pstr(",Battery 2 Voltage Term. Charge Limit [V]"); //66
  bout << pstr(",Battery 2 Voltage Init. Charge Limit [V]"); //67
  bout << pstr(",Battery Active Temp Limit High [C]");       //68
  bout << pstr(",Battery Active Temp Limit Low [C]");        //68
  bout << pstr(",Battery Temp Sanity Check High [C]");       //70
  bout << pstr(",Battery Normal Temp Limit High [C]");       //71
  bout << pstr(",Battery Normal Temp Limit Low [C]");        //72
  bout << pstr(",Battery Survival Temp Limit High [C]");     //73
  bout << pstr(",Battery Survival Temp Limit Low [C]");      //74
  bout << pstr(",Battery Temp Sanity Check Low [C]");        //75  
  bout << pstr(",Loadshed Entry Voltage Limit [V]");         //76
  bout << pstr(",Auto Cutdown Voltage Limit [V]");           //77
  bout << pstr(",Low Voltage Time until Cut-down [s]");      //78
  bout << pstr(",Altitude Limit Low [m]");                   //79  
  bout << pstr(",Altiude Sanity Check Low [m]");             //80
  bout << pstr(",Pyro Pulse Width [?ms]");                   //81
  bout << pstr(",Camera Period [ms]");                       //82
  bout << pstr(",Camera On Time [s]");                       //83
  bout << pstr(",Battery 1 Charging Status [-]");            //84
  bout << pstr(",Battery 2 Charging Status [-]");            //85
  bout << pstr(",Battery Bus Low Voltage Flag [-]");         //86  
  bout << pstr(",Heater State 1 [-]");                       //87
  bout << pstr(",Heater State 2 [-]");                       //88
  bout << pstr(",Cut-down Enable");                          //89
  bout << pstr(",Cut-down 1 Fire Status [-]");               //90
  bout << pstr(",Cut-down 2 Fire Status [-]");               //91
  bout << pstr(",Altitude Valid Flag [-]");                  //92
  bout << pstr(",Camera Enabled [-]");                       //93
  bout << pstr(",Camera Status [-]");                        //94
  bout << pstr(",Battery 1 Temp TLM Valid Flag [-]");        //95  
  bout << pstr(",Battery 2 Temp TLM Valid Flag [-]");        //96  
  bout << pstr(",Bus Voltage TLM Valid Flag [-]");           //97  
  bout << pstr(",Battery 1 Current TLM Valid Flag [-]");     //98
  bout << pstr(",Battery 2 Current TLM Valid Flag []");      //99  
  bout << pstr(",Altitude [ft]");                            //100
  bout << pstr(",Alt Temp [C]");                             //101
  bout << pstr(",Alt Pressure [?]");                         //102
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
  double dummy_value;

 dummy_value=0;
  
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
  
  bout << ',' << gps_isvalid_str;                                      //39
                                                                       
  bout << ',' << gyro.ax;                                              //40
  bout << ',' << gyro.ay;                                              //41
  bout << ',' << gyro.az;                                              //42
  bout << ',' << gyro.mx;                                              //43
  bout << ',' << gyro.my;                                              //44
  bout << ',' << gyro.mz;                                              //45
  bout << ',' << gyro.gx;                                              //46
  bout << ',' << gyro.gy;                                              //47
  bout << ',' << gyro.gz;                                              //48
  bout << ',' << gyro.gyro_temp;                                       //49
  bout << ',' << parameters.voltage_sanity_check_high;                 //50
  bout << ',' << parameters.voltage_sanity_check_low;                  //51
  bout << ',' << parameters.charge_current_sanity_check_high;          //52
  bout << ',' << parameters.charge_current_sanity_check_low;           //53
  bout << ',' << parameters.battery_1_recharge_ratio;                  //54
  bout << ',' << parameters.battery_1_amphrs_charging;                 //55
  bout << ',' << parameters.battery_1_amphrs_discharging;              //56
  bout << ',' << parameters.battery_1_amphrs_term_threshold;           //57
  bout << ',' << parameters.battery_1_amphrs_init_threshold;           //58
  bout << ',' << parameters.battery_1_voltage_term_threshold;          //59
  bout << ',' << parameters.battery_1_voltage_init_threshold;          //60
  bout << ',' << parameters.battery_2_recharge_ratio;                  //61
  bout << ',' << parameters.battery_2_amphrs_charging;                 //62
  bout << ',' << parameters.battery_2_amphrs_discharging;              //63
  bout << ',' << parameters.battery_2_amphrs_term_threshold;           //64
  bout << ',' << parameters.battery_2_amphrs_init_threshold;           //65
  bout << ',' << parameters.battery_2_voltage_term_threshold;          //66
  bout << ',' << parameters.battery_2_voltage_init_threshold;          //67
  bout << ',' << parameters.battery_temperature_limit_high;            //68
  bout << ',' << parameters.battery_temperature_limit_low;             //69
  bout << ',' << parameters.battery_temperature_sanity_check_high;     //70
  bout << ',' << thresholds.normal_battery_temperature_limit_high;     //71
  bout << ',' << thresholds.normal_battery_temperature_limit_low;      //72
  bout << ',' << thresholds.survival_battery_temperature_limit_high;   //73
  bout << ',' << thresholds.survival_battery_temperature_limit_low;    //74
  bout << ',' << parameters.battery_temperature_sanity_check_low;      //75
  bout << ',' << parameters.low_voltage_limit_for_loadshed_entry;      //76
  bout << ',' << parameters.low_voltage_limit_for_auto_cutdown;        //77
  bout << ',' << parameters.low_voltage_time_limit;                    //78
  bout << ',' << parameters.altitude_limit_low;                        //79
  bout << ',' << parameters.altitude_sanity_check_low;                 //80
  bout << ',' << parameters.cutdown_pulse_width;                       //81
  bout << ',' << parameters.camera_period;                             //82
  bout << ',' << parameters.camera_on_time;                            //83
  
// BEGINNING OF THE PARENT BILEVEL WORD                                
  bout << ',' << parameters.battery_1_charging_status;                 //84
  bout << ',' << parameters.battery_2_charging_status;                 //85
  bout << ',' << parameters.battery_bus_low_voltage_flag;              //86   
  bout << ',' << parameters.heater_state_1;                            //87
  bout << ',' << parameters.heater_state_2;                            //88
  bout << ',' << parameters.cutdown_enable_state;                      //89
  bout << ',' << parameters.cutdown_1_status;                          //90
  bout << ',' << parameters.cutdown_2_status;                          //91
  bout << ',' << parameters.altitude_valid_flag;                       //92
  bout << ',' << parameters.camera_enabled;                            //93
  bout << ',' << parameters.camera_status;                             //94
  bout << ',' << parameters.battery_1_temp_tlm_valid_flag;             //95
  bout << ',' << parameters.battery_2_temp_tlm_valid_flag;             //96
  bout << ',' << parameters.battery_voltage_tlm_valid_flag;            //97
  bout << ',' << parameters.battery_1_current_tlm_valid_flag;          //98
  bout << ',' << parameters.battery_2_current_tlm_valid_flag;          //99
  bout << ',' << alt.altitude_in_feet;                                 //100
  bout << ',' << alt.temperature;                                      //101
  bout << ',' << alt.pressure;                                         //102
  bout << ',' << "00000000000000000000000000000000";                   //103

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
