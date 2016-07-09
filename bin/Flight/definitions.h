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
#define DEFAULT_NORMAL_TRANSMIT_RATE               15000 // TEST ONLY!!!
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
#define DEFAULT_SD_CARD_WRITE_PERIOD               6000 // TEST ONLY!!!

//#define DEFAULT_SD_CARD_WRITE_PERIOD             60000  // FOR FLIGHT !!!
#define DEFAULT_TLM_PROCESSING_PERIOD              5000
#define DEFAULT_CAMERA_PERIOD                      1200000
#define DEFAULT_CAMERA_ON_TIME                     10000
#define HIGH_RATE_PERIOD                           50 // ms
#define MEDIUM_RATE_PERIOD                         20000 // ms
#define LOW_RATE_PERIOD                            20000 // ms
#define HIGH_RATE_PERIOD_CUTDOWN                   20 // ms
#define MEDIUM_RATE_PERIOD_CUTDOWN                 20000 // ms
#define LOW_RATE_PERIOD_CUTDOWN                    20000 // ms

#define DEFAULT_ALTITUDE_LIMIT_LOW                 65000  // 97:for test in living room  //  A2 ft
#define DEFAULT_ALTITUDE_SANITY_CHECK_LOW          75000  // 101:for test in living room //  A1 ft // for the new code (line 400ish in telemetry_module) A1 > A2

#define DEFAULT_B1_RECHARGE_RATIO                  1.1
#define DEFAULT_B1_AMPHRS_TERM_THRESHOLD           -0.1
#define DEFAULT_B1_AMPHRS_INIT_THRESHOLD           -0.5
#define DEFAULT_B1_VOLTAGE_TERM_THRESHOLD           12.5
#define DEFAULT_B1_VOLTAGE_INIT_THRESHOLD           10.0

#define DEFAULT_B2_RECHARGE_RATIO                   1.1
#define DEFAULT_B2_AMPHRS_TERM_THRESHOLD           -0.1
#define DEFAULT_B2_AMPHRS_INIT_THRESHOLD           -0.5
#define DEFAULT_B2_VOLTAGE_TERM_THRESHOLD           12.5
#define DEFAULT_B2_VOLTAGE_INIT_THRESHOLD           10.0

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
#define PIN_BATTERY_1_CHARGE_CURRENT_1         A1
#define PIN_BATTERY_1_CHARGE_CURRENT_2         A2
#define PIN_BATTERY_1_VOLTAGE_1                A3
#define PIN_BATTERY_1_VOLTAGE_2                A4
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

// GYRO CONSTANTS:

struct alt_struct
{
  double altitude_in_feet;
  double temperature;
  double pressure;
};

struct gyro_struct
{
  double gyro_temp;
  double ax;
  double ay;
  double az;
  double gx;
  double gy;
  double gz;
  double mx;
  double my;
  double mz;
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
  double battery_1_voltage_1;
  double battery_1_voltage_2;


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

struct rb_data_struct
{
  unsigned int message_id : 7;
  unsigned int delta_encoded : 1; // always 0 for this struct
  unsigned int balloon_id : 8;
  unsigned int vehicle_mode : 8;
  unsigned int command_count : 8;
  unsigned int normal_transmit_period : 8; // seconds?
  unsigned int load_shed_transmit_period : 8;
  unsigned int transit_transmit_period : 8;
  unsigned int emergency_transit_transmit_period : 8;
  unsigned int spare_transmit_period : 8;
  unsigned int sd_card_write_period : 8;
  unsigned int raw_battery_1_temp_1 : 12;
  unsigned int raw_battery_1_temp_2 : 12;
  unsigned int raw_battery_2_temp_1 : 12;
  unsigned int raw_battery_2_temp_2 : 12;
  unsigned int raw_inner_external_temp : 12;
  unsigned int raw_outer_external_temp : 12;
  unsigned int raw_internal_temp : 12;
  unsigned int raw_air_pressure : 12;
  unsigned int raw_battery_1_voltage_1 : 12;
  unsigned int raw_battery_1_voltage_2 : 12;
  unsigned int battery_1_charge_current : 12; // milliamps? offset?
  unsigned int battery_2_charge_current : 12; // milliamps? offset?
  unsigned int sa_current : 12;
  unsigned int load_path_current : 12;
  unsigned int tlm_processing_period : 8;

  unsigned int gps_processed_lat : 16;
  unsigned int gps_processed_long : 16;
  unsigned int gps_altitude : 16;
  unsigned int gps_age : 8; // MAX(gps_alt_age, gps_pos_age) in seconds
  unsigned int gps_heading : 8; // 0-255
  unsigned int gps_speed : 8; // km/hr
  unsigned int gps_time : 32;
  unsigned int gps_valid : 1;

  unsigned int gyro_temp : 8;

