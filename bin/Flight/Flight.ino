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
// Version 0.2: 2/3/2016
//   - Electrical Architecture only deals performs charge control and monitoring
//     on one Battery (assumes other battery is not connected to the Bus).
//   - Parity bit on the Tx and Rx are not calculated
//
//
//
//     To send / recieve commands throught rockblock connect to: https://core.rock7.com/Operations
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
#include <Adafruit_INA219.h>

struct telemetry_data_struct telemetry_data;
struct raw_telemetry_data_struct raw_telemetry_data;
struct satellite_data_struct satellite_data;
struct parameter_struct parameters;
struct threshold_struct thresholds;
struct alt_struct alt;
struct gyro_struct gyro;
struct Flag_RB_struct Flag_RB;
struct debug_struct debug;
#define debug_println(...) if(debug.mode){Serial.println(__VA_ARGS__);}


String output_dataword;

#define MAX_RX_BUFFER_SIZE  270
uint8_t rx_buffer[MAX_RX_BUFFER_SIZE]; // max size of a received packet is 270 bytes

IridiumSBD isbd(Serial3, 50);
Intersema::BaroPressure_MS5607 baro;
Adafruit_INA219 ina219_1; // Battery 1 Current Sense default (0x40)
Adafruit_INA219 ina219_2(0x41); // Battery 2 Current Sense
Adafruit_INA219 ina219_3(0x44); // Solar Array Current Sense
Adafruit_INA219 ina219_4(0x45); // Load Path Current Sense

char get_user_input()
{
  elapsedMillis timeout_time = 0;
  char user_input =0;
  while (!user_input && timeout_time < INITIALIZATION_TIMEOUT) {
    if (Serial.available() > 0) {
      user_input = Serial.read();
    }
    delay(10);
  }
  return user_input;
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(4800);
  Serial.println("\nASTRONEWTS CODE START: \n");
  //
  Serial.println("**** Set_output_pins:");
  set_output_pins();
  Serial.println("===> Set_output_pins done!");
  Serial.println("");
  Serial.println("**** Set_defaults:");
  set_defaults();
  Serial.println("===> Set_defaults done!");
  Serial.println("");
  Serial.println("**** Set_normal_mode:");
  set_normal_mode();
  Serial.println("===> Set_normal_mode done!");
  Serial.println("");
  Serial.println("");
  //
  Serial.println("PROMPT: Type one of the following option: (f=Flight, d=Flight w/ debug, w=Flight without RB, c=Cutdown-Test, t=Terminal-Test, s=Sig Qual-Test)");
  Serial.print(" (you have ");
  Serial.print(INITIALIZATION_TIMEOUT/1000);
  Serial.println(" seconds): ");

  char user_input = get_user_input();
  // say what you got:
  Serial.println("I received: ");
  Serial.println(user_input);

  if (user_input =='t') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Terminal-Test Mode");
    parameters.vehicle_mode=TERMINAL_TEST_MODE;
    Init_components();
  }

  if (user_input =='c') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Cutdown-Test Mode");
    parameters.vehicle_mode=CUTDOWN_TEST_MODE;
    Init_components();
  }

  if (user_input =='d') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Flight Mode with Debug Output");
    parameters.vehicle_mode=FLIGHT_MODE_WITH_DEBUG;
    debug.mode=1;
    Init_components();
    Init_RB();
  }
  if (user_input =='w') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Flight Mode without RB");
    parameters.vehicle_mode=FLIGHT_MODE_WITHOUT_RB;
    Init_components();
  }

  if (user_input =='s') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Signal Quality mode to test RB");
    Serial.println("Time [ms]  ;   Signal Quality (0=no bueno, 5=bueno)  ;  sig_qual_err (0=rb connected, 3=rb not connected)");
    parameters.vehicle_mode=SIGNAL_TEST_MODE;
    Flag_RB.loop_started=0;
    Init_RB();
  }

  if (user_input =='f') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Flight Mode");
    parameters.vehicle_mode=FLIGHT_MODE;
    Init_components();
    Flag_RB.loop_started=0;
    Init_RB();
  }

  if (user_input &&(parameters.prompt_from_user_makes_sense==0)) {
    Serial.println("I don't get what you want so I pick: we are in Flight Mode");
    parameters.vehicle_mode=FLIGHT_MODE;
    Init_components();
    Flag_RB.loop_started=0;
    Init_RB();
  }

  if (user_input == 0) {
    Serial.println("\n User ... you are too slow I picked the mode for you: Flight Mode\n");
    parameters.vehicle_mode=FLIGHT_MODE;
    Init_components();
    Flag_RB.loop_started=0;
    Init_RB();
  }

  // End of the setup

  Serial.println("*****************************************************************************");
  Serial.println("*****************************************************************************");
  Serial.println("*****************************************************************************");
  Serial.println("********************************START OF THE LOOP****************************");
  Serial.println("*****************************************************************************");
  Serial.println("*****************************************************************************");
  Serial.println("*****************************************************************************");

}

