
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
#define DEFAULT_RB_SEND_RECIEVE_TIMEOUT            240 // seconds // Default is 300 seconds

// Use "1" for low current applications; when powered by a low-power 90 mA max USB supply, the interval between transmit retries is extended to as much as 60 seconds
// Use "0" for "high current" applications; interval between transmit retries is 20 seconds    
#define DEFAULT_RB_POWER_MODE                      1 

#define DEFAULT_PYRO_PULSE_WIDTH                   8000

#define DEFAULT_SD_CARD_WRITE_PERIOD               6000 // TEST ONLY!!!

//#define DEFAULT_SD_CARD_WRITE_PERIOD             60000  // FOR FLIGHT !!!
#define DEFAULT_TLM_PROCESSING_PERIOD              5000
#define DEFAULT_CAMERA_PERIOD                      1200000
#define DEFAULT_CAMERA_ON_TIME                     10000

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
#define CUTDOWN_TEST_TIME                           20 // 30000 // those are cycle of 1s
#define INITIALIZATION_TIMEOUT                      10000

//Vehicle Modes
#define FLIGHT_MODE                               1
#define LOADSHED_MODE                             2
#define TRANSIT_MODE                              3
#define EMERGENCY_DESCENT_MODE                    4
#define SPARE_MODE                                 5
#define CUTDOWN_TEST_MODE                         6
#define TERMINAL_TEST_MODE                        7

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


#define RESOLUTION_PRESSURE_SENSOR   12

// SD CARD CONSTANTS
#define SD_CHIP_SELECT  10  // SD chip select pin
#define USE_DS1307       0  // set nonzero to use DS1307 RTC
#define LOG_INTERVAL  1000  // mills between entries
//#define SENSOR_COUNT     3  // number of analog pins to log
#define ECHO_TO_SERIAL   1  // echo data to serial port if nonzero
#define WAIT_TO_START    1  // Wait for serial input in setup(), 1=true
#define ADC_DELAY       10  // ADC delay for high impedence sensors

// GYRO CONSTANTS:

struct alt_struct
{
  double altitude_in_feet;
  double temperature;
  double pressure;
};

struct gps_struct
{
  
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
  double battery_1_charge_current_1;
  double battery_1_charge_current_2;
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

struct raw_telemetry_data_struct
{
  int raw_battery_1_temp_1;
  int raw_battery_1_temp_2;
  int raw_battery_2_temp_1;
  int raw_battery_2_temp_2;
  int raw_inner_external_temp;
  int raw_outter_external_temp;
  int raw_internal_temp;
  int raw_air_pressure;
  int raw_battery_1_voltage_1;
  int raw_battery_1_voltage_2;
  int raw_battery_1_charge_current_1;
  int raw_battery_1_charge_current_2;
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

  int test_count;
  String user_intialization_input;
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
  elapsedMillis battery_1_charge_current_read_elapsed_time;
  elapsedMillis battery_2_charge_current_read_elapsed_time;
  elapsedMillis intialization_timeout_time;
  elapsedMillis rb_reinitialize_time;

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
  
