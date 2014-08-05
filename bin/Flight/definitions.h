
#define NO_COMMANDS_TO_PROCESS        0 
#define COMMANDS_TO_PROCESS           1

//Constants
#define PRESSURE_CONSTANT  0.230769231
#define TEMP_CONSTANT_1     0.0497
#define TEMP_CONSTANT_2     54.794
#define VOLTAGE_CONSTANT_1  3.3
#define VOLTAGE_CONSTANT_2  4095
#define VOLTAGE_CONSTANT_3  3.83


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

#define RESOLUTION_PRESSURE_SENSOR   12



struct gps_struct
{
  
};

struct gyro_struct
{
  
};

struct telemetry_data_struct
{
  float air_pressure;
  float battery_temp_1_1;
  float battery_temp_1_2;
  float battery_temp_2_1;
  float battery_temp_2_2;
  float inner_external_temp;
  float outter_external_temp;
  float internal_temp;
  float battery_voltage_1;
  float battery_voltage_2;
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
};
