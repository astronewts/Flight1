#ifndef _FLIGHT_DEFINITIONS_
#define _FLIGHT_DEFINITIONS_

#include <elapsedMillis.h>

#define DEBUG

//Command Sentinels
#define NO_COMMANDS_TO_PROCESS        0
#define COMMANDS_TO_PROCESS           1

//Constants
#define PRESSURE_CONSTANT     0.0036140
#define TEMP_BREAK_COUNT      160
#define TEMP_CONSTANT_1_1     91.31522494
#define TEMP_CONSTANT_1_2     -0.173536021
#define TEMP_CONSTANT_1_3     0.000162517
#define TEMP_CONSTANT_1_4     -8.30807E-08
#define TEMP_CONSTANT_1_5     2.04376E-11
#define TEMP_CONSTANT_1_6     -1.94001E-15
#define TEMP_CONSTANT_2_1     201.0543745
#define TEMP_CONSTANT_2_2     -4.089196897
#define TEMP_CONSTANT_2_3     0.071827729
#define TEMP_CONSTANT_2_4     -0.000719311
#define TEMP_CONSTANT_2_5     3.65125E-06
#define TEMP_CONSTANT_2_6     -7.29058E-09

#define VOLTAGE_CONSTANT_1  3.3
#define VOLTAGE_CONSTANT_2  4095.0
#define VOLTAGE_CONSTANT_3  5.0151
#define VOLTAGE_CONSTANT_4  0.6301
#define CHARGE_CONSTANT_1   3.3
#define CHARGE_CONSTANT_2   4095.0
#define CHARGE_CONSTANT_3   1.40
#define CHARGE_CONSTANT_4   20.37
#define MS_IN_SEC           1000.0
#define SECS_IN_HOUR        3600.0

//TODO: ADD UNITS !!!!!

//Defaults
#define DEFAULT_MODE                               1
#define DEFAULT_LOOP_SLEEP                         3000
#define DEFAULT_VOLTAGE_LOW_LIMIT_LOADSHED_ENTRY   8.5
#define DEFAULT_VOLTAGE_LOW_LIMIT_CUTDOWN_ENTRY    8.0
#define DEFAULT_LOW_VOLTAGE_TIME_LIMIT             1800
#define DEFAULT_VOLTAGE_SANITY_CHECK_HIGH          2.4
#define DEFAULT_VOLTAGE_SANITY_CHECK_LOW           4.0
#define DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_HIGH     20
#define DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_LOW      18
#define DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_HIGH   16
#define DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_LOW    14
#define DEFAULT_BATTERY_TEMP_SANITY_CHECK_HIGH     150
#define DEFAULT_BATTERY_TEMP_SANITY_CHECK_LOW      -80
#define DEFAULT_NORMAL_TRANSMIT_RATE               6000//1800000 // TEST ONLY!!! (Once every half hour)
//#define DEFAULT_NORMAL_TRANSMIT_RATE             1200000 // FOR FLIGHT!!!
#define DEFAULT_SPARE_TRANSMIT_RATE                60000
#define DEFAULT_TRANSIT_TRANSMIT_RATE              30000
#define DEFAULT_EMERGENCY_TRANSIT_TRANSMIT_RATE    45000
#define DEFAULT_LOAD_SHED_TRANSMIT_RATE            60000

#define DEFAULT_RB_AT_BUS_TIMEOUT                  20 // seconds // Default is 20 seconds
#define DEFAULT_RB_SEND_RECIEVE_TIMEOUT            300 // seconds // Default is 300 seconds

#define DEFAULT_RB_POWER_MODE                      1
// Use "1" for low current applications; when powered by a low-power 90 mA max USB supply, the interval between transmit retries is extended to as much as 60 seconds
// Use "0" for "high current" applications; interval between transmit retries is 20 seconds

#define DEFAULT_PYRO_PULSE_WIDTH                   8000
#define DEFAULT_SD_CARD_WRITE_PERIOD               100 // TEST ONLY!!!
//#define DEFAULT_SD_CARD_WRITE_PERIOD             60000  // FOR FLIGHT !!!
#define DEFAULT_TLM_PROCESSING_PERIOD              5000
#define DEFAULT_CAMERA_PERIOD                      1200000
#define DEFAULT_CAMERA_ON_TIME                     10000
#define HIGH_RATE_PERIOD                           50 // ms
#define MEDIUM_RATE_PERIOD                         10000 // ms
#define LOW_RATE_PERIOD                            10000 // ms
#define HIGH_RATE_PERIOD_CUTDOWN                   20 // ms
#define MEDIUM_RATE_PERIOD_CUTDOWN                 10000 // ms
#define LOW_RATE_PERIOD_CUTDOWN                    10000 // ms

