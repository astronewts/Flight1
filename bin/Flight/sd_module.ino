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
  bout << pstr(",Data to RB  [-]");                         //0
  bout << pstr(",Veh Mode [-]");                         //1
  bout << pstr(",Command Count [-]");                        //2
  bout << pstr(",Normal Trans Period [s]");              //3
  bout << pstr(",Loadshed Trans Period [s]");            //4
  bout << pstr(",Transit Tran Period [s]");             //5
  bout << pstr(",Emergency Trans Transmit Period [s]");   //6
  bout << pstr(",Spare Trans Period [s]");               //7
  bout << pstr(",SD Card Write Period [s]");             //8
  bout << pstr(",Bat 1 Temp 1 [C]");                     //9
  bout << pstr(",Bat 1 Temp 2 [C]");                     //10
  bout << pstr(",Bat 2 Temp 1 [C]");                     //11
  bout << pstr(",Bat 2 Temp 2 [C]");                     //12
  bout << pstr(",Inner Ext Temp [C]");                   //13
  bout << pstr(",Outer Ext Temp [C]");                   //14
  bout << pstr(",Int Temp [C]");                         //15
  bout << pstr(",Air Pres [Pa]");                        //16
  bout << pstr(",Bat 1 Volt [V]");                       //17
  bout << pstr(",Bat 2 Volt [V]");                       //18
  bout << pstr(",Bat 1 Charge Current [A]");             //19
  bout << pstr(",Bat 2 Charge Current [A]");             //20
  bout << pstr(",Solar Array Current [A]");              //21
  bout << pstr(",Load Path Current [A]");                //22
  bout << pstr(",Tel Processing Period [s]");            //23
  bout << pstr(",Spare 3");                              //24
  bout << pstr(",GPS Lat [deg]");                        //25
  bout << pstr(",GPS Long [deg]");                       //26
  bout << pstr(",GPS Alt [m]");                          //27
  bout << pstr(",GPS Loc Age [s]");                      //28
  bout << pstr(",GPS Alt Age [s]");                      //29
  bout << pstr(",GPS Course [deg]");                     //30
  bout << pstr(",GPS Speed [km/h]");                     //31
  bout << pstr(",GPS # of Sat [-]");                     //32
  bout << pstr(",GPS Date [-]");                         //33
  bout << pstr(",GPS Time [-]");                         //34
  bout << pstr(",HDOP Value [?]");                       //35
  bout << pstr(",GPS Chars Processed [-]");              //36
  bout << pstr(",GPS Sentences with Fix [-]");           //37
  bout << pstr(",GPS Failed Checksum [-]");              //38
  bout << pstr(",GPS Isvalid Conglomerate [-]");          //39
  bout << pstr(",Acc X [g]");                               //40
  bout << pstr(",Acc Y [g]");                               //41
  bout << pstr(",Acc Z [g]");                               //42
  bout << pstr(",Gyro mag X [G]");                          //43
  bout << pstr(",Gyro mag Y [G]");                          //44
  bout << pstr(",Gyro mag Z [G]");                          //45
  bout << pstr(",Gyro X [deg/s]");                           //46
  bout << pstr(",Gyro Y [deg/s]");                           //47
  bout << pstr(",Gyro Z [deg/s]");                           //48
  bout << pstr(",Gyro Temp [C]");                            //49
  bout << pstr(",Volt Sanity Check High [V]");            //50
  bout << pstr(",Volt Sanity Check Low [V]");             //51
  bout << pstr(",Charge Cur Sanity High [A]");           //52
  bout << pstr(",Charge Cur Sanity Low [A]");            //53
  bout << pstr(",Bat 1 Recharge Ratio [-]");             //54
  bout << pstr(",Bat 1 Amp Hours Charging [A]");         //55
  bout << pstr(",Bat 1 Amp Hours Discharging [A]");      //56
  bout << pstr(",Bat 1 Cap Lim High [A.h]");      //57
  bout << pstr(",Bat 1 Cap Lim Low [A.h]");       //58
  bout << pstr(",Bat 1 Volt Term. Charge Limit [V]"); //59
  bout << pstr(",Bat 1 Volt Init. Charge Limit [V]"); //60
  bout << pstr(",Bat 2 Recharge Ratio [-]");             //61
  bout << pstr(",Bat 2 Amp Hours Charging [A]");         //62
  bout << pstr(",Bat 2 Amp Hours Discharging [A]");      //63
  bout << pstr(",Bat 2 Capacity Limit High [A.h]");      //64
  bout << pstr(",Bat 2 Capacity Limit Low [A.h]");       //65
  bout << pstr(",Bat 2 Voltage Term. Charge Limit [V]"); //66
  bout << pstr(",Bat 2 Voltage Init. Charge Limit [V]"); //67
  bout << pstr(",Bat Active Temp Lim High [C]");       //68
  bout << pstr(",Bat Active Temp Lim Low [C]");        //68
  bout << pstr(",Bat Temp Sanity Check High [C]");       //70
  bout << pstr(",Bat Normal Temp Lim High [C]");       //71
  bout << pstr(",Bat Normal Temp Lim Low [C]");        //72
  bout << pstr(",Bat Survival Temp Lim High [C]");     //73
  bout << pstr(",Bat Survival Temp Lim Low [C]");      //74
  bout << pstr(",Bat Temp Sanity Check Low [C]");        //75
  bout << pstr(",Loadshed Entry Volt Lim [V]");         //76
  bout << pstr(",Auto Cutdown Volt Lim [V]");           //77
  bout << pstr(",Low Volt Time until Cut-down [s]");      //78
  bout << pstr(",Alt Limit Low [ft]");                   //79
  bout << pstr(",Alt Sanity Check Low [ft]");             //80
  bout << pstr(",Pyro Pulse Width [s]");                   //81
  bout << pstr(",Cam Period [s]");                       //82
  bout << pstr(",Cam On Time [s]");                       //83
  bout << pstr(",Bat 1 Charg Status [-]");            //84
  bout << pstr(",Bat 2 Charg Status [-]");            //85
  bout << pstr(",Battery Bus Low Voltage Flag [-]");         //86
  bout << pstr(",Heater State 1 [-]");                       //87
  bout << pstr(",Heater State 2 [-]");                       //88
  bout << pstr(",Cut-down Enable");                          //89
  bout << pstr(",Cut-down 1 Fire Status [-]");               //90
  bout << pstr(",Cut-down 2 Fire Status [-]");               //91
  bout << pstr(",Alt Valid Flag [-]");                  //92
  bout << pstr(",Cam Enabled [-]");                       //93
  bout << pstr(",Cam Status [-]");                        //94
  bout << pstr(",Bat 1 Temp TLM Val Flag [-]");        //95
  bout << pstr(",Bat 2 Temp TLM Val Flag [-]");        //96
  bout << pstr(",Bus Voltage TLM Val Flag [-]");           //97
  bout << pstr(",Bat 1 Cur TLM Val Flag [-]");     //98
  bout << pstr(",Bat 2 Cur TLM Val Flag []");      //99
  bout << pstr(",Alt [ft]");                            //100
  bout << pstr(",Alt Temp [C]");                             //101
  bout << pstr(",Alt Pressure [?]");                         //102
  bout << pstr(",RB Words Recieved");                //103
  bout << pstr(",The End");                         //104
  logfile << buf << endl << flush;

}

