//Command Sentinels
#define NO_COMMANDS_TO_PROCESS        0 
#define COMMANDS_TO_PROCESS           1

//Constants
#define PRESSURE_CONSTANT   0.0036140
#define TEMP_CONSTANT_1     0.0497
#define TEMP_CONSTANT_2     54.794
#define VOLTAGE_CONSTANT_1  3.3
#define VOLTAGE_CONSTANT_2  4095
#define VOLTAGE_CONSTANT_3  3.83


//Defaults
#define DEFAULT_LOOP_SLEEP                1
#define DEFAULT_VOLTAGE_LOW_LIMIT         8.5
#define DEFAULT_BATTERY_TEMP_LIMIT_HIGH   27
#define DEFAULT_BATTERY_TEMP_LIMIT_LOW    23
#define DEFAULT_LOW_VOLTAGE_TIME_LIMIT    1800


//Pins
#define PIN_PRESSURE_SENSOR          A0
#define PIN_BATTERY1_1_TEMP          A8
#define PIN_BATTERY1_2_TEMP          A9
#define PIN_BATTERY2_1_TEMP          A10
#define PIN_BATTERY2_2_TEMP          A11
#define PIN_EXTERNAL_OUTTER_TEMP     A5
#define PIN_EXTERNAL_INNER_TEMP      A7
#define PIN_INTERNAL_TEMP            A6
#define PIN_BATTERY_VOLTAGE_1        A3
#define PIN_BATTERY_VOLTAGE_2        A4
#define PIN_TEMP_FAULT_FLAG          5
#define PIN_CHARGE_FLAG              6
#define PIN_POWER_SHUTDOWN           7
#define PIN_PYRO_ENABLE              2
#define PIN_PYRO_1_FIRE              3
#define PIN_PYRO_2_FIRE              4 
#define PIN_HEATER_CONTROL_1         11
#define PIN_HEATER_CONTROL_2         12
#define PIN_CAMERA_SWITCH            9


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
  int loop_sleep;
  
  int battery_temperature_limit_high;
  int battery_temperature_limit_low;
  double low_voltage_limit;
  int low_voltage_time_limit;
  bool battery_1_low_voltage_flag;
  bool battery_2_low_voltage_flag;
  int battery_1_low_voltage_start_time;
  int battery_2_low_voltage_start_time;
};