#define DEFAULT_ALTITUDE_LIMIT_LOW                 65000  // 97:for test in living room  //  A2 ft
#define DEFAULT_ALTITUDE_SANITY_CHECK_LOW          75000  // 101:for test in living room //  A1 ft // for the new code (line 400ish in telemetry_module) A1 > A2

#define DEFAULT_B1_RECHARGE_RATIO                  1.1
#define DEFAULT_B1_AMPHRS_TERM_THRESHOLD           1.0  //Prev:-0.1  Note: These are purposely set too high to trigger, due to HW perfoming this action
#define DEFAULT_B1_AMPHRS_INIT_THRESHOLD           0.0  //Prev:-0.5  Note: These are purposely set too high to trigger, due to HW perfoming this action
#define DEFAULT_B1_VOLTAGE_TERM_THRESHOLD          15.0 //Prev:12.5  Note: These are purposely set too high to trigger, due to HW perfoming this action
#define DEFAULT_B1_VOLTAGE_INIT_THRESHOLD          14.0 //Prev:10.0  Note: These are purposely set too high to trigger, due to HW perfoming this action

#define DEFAULT_B2_RECHARGE_RATIO                  1.1
#define DEFAULT_B2_AMPHRS_TERM_THRESHOLD           1.0  //Prev:-0.1  Note: These are purposely set too high to trigger, due to HW perfoming this action
#define DEFAULT_B2_AMPHRS_INIT_THRESHOLD           0.0  //Prev:-0.5  Note: These are purposely set too high to trigger, due to HW perfoming this action
#define DEFAULT_B2_VOLTAGE_TERM_THRESHOLD          15.0 //Prev:12.5  Note: These are purposely set too high to trigger, due to HW perfoming this action
#define DEFAULT_B2_VOLTAGE_INIT_THRESHOLD          14.0 //Prev:10.0  Note: These are purposely set too high to trigger, due to HW perfoming this action

#define DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW    3.0
#define DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH   -2.0

//Test Variables
#define INITIAL_TEST_COUNT                          0
#define CUTDOWN_TEST_TIME                           (15 * 60) // 30000 // Put here a number +20. Exemple 50 will correspond to a cut-down time of 30sec
#define INITIALIZATION_TIMEOUT                      10000


//Vehicle Modes
#define FLIGHT_MODE                               1
#define LOADSHED_MODE                             2
#define TRANSIT_MODE                              3
#define EMERGENCY_DESCENT_MODE                    4
#define FLIGHT_MODE_WITHOUT_RB                    5
#define CUTDOWN_TEST_MODE                         6
#define TERMINAL_TEST_MODE                        7
#define SIGNAL_TEST_MODE                          8
#define FLIGHT_MODE_WITH_DEBUG                    9

//Analog Pins
#define PIN_PRESSURE_SENSOR                    A0
// SPARE                                       A1
// SPARE                                       A2
#define PIN_ANALOG_VIN_VOLTAGE                 A3
// SPARE                                       A4
#define PIN_EXTERNAL_OUTTER_TEMP               A5
#define PIN_INTERNAL_TEMP                      A6
#define PIN_EXTERNAL_INNER_TEMP                A7
#define PIN_BATTERY1_1_TEMP                    A8
#define PIN_BATTERY1_2_TEMP                    A9
#define PIN_BATTERY2_1_TEMP                    A10
#define PIN_BATTERY2_2_TEMP                    A11

//Digital Pins
#define PIN_CUTDOWN_ENABLE              2
#define PIN_CUTDOWN_1_FIRE              3
#define PIN_CUTDOWN_2_FIRE              4
#define PIN_BATTERY_1_CHARGE_CUTOFF     6
#define PIN_BATTERY_2_CHARGE_CUTOFF     7
#define PIN_CAMERA_SWITCH               9
#define PIN_HEATER_CONTROL_1            5 // was 22: correct pin num
#define PIN_HEATER_CONTROL_2            8 // was 23
#define PIN_GYRO_POWER                  28


#define RESOLUTION_PRESSURE_SENSOR      12

