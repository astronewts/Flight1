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
    break;
  }
  
  if (!logfile.is_open()) error("file.open");

  // format header in buffer
  obufstream bout(buf, sizeof(buf));

  bout << pstr("millis");                                 

  initialize_database_2();
  
  for (int i=1; i<(DB_SIZE); i++) 
  { 
    bout << ','; 
    bout << db[i].SD_Card_Title;
  }
  
   
//#if USE_DS1307
//  bout << pstr(",date,time");
//#endif  // USE_DS1307
//  bout << pstr(",Data to RB  [-]");                       //1  
//  bout << pstr(",Veh Mode [-]");                          //2
//  bout << pstr(",Command Count [-]");                     //3   
//  bout << pstr(",Normal Trans Per [s]");                  //4
//  bout << pstr(",Loadshed Trans Per [s]");                //5
//  bout << pstr(",Transit Trans Per [s]");                 //6
//  bout << pstr(",Emer Trans Per [s]");                    //7
//  bout << pstr(",Spare Trans Per [s]");                   //8
//  bout << pstr(",SD Card Write Per [s]");                 //9
//  bout << pstr(",B 1 T 1 [C]");                           //10
//  bout << pstr(",B 1 T 2 [C]");                           //11
//  bout << pstr(",B 2 T 1 [C]");                           //12
//  bout << pstr(",B 2 T 2 [C]");                           //13
//  bout << pstr(",Inner Ext T [C]");                       //14
//  bout << pstr(",Outer Ext T [C]");                       //15
//  bout << pstr(",Int T [C]");                             //16
//  bout << pstr(",Air Pres [Pa]");                         //17
//  bout << pstr(",VIN Voltage [V]");                       //18
//  bout << pstr(",SPARE [na]");                            //19
//  bout << pstr(",B 1 Charge Current [A]");                //20
//  bout << pstr(",B 2 Charge Current [A]");                //21
//  bout << pstr(",SA Current [A]");                        //22
//  bout << pstr(",Load Path Current [A]");                 //23
//  bout << pstr(",B1_shunt_V        [V]") ;                //24
//  bout << pstr(",B1_Bus_V          [V]") ;                //25
//  bout << pstr(",B1_load_V         [V]") ;                //26
//  bout << pstr(",B2_shunt_V        [V]") ;                //27
//  bout << pstr(",B2_Bus_V          [V]") ;                //28
//  bout << pstr(",B2_load_V         [V]") ;                //29
//  bout << pstr(",SA_shuntV         [V]") ;                //30
//  bout << pstr(",SA_busV           [V]") ;                //31
//  bout << pstr(",SA_load_V         [V]") ;                //32
//  bout << pstr(",Loadpath_shunt_V  [V]") ;                //33
//  bout << pstr(",Loadpath_Bus_V    [V]") ;                //34
//  bout << pstr(",Loadpath_load_V   [V]") ;                //35
//  bout << pstr(",Tel Proc Per [s]");                      //36
//  bout << pstr(",Spare 3");                               //37
//  bout << pstr(",GPS Lat [deg]");                         //38
//  bout << pstr(",GPS Long [deg]");                        //39
//  bout << pstr(",GPS Alt [m]");                           //40
//  bout << pstr(",GPS Loc Age [s]");                       //41
//  bout << pstr(",GPS Alt Age [s]");                       //42
//  bout << pstr(",GPS Course [deg]");                      //43
//  bout << pstr(",GPS Speed [km/h]");                      //44
//  bout << pstr(",GPS # of Sat [-]");                      //45
//  bout << pstr(",GPS Date [-]");                          //46
//  bout << pstr(",GPS Time [-]");                          //47
//  bout << pstr(",HDOP Value [?]");                        //48
//  bout << pstr(",GPS Chars Processed [-]");               //49
//  bout << pstr(",GPS Sentences with Fix [-]");            //50
//  bout << pstr(",GPS Failed Checksum [-]");               //51
//  bout << pstr(",GPS Isvalid Congl [-]");                 //52
//  bout << pstr(",Acc X [g]");                             //53
//  bout << pstr(",Acc Y [g]");                             //54
//  bout << pstr(",Acc Z [g]");                             //55
//  bout << pstr(",Gyro mag X [G]");                        //56
//  bout << pstr(",Gyro mag Y [G]");                        //57
//  bout << pstr(",Gyro mag Z [G]");                        //58
//  bout << pstr(",Gyro X [deg/s]");                        //59
//  bout << pstr(",Gyro Y [deg/s]");                        //60
//  bout << pstr(",Gyro Z [deg/s]");                        //61
//  bout << pstr(",Gyro T [C]");                            //62
//  bout << pstr(",Volt San Check High [V]");               //63
//  bout << pstr(",Volt San Check Low [V]");                //64
//  bout << pstr(",Charge Cur Sanity High [A]");            //65
//  bout << pstr(",Charge Cur Sanity Low [A]");             //66
//  bout << pstr(",B 1 Recharge Ratio [-]");                //67
//  bout << pstr(",B 1 AH Charging [A]");                   //68
//  bout << pstr(",B 1 AH Discharging [A]");                //69
//  bout << pstr(",B 1 Cap Lim High [A.h]");                //70
//  bout << pstr(",B 1 Cap Lim Low [A.h]");                 //71
//  bout << pstr(",B 1 Volt Term. Charge Limit [V]");       //72
//  bout << pstr(",B 1 Volt Init. Charge Limit [V]");       //73
//  bout << pstr(",B 2 Recharge Ratio [-]");                //74
//  bout << pstr(",B 2 Amp Hours Charging [A]");            //75
//  bout << pstr(",B 2 Amp Hours Discharging [A]");         //76
//  bout << pstr(",B 2 Cap Limit High [A.h]");              //77
//  bout << pstr(",B 2 Cap Limit Low [A.h]");               //78
//  bout << pstr(",B 2 Volt Term. Charge Lim [V]");         //79
//  bout << pstr(",B 2 Volt Init. Charge Lim [V]");         //80
//  bout << pstr(",B Active T Lim High [C]");               //81
//  bout << pstr(",B Active T Lim Low [C]");                //82
//  bout << pstr(",B T Sanity Check High [C]");             //83
//  bout << pstr(",B Normal T Lim High [C]");               //84
//  bout << pstr(",B Normal T Lim Low [C]");                //85
//  bout << pstr(",B Surv T Lim High [C]");                 //86
//  bout << pstr(",B Surv T Lim Low [C]");                  //87
//  bout << pstr(",B T Sanity Check Low [C]");              //88
//  bout << pstr(",Loadshed Entry Volt Lim [V]");           //89
//  bout << pstr(",Auto Cutdown Volt Lim [V]");             //90
//  bout << pstr(",Low Volt Time until Cut-down [s]");      //91
//  bout << pstr(",Alt Limit Low [ft]");                    //92
//  bout << pstr(",Alt Sanity Check Low [ft]");             //93
//  bout << pstr(",Pyro Pulse Width [s]");                  //94
//  bout << pstr(",Cam Per [s]");                           //95
//  bout << pstr(",Cam On Time [s]");                       //96
//  bout << pstr(",B 1 Charg Status [-]");                  //97
//  bout << pstr(",B 2 Charg Status [-]");                  //98
//  bout << pstr(",Battery Bus Low V Flag [-]");            //99
//  bout << pstr(",Heater State 1 [-]");                    //100
//  bout << pstr(",Heater State 2 [-]");                    //101
//  bout << pstr(",Cut-down Enable");                       //102
//  bout << pstr(",Cut-down 1 Fire Status [-]");            //103
//  bout << pstr(",Cut-down 2 Fire Status [-]");            //104
//  bout << pstr(",Alt Valid Flag [-]");                    //105
//  bout << pstr(",Cam Enabled [-]");                       //106
//  bout << pstr(",Cam Status [-]");                        //107
//  bout << pstr(",B 1 T TLM Val Flag [-]");                //108
//  bout << pstr(",B 2 T TLM Val Flag [-]");                //109
//  bout << pstr(",Bus V TLM Val Flag [-]");                //110
//  bout << pstr(",B 1 Cur TLM Val Flag [-]");              //111
//  bout << pstr(",B 2 Cur TLM Val Flag []");               //112
//  bout << pstr(",Alt [ft]");                              //113
//  bout << pstr(",Alt T [C]");                             //114
//  bout << pstr(",Alt Pressure [?]");                      //115
//  bout << pstr(",RB Words Recieved");                     //116

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

  initialize_database_2();
  
  for (int i=1; i<(DB_SIZE); i++) 
  { 
    if (db[i].tlm_type == "float") { bout << ',' << db[i].float_pointer; }
    else if (db[i].tlm_type == "long") { bout << ',' << db[i].long_pointer; }
    else if (db[i].tlm_type == "int") { bout << ',' << db[i].int_pointer; }
    else if (db[i].tlm_type == "header") { bout << ',' << "10101010"; }
  }
 