void loop()
{
  Flag_RB.loop_started=1;

  // Mode Check
  //Serial.print("MODE: ");
  //Serial.println(parameters.vehicle_mode);

  // Cut-Down Test Mode Loop
  if(parameters.vehicle_mode == CUTDOWN_TEST_MODE)
  {
    Cutdown_test_loop();
  }
  // Signal Test Mode Loop
  else if(parameters.vehicle_mode == SIGNAL_TEST_MODE)
  {
    Signal_test_loop();
  }
  // Terminal Test Mode Loop
  else if(parameters.vehicle_mode == TERMINAL_TEST_MODE)
  {
    Terminal_test_loop();
  }
  // Flight mode without RB
  else if(parameters.vehicle_mode == FLIGHT_MODE_WITHOUT_RB)
  {
    Main_flight_loop();
  }
  else
  {
    /////////////////////////
    // EXECUTE FLIGHT CODE //
    /////////////////////////
    Main_flight_loop();
    RB_Send_Receive_data();
  }
}

void Main_flight_loop()
{
  static struct rb_data_struct rb_data;
  struct rb_data_struct rb_data_new = rb_data;
  static struct rb_data_delta_struct rb_delta;

  if(parameters.high_rate_elapsed_time > HIGH_RATE_PERIOD)
  {
    debug_println("===> DEBUG: HIGH-RATE PROCESS");
    collect_gps_data(rb_data_new);
    if (debug.mode==1)
    {
      Serial.println("=> DEBUG: GPS DATA");
      print_gps_data();
    }
    collect_gyro_data();
    if (debug.mode==1)
    {
      Serial.println("=> DEBUG: GYRO DATA");
      print_gyro_data();
    }
    collect_charge_current_data();
    collect_low_rate_current_data();
    if (debug.mode==1)
    {
      Serial.println("=> DEBUG: BATTERY CHARGE CURRENT DATA");
      print_battery_charge_current_data();
    }
    process_charge_current_tlm();

    write_telemetry_data_to_sd();
    parameters.high_rate_elapsed_time = 0;
  }

  // Medium rate processes
  if(parameters.medium_rate_elapsed_time > MEDIUM_RATE_PERIOD)
  {
    debug_println();
    debug_println("===> DEBUG: MEDIUM-RATE PROCESS");

    debug_println("collect_analog_telemetry();");
    collect_analog_telemetry();

    debug_println("collect_alt_data();");
    collect_alt_data();
    debug_println("=> DEBUG: ALTIMETER DATA");
    if (debug.mode) {
      print_alt_data();
    }
    parameters.medium_rate_elapsed_time =  0;
  }

  // Low-rate processes
  if (parameters.low_rate_elapsed_time > LOW_RATE_PERIOD)
  {
    parameters.low_rate_elapsed_time=0;
  }

  // House-Keeping processes
  if(parameters.tlm_processing_time > parameters.tlm_processing_period)
  {
    debug_println("===> DEBUG: RUN FLIGHT HOUSEKEEPING CODE");
    // RUN FLIGHT HOUSEKEEPING CODE
    execute_thermal_control_check();
    execute_electrical_control_check();

    // Process Camera
    // TODO: Figure out How to Write process_camera_function();
    parameters.tlm_processing_time = 0.0;
  }
}

void RB_Send_Receive_data()
{
  if(parameters.transmit_elapsed_time > parameters.transmit_period)
  {
    debug_println();
    debug_println("===> DEBUG: ROCK-BLOCK CALL (SEND/RECEIVE DATA OR RE-INITIALIZE RB)");
    if (parameters.rb_initialization_error_status == 5 && parameters.rb_reinitialize_time > 300000)
    {
      debug_println("===> DEBUG: WE ARE TRYING TO RE-INITIALIZE THE RB");
      initialize_rb();
      parameters.rb_reinitialize_time = 0;
    }
    debug_println("===> DEBUG: STARTING TO SEND/RECIEVE DATA WITH RB");
    parameters.transmit_elapsed_time = 0;
    Flag_RB.try_send_reveive = 0;
    sendreceive_satellite_data();
  }
}