// SD CARD CONSTANTS
#define SD_CHIP_SELECT                  10  // SD chip select pin
#define USE_DS1307                      0  // set nonzero to use DS1307 RTC
#define LOG_INTERVAL                    10 // mills between entries
//#define SENSOR_COUNT                  3  // number of analog pins to log
#define ECHO_TO_SERIAL                  1  // echo data to serial port if nonzero
#define WAIT_TO_START                   1  // Wait for serial input in setup(), 1=true
#define ADC_DELAY                       10  // ADC delay for high impedence sensors

// DB CONSTANTS
#define DB_SIZE                         135 //132 // Size of the Database
#define MAX_TLM_TYPE_SIZE               6 //
#define MAX_SD_TITLE_SIZE               32 //

// DEFINE FORMATS
#define FORMAT_1                        1
#define FORMAT_2                        2


typedef struct {
  String tlm_type;
  int bitsize;
  int int_pointer;
  long long_pointer;
  float float_pointer;
  String SD_Card_Title;
  int calibration_type;
  bool format_1;
  bool format_2;
} database;

//typedef struct {
//  String tlm_type;
//  int bitsize;
//  int* int_pointer;
//  long int* long_pointer;
//  float* float_pointer;
//  String SD_Card_Title;
//  int calibration_type;
//  bool RB_Flag;
//} database;

int null_int = 12345;
long null_long = 12345;
float null_float = 12345;

struct alt_struct
{
  float altitude_in_feet;
  float temperature;
  float pressure;
};

struct gyro_struct
{
  float gyro_temp;
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  float mx;
  float my;
  float mz;
  int count_between_RB;
  float mean_ax;
};

struct debug_struct
{
  bool mode;
};

struct Flag_RB_struct
{
  int try_send_reveive;
  int loop_started;
};

struct telemetry_data_struct
{
  // char "\xA30133";
  //uint16_t air_pressure;
  double battery_1_temp_1;
  double battery_1_temp_2;
  double battery_2_temp_1;
  double battery_2_temp_2;
  double inner_external_temp;
  double outter_external_temp;
  double internal_temp;
  double air_pressure;
  double analog_VIN_voltage;

  float shuntvoltage_batt1;
  float busvoltage_batt1;
  float battery_1_charge_current;
  float loadvoltage_batt1;

  float shuntvoltage_batt2;
  float busvoltage_batt2;
  float battery_2_charge_current;
  float loadvoltage_batt2;

  float shuntvoltage_sa;
  float busvoltage_sa;
  float sa_current;
  float loadvoltage_sa;

  float shuntvoltage_load_path;
  float busvoltage_load_path;
  float load_path_current;
  float loadvoltage_load_path;

  //struct gps_struct gps_values;
  //struct gyro_struct gyro_values;
};

// CODE EXAMPLE:
// bit representation
// uint_8* new_telemetry_data_struct = (uint_8*) telemetry_data_struct;

struct satellite_data_struct
{
  char* command;
};

struct gps_data_struct
{
  
  float gps_processed_lat;
  float gps_processed_long;
  float gps_location_age; // MAX(gps_alt_age, gps_pos_age) in seconds
  float gps_altitude;
  float gps_altitude_age;
  float gps_heading; // 0-255
  float gps_speed; // km/hr
  long gps_date;
  long gps_time;
  
  unsigned int gps_location_valid;
  unsigned int gps_altitude_valid;
  unsigned int gps_heading_valid;
  unsigned int gps_speed_valid;
  unsigned int gps_num_satellites_valid;
  unsigned int gps_date_valid;
  unsigned int gps_time_valid;
  unsigned int gps_hdop_valid;
  
  unsigned int gps_num_satellites;
  unsigned int gps_hdop;
  unsigned int gps_chars_processed;
  unsigned int gps_sentances_with_fix;
  unsigned int gps_failed_checksum;
  
  unsigned int gyro_temp;
};

// Use this enum whenever you need to ensure that EVERY telemetry item is iterated through

struct raw_telemetry_data_struct
{
  unsigned long raw_battery_1_temp_1;
  unsigned long raw_battery_1_temp_2;
  unsigned long raw_battery_2_temp_1;
  unsigned long raw_battery_2_temp_2;
  unsigned long raw_inner_external_temp;
  unsigned long raw_outter_external_temp;
  unsigned long raw_internal_temp;
  unsigned long raw_air_pressure;
  unsigned long raw_analog_VIN_voltage;
};