//  bout << ',' << Flag_RB.try_send_reveive;                             //1
//  bout << ',' << parameters.vehicle_mode;                              //2
//  bout << ',' << parameters.command_count;                             //3
//  bout << ',' << thresholds.normal_transmit_period/1000;               //4
//  bout << ',' << thresholds.load_shed_transmit_period/1000;            //5
//  bout << ',' << thresholds.transit_transmit_period/10000;             //6
//  bout << ',' << thresholds.emergency_transit_transmit_period/1000;    //7
//  bout << ',' << thresholds.spare_transmit_period/1000;                //8
//  bout << ',' << parameters.sd_card_write_period/1000;                 //9
//  bout << ',' << telemetry_data.battery_1_temp_1;                      //10
//  bout << ',' << telemetry_data.battery_1_temp_2;                      //11
//  bout << ',' << telemetry_data.battery_2_temp_1;                      //12
//  bout << ',' << telemetry_data.battery_2_temp_2;                      //13
//  bout << ',' << telemetry_data.inner_external_temp;                   //14
//  bout << ',' << telemetry_data.outter_external_temp;                  //15
//  bout << ',' << telemetry_data.internal_temp;                         //16
//  bout << ',' << telemetry_data.air_pressure;                          //17
//  bout << ',' << telemetry_data.analog_VIN_voltage;                    //18
//  bout << ',' << "00000000000000000000000000000000";                   //19  SPARE
//  bout << ',' << telemetry_data.battery_1_charge_current;              //20
//  bout << ',' << telemetry_data.battery_2_charge_current;              //21
//  bout << ',' << telemetry_data.sa_current;                            //22
//  bout << ',' << telemetry_data.load_path_current;                     //23
//  bout << ',' << telemetry_data.shuntvoltage_batt1;                    //24
//  bout << ',' << telemetry_data.busvoltage_batt1;                      //25
//  bout << ',' << telemetry_data.loadvoltage_batt1;                     //26
//  bout << ',' << telemetry_data.shuntvoltage_batt2;                    //27
//  bout << ',' << telemetry_data.busvoltage_batt2;                      //28
//  bout << ',' << telemetry_data.loadvoltage_batt2;                     //29
//  bout << ',' << telemetry_data.shuntvoltage_sa;                       //30
//  bout << ',' << telemetry_data.busvoltage_sa;                         //31
//  bout << ',' << telemetry_data.loadvoltage_sa;                        //32
//  bout << ',' << telemetry_data.shuntvoltage_load_path;                //33
//  bout << ',' << telemetry_data.busvoltage_load_path;                  //34
//  bout << ',' << telemetry_data.loadvoltage_load_path;                 //35
//  bout << ',' << parameters.tlm_processing_period/1000;                //36
//  bout << ',' << "00000000000000000000000000000000";                   //37  SPARE
//  
//  bout << ',' << gps.location.lat();                                   //38
//  bout << ',' << gps.location.lng();                                   //39
//  bout << ',' << gps.altitude.meters();                                //40
//  bout << ',' << gps.location.age();                                   //41
//  bout << ',' << gps.altitude.age();                                   //42
//  bout << ',' << gps.course.deg();                                     //43
//  bout << ',' << gps.speed.kmph();                                     //44
//  bout << ',' << gps.satellites.value();                               //45
//  bout << ',' << gps.date.value();                                     //46
//  bout << ',' << gps.time.value();                                     //47
//  bout << ',' << gps.hdop.value();                                     //48
//  bout << ',' << gps.charsProcessed();                                 //49
//  bout << ',' << gps.sentencesWithFix();                               //50
//  bout << ',' << gps.failedChecksum();                                 //51
//
//  // ISVALID STRINGS FOR GPS!!!
//  gps_isvalid_str = String(gps.hdop.isValid());                        //52-1
//  gps_isvalid_str = gps_isvalid_str + String(gps.location.isValid());  //52-2
//  gps_isvalid_str = gps_isvalid_str + String(gps.altitude.isValid());  //52-3
//  gps_isvalid_str = gps_isvalid_str + String(gps.course.isValid());    //52-4
//  gps_isvalid_str = gps_isvalid_str + String(gps.speed.isValid());     //52-5
//  gps_isvalid_str = gps_isvalid_str + String(gps.satellites.isValid());//52-6
//  gps_isvalid_str = gps_isvalid_str + String(gps.date.isValid());      //52-7
//  gps_isvalid_str = gps_isvalid_str + String(gps.time.isValid());      //52-8
//  gps_isvalid_str = gps_isvalid_str + String(gps.hdop.isValid());      //52-9
//  bout << ',' << gps_isvalid_str;                                      //52
//
//  bout << ',' << gyro.ax;                                              //53
//  bout << ',' << gyro.ay;                                              //54
//  bout << ',' << gyro.az;                                              //55
//  bout << ',' << gyro.mx;                                              //56
//  bout << ',' << gyro.my;                                              //57
//  bout << ',' << gyro.mz;                                              //58
//  bout << ',' << gyro.gx;                                              //59
//  bout << ',' << gyro.gy;                                              //60
//  bout << ',' << gyro.gz;                                              //61
//  bout << ',' << gyro.gyro_temp;                                       //62
//  bout << ',' << parameters.voltage_sanity_check_high;                 //63
//  bout << ',' << parameters.voltage_sanity_check_low;                  //64
//  bout << ',' << parameters.charge_current_sanity_check_high;          //65
//  bout << ',' << parameters.charge_current_sanity_check_low;           //66
//  bout << ',' << parameters.battery_1_recharge_ratio;                  //67
//  bout << ',' << parameters.battery_1_amphrs_charging;                 //68
//  bout << ',' << parameters.battery_1_amphrs_discharging;              //69
//  bout << ',' << parameters.battery_1_amphrs_term_threshold;           //70
//  bout << ',' << parameters.battery_1_amphrs_init_threshold;           //71
//  bout << ',' << parameters.battery_1_voltage_term_threshold;          //72
//  bout << ',' << parameters.battery_1_voltage_init_threshold;          //73
//  bout << ',' << parameters.battery_2_recharge_ratio;                  //74
//  bout << ',' << parameters.battery_2_amphrs_charging;                 //75
//  bout << ',' << parameters.battery_2_amphrs_discharging;              //76
//  bout << ',' << parameters.battery_2_amphrs_term_threshold;           //77
//  bout << ',' << parameters.battery_2_amphrs_init_threshold;           //78
//  bout << ',' << parameters.battery_2_voltage_term_threshold;          //79
//  bout << ',' << parameters.battery_2_voltage_init_threshold;          //80
//  bout << ',' << parameters.battery_temperature_limit_high;            //81
//  bout << ',' << parameters.battery_temperature_limit_low;             //82
//  bout << ',' << parameters.battery_temperature_sanity_check_high;     //83
//  bout << ',' << thresholds.normal_battery_temperature_limit_high;     //84
//  bout << ',' << thresholds.normal_battery_temperature_limit_low;      //85
//  bout << ',' << thresholds.survival_battery_temperature_limit_high;   //86
//  bout << ',' << thresholds.survival_battery_temperature_limit_low;    //87
//  bout << ',' << parameters.battery_temperature_sanity_check_low;      //88
//  bout << ',' << parameters.low_voltage_limit_for_loadshed_entry;      //89
//  bout << ',' << parameters.low_voltage_limit_for_auto_cutdown;        //90
//  bout << ',' << parameters.low_voltage_time_limit;                    //91
//  bout << ',' << parameters.altitude_limit_low;                        //92
//  bout << ',' << parameters.altitude_sanity_check_low;                 //93
//  bout << ',' << parameters.cutdown_pulse_width/1000;                  //94
//  bout << ',' << parameters.camera_period/1000;                        //95
//  bout << ',' << parameters.camera_on_time/1000;                       //96
//
//  // BEGINNING OF THE PARENT BILEVEL WORD
//  bout << ',' << parameters.battery_1_charging_status;                 //97
//  bout << ',' << parameters.battery_2_charging_status;                 //98
//  bout << ',' << parameters.battery_bus_low_voltage_flag;              //99
//  bout << ',' << parameters.heater_state_1;                            //100
//  bout << ',' << parameters.heater_state_2;                            //101
//  bout << ',' << parameters.cutdown_enable_state;                      //102
//  bout << ',' << parameters.cutdown_1_status;                          //103
//  bout << ',' << parameters.cutdown_2_status;                          //104
//  bout << ',' << parameters.altitude_valid_flag;                       //105
//  bout << ',' << parameters.camera_enabled;                            //106
//  bout << ',' << parameters.camera_status;                             //107
//  bout << ',' << parameters.battery_1_temp_tlm_valid_flag;             //108
//  bout << ',' << parameters.battery_2_temp_tlm_valid_flag;             //109
//  bout << ',' << parameters.battery_voltage_tlm_valid_flag;            //110
//  bout << ',' << parameters.battery_1_current_tlm_valid_flag;          //111
//  bout << ',' << parameters.battery_2_current_tlm_valid_flag;          //112
//  bout << ',' << alt.altitude_in_feet;                                 //113
//  bout << ',' << alt.temperature;                                      //114
//  bout << ',' << alt.pressure;                                         //115
//  bout << ',' << parameters.num_rb_words_recieved;                     //116
//  bout << ',' << "00000000000000000000000000000000";                   //117
//

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
