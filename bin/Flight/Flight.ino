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
#include <SdFat.h>
#include <SdFatUtil.h>

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
   set_normal_mode();
   sd_setup();
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
   //collect_telemetry();
   
   //Process telemetry
   process_telemetry();

   //Print telemetry
   #ifdef DEBUG
   print_telemetry();
   #endif
   
   //Process Camera
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
      write_telemetry_data_to_sd();
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

  parameters.vehicle_mode = DEFAULT_MODE;
  parameters.command_count = 0.0;
  parameters.loop_sleep = DEFAULT_LOOP_SLEEP;
  parameters.low_voltage_limit = DEFAULT_VOLTAGE_LOW_LIMIT;
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
  parameters.battery_temperature_sanity_check_high = DEFAULT_BATTERY_TEMP_SANITY_CHECK_HIGH;
  parameters.battery_temperature_sanity_check_low = DEFAULT_BATTERY_TEMP_SANITY_CHECK_LOW;
  parameters.heater_state_1 = false;
  parameters.heater_state_2 = false;
  parameters.low_voltage_time_limit = DEFAULT_LOW_VOLTAGE_TIME_LIMIT;
  parameters.battery_low_voltage_flag = false;
  parameters.voltage_sanity_check_high = DEFAULT_VOLTAGE_SANITY_CHECK_HIGH;
  parameters.voltage_sanity_check_low = DEFAULT_VOLTAGE_SANITY_CHECK_LOW;
  parameters.charge_current_sanity_check_high = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH;
  parameters.charge_current_sanity_check_low = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW;
  parameters.transmit_rate = thresholds.normal_transmit_rate;
  parameters.sd_card_write_rate = DEFAULT_SD_CARD_WRITE_RATE;
  parameters.pyro_pulse_width = DEFAULT_PYRO_PULSE_WIDTH;
  parameters.pyro_enable = false;
  parameters.pyro_1_status = false;
  parameters.pyro_2_status = false;
  parameters.camera_flag = true;
  parameters.batttery_charge_shutdown = false;
  parameters.camera_period = DEFAULT_CAMERA_PERIOD;
  parameters.camera_on_time = DEFAULT_CAMERA_ON_TIME;
  parameters.altitude_valid_flag = false;
  parameters.altitude_limit_low = DEFAULT_ALTITUDE_LIMIT_LOW;
  parameters.altitude_sanity_check_low = DEFAULT_ALTITUDE_SANITY_CHECK_LOW;
  parameters.recharge_ratio = DEFAULT_RECHARGE_RATIO;
  parameters.voltage_power_limit_high = DEFAULT_VOLTAGE_POWER_LIMIT_HIGH;
  parameters.voltage_power_limit_low = DEFAULT_VOLTAGE_POWER_LIMIT_LOW;
  parameters.voltage_sanity_check_high = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH;
  parameters.voltage_sanity_check_low = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW;
  parameters.capacity_limit_high = DEFAULT_CAPACITY_LIMIT_HIGH;
  parameters.capacity_limit_low = DEFAULT_CAPACITY_LIMIT_LOW;
  parameters.amphrs_charging = 0.0;
  parameters.amphrs_discharging = 0.0;
  
  //parameters.output_dataword[340];
  parameters.output_dataword = "0";
  //parameters.valid_str = "0";
  
  //Set Digital Pin States
  digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
  digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, LOW);
  digitalWrite(PIN_PYRO_ENABLE, LOW);
  digitalWrite(PIN_PYRO_1_FIRE, LOW);
  digitalWrite(PIN_PYRO_2_FIRE, LOW);
  
  digitalWrite(PIN_HEATER_CONTROL_1, LOW);
  digitalWrite(PIN_HEATER_CONTROL_2, LOW);
  
  // Change the analog read resolution to 12 bits
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
}

void set_output_pins()
{
   //Setup outout pins
   pinMode(SD_CHIP_SELECT, OUTPUT);
   pinMode(PIN_BATTERY_1_CHARGE_CUTOFF, OUTPUT);
   pinMode(PIN_BATTERY_2_CHARGE_CUTOFF, OUTPUT);
   pinMode(PIN_HEATER_CONTROL_1, OUTPUT);
   pinMode(PIN_HEATER_CONTROL_2, OUTPUT);
   pinMode(PIN_PYRO_ENABLE, OUTPUT);
   pinMode(PIN_PYRO_1_FIRE, OUTPUT);
   pinMode(PIN_PYRO_2_FIRE, OUTPUT);
   pinMode(PIN_CAMERA_SWITCH, OUTPUT);
   
   
}

void set_load_shed_mode()
{
   //Turn Camera Off
   digitalWrite(PIN_CAMERA_SWITCH, LOW);
   
   //Set Camera flag to false
   parameters.camera_flag = false;
  
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
   
   //Turn Power On
   digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
   digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, LOW);
   parameters.batttery_charge_shutdown = false;
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.load_shed_transmit_rate;
   parameters.vehicle_mode = 2;
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
   parameters.vehicle_mode = 1;
}

void set_transit_mode()
{ 
   //Set Camera flag to true
   parameters.camera_flag = true;
   
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.transit_transmit_rate;
   parameters.vehicle_mode = 3;
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
   parameters.vehicle_mode = 5;
}

void set_emergency_decent_mode()
{
   //Turn Camera Off
   digitalWrite(PIN_CAMERA_SWITCH, LOW);

   //Turn Camera Flag to false
   parameters.camera_flag = false;
   
   //Set Heater Thresholds to survival settings
   parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_rate = thresholds.emergency_transit_transmit_rate;
   parameters.vehicle_mode = 4;
   //Fire Pyro
   pyro_fire();
}

void pyro_fire()
{
   //Enable Pyro pin
   digitalWrite(PIN_PYRO_ENABLE, HIGH);
   parameters.pyro_enable = true;
   
   //Set primary pin to high
   digitalWrite(PIN_PYRO_1_FIRE, HIGH);
   parameters.pyro_1_status = true;
   
   //Mark time that pyro was initiated 
   parameters.pyro_initiation_elapsed_time = 0;
}