void Signal_test_loop()
{
  if (parameters.rb_initialization_error_status == 5 && parameters.rb_reinitialize_time > 300000)
  {
    initialize_rb();
    Serial.println("initializing rb");
    parameters.rb_reinitialize_time = 0;
  }
  if (isbd.isAsleep() == 1)
  {
    Serial.println("RB is asleep: time to wake up !!");
    parameters.rb_initialization_error_status = isbd.begin();
    Serial.print("Error number at initialization: ");
    Serial.println(parameters.rb_initialization_error_status);
  }
  delay(1000);
  int signalQuality = -1;
  int sig_qual_err = -1;
  sig_qual_err = isbd.getSignalQuality(signalQuality);
  Serial.print(parameters.elasped_time_for_rb_quality_test);
  Serial.print("  ");
  Serial.print(signalQuality);
  Serial.print("  ");
  Serial.println(sig_qual_err);
}

void Cutdown_test_loop()
{
  static struct rb_data_struct rb_data;
  struct rb_data_struct rb_data_new = rb_data;
  static struct rb_data_delta_struct rb_delta;

  parameters.test_count = parameters.test_count + 1;
  if(parameters.test_count <= CUTDOWN_TEST_TIME)
  {
    Serial.print("\n Time left before cutdown ... ");
    Serial.print(CUTDOWN_TEST_TIME - parameters.test_count -20);
    Serial.println(" seconds ... tic tac tic tac");

    if( CUTDOWN_TEST_TIME - parameters.test_count > 20)
    {
      delay(1000);
    }
  }
  else
  {
    Serial.println("\n Cutdown has been fired ... we are falling (hopefully) ");
  }

  if(parameters.test_count == CUTDOWN_TEST_TIME)
  {
    cutdown_fire();
  }
  cutdown_check();

  // High rate processes
  if(parameters.high_rate_elapsed_time > HIGH_RATE_PERIOD_CUTDOWN)
  {
    collect_gyro_data();
    collect_gps_data(rb_data_new);
    // Process Camera
    // TODO: Figure out How to Write process_camera_function();
    write_telemetry_data_to_sd();
    parameters.high_rate_elapsed_time = 0;
  }

  // Medium rate processes
  if(parameters.medium_rate_elapsed_time > MEDIUM_RATE_PERIOD_CUTDOWN)
  {
    collect_alt_data();
    collect_analog_telemetry();
    collect_charge_current_data();
    collect_low_rate_current_data();
    parameters.medium_rate_elapsed_time = 0;
  }

  // Low rate processes
  if (parameters.low_rate_elapsed_time > LOW_RATE_PERIOD_CUTDOWN)
  {
    parameters.low_rate_elapsed_time = 0;
  }
  print_cutdown_telemetry();
}

void Terminal_test_loop()
{
  static struct rb_data_struct rb_data;
  struct rb_data_struct rb_data_new = rb_data;
  static struct rb_data_delta_struct rb_delta;

  //Collect Analog & Current Telemetry
  collect_analog_telemetry();
  collect_charge_current_data();
  collect_low_rate_current_data();

  // Collect GPS Data
  collect_gps_data(rb_data_new);

  // Collect Altimiter Data
  collect_alt_data();

  //Collect gyro data
  collect_gyro_data();

  // Process All Software Data

  // TODO: DETERMINE WHAT FUNCTIONS WE WANT TO RUN IN TERMINAL TEST MODE

  // Print All Collected TLM to the Terminal Window
  print_telemetry();

  write_telemetry_data_to_sd();

  delay(1000);
}


void Init_components()
{
  Serial.println("------- INIT COMPONENTS (NO RB) -------");
  Serial.println("");
  Serial.println("**** Initialize baro");
  baro.init();
  Serial.println("===> Initialize baro done!");
  Serial.println("");
  Serial.print("**** sd setup");
  sd_setup();
  Serial.println("===> sd setup done ");
  Serial.println("");
  Serial.println("****current_sense_setup");
  current_sense_setup();
  Serial.println("===> current_sense_setup done ");
  Serial.println("");
  Serial.println("****gyro_setup");
  gyro_setup();
  Serial.println("===> gyro setup done ");
  Serial.println("");
}

