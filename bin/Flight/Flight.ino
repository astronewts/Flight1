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
#include <elapsedMillis.h>

struct telemetry_data_struct telemetry_data;
struct satellite_data_struct satellite_data;
struct parameter_struct parameters;
struct threshold_struct thresholds;

int ret_val = 0;


void setup() 
{
   Serial.begin(9600); 
   Serial1.begin(4800);
   Serial.println("Flight1 starting up");
   set_output_pins();
   set_defaults();
   gyro_setup();
}

void loop() 
{ 
   // Check if there is any data waiting for us from ROCKBLOCK
   ret_val = read_satellite_data();
   
   if(ret_val == COMMANDS_TO_PROCESS)
   {
     process_satellite_data();
   }
   
   //Collect telemetry
   colect_telemetry();
   
   //Process telemetry
   process_telemetry();

   //Print telemetry
   #ifdef DEBUG
   print_telemetry();
   #endif
   
   //Is it time to turn camera on/off?
   process_camera_function();
   
   //Check if time to write data to ROCKBLOCK
   if(parameters.transmit_elapsed_time > parameters.transmit_rate)
   {
      write_satellite_data();
      parameters.transmit_elapsed_time = 0;
   }
   
   //Check if time to write data to SD Card 
   if(parameters.sd_card_write_elapsed_time > parameters.sd_card_write_rate)
   {
   
      //write_telemetry_data_to_sd();
      parameters.sd_card_write_elapsed_time = 0;
   }
}

void set_defaults()
{
  thresholds.normal_battery_temperature_limit_high = DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_HIGH;
  thresholds.normal_battery_temperature_limit_low  = DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_LOW;
  thresholds.survival_battery_temperature_limit_high = DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_HIGH;
  thresholds.survival_battery_temperature_limit_low  = DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_LOW;
  
  thresholds.normal_transmit_rate = DEFAULT_NORMAL_TRANSMIT_RATE;
  thresholds.test_transmit_rate = DEFAULT_TEST_TRANSMIT_RATE;
  thresholds.transit_transmit_rate = DEFAULT_TRANSIT_TRANSMIT_RATE;
  thresholds.load_shed_transmit_rate = DEFAULT_LOAD_SHED_TRANSMIT_RATE;
  thresholds.emergency_transit_transmit_rate = DEFAULT_EMERGENCY_TRANSIT_TRANSMIT_RATE;

  parameters.loop_sleep = DEFAULT_LOOP_SLEEP;
  parameters.low_voltage_limit = DEFAULT_VOLTAGE_LOW_LIMIT;
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
  parameters.low_voltage_time_limit = DEFAULT_LOW_VOLTAGE_TIME_LIMIT;
  parameters.battery_low_voltage_flag = false;
  parameters.battery_low_voltage_flag = false;
  parameters.transmit_rate = thresholds.normal_transmit_rate;
  parameters.sd_card_write_rate = DEFAULT_SD_CARD_WRITE_RATE;
  parameters.pyro_pulse_width = DEFAULT_PYRO_PULSE_WIDTH;
  
  //Set Digital Pin States
  digitalWrite(PIN_POWER_SHUTDOWN, LOW);
  digitalWrite(PIN_PYRO_ENABLE, LOW);
  digitalWrite(PIN_PYRO_1_FIRE, LOW);
  digitalWrite(PIN_PYRO_2_FIRE, LOW);
  
  // Change the analog read resolution to 12 bits
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
}

void set_output_pins()
{
   //Setup outout pins
   pinMode(PIN_POWER_SHUTDOWN, OUTPUT);
   pinMode(PIN_HEATER_CONTROL_1, OUTPUT);
   pinMode(PIN_HEATER_CONTROL_2, OUTPUT);
   pinMode(PIN_PYRO_ENABLE, OUTPUT);
   pinMode(PIN_PYRO_1_FIRE, OUTPUT);
   pinMode(PIN_PYRO_2_FIRE, OUTPUT);
   pinMode(PIN_CAMERA_SWITCH, OUTPUT);
}

void set_load_shed_mode()
{
   //Set Camera flag to false
   parameters.camera_flag = false;
  
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
   
   //Turn Power On
   digitalWrite(PIN_POWER_SHUTDOWN, LOW);
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.load_shed_transmit_rate;
}

void set_normal_mode()
{ 
   //Set Camera flag to true
   parameters.camera_flag = true;
   
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.normal_transmit_rate;
}

void set_test_mode()
{   
   //Set Camera flag to true
   parameters.camera_flag = true;
   
   //Set Heater Thresholds to normal settings
   parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.test_transmit_rate;
}

void set_emergency_decent_mode()
{
   //Turn Camera Flag to false
   parameters.camera_flag = false;
   
   //Set Heater Thresholds to survival settings
   parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.emergency_transit_transmit_rate;
   
   //Fire Pyro
   pyro_fire();
}

void pyro_fire()
{
   //Enable Pyro pin
   digitalWrite(PIN_PYRO_ENABLE, HIGH);
   
   //Set primary pin to high
   digitalWrite(PIN_PYRO_1_FIRE, HIGH);
   
   //Mark time that pyro was initiated 
   parameters.pyro_initiation_elapsed_time = 0;
}