struct parameter_struct
{
  unsigned long loop_sleep;
  unsigned long transmit_period;
  unsigned long sd_card_write_period;
  unsigned long tlm_processing_period;
  int vehicle_mode;
  int telemetry_format;
  int command_count;
  int battery_temperature_limit_high;
  int battery_temperature_limit_low;
  int battery_temperature_sanity_check_high;
  int battery_temperature_sanity_check_low;
  bool heater_state_1;
  bool heater_state_2;
  float low_voltage_limit_for_loadshed_entry;
  float low_voltage_limit_for_auto_cutdown;
  unsigned int low_voltage_time_limit;
  bool battery_bus_low_voltage_flag;
  float voltage_sanity_check_high;
  float voltage_sanity_check_low;
  float charge_current_sanity_check_high;
  float charge_current_sanity_check_low;
  unsigned int cutdown_pulse_width;
  bool cutdown_enable_state;
  bool cutdown_1_status;
  bool cutdown_2_status;
  bool camera_enabled;
  bool camera_status;
  unsigned int camera_period;
  unsigned int camera_on_time;
  bool battery_1_charging_status;
  bool battery_2_charging_status;
  bool battery_1_temp_tlm_valid_flag;
  bool battery_2_temp_tlm_valid_flag;
  bool battery_voltage_tlm_valid_flag;
  bool battery_1_current_tlm_valid_flag;
  bool battery_2_current_tlm_valid_flag;

  bool altitude_valid_flag;
  int altitude_limit_low;
  int altitude_sanity_check_low;

  int rb_initialization_error_status;
  int num_rb_words_recieved;

  int test_count;
  int prompt_from_user_makes_sense;

  float battery_1_recharge_ratio;
  float battery_1_amphrs_charging;
  float battery_1_amphrs_discharging;
  float battery_1_amphrs_term_threshold;
  float battery_1_amphrs_init_threshold;
  float battery_1_voltage_term_threshold;
  float battery_1_voltage_init_threshold;

  float battery_2_recharge_ratio;
  float battery_2_amphrs_charging;
  float battery_2_amphrs_discharging;
  float battery_2_amphrs_term_threshold;
  float battery_2_amphrs_init_threshold;
  float battery_2_voltage_term_threshold;
  float battery_2_voltage_init_threshold;

  float sa_amphrs;
  float load_amphrs;

  elapsedMillis time_since_start;
  elapsedMillis camera_period_elapsed_time;
  elapsedMillis camera_on_elapsed_time;
  elapsedMillis battery_low_voltage_elapsed_time;
  elapsedMillis cutdown_initiation_elapsed_time;
  elapsedMillis transmit_elapsed_time;
  elapsedMillis sd_card_write_elapsed_time;
  elapsedMillis tlm_processing_time;
  elapsedMillis high_rate_elapsed_time;
  elapsedMillis medium_rate_elapsed_time;
  elapsedMillis low_rate_elapsed_time;
  elapsedMillis battery_1_charge_current_read_elapsed_time;
  elapsedMillis battery_2_charge_current_read_elapsed_time;
  elapsedMillis sa_current_read_elapsed_time;
  elapsedMillis load_current_read_elapsed_time;
  elapsedMillis rb_reinitialize_time;
  elapsedMillis elasped_time_for_rb_quality_test;

  String output_dataword;
  String valid_str;
};

struct threshold_struct
{
  int normal_battery_temperature_limit_high;
  int normal_battery_temperature_limit_low;
  int survival_battery_temperature_limit_high;
  int survival_battery_temperature_limit_low;
  unsigned int normal_transmit_period;
  unsigned int spare_transmit_period;
  unsigned int transit_transmit_period;
  unsigned int emergency_transit_transmit_period;
  unsigned int load_shed_transmit_period;
};