void Init_RB()
{
  Serial.println("------- INIT ROCKBLOCK -------");
  Serial3.begin(19200); // Wake up the rockblock and prepare it to communicate (since it will never be put to sleep, ok to call in Setup)
  initialize_rb();
  Serial.println("===> Initialize rb done ");
  Serial.println("\n");
}

void set_defaults()
{
  thresholds.normal_battery_temperature_limit_high = DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_HIGH;
  thresholds.normal_battery_temperature_limit_low  = DEFAULT_NORMAL_BATTERY_TEMP_LIMIT_LOW;
  thresholds.survival_battery_temperature_limit_high = DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_HIGH;
  thresholds.survival_battery_temperature_limit_low  = DEFAULT_SURVIVAL_BATTERY_TEMP_LIMIT_LOW;

  thresholds.normal_transmit_period = DEFAULT_NORMAL_TRANSMIT_RATE;
  thresholds.spare_transmit_period = DEFAULT_SPARE_TRANSMIT_RATE;
  thresholds.transit_transmit_period = DEFAULT_TRANSIT_TRANSMIT_RATE;
  thresholds.load_shed_transmit_period = DEFAULT_LOAD_SHED_TRANSMIT_RATE;
  thresholds.emergency_transit_transmit_period = DEFAULT_EMERGENCY_TRANSIT_TRANSMIT_RATE;

  telemetry_data.shuntvoltage_batt1 = 0.0;
  telemetry_data.busvoltage_batt1 = 0.0;
  telemetry_data.battery_1_charge_current = 0.0;
  telemetry_data.loadvoltage_batt1 = 0.0;

  telemetry_data.shuntvoltage_batt2 = 0.0;
  telemetry_data.busvoltage_batt2 = 0.0;
  telemetry_data.battery_2_charge_current = 0.0;
  telemetry_data.loadvoltage_batt2 = 0.0;

  telemetry_data.shuntvoltage_sa = 0.0;
  telemetry_data.busvoltage_sa = 0.0;
  telemetry_data.sa_current = 0.0;
  telemetry_data.loadvoltage_sa = 0.0;

  telemetry_data.shuntvoltage_load_path = 0.0;
  telemetry_data.busvoltage_load_path = 0.0;
  telemetry_data.load_path_current = 0.0;
  telemetry_data.loadvoltage_load_path = 0.0;

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
  parameters.tlm_processing_period = DEFAULT_TLM_PROCESSING_PERIOD;
  parameters.cutdown_pulse_width = DEFAULT_PYRO_PULSE_WIDTH;
  parameters.cutdown_enable_state = false;
  parameters.cutdown_1_status = false;
  parameters.cutdown_2_status = false;
  parameters.camera_enabled = true;
  parameters.camera_status = false;
  parameters.battery_1_charging_status = true;
  parameters.battery_2_charging_status = true;
  parameters.battery_1_temp_tlm_valid_flag = false;
  parameters.battery_2_temp_tlm_valid_flag = false;
  parameters.battery_voltage_tlm_valid_flag = false;
  parameters.battery_1_current_tlm_valid_flag = false;
  parameters.battery_2_current_tlm_valid_flag = false;

  parameters.camera_period = DEFAULT_CAMERA_PERIOD;
  parameters.camera_on_time = DEFAULT_CAMERA_ON_TIME;
  parameters.altitude_valid_flag = false;
  parameters.altitude_limit_low = DEFAULT_ALTITUDE_LIMIT_LOW;
  parameters.altitude_sanity_check_low = DEFAULT_ALTITUDE_SANITY_CHECK_LOW;

  parameters.voltage_sanity_check_high = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_HIGH;
  parameters.voltage_sanity_check_low = DEFAULT_CHARGE_CURRENT_SANITY_CHECK_LOW;

  parameters.test_count = INITIAL_TEST_COUNT;
  parameters.rb_initialization_error_status = 0;
  parameters.num_rb_words_recieved = 0;

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
  pinMode(PIN_GYRO_POWER, OUTPUT);
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
  parameters.vehicle_mode = LOADSHED_MODE;
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
  parameters.vehicle_mode = FLIGHT_MODE;
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
  parameters.vehicle_mode = TRANSIT_MODE;
}

void set_spare_mode()
{
  //Set Camera flag to true
  parameters.camera_status = true;

  //Set Heater Thresholds to normal settings
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;

  //Set Transmit Rate
  parameters.transmit_period = thresholds.spare_transmit_period;
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
  Serial.println("Pyro fire command initiated!");
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