//Write telemetry data to SD card
//void write_telemetry_data_to_sd(char* string_buffer)
void write_telemetry_data_to_sd()
{
  uint32_t m;
  double dummy_value;

  dummy_value=0;

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
  bout << ',' << now;
#endif  // USE_DS1307

  //#################################################
  //############### ASTRONEWTS SD TLM ###############
  //#################################################

  bout << ',' << Flag_RB.try_send_reveive;                             //0
  bout << ',' << parameters.vehicle_mode;                              //1
  bout << ',' << parameters.command_count;                             //2
  bout << ',' << thresholds.normal_transmit_period/1000;               //3
  bout << ',' << thresholds.load_shed_transmit_period/1000;            //4
  bout << ',' << thresholds.transit_transmit_period/10000;             //5
  bout << ',' << thresholds.emergency_transit_transmit_period/1000;    //6
  bout << ',' << thresholds.spare_transmit_period/1000;                //7
  bout << ',' << parameters.sd_card_write_period/1000;                 //8
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
  bout << ',' << telemetry_data.battery_1_charge_current;              //19
  bout << ',' << telemetry_data.battery_2_charge_current;              //20
  bout << ',' << telemetry_data.sa_current;                            //21
  bout << ',' << telemetry_data.load_path_current;                     //22
  bout << ',' << parameters.tlm_processing_period/1000;                //23
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
  bout << ',' << parameters.cutdown_pulse_width/1000;                       //81
  bout << ',' << parameters.camera_period/1000;                             //82
  bout << ',' << parameters.camera_on_time/1000;                            //83

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
  bout << ',' << parameters.num_rb_words_recieved;                     //103
  bout << ',' << "00000000000000000000000000000000";                   //104


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
  if (!Serial.available()) return;
  logfile.close();
  cout << pstr("Done!");
  while (1);

}