void initialize_database()
{
  long temp_zero = 0;
  
  //{Type,Bitsize,int,long,float,SD_Title,Calibration,Format1,Format2}
  
  db[0] = {"header",8,null_int,null_long,null_float,"Blank",0,0,0};
  db[1] = {"header",8,null_int,null_long,null_float,"Header [-]",0,1,1};         // FIX
  db[2] = {"int",8,parameters.telemetry_format,null_long,null_float,"Format [-]",0,1,1};         // FIX
  db[3] = {"int",32,parameters.time_since_start/1000,null_long,null_float,"Elapsed Time [s]",0,1,1};  // FIX
  db[4] = {"int",8,parameters.vehicle_mode,null_long,null_float,"Veh Mode",0,1,1};
  db[5] = {"int",8,parameters.command_count,null_long,null_float,"Command Count",0,1,0};
  db[6] = {"int",8,thresholds.normal_transmit_period,null_long,null_float/1000.0,"Normal Trans Per [s]",0,1,0};
  db[7] = {"int",8,thresholds.load_shed_transmit_period,null_long,null_float/1000.0,"Loadshed Trans Per [s]",0,1,0};
  db[8] = {"int",8,thresholds.transit_transmit_period,null_long,null_float/1000.0,"Transit Trans Per [s]",0,1,0};
  db[9] = {"int",8,thresholds.emergency_transit_transmit_period/1000.0,null_long,null_float,"Emer Trans Per [s]",0,1,0};
  db[10] = {"int",8,thresholds.spare_transmit_period/1000,null_long,null_float,"Spare Trans Per [s]",0,1,0};
  db[11] = {"long",8,null_int,parameters.sd_card_write_period/1000,null_float,"SD Card Write Per [s]",0,1,0};
  db[12] = {"long",12,null_int,raw_telemetry_data.raw_battery_1_temp_1,null_float,"B 1 T 1 [C]",2,1,1};
  db[13] = {"long",12,null_int,raw_telemetry_data.raw_battery_1_temp_2,null_float,"B 1 T 2 [C]",2,1,1};
  db[14] = {"long",12,null_int,raw_telemetry_data.raw_battery_2_temp_1,null_float,"B 2 T 1 [C]",2,1,1};
  db[15] = {"long",12,null_int,raw_telemetry_data.raw_battery_2_temp_2,null_float,"B 2 T 2 [C]",2,1,1};
  db[16] = {"long",12,null_int,raw_telemetry_data.raw_inner_external_temp,null_float,"Inner Ext T [C]",2,1,0};
  db[17] = {"long",12,null_int,raw_telemetry_data.raw_outter_external_temp,null_float,"Outer Ext T [C]",2,1,0};
  db[18] = {"long",12,null_int,raw_telemetry_data.raw_internal_temp,null_float,"Int T [C]",2,1,1};
  db[19] = {"long",12,null_int,raw_telemetry_data.raw_air_pressure,null_float,"Air Pres [Pa]",3,1,0};
  db[20] = {"long",12,null_int,raw_telemetry_data.raw_analog_VIN_voltage,null_float,"VIN Voltage [V]",4,1,0};
  db[21] = {"float",32,null_int,null_long,telemetry_data.battery_1_charge_current,"B 1 Charge Current [A]",0,1,0};
  db[22] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_batt1,"B 2 Charge Current [A]",0,1,0};
  db[23] = {"float",32,null_int,null_long,telemetry_data.busvoltage_batt1,"SA Current [A]",0,1,0};
  db[24] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_batt1,"Load Path Current [A]",0,1,0};
  db[25] = {"float",32,null_int,null_long,telemetry_data.battery_2_charge_current,"B1_shunt_V [V]",0,1,0};
  db[26] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_batt2,"B1_Bus_V [V]",0,1,1};
  db[27] = {"float",32,null_int,null_long,telemetry_data.busvoltage_batt2,"B1_load_V [V]",0,1,0};
  db[28] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_batt2,"B2_shunt_V [V]",0,1,0};
  db[29] = {"float",32,null_int,null_long,telemetry_data.sa_current,"B2_Bus_V [V]",0,1,1};
  db[30] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_sa,"B2_load_V [V]",0,1,0};
  db[31] = {"float",32,null_int,null_long,telemetry_data.busvoltage_sa,"SA_shuntV [V]",0,1,0};
  db[32] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_sa,"SA_busV [V]",0,1,0};
  db[33] = {"float",32,null_int,null_long,telemetry_data.load_path_current,"SA_load_V [V]",0,1,0};
  db[34] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_load_path,"Loadpath_shunt_V  [V]",0,1,0};
  db[35] = {"float",32,null_int,null_long,telemetry_data.busvoltage_load_path,"Loadpath_Bus_V    [V]",0,1,0};
  db[36] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_load_path,"Loadpath_load_V   [V]",0,1,0};
  db[37] = {"long",8,null_int,parameters.tlm_processing_period,null_float,"Tel Proc Per [s]",1,1,0};
  db[38] = {"float",32,null_int,null_long,gps_data.gps_processed_lat,"GPS Lat [deg]",0,1,1};
  db[39] = {"float",32,null_int,null_long,gps_data.gps_processed_long,"GPS Long [deg]",0,1,1};
  db[40] = {"float",32,null_int,null_long,gps_data.gps_altitude,"GPS Alt [m]",0,1,1};
  db[41] = {"float",32,null_int,null_long,null_float,"Max GPS Alt [m]",0,1,0}; 
  db[42] = {"float",32,null_int,null_long,null_float,"Min GPS Alt [m]",0,1,0};  
  db[43] = {"float",32,null_int,null_long,gps_data.gps_location_age,"GPS Loc Age [s]",0,1,0};
  db[44] = {"float",32,null_int,null_long,gps_data.gps_altitude_age,"GPS Alt Age [s]",0,1,0};
  db[45] = {"float",32,null_int,null_long,gps_data.gps_heading,"GPS Course [deg]",0,1,0};
  db[46] = {"float",32,null_int,null_long,gps_data.gps_speed,"GPS Speed [km/h]",0,1,0}; 
  db[47] = {"int",4,gps_data.gps_num_satellites,null_long,null_float,"GPS # of Sat [-]",0,1,0}; 
  db[48] = {"long",32,null_int,gps_data.gps_date,null_float,"GPS Date [-]",0,1,1}; 
  db[49] = {"long",32,null_int,gps_data.gps_time,null_float,"GPS Time [-]",0,1,1}; 
  db[50] = {"int",12,gps_data.gps_hdop,null_long,null_float,"HDOP Value [?]",0,1,0};
  db[51] = {"int",16,gps_data.gps_chars_processed,null_long,null_float,"GPS Chars Processed [-]",0,1,0};
  db[52] = {"int",16,gps_data.gps_sentances_with_fix,null_long,null_float,"GPS Sentences with Fix [-]",0,1,0}; 
  db[53] = {"int",16,gps_data.gps_failed_checksum,null_long,null_float,"GPS Failed Checksum [-]",0,1,0};             
  db[54] = {"int",1,gps_data.gps_location_valid,null_long,null_float,"GPS Location Isvalid [-]",0,1,0};
  db[55] = {"int",1,gps_data.gps_altitude_valid,null_long,null_float,"GPS Altitude Isvalid [-]",0,1,0};
  db[56] = {"int",1,gps_data.gps_heading_valid,null_long,null_float,"GPS Course Isvalid [-]",0,1,0};
  db[57] = {"int",1,gps_data.gps_speed_valid,null_long,null_float,"GPS Speed Isvalid [-]",0,1,0};
  db[58] = {"int",1,gps_data.gps_num_satellites_valid,null_long,null_float,"GPS Satellites Isvalid [-]",0,1,0};
  db[59] = {"int",1,gps_data.gps_date_valid,null_long,null_float,"GPS Date Isvalid [-]",0,1,0};
  db[60] = {"int",1,gps_data.gps_time_valid,null_long,null_float,"GPS Time Isvalid [-]",0,1,0};
  db[61] = {"int",1,gps_data.gps_hdop_valid,null_long,null_float,"GPS HDOP Isvalid [-]",0,1,0};
  db[62] = {"float",32,null_int,null_long,null_float,"Max Acc X [g]",0,1,0};   
  db[63] = {"float",32,null_int,null_long,null_float,"Min Acc X [g]",0,1,0}; 
  db[64] = {"float",32,null_int,null_long,null_float,"Mean Acc X [g]",0,1,0}; 
  db[65] = {"float",32,null_int,null_long,null_float,"Max Acc Y [g]",0,1,0};   
  db[66] = {"float",32,null_int,null_long,null_float,"Min Acc Y [g]",0,1,0}; 
  db[67] = {"float",32,null_int,null_long,null_float,"Mean Acc Y [g]",0,1,0};
  db[68] = {"float",32,null_int,null_long,null_float,"Max Acc Z [g]",0,1,0};   
  db[69] = {"float",32,null_int,null_long,null_float,"Min Acc Z [g]",0,1,0}; 
  db[70] = {"float",32,null_int,null_long,null_float,"Mean Acc z [g]",0,1,0};
  db[71] = {"float",32,null_int,null_long,null_float,"Max Gyro X [deg/s]",0,1,0};   
  db[72] = {"float",32,null_int,null_long,null_float,"Min Gyro X [deg/s]",0,1,0}; 
  db[73] = {"float",32,null_int,null_long,null_float,"Mean Gyro X [deg/s]",0,1,0}; 
  db[74] = {"float",32,null_int,null_long,null_float,"Max Gyro Y [deg/s]",0,1,0};   
  db[75] = {"float",32,null_int,null_long,null_float,"Min Gyro Y [deg/s]",0,1,0}; 
  db[76] = {"float",32,null_int,null_long,null_float,"Mean Gyro Y [deg/s]",0,1,0}; 
  db[77] = {"float",32,null_int,null_long,null_float,"Max Gyro Z [deg/s]",0,1,0};   
  db[78] = {"float",32,null_int,null_long,null_float,"Min Gyro Z [deg/s]",0,1,0}; 
  db[79] = {"float",32,null_int,null_long,null_float,"Mean Gyro Z [deg/s]",0,1,0};     
  db[80] = {"float",32,null_int,null_long,gyro.mx,"Gyro mag X [G]",0,1,0}; 
  db[81] = {"float",32,null_int,null_long,gyro.my,"Gyro mag Y [G]",0,1,0}; 
  db[82] = {"float",32,null_int,null_long,gyro.mz,"Gyro mag Z [G]",0,1,0};  
  db[83] = {"float",32,null_int,null_long,gyro.gyro_temp,"Gyro T [C]",0,1,0};
  db[84] = {"float",32,null_int,null_long,parameters.voltage_sanity_check_high,"Volt San Check High [V]",0,1,0};
  db[85] = {"float",32,null_int,null_long,parameters.voltage_sanity_check_low,"Volt San Check Low [V]",0,1,0};
  db[86] = {"float",32,null_int,null_long,parameters.charge_current_sanity_check_high,"Charge Cur Sanity High [A]",0,1,0};
  db[87] = {"float",32,null_int,null_long,parameters.charge_current_sanity_check_low,"Charge Cur Sanity Low [A]",0,1,0};
  db[88] = {"float",32,null_int,null_long,parameters.battery_1_recharge_ratio,"B 1 Recharge Ratio [-]",0,0,0};
  db[89] = {"float",32,null_int,null_long,parameters.battery_1_amphrs_charging,"B 1 AH Charging [A]",0,1,0};
  db[90] = {"float",32,null_int,null_long,parameters.battery_1_amphrs_discharging,"B 1 AH Discharging [A]",0,1,0};
  db[91] = {"float",32,null_int,null_long,parameters.battery_2_recharge_ratio,"B 2 Recharge Ratio [-]",0,0,0};
  db[92] = {"float",32,null_int,null_long,parameters.battery_2_amphrs_charging,"B 2 Amp Hours Charging [A]",0,1,0};
  db[93] = {"float",32,null_int,null_long,parameters.battery_2_amphrs_discharging,"B 2 Amp Hours Discharging [A]",0,1,0};
  db[94] = {"float",32,null_int,null_long,parameters.sa_amphrs,"SA Total Amp Hrs [A.h]",0,1,0};
  db[95] = {"float",32,null_int,null_long,parameters.load_amphrs,"Load Total Amp Hrs [A.h]",0,1,0};
  db[96] = {"int",16,parameters.battery_temperature_limit_high,null_long,null_float,"B Active T Lim High [C]",0,1,0};
  db[97] = {"int",16,parameters.battery_temperature_limit_low,null_long,null_float,"B Active T Lim Low [C]",0,1,0}; 
  db[98] = {"int",16,parameters.battery_temperature_sanity_check_high,null_long,null_float,"B T Sanity Check High [C]",0,1,0};
  db[99] = {"int",16,thresholds.normal_battery_temperature_limit_high,null_long,null_float,"B Normal T Lim High [C]",0,1,0};
  db[100] = {"int",16,thresholds.normal_battery_temperature_limit_low,null_long,null_float,"B Normal T Lim Low [C]",0,1,0};
  db[101] = {"int",16,thresholds.survival_battery_temperature_limit_high,null_long,null_float,"B Surv T Lim High [C]",0,1,0};
  db[102] = {"int",16,thresholds.survival_battery_temperature_limit_low,null_long,null_float,"B Surv T Lim Low [C]",0,1,0};         
  db[103] = {"int",16,parameters.battery_temperature_sanity_check_low,null_long,null_float,"B T Sanity Check Low [C]",0,1,0};     
  db[104] = {"float",32,null_int,null_long,parameters.low_voltage_limit_for_loadshed_entry,"Loadshed Entry Volt Lim [V]",0,1,0};      
  db[105] = {"float",32,null_int,null_long,parameters.low_voltage_limit_for_auto_cutdown,"Auto Cutdown Volt Lim [V]",0,1,0};
  db[106] = {"int",16,parameters.low_voltage_time_limit,null_long,null_float,"Low Volt Time until Cut-down [s]",0,1,0};
  db[107] = {"int",16,parameters.altitude_limit_low,null_long,null_float,"Alt Limit Low [ft]",0,1,0};
  db[108] = {"int",16,parameters.altitude_sanity_check_low,null_long,null_float,"Alt Sanity Check Low [ft]",0,1,0}; 
  db[109] = {"int",12,parameters.cutdown_pulse_width,null_long,null_float,"Pyro Pulse Width [s]",0,1,0};
  db[110] = {"int",12,parameters.camera_period,null_long,null_float,"Cam Per [s]",0,1,0};
  db[111] = {"int",12,parameters.camera_on_time,null_long,null_float,"Cam On Time [s]",0,1,0};
  db[112] = {"int",1,parameters.battery_bus_low_voltage_flag,null_long,null_float,"Battery Bus Low V Flag [-]",0,1,0};
  db[113] = {"int",1,parameters.heater_state_1,null_long,null_float,"Heater State 1 [-]",0,1,0};
  db[114] = {"int",1,parameters.heater_state_2,null_long,null_float,"Heater State 2 [-]",0,1,0};
  db[115] = {"int",1,parameters.cutdown_enable_state,null_long,null_float,"Cut-down Enable [-]",0,1,0};
  db[116] = {"int",1,parameters.cutdown_1_status,null_long,null_float,"Cut-down 1 Fire Status [-]",0,1,0};
  db[117] = {"int",1,parameters.cutdown_2_status,null_long,null_float,"Cut-down 2 Fire Status [-]",0,1,0};
  db[118] = {"int",1,parameters.altitude_valid_flag,null_long,null_float,"Alt Valid Flag [-]",0,1,0};
  db[119] = {"int",1,parameters.camera_enabled,null_long,null_float,"Cam Enabled [-]",0,1,0};
  db[120] = {"int",1,parameters.camera_status,null_long,null_float,"Cam Status [-]",0,1,0};
  db[121] = {"int",1,parameters.battery_1_temp_tlm_valid_flag,null_long,null_float,"B 1 T TLM Val Flag [-]",0,1,0};         
  db[122] = {"int",1,parameters.battery_2_temp_tlm_valid_flag,null_long,null_float,"B 2 T TLM Val Flag [-]",0,1,0}; 
  db[123] = {"int",1,parameters.battery_voltage_tlm_valid_flag,null_long,null_float,"Bus V TLM Val Flag [-]",0,1,0};
  db[124] = {"int",1,parameters.battery_1_current_tlm_valid_flag,null_long,null_float,"B 1 Cur TLM Val Flag [-]",0,1,0};
  db[125] = {"int",1,parameters.battery_2_current_tlm_valid_flag,null_long,null_float,"B 2 Cur TLM Val Flag []",0,1,0};
  db[126] = {"float",32,null_int,null_long,alt.altitude_in_feet,"Alt [ft]",0,1,1};
  db[127] = {"float",32,null_int,null_long,null_float,"Max Alt [ft]",0,1,0}; 
  db[128] = {"float",32,null_int,null_long,null_float,"Min Alt [ft]",0,1,0}; 
  db[129] = {"float",32,null_int,null_long,alt.temperature,"Alt T [C]",0,1,0};
  db[130] = {"float",32,null_int,null_long,alt.pressure,"Alt Pressure [?]",0,1,0};
  db[131] = {"float",32,null_int,null_long,null_float,"Max Pressure [?]",0,1,0}; 
  db[132] = {"float",32,null_int,null_long,null_float,"Min Pressure [?]",0,1,0}; 
  db[133] = {"int",8,parameters.num_rb_words_recieved,null_long,null_float,"RB Words Recieved",0,1,0}; 
}

#endif /* _FLIGHT_DEFINITIONS_ */
