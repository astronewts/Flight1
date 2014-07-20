#include <TinyGPS++.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU9150Lib.h"
#include "CalLib.h"
#include "DueFlash.h"
#include <dmpKey.h>
#include <dmpmap.h>
#include <inv_mpu.h>
#include <inv_mpu_dmp_motion_driver.h>

#define NO_COMMANDS_TO_PROCESS        0 
#define COMMANDS_TO_PROCESS           1
#define PIN_PRESSURE_SENSOR          A0
#define RESOLUTION_PRESSURE_SENSOR   12

struct telemetry_data_struct
{
  int air_pressure_value;
};

struct satellite_data_struct
{
  char* command;
};

struct parameter_struct
{
  int loop_sleep;
};
