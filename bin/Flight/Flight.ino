#include "definitions.h"
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
#include "Time.h"


struct telemetry_data_struct telemetry_data;
struct satellite_data_struct satellite_data;
struct parameter_struct parameters;

int ret_val = 0;

void setup() 
{
   Serial.begin(9600); 
   Serial1.begin(4800);
   Serial.println("Flight1 starting up");
   set_defaults();
   set_pins();
   gyro_setup();
}

void loop() 
{
   // Check if there is any data waiting for us from ROCKBLOCK
   ret_val = read_satellite_data();
   
   if(ret_val == COMMANDS_TO_PROCESS)
   {
     //Process satellite commands from ROCKBLOCK, blocking unless it's time to make a loop cycle
     // 1 - Cord Snap
     // 2 - Turn on/off feed from solar panels
     // 3 - Set hi/low temperature thresholds 
     // 4 - Set sampling rate
     // 5 - Set camera rate
     // 6 - Set data format
     process_satellite_data();
   }
  
   //Collect telemetry
   colect_telemetry();
   
   //Is there anything to act on from tolemety read?
   // 1 - Turn heating element on/off if temperature breaks threshold()
   // 2 - Check voltage time() to check if cutdown is needed
   // 3 - Mode change (Low/Normal/High)
   process_telemetry();

   print_telemetry();
   
   //Is it time to turn camera on/off?
   process_camera_function();
 
   //Either sleep or continue looping, based on battery draw tests
   delay(parameters.loop_sleep);
}

void set_defaults()
{
  parameters.loop_sleep = DEFAULT_LOOP_SLEEP;
  parameters.low_voltage_limit = DEFAULT_VOLTAGE_LOW_LIMIT;
  parameters.battery_temperature_limit_high = DEFAULT_BATTERY_TEMP_LIMIT_HIGH;
  parameters.battery_temperature_limit_low = DEFAULT_BATTERY_TEMP_LIMIT_LOW;
  parameters.low_voltage_time_limit = DEFAULT_LOW_VOLTAGE_TIME_LIMIT;
  parameters.battery_1_low_voltage_flag = false;
  parameters.battery_2_low_voltage_flag = false;
}

void set_pins()
{
   //Setup outout pins
   pinMode(PIN_POWER_SHUTDOWN, OUTPUT);
   pinMode(PIN_PYRO_ENABLE, OUTPUT);
   pinMode(PIN_PYRO_1_FIRE, OUTPUT);
   pinMode(PIN_PYRO_2_FIRE, OUTPUT);
   pinMode(PIN_HEATER_CONTROL_1, OUTPUT);
   pinMode(PIN_HEATER_CONTROL_2, OUTPUT);
   pinMode(PIN_CAMERA_SWITCH, OUTPUT);
}
