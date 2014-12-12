#include <elapsedMillis.h>

#define DEBUG

//Command Sentinels
#define NO_COMMANDS_TO_PROCESS        0 
#define COMMANDS_TO_PROCESS           1

//Constants
#define PRESSURE_CONSTANT   0.0036140
#define TEMP_CONSTANT_1     0.0497
#define TEMP_CONSTANT_2     54.794
#define VOLTAGE_CONSTANT_1  3.3
#define VOLTAGE_CONSTANT_2  4095.0
#define VOLTAGE_CONSTANT_3  3.83
#define CHARGE_CONSTANT_1   3.3
#define CHARGE_CONSTANT_2   26.0
#define CHARGE_CONSTANT_3   4095.0
#define MS_IN_SEC           1000.0
#define SECS_IN_HOUR        3600.0

//Defaults
#define DEFAULT_MODE                               1
#define DEFAULT_LOOP_SLEEP                         3000
#define DEFAULT_VOLTAGE_LOW_LIMIT                  8.5
#define DEFAULT_LOW_VOLTAGE_TIME_LIMIT             1800
#define DEFAULT_VOLTAGE_SANITY_CHECK_HIGH          2.4
#define DEFAULT_VOLTAGE_SANITY_CHECK_LOW           4.0
#define DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_HIGH     300
#define DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_LOW      23
#define DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_HIGH   300
#define DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_LOW    23
#define DEFAULT_BATTERY_TEMP_SANITY_CHECK_HIGH     150
#define DEFAULT_BATTERY_TEMP_SANITY_CHECK_LOW      -80    
#define DEFAULT_NORMAL_TRANSMIT_RATE               10000
#define DEFAULT_TEST_TRANSMIT_RATE                 5000
#define DEFAULT_TRANSIT_TRANSMIT_RATE              30000
#define DEFAULT_EMERGENCY_TRANSIT_TRANSMIT_RATE    45000
#define DEFAULT_LOAD_SHED_TRANSMIT_RATE            60000
#define DEFAULT_PYRO_PULSE_WIDTH                   2000
#define DEFAULT_SD_CARD_WRITE_RATE                 60000
#define DEFAULT_CAMERA_PERIOD                      1200000
#define DEFAULT_CAMERA_ON_TIME                     10000
#define DEFAULT_ALTITUDE_LIMIT_LOW                 21336
#define DEFAULT_ALTITUDE_SANITY_CHECK_LOW          1000
#define DEFAULT_RECHARGE_RATIO                     1.1
#define DEFAULT_VOLTAGE_POWER_LIMIT_HIGH           12.4
#define DEFAULT_VOLTAGE_POWER_LIMIT_LOW            10.0
#define DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW    3.0
#define DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH   -2.0
#define DEFAULT_CAPACITY_LIMIT_HIGH                -0.1
#define DEFAULT_CAPACITY_LIMIT_LOW                 -0.5          
           

//Analog Pins
#define PIN_PRESSURE_SENSOR          A0
#define PIN_CHARGE_CURRENT_1         A1
#define PIN_CHARGE_CURRENT_2         A2
#define PIN_BATTERY_VOLTAGE_1        A3
#define PIN_BATTERY_VOLTAGE_2        A4
#define PIN_EXTERNAL_OUTTER_TEMP     A5
#define PIN_INTERNAL_TEMP            A6
#define PIN_EXTERNAL_INNER_TEMP      A7
#define PIN_BATTERY1_1_TEMP          A8
#define PIN_BATTERY1_2_TEMP          A9
#define PIN_BATTERY2_1_TEMP          A10
#define PIN_BATTERY2_2_TEMP          A11

//Digital Pins
#define PIN_PYRO_ENABLE              2
#define PIN_PYRO_1_FIRE              3
#define PIN_PYRO_2_FIRE              4 
#define PIN_TEMP_FAULT_FLAG          5
#define PIN_CHARGE_FLAG              6
#define PIN_POWER_SHUTDOWN           7
#define PIN_CAMERA_SWITCH            9
#define PIN_HEATER_CONTROL_1         11
#define PIN_HEATER_CONTROL_2         12


#define RESOLUTION_PRESSURE_SENSOR   12



struct gps_struct
{
  
};

struct gyro_struct
{
  
};

struct telemetry_data_struct
{
  double air_pressure;
  double battery_temp_1_1;
  double battery_temp_1_2;
  double battery_temp_2_1;
  double battery_temp_2_2;
  double inner_external_temp;
  double outter_external_temp;
  double internal_temp;
  double battery_voltage_1;
  double battery_voltage_2;
  double charge_current_1;
  double charge_current_2;
  struct gps_struct gps_values;
  struct gyro_struct gyro_values;
  int charge_flag;
  int temp_fault_flag;  
  
};

struct satellite_data_struct
{
  char* command;
};

struct parameter_struct
{
  unsigned long loop_sleep;
  unsigned long transmit_rate;  
  unsigned long sd_card_write_rate;
  int vehicle_mode;
  int command_count;
  int battery_temperature_limit_high;
  int battery_temperature_limit_low;
  int battery_temperature_sanity_check_high;
  int battery_temperature_sanity_check_low;
  bool heater_state_1;
  bool heater_state_2;
  double low_voltage_limit;
  unsigned long low_voltage_time_limit;
  bool battery_low_voltage_flag;
  double voltage_sanity_check_high;
  double voltage_sanity_check_low;
  double voltage_power_limit_high;
  double voltage_power_limit_low;
  double charge_current_sanity_check_high;
  double charge_current_sanity_check_low;
  double amphrs_charging;
  double amphrs_discharging;
  double capacity_limit_high;
  double capacity_limit_low;
  
  unsigned long pyro_pulse_width;
  bool pyro_enable;
  bool pyro_1_status;
  bool pyro_2_status;
  bool camera_flag;
  unsigned long camera_period;
  unsigned long camera_on_time;
  bool batttery_charge_shutdown;
  bool altitude_valid_flag;
  int altitude_limit_low;
  int altitude_sanity_check_low;
  double recharge_ratio;
  elapsedMillis camera_period_elapsed_time;
  elapsedMillis camera_on_elapsed_time;
  elapsedMillis battery_low_voltage_elapsed_time;
  elapsedMillis pyro_initiation_elapsed_time;
  elapsedMillis transmit_elapsed_time;
  elapsedMillis sd_card_write_elapsed_time;
  elapsedMillis charge_current_read_elapsed_time;
  
  String output_dataword;
  String valid_str;
  
};

struct threshold_struct
{
  int normal_battery_temperature_limit_high;
  int normal_battery_temperature_limit_low;
  int survival_battery_temperature_limit_high;
  int survival_battery_temperature_limit_low;
  unsigned long normal_transmit_rate;
  unsigned long test_transmit_rate;
  unsigned long transit_transmit_rate;
  unsigned long emergency_transit_transmit_rate;
  unsigned long load_shed_transmit_rate;
};
  
