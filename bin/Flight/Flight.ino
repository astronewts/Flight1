// *****************************************************************************
// *****************************************************************************
//                           ASTRONEWTS FLIGHT CODE
// *****************************************************************************
// *****************************************************************************
//
// Vehicle: 3
// Version: 0.1
// Release Date: 4/30/2015
//
// *****************************************************************************
// *****************************************************************************
// IMPORTANT NOTES: 
// *****************************************************************************
// Version 0.1: 4/30/2015
//   - Electrical Architecture only deals performs charge control and monitoring
//     on one Battery (assumes other battery is not connected to the Bus).
//   - Automatic cut down voltage is not processed.
//   - Parity bit on the Tx and Rx are not calculated
//   
// *****************************************************************************
// *****************************************************************************
// Open Source / Public Domain
// *****************************************************************************

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
#include <IridiumSBD.h>
#include "IntersemaBaro.h"

struct telemetry_data_struct telemetry_data;
struct satellite_data_struct satellite_data;
struct parameter_struct parameters;
struct threshold_struct thresholds;
struct alt_struct alt;
struct gyro_struct gyro;
struct gps_struct gps_chip;

String output_dataword;

#define MAX_RX_BUFFER_SIZE  270 
uint8_t rx_buffer[MAX_RX_BUFFER_SIZE]; // max size of a received packet is 270 bytes

IridiumSBD isbd(Serial3, 50);
Intersema::BaroPressure_MS5607 baro;

void setup() 
{
   Serial.begin(9600); 
   Serial1.begin(4800);
   Serial3.begin(19200); // Wake up the rockblock and prepare it to communicate (since it will never be put to sleep, ok to call in Setup)
   Serial.println("Flight1 starting up");
   set_output_pins();
   set_defaults();
   gyro_setup();
   baro.init();
   set_normal_mode();
   sd_setup();
}

void loop() 
{ 
   //Collect Analog Telemetry
   collect_telemetry();
   collect_alt_data();
  
   //Process telemetry
   process_telemetry();

   //Print telemetry
   #ifdef DEBUG
   print_telemetry();
   #endif
   
   //Process Camera
   process_camera_function();

   //Check if time to write data to SD Card 
   if(parameters.sd_card_write_elapsed_time > parameters.sd_card_write_period)
   {
     write_telemetry_data_to_sd();
      parameters.sd_card_write_elapsed_time = 0;
   }

//***********************************
     //Check if time to write data to ROCKBLOCK
//   if(parameters.transmit_elapsed_time > parameters.transmit_period)
//   {
//      write_satellite_data();      
//      parameters.transmit_elapsed_time = 0;
//   }
 

   // Check if there is any data waiting for us from ROCKBLOCK
//   ret_val = read_satellite_data();
   
//   if(ret_val == COMMANDS_TO_PROCESS)
//   {
//     read_satellite_data();
//   }
   
//***********************************  
     // Perform RockBlock module functions if elapsed time has exceeded specified transmit rate
     if(parameters.transmit_elapsed_time > parameters.transmit_period)
     {
      sendreceive_satellite_data();
      parameters.transmit_elapsed_time = 0;
     }
   
}