  // What else do we really need from 40-89?
  unsigned int battery_1_recharge_ratio : 8;
  unsigned int battery_1_amphrs_charging : 16;
  unsigned int battery_1_amphrs_discharging : 16;
  unsigned int battery_2_recharge_ratio : 8;
  unsigned int battery_2_amphrs_charging : 16;
  unsigned int battery_2_amphrs_discharging : 16;

  unsigned int battery_1_charging_status : 1;
  unsigned int battery_2_charging_status : 1;
  unsigned int battery_bus_low_voltage_flag : 1;
  unsigned int heater_state_1 : 1;
  unsigned int heater_state_2 : 1;
  unsigned int cutdown_enable_state : 1;
  unsigned int cutdown_1_status : 1;
  unsigned int cutdown_2_status : 1;
};

struct rb_data_delta_struct
{
  unsigned int message_id : 7;
  unsigned int delta_encoded : 1; // always 1 for this struct

  unsigned int raw_battery_1_temp_1 : 8;
  unsigned int raw_battery_1_temp_2 : 8;
  unsigned int raw_battery_2_temp_1 : 8;
  unsigned int raw_battery_2_temp_2 : 8;
  unsigned int raw_inner_external_temp : 8;
  unsigned int raw_outer_external_temp : 8;
  unsigned int raw_internal_temp : 8;
  unsigned int raw_air_pressure : 8;
  unsigned int raw_battery_1_voltage_1 : 8;
  unsigned int raw_battery_1_voltage_2 : 8;
  unsigned int battery_1_charge_current : 8; // milliamps? offset?
  unsigned int battery_2_charge_current : 8; // milliamps? offset?
  unsigned int sa_current : 8;
  unsigned int load_path_current : 8;

  unsigned int gps_processed_lat : 8;
  unsigned int gps_processed_long : 8;
  unsigned int gps_altitude : 8;
  unsigned int gps_age : 8; // MAX(gps_alt_age, gps_pos_age) in seconds
  unsigned int gps_heading : 8; // 0-255
  unsigned int gps_speed : 8; // km/hr
  unsigned int gps_time : 12;
  unsigned int gyro_temp : 8;
};

// Use this enum whenever you need to ensure that EVERY telemetry item is iterated through

struct raw_telemetry_data_struct
{
  int raw_battery_1_temp_1 : 12;
  int raw_battery_1_temp_2 : 12;
  int raw_battery_2_temp_1 : 12;
  int raw_battery_2_temp_2 : 12;
  int raw_inner_external_temp : 12;
  int raw_outter_external_temp : 12;
  int raw_internal_temp : 12;
  int raw_air_pressure : 12;
  int raw_battery_1_voltage_1 : 12;
  int raw_battery_1_voltage_2 : 12;

  float shuntvoltage_batt1;
  float busvoltage_batt1;
  float battery_1_charge_current;
  float loadvoltage_batt1;

  float shuntvoltage_batt2;
  float busvoltage_batt2;
  float battery_2_charge_current;
  float loadvoltage_batt2;
};

struct parameter_struct
{
  unsigned long loop_sleep;
  unsigned long transmit_period;
  unsigned long sd_card_write_period;
  unsigned long tlm_processing_period;
  int vehicle_mode;
  int command_count;
  int battery_temperature_limit_high;
  int battery_temperature_limit_low;
  int battery_temperature_sanity_check_high;
  int battery_temperature_sanity_check_low;
  bool heater_state_1;
  bool heater_state_2;
  double low_voltage_limit_for_loadshed_entry;
  double low_voltage_limit_for_auto_cutdown;
  unsigned long low_voltage_time_limit;
  bool battery_bus_low_voltage_flag;
  double voltage_sanity_check_high;
  double voltage_sanity_check_low;
  double charge_current_sanity_check_high;
  double charge_current_sanity_check_low;
  unsigned long cutdown_pulse_width;
  bool cutdown_enable_state;
  bool cutdown_1_status;
  bool cutdown_2_status;
  bool camera_enabled;
  bool camera_status;
  unsigned long camera_period;
  unsigned long camera_on_time;
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

  double battery_1_recharge_ratio;
  double battery_1_amphrs_charging;
  double battery_1_amphrs_discharging;
  double battery_1_amphrs_term_threshold;
  double battery_1_amphrs_init_threshold;
  double battery_1_voltage_term_threshold;
  double battery_1_voltage_init_threshold;

  double battery_2_recharge_ratio;
  double battery_2_amphrs_charging;
  double battery_2_amphrs_discharging;
  double battery_2_amphrs_term_threshold;
  double battery_2_amphrs_init_threshold;
  double battery_2_voltage_term_threshold;
  double battery_2_voltage_init_threshold;

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
  unsigned long normal_transmit_period;
  unsigned long spare_transmit_period;
  unsigned long transit_transmit_period;
  unsigned long emergency_transit_transmit_period;
  unsigned long load_shed_transmit_period;
};

#endif /* _FLIGHT_DEFINITIONS_ */
