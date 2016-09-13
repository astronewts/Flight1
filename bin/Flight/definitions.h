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
#define DB_SIZE                         134 //132 // Size of the Database
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
  float max_altitude_in_feet;
  float min_altitude_in_feet;
  float temperature;
  float pressure;
  float max_pressure;
  float min_pressure;

  unsigned int count_between_RB;
};

struct gyro_struct
{
  float gyro_temp;
  float ax;
  float sum_ax;
  float mean_ax;
  float max_ax;
  float min_ax;  
  float ay;
  float sum_ay;
  float mean_ay;
  float max_ay;
  float min_ay; 
  float az;
  float sum_az;
  float mean_az;
  float max_az;
  float min_az; 
  float gx;
  float sum_gx;
  float mean_gx;
  float max_gx;
  float min_gx; 
  float gy;
  float sum_gy;
  float mean_gy;
  float max_gy;
  float min_gy; 
  float gz;
  float sum_gz;
  float mean_gz;
  float max_gz;
  float min_gz; 
  float mx;
  float my;
  float mz;
  int count_between_RB;
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
  float max_gps_altitude;
  float min_gps_altitude;
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

  unsigned int count_between_RB;
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




#endif /* _FLIGHT_DEFINITIONS_ */