void set_defaults()
{
  thresholds.normal_battery_temperature_limit_high = DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_HIGH;
  thresholds.normal_battery_temperature_limit_low  = DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_LOW;
  thresholds.survival_battery_temperature_limit_high = DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_HIGH;
  thresholds.survival_battery_temperature_limit_low  = DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_LOW;
  
  thresholds.normal_transmit_period = DEFAULT_NORMAL_TRANSMIT_RATE;
  thresholds.test_transmit_period = DEFAULT_TEST_TRANSMIT_RATE;
  thresholds.transit_transmit_period = DEFAULT_TRANSIT_TRANSMIT_RATE;
  thresholds.load_shed_transmit_period = DEFAULT_LOAD_SHED_TRANSMIT_RATE;
  thresholds.emergency_transit_transmit_period = DEFAULT_EMERGENCY_TRANSIT_TRANSMIT_RATE;

  parameters.vehicle_mode = DEFAULT_MODE;
  parameters.command_count = 0.0;
  parameters.loop_sleep = DEFAULT_LOOP_SLEEP;
  parameters.low_voltage_limit_for_loadshed_entry = DEFAULT_VOLTAGE_LOW_LIMIT_LOADSHED_ENTRY;
  parameters.low_voltage_limit_for_auto_cutdown = DEFAULT_VOLTAGE_LOW_LIMIT_CUTDOWN_ENTRY;
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
  parameters.battery_temperature_sanity_check_high = DEFAULT_BATTERY_TEMP_SANITY_CHECK_HIGH;
  parameters.battery_temperature_sanity_check_low = DEFAULT_BATTERY_TEMP_SANITY_CHECK_LOW;
  parameters.heater_state_1 = false;
  parameters.heater_state_2 = false;
  parameters.low_voltage_time_limit = DEFAULT_LOW_VOLTAGE_TIME_LIMIT;
  parameters.battery_bus_low_voltage_flag = false;
  parameters.voltage_sanity_check_high = DEFAULT_VOLTAGE_SANITY_CHECK_HIGH;
  parameters.voltage_sanity_check_low = DEFAULT_VOLTAGE_SANITY_CHECK_LOW;
  parameters.charge_current_sanity_check_high = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH;
  parameters.charge_current_sanity_check_low = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW;
  parameters.transmit_period = thresholds.normal_transmit_period;
  parameters.sd_card_write_period = DEFAULT_SD_CARD_WRITE_PERIOD;
  parameters.cutdown_pulse_width = DEFAULT_PYRO_PULSE_WIDTH;
  parameters.cutdown_enable_state = false;
  parameters.cutdown_1_status = false;
  parameters.cutdown_2_status = false;
  parameters.camera_enabled = true;
  parameters.camera_status = false;
  parameters.battery_1_charging_status = true;
  parameters.battery_2_charging_status = true;
  parameters.camera_period = DEFAULT_CAMERA_PERIOD;
  parameters.camera_on_time = DEFAULT_CAMERA_ON_TIME;
  parameters.altitude_valid_flag = false;
  parameters.altitude_limit_low = DEFAULT_ALTITUDE_LIMIT_LOW;
  parameters.altitude_sanity_check_low = DEFAULT_ALTITUDE_SANITY_CHECK_LOW;
  
  parameters.voltage_sanity_check_high = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH;
  parameters.voltage_sanity_check_low = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW;
  
  parameters.battery_1_recharge_ratio = DEFAULT_B1_RECHARGE_RATIO;
  parameters.battery_1_amphrs_charging = 0.0;
  parameters.battery_1_amphrs_discharging = 0.0;
  parameters.battery_1_amphrs_term_threshold = DEFAULT_B1_AMPHRS_TERM_THRESHOLD;
  parameters.battery_1_amphrs_init_threshold = DEFAULT_B1_AMPHRS_INIT_THRESHOLD;
  parameters.battery_1_voltage_term_threshold = DEFAULT_B1_VOLTAGE_TERM_THRESHOLD;
  parameters.battery_1_voltage_init_threshold = DEFAULT_B1_VOLTAGE_INIT_THRESHOLD;
  
  parameters.battery_2_recharge_ratio = DEFAULT_B2_RECHARGE_RATIO;
  parameters.battery_2_amphrs_charging = 0.0;
  parameters.battery_2_amphrs_discharging = 0.0;
  parameters.battery_2_amphrs_term_threshold = DEFAULT_B2_AMPHRS_TERM_THRESHOLD;
  parameters.battery_2_amphrs_init_threshold = DEFAULT_B2_AMPHRS_INIT_THRESHOLD;
  parameters.battery_2_voltage_term_threshold = DEFAULT_B2_VOLTAGE_TERM_THRESHOLD;
  parameters.battery_2_voltage_init_threshold = DEFAULT_B2_VOLTAGE_INIT_THRESHOLD;
 
  //parameters.output_dataword[340];
  parameters.output_dataword = "0";
  //parameters.valid_str = "0";
  
  //Set Digital Pin States
  digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
  digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, LOW);
  digitalWrite(PIN_CUTDOWN_ENABLE, LOW);
  digitalWrite(PIN_CUTDOWN_1_FIRE, LOW);
  digitalWrite(PIN_CUTDOWN_2_FIRE, LOW);
  
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
   pinMode(PIN_CUTDOWN_ENABLE, OUTPUT);
   pinMode(PIN_CUTDOWN_1_FIRE, OUTPUT);
   pinMode(PIN_CUTDOWN_2_FIRE, OUTPUT);
   pinMode(PIN_CAMERA_SWITCH, OUTPUT);
   
   
}

void set_load_shed_mode()
{
   //Turn Camera Off
   digitalWrite(PIN_CAMERA_SWITCH, LOW);
   
   //Set Camera flag to false
   parameters.camera_status = false;
  
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
   
   //Turn Power On
   digitalWrite(PIN_BATTERY_1_CHARGE_CUTOFF, LOW);
   digitalWrite(PIN_BATTERY_2_CHARGE_CUTOFF, LOW);
   parameters.battery_1_charging_status = true;
   
   //Set Transmit Rate
   parameters.transmit_period = thresholds.load_shed_transmit_period;
   parameters.vehicle_mode = 2;
}

void set_normal_mode()
{ 
   //Set Camera flag to true
   parameters.camera_status = true;
   
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_period = thresholds.normal_transmit_period;
   parameters.vehicle_mode = 1;
}

void set_transit_mode()
{ 
   //Set Camera flag to true
   parameters.camera_status = true;
   
   //Set Heater Threshols to Survival settings
   parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_period = thresholds.transit_transmit_period;
   parameters.vehicle_mode = 3;
}

void set_test_mode()
{   
   //Set Camera flag to true
   parameters.camera_status = true;
   
   //Set Heater Thresholds to normal settings
   parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_period = thresholds.test_transmit_period;
   parameters.vehicle_mode = 5;
}

void set_emergency_decent_mode()
{
   //Turn Camera Off
   digitalWrite(PIN_CAMERA_SWITCH, LOW);

   //Turn Camera Flag to false
   parameters.camera_status = false;
   
   //Set Heater Thresholds to survival settings
   parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
   parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
   
   //Set Transmit Rate
   parameters.transmit_period = thresholds.emergency_transit_transmit_period;
   parameters.vehicle_mode = 4;
   //Fire Pyro
   cutdown_fire();
}

void cutdown_fire()
{
   //Enable Pyro pin
   digitalWrite(PIN_CUTDOWN_ENABLE, HIGH);
   parameters.cutdown_enable_state = true;
   
   //Set primary pin to high
   digitalWrite(PIN_CUTDOWN_1_FIRE, HIGH);
   parameters.cutdown_1_status = true;
   
   //Mark time that pyro was initiated 
   parameters.cutdown_initiation_elapsed_time = 0;
}
