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
//     To send / recieve commands throught rockblock connect to: https://rockblock.rock7.com/Operations
// *****************************************************************************
// *****************************************************************************
// Open Source / Public Domain
// *****************************************************************************

#include "definitions.h"
#include <TinyGPS++.h>
#include <Wire.h>
#include <SPI.h>
#include "I2Cdev.h"
// From SparkFun: does not work for our Arduino #include "MPU9250Lib.h"
//#include "MPU9250_asukiaaa.h"
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
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

struct telemetry_data_struct telemetry_data;
struct raw_telemetry_data_struct raw_telemetry_data;
struct satellite_data_struct satellite_data;
struct parameter_struct parameters;
struct threshold_struct thresholds;
struct alt_struct alt;
struct gyro_struct gyro;
struct Flag_RB_struct Flag_RB;
struct debug_struct debug;
static struct gps_data_struct gps_data;
struct gps_data_struct gps_data_new = gps_data;

#define debug_println(...) if(debug.mode){Serial.println(__VA_ARGS__);}

  database db[DB_SIZE];
  
String output_dataword;

#define MAX_RX_BUFFER_SIZE  270
uint8_t rx_buffer[MAX_RX_BUFFER_SIZE]; // max size of a received packet is 270 bytes

IridiumSBD isbd(Serial3, 50);
Adafruit_INA219 ina219_1; // Battery 1 Current Sense default (0x40)
Adafruit_INA219 ina219_2(0x41); // Battery 2 Current Sense
Adafruit_INA219 ina219_3(0x44); // Solar Array Current Sense
Adafruit_INA219 ina219_4(0x45); // Load Path Current Sense

Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
//Intersema::BaroPressure_MS5607 baro; // DELETE THIS

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
  Serial1.begin(9600);
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
    
    debug.mode=1;
    Init_components();
  }

  if (user_input =='s') {
    parameters.prompt_from_user_makes_sense=1;
    Serial.println("Then we are in Signal Quality mode to test RB");
    Serial.println("Time [ms]  ;   Signal Quality (0=no bueno, 5=bueno)  ;  sig_qual_err (0=rb connected, 3=rb not connected)");
    parameters.vehicle_mode=SIGNAL_TEST_MODE;
    Init_components();
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
  
  delay(2000);
  Serial.println("**** Initialize Database:");
  initialize_database();
  Serial.println("===> The database has been initialized!");
  Serial.println("");
  
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
    //Serial.println("MADE IT TO TEST LOOP!!!");
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
    debug.mode = 1;
    RB_Send_Receive_data();
  }
    //RB_Send_Receive_data();
}

void Main_flight_loop()
{
  //debug.mode = 0;

  // Set active temperatures and transmit rate corresponding to normal mode 
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
  parameters.transmit_period = thresholds.normal_transmit_period;
  //
  if(parameters.vehicle_mode == LOADSHED_MODE)
  {
  parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
  parameters.transmit_period = thresholds.load_shed_transmit_period;
  }
  if(parameters.vehicle_mode == TRANSIT_MODE)
  {
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;
  parameters.transmit_period = thresholds.transit_transmit_period;
  }
  if(parameters.vehicle_mode == EMERGENCY_DESCENT_MODE)
  {
  parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;
  parameters.transmit_period = thresholds.emergency_transit_transmit_period;
  }  
  // HIGH rate processes
  if(parameters.high_rate_elapsed_time > HIGH_RATE_PERIOD)
  {    
    collect_charge_current_data();
    collect_low_rate_current_data();
    get_signal_quality(); //DEBUG123
    
    process_charge_current_tlm();
    write_telemetry_data_to_sd(); //DEBUG456

    parameters.high_rate_elapsed_time = 0;

    //TEST CRAP
    if (debug.mode==1)
    {
      Serial.println("=> DEBUG: BATTERY CHARGE CURRENT DATA");
      print_battery_charge_current_data();
    }
  }

  // Specific frequency for GPS:
  if(parameters.gps_rate_elapsed_time > GPS_RATE_PERIOD)
  {  
    collect_gps_data(gps_data_new); //DEBUG789
    post_process_gps_alt(); //DEBUG789
    //TEST CRAP
    if (debug.mode==1)
    {
      print_heater_test_autocutdown_test();
      print_alt_data();
      Serial.println("=> DEBUG: GPS DATA");
      print_gps_data(); //DEBUG789
    } 
    parameters.gps_rate_elapsed_time = 0;
  }
  
  // Medium rate processes
  if(parameters.medium_rate_elapsed_time > MEDIUM_RATE_PERIOD)
  {    
    readGyroData(); //DEBUG789
    post_process_gyro(); //DEBUG789

    collect_alt_data(); //DEBUG789-2
    post_process_alt_data(); //DEBUG789-2

    collect_analog_telemetry();
    execute_thermal_control_check();

    execute_electrical_control_check();

    write_telemetry_data_to_sd();

    parameters.medium_rate_elapsed_time =  0;

    //TEST CRAP
    if (debug.mode==1)
    {
      print_heater_test_autocutdown_test();
      print_alt_data(); //DEBUG789-2
      Serial.println("=> DEBUG: GYRO DATA");
      print_gyro_data(); //DEBUG789
    }
  }

  // Low-rate processes
  if (parameters.low_rate_elapsed_time > LOW_RATE_PERIOD)
  {
    execute_altitude_control_check();


    
    
    parameters.low_rate_elapsed_time=0;
  }

  // House-Keeping processes
  if(parameters.tlm_processing_time > parameters.tlm_processing_period)
  {
    // Process Camera
    // TODO: Figure out How to Write process_camera_function();
    parameters.tlm_processing_time = 0.0;
  }
  //debug.mode = 0;
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
    Flag_RB.try_send_receive = 0;
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
  get_signal_quality();
  
  Serial.print(parameters.elasped_time_for_rb_quality_test);
  Serial.print("  ");
  Serial.print(parameters.signal_quality_record);
  Serial.print("  ");
  Serial.println(parameters.signal_quality_error_record);
  write_telemetry_data_to_sd();
}

void Cutdown_test_loop()
{
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
    readGyroData();
    post_process_gyro();
    
    collect_gps_data(gps_data_new);
    post_process_gps_alt();
    
    // Process Camera
    // TODO: Figure out How to Write process_camera_function();
    write_telemetry_data_to_sd();
    parameters.high_rate_elapsed_time = 0;
  }

  // Medium rate processes
  if(parameters.medium_rate_elapsed_time > MEDIUM_RATE_PERIOD_CUTDOWN)
  {
    collect_alt_data();
    post_process_alt_data();
    collect_analog_telemetry();
    collect_charge_current_data();
    //collect_low_rate_current_data();
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
  //Collect Analog & Current Telemetry
  collect_analog_telemetry();
  collect_charge_current_data();
  collect_low_rate_current_data();

  // Collect GPS Data
  collect_gps_data(gps_data_new);
  post_process_gps_alt();

  // Collect Altimiter Data
  collect_alt_data();
  post_process_alt_data();

  //Collect gyro data
  readGyroData();
  post_process_gyro();

  // Process All Software Data

  // TODO: DETERMINE WHAT FUNCTIONS WE WANT TO RUN IN TERMINAL TEST MODE

  // Print All Collected TLM to the Terminal Window
  print_telemetry();

  initialize_database();

  write_telemetry_data_to_sd();
  delay(1000);
}


void Init_components()
{
  Serial.println("------- INIT COMPONENTS (NO RB) -------");
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
  gyroSetup();  //DEBUG789
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
  parameters.frame_counter = 0;
  parameters.sd_card_num = 0;
  parameters.telemetry_format = FORMAT_1;  //FORMAT_1;
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
  parameters.cutdown_event_flag = false;
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
  parameters.invalid_command_recieved_count = 0;

  parameters.camera_period = DEFAULT_CAMERA_PERIOD;
  parameters.camera_on_time = DEFAULT_CAMERA_ON_TIME;
  parameters.camera_delay_take_picture = DEFAULT_CAMERA_DELAY_TAKE_PICTURE;
  
  parameters.altimeter_altitude_valid_flag = false;
  parameters.gps_alt_valid_flag = false;
  
  parameters.alt_altitude_limit_low = DEFAULT_ALT_ALTITUDE_LIMIT_LOW;
  parameters.alt_altitude_sanity_check_low = DEFAULT_ALT_ALTITUDE_SANITY_CHECK_LOW;
  parameters.alt_altitude_sanity_check_high = DEFAULT_ALT_ALTITUDE_SANITY_CHECK_HIGH;
  parameters.gps_altitude_limit_low = DEFAULT_GPS_ALTITUDE_LIMIT_LOW;
  parameters.gps_altitude_sanity_check_low = DEFAULT_GPS_ALTITUDE_SANITY_CHECK_LOW;
  parameters.gps_altitude_sanity_check_high = DEFAULT_GPS_ALTITUDE_SANITY_CHECK_HIGH;

  parameters.count_low_alt = 0;
  parameters.count_low_voltage = 0;
  
  parameters.test_count = INITIAL_TEST_COUNT;
  parameters.rb_initialization_error_status = 0;
  parameters.signal_quality_record = -1;
  parameters.signal_quality_error_record = -1;
  parameters.rb_send_receive_err = -1;
  parameters.edm_flag_type = 0;
  
  alt.count_between_RB = 0;
  gps_data.count_between_RB = 0;
  gyro.count_between_RB = 0;  

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

  parameters.sa_amphrs = 0.0;
  parameters.load_amphrs = 0.0;

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

  digitalWrite(GOPRO_TRIG, HIGH);
  digitalWrite(GOPRO_ID2, HIGH);
  digitalWrite(GOPRO_ID3, LOW);  

  digitalWrite(GPS_EN, LOW);

  // Change the analog read resolution to 12 bits
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);

  debug.mode = 0;
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
  pinMode(GOPRO_ID1, INPUT);
  pinMode(GOPRO_ID2, OUTPUT);
  pinMode(GOPRO_ID3, OUTPUT);
  pinMode(GOPRO_TRIG, OUTPUT);
  pinMode(GPS_EN, OUTPUT);
}

void initialize_database()
{

  long temp_zero = 0;
  
  //{Type,Bitsize,int,long,float,SD_Title,Calibration,Format1,Format2}
  //Note: Format 1: Nearly All TLM, Format 2: Minimum State-of-Health
  // last 3 flags of the database: 
  // 1st one: calibration or not in the post processing (Matlab).
  // 2nd one: Format1
  // 3rd one: Format2
  uint i = 0;
  db[i] = {"header",8,null_int,null_long,null_float,"Header",0,1,1};        
  i += 1;
  db[i] = {"int",8,parameters.telemetry_format,null_long,null_float,"Format",0,1,1};
  i += 1;
  db[i] = {"int",8,parameters.sd_card_num,null_long,null_float,"SD_Num",0,1,1}; 
  i += 1;
  db[i] = {"int",8,parameters.vehicle_mode,null_long,null_float,"Veh Mode",0,1,1};
  i += 1;
  db[i] = {"int",8,parameters.frame_counter,null_long,null_float,"Frame Count",0,1,1};
  i += 1;
  db[i] = {"int",8,parameters.command_count,null_long,null_float,"CMD Count",0,1,0};            
  i += 1;
  db[i] = {"int",32,parameters.time_since_start/1000,null_long,null_float,"Elapsed Time [s]",0,1,1};
  i += 1;
  db[i] = {"int",8,parameters.transmit_period/(1000*60),null_long,null_float,"Active Period [min]",0,1,0};
  i += 1;
  db[i] = {"int",8,thresholds.normal_transmit_period/(1000*60),null_long,null_float,"Normal Period [min]",0,1,0};
  i += 1;
  db[i] = {"int",8,thresholds.load_shed_transmit_period/(1000*60),null_long,null_float,"Loadshed Period [min]",0,1,0};
  i += 1;
  db[i] = {"int",8,thresholds.transit_transmit_period/(1000*60),null_long,null_float,"Transit Period [min]",0,1,0};
  i += 1;
  db[i] = {"int",8,thresholds.emergency_transit_transmit_period/(1000*60),null_long,null_float,"Emer Period [min]",0,1,0};
  i += 1;
  db[i] = {"int",8,thresholds.spare_transmit_period/(1000*60),null_long,null_float,"Spare Period [min]",0,1,0};
  i += 1;
  db[i] = {"long",8,null_int,parameters.sd_card_write_period/1000,null_float,"SD Card Write Per [s]",0,1,0};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_battery_1_temp_1,null_float,"B1 T 1 [Cts]",2,1,1};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_battery_1_temp_2,null_float,"B1 T 2 [Cts]",2,1,1};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_battery_2_temp_1,null_float,"B2 T 1 [Cts]",2,1,1};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_battery_2_temp_2,null_float,"B2 T 2 [Cts]",2,1,1};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_inner_external_temp,null_float,"Inner Ext T [Cts]",2,1,0};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_outter_external_temp,null_float,"Outer Ext T [Cts]",2,1,0};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_internal_temp,null_float,"Int T [Cts]",2,1,1};
  i += 1;
  db[i] = {"int",8,Flag_RB.try_send_receive,null_long,null_float,"RB Flag",0,1,1};
  i += 1;
  db[i] = {"long",12,null_int,raw_telemetry_data.raw_analog_VIN_voltage,null_float,"VIN Voltage [Cts]",4,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.battery_1_charge_current,"B1 Charge Current [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_batt1,"B1 Shunt V [V]",0,0,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.busvoltage_batt1,"B1 Bus V [V]",0,1,1};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_batt1,"B1 Load Voltage [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.battery_2_charge_current,"B2 Charge Current [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_batt2,"B2 Shunt V [V]",0,0,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.busvoltage_batt2,"B2 Bus V [V]",0,1,1};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_batt2,"B2 Load V [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.sa_current,"SA Current [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_sa,"SA Shunt V [V]",0,0,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.busvoltage_sa,"SA Bus V [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_sa,"SA Load V [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.load_path_current,"Loadpath Current [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_load_path,"Loadpath Shunt V [V]",0,0,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.busvoltage_load_path,"Loadpath Bus V [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_load_path,"Loadpath Load V [V]",0,1,0};
  i += 1;
  db[i] = {"long",8,null_int,parameters.tlm_processing_period/1000,null_float,"Tel Proc Per [s]",1,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_processed_lat,"GPS Lat [deg]",0,1,1};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_processed_long,"GPS Long [deg]",0,1,1};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_altitude,"GPS Alt [m]",0,1,1};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.max_gps_altitude,"Max GPS Alt [m]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.mean_gps_altitude,"Mean GPS Alt [m]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.min_gps_altitude,"Min GPS Alt [m]",0,1,0};  
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_location_age,"GPS Loc Age [s]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_altitude_age,"GPS Alt Age [s]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_heading,"GPS Course [deg]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gps_data.gps_speed,"GPS Speed [km/h]",0,1,0}; 
  i += 1;
  db[i] = {"int",4,gps_data.gps_num_satellites,null_long,null_float,"GPS # of Sat [-]",0,1,0}; 
  i += 1;
  db[i] = {"long",32,null_int,gps_data.gps_date,null_float,"GPS Date [-]",0,1,1}; 
  i += 1;
  db[i] = {"long",32,null_int,gps_data.gps_time,null_float,"GPS Time [-]",0,1,1}; 
  i += 1;
  db[i] = {"int",12,gps_data.gps_hdop,null_long,null_float,"HDOP Value [?]",0,1,0};
  i += 1;
  db[i] = {"int",32,gps_data.gps_chars_processed,null_long,null_float,"GPS Chars Proc. [-]",0,0,0};
  i += 1;
  db[i] = {"int",16,gps_data.gps_sentances_with_fix,null_long,null_float,"GPS Sent with Fix [-]",0,1,0}; 
  i += 1;
  db[i] = {"int",8,gps_data.gps_failed_checksum,null_long,null_float,"GPS Failed Checksum [-]",0,1,0};             
  i += 1;
  db[i] = {"int",1,gps_data.gps_location_valid,null_long,null_float,"GPS Location Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_altitude_valid,null_long,null_float,"GPS Alt Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_heading_valid,null_long,null_float,"GPS Course Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_speed_valid,null_long,null_float,"GPS Speed Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_num_satellites_valid,null_long,null_float,"GPS Satellites Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_date_valid,null_long,null_float,"GPS Date Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_time_valid,null_long,null_float,"GPS Time Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"int",1,gps_data.gps_hdop_valid,null_long,null_float,"GPS HDOP Isvalid [-]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.max_ax,"Max AX [g]",0,0,0};   
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.min_ax,"Min AX [g]",0,0,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mean_ax,"Mean AX [g]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.max_ay,"Max AY [g]",0,0,0};   
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.min_ay,"Min AY [g]",0,0,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mean_ay,"Mean AY [g]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.max_az,"Max AZ [g]",0,0,0};   
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.min_az,"Min AZ [g]",0,0,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mean_az,"Mean AZ [g]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.max_gx,"Max GX [d/s]",0,0,0};   
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.min_gx,"Min GX [d/s]",0,0,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mean_gx,"Mean GX [d/s]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.max_gy,"Max GY [d/s]",0,0,0};   
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.min_gy,"Min GY [d/s]",0,0,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mean_gy,"Mean GY [d/s]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.max_gz,"Max GZ [d/s]",0,0,0};   
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.min_gz,"Min GZ [d/s]",0,0,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mean_gz,"Mean GZ [d/s]",0,1,0};     
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mx,"magX [G]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.my,"magY [G]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.mz,"magZ [G]",0,1,0};  
  i += 1;
  db[i] = {"float",32,null_int,null_long,gyro.gyro_temp,"Gyro T [C]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.voltage_sanity_check_high,"Volt San Chck High [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.voltage_sanity_check_low,"Volt San Chck Low [V]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.charge_current_sanity_check_high,"Chrg Cur Sanity High [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.charge_current_sanity_check_low,"Chrg Cur Sanity Low [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.battery_1_recharge_ratio,"B1 Rech Ratio [-]",0,0,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.battery_1_amphrs_charging,"B1 AH Chrg [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.battery_1_amphrs_discharging,"B1 AH Discharging [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.battery_2_recharge_ratio,"B2 Rech Ratio [-]",0,0,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.battery_2_amphrs_charging,"B2 Amp Hours Charg [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.battery_2_amphrs_discharging,"B2 Amp Hours Discharg [A]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.sa_amphrs,"SA Total Amp Hrs [A.h]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.load_amphrs,"Load Total Amp Hrs [A.h]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.battery_temperature_limit_high+273,null_long,null_float,"B Active T Lim High [K]",5,1,0};
  i += 1;
  db[i] = {"int",16,parameters.battery_temperature_limit_low+273,null_long,null_float,"B Active T Lim Low [K]",5,1,0}; 
  i += 1;
  db[i] = {"int",16,parameters.battery_temperature_sanity_check_high+273,null_long,null_float,"B T Sanity Check High [K]",5,1,0};
  i += 1;
  db[i] = {"int",16,thresholds.normal_battery_temperature_limit_high+273,null_long,null_float,"B Norm T Lim High [K]",5,1,0};
  i += 1;
  db[i] = {"int",16,thresholds.normal_battery_temperature_limit_low+273,null_long,null_float,"B Norm T Lim Low [K]",5,1,0};
  i += 1;
  db[i] = {"int",16,thresholds.survival_battery_temperature_limit_high+273,null_long,null_float,"B Surv T Lim High [K]",5,1,0};
  i += 1;
  db[i] = {"int",16,thresholds.survival_battery_temperature_limit_low+273,null_long,null_float,"B Surv T Lim Low [K]",5,1,0};         
  i += 1;
  db[i] = {"int",16,parameters.battery_temperature_sanity_check_low+273,null_long,null_float,"B T San Check Low [K]",5,1,0};     
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.low_voltage_limit_for_loadshed_entry,"Loadshed Entry Volt Lim [V]",0,1,0};      
  i += 1;
  db[i] = {"float",32,null_int,null_long,parameters.low_voltage_limit_for_auto_cutdown,"Auto Cut Volt Lim [V]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.low_voltage_time_limit/1000,null_long,null_float,"Low Volt Time until Cut [s]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.gps_altitude_limit_low,null_long,null_float,"GPS Alt Limit Low [m]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.gps_altitude_sanity_check_low,null_long,null_float,"GPS Alt San Check Low [m]",0,1,0}; 
  i += 1;
  db[i] = {"int",16,parameters.gps_altitude_sanity_check_high,null_long,null_float,"GPS Alt San Check High [m]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.alt_altitude_limit_low,null_long,null_float,"ALT Alt Limit Low [m]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.alt_altitude_sanity_check_low,null_long,null_float,"ALT Alt San Check Low [m]",0,1,0}; 
  i += 1;
  db[i] = {"int",16,parameters.alt_altitude_sanity_check_high,null_long,null_float,"ALT Alt San Check High [m]",0,1,0};
  i += 1;
  db[i] = {"int",8,parameters.cutdown_pulse_width/1000,null_long,null_float,"Pyro Pulse Width [s]",0,1,0};
  i += 1;
  db[i] = {"int",16,parameters.camera_period/1000,null_long,null_float,"Cam Per [s]",0,0,0};
  i += 1;
  db[i] = {"int",16,parameters.camera_on_time/1000,null_long,null_float,"Cam On Time [s]",0,0,0};
  i += 1;
  db[i] = {"int",16,parameters.camera_delay_take_picture/1000,null_long,null_float,"Cam Delay [s]",0,0,0};
  i += 1;
  db[i] = {"int",1,parameters.battery_bus_low_voltage_flag,null_long,null_float,"Batt Bus Low V Flag",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.heater_state_1,null_long,null_float,"Heat State 1",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.heater_state_2,null_long,null_float,"Heat State 2",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.cutdown_event_flag,null_long,null_float,"Cut Event Flag",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.cutdown_1_status,null_long,null_float,"Cut 1 stat",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.cutdown_2_status,null_long,null_float,"Cut 2 stat",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.gps_alt_valid_flag,null_long,null_float,"GPS Alt Valid",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.altimeter_altitude_valid_flag,null_long,null_float,"ALT Alt Valid",0,1,0};  
  i += 1;
  db[i] = {"int",1,parameters.camera_enabled,null_long,null_float,"Cam Enabled",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.camera_status,null_long,null_float,"Cam Status",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.battery_1_temp_tlm_valid_flag,null_long,null_float,"B1 T TLM Val Flag",0,1,0};         
  i += 1;
  db[i] = {"int",1,parameters.battery_2_temp_tlm_valid_flag,null_long,null_float,"B2 T TLM Val Flag",0,1,0}; 
  i += 1;
  db[i] = {"int",1,parameters.battery_voltage_tlm_valid_flag,null_long,null_float,"Bus V TLM Val Flag",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.battery_1_current_tlm_valid_flag,null_long,null_float,"B1 Cur TLM Val Flag",0,1,0};
  i += 1;
  db[i] = {"int",1,parameters.battery_2_current_tlm_valid_flag,null_long,null_float,"B2 Cur TLM Val Flag",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.altitude_in_meters,"Alt [m]",0,1,1};
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.max_altitude_in_meters,"Max Alt [m]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.min_altitude_in_meters,"Min Alt [m]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.mean_altitude_in_meters,"Mean Alt [m]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.temperature,"Alt T [C]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.pressure,"Alt P [Pa]",0,1,0};
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.max_pressure,"Max P [Pa]",0,1,0}; 
  i += 1;
  db[i] = {"float",32,null_int,null_long,alt.min_pressure,"Min P [Pa]",0,1,0};
  i += 1;
  db[i] = {"int",8,parameters.edm_flag_type,null_long,null_float,"EDM Flag Type",0,1,0};
  i += 1;
  db[i] = {"int",8,parameters.signal_quality_record,null_long,null_float,"SQ_Rec",0,1,0};
  i += 1;
  db[i] = {"int",8,parameters.signal_quality_error_record,null_long,null_float,"SQ_Err_Rec",0,1,0};
  i += 1;
  db[i] = {"int",8,parameters.rb_send_receive_err,null_long,null_float,"Send Rcv Err",0,1,0}; 
  i += 1;
  db[i] = {"int",8,parameters.invalid_command_recieved_count,null_long,null_float,"Invalid CMD Recd Flag",0,1,0};
  i += 1;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx     
//  db[0] = {"header",8,null_int,null_long,null_float,"Header",0,1,1};        
//  db[1] = {"int",8,parameters.telemetry_format,null_long,null_float,"Format",0,1,1};
//  db[2] = {"int",8,parameters.sd_card_num,null_long,null_float,"SD_Num",0,1,1}; 
//  db[3] = {"int",8,parameters.vehicle_mode,null_long,null_float,"Veh Mode",0,1,1};
//  db[4] = {"int",8,parameters.command_count,null_long,null_float,"CMD Count",0,1,0};            
//  db[5] = {"int",32,parameters.time_since_start/1000,null_long,null_float,"Elapsed Time [s]",0,1,1};  // FIX?
//  db[6] = {"int",8,parameters.transmit_period/(1000*60),null_long,null_float,"Active Period [min]",0,1,0};
//  db[7] = {"int",8,thresholds.normal_transmit_period/(1000*60),null_long,null_float,"Normal Period [min]",0,1,0};
//  db[8] = {"int",8,thresholds.load_shed_transmit_period/(1000*60),null_long,null_float,"Loadshed Period [min]",0,1,0};
//  db[9] = {"int",8,thresholds.transit_transmit_period/(1000*60),null_long,null_float,"Transit Period [min]",0,1,0};
//  db[10] = {"int",8,thresholds.emergency_transit_transmit_period/(1000*60),null_long,null_float,"Emer Period [min]",0,1,0};
//  db[11] = {"int",8,thresholds.spare_transmit_period/(1000*60),null_long,null_float,"Spare Period [min]",0,1,0};
//  db[12] = {"long",8,null_int,parameters.sd_card_write_period/1000,null_float,"SD Card Write Per [s]",0,1,0};
//  db[13] = {"long",12,null_int,raw_telemetry_data.raw_battery_1_temp_1,null_float,"B1 T 1 [Cts]",2,1,1};
//  db[14] = {"long",12,null_int,raw_telemetry_data.raw_battery_1_temp_2,null_float,"B1 T 2 [Cts]",2,1,1};
//  db[15] = {"long",12,null_int,raw_telemetry_data.raw_battery_2_temp_1,null_float,"B2 T 1 [Cts]",2,1,1};
//  db[16] = {"long",12,null_int,raw_telemetry_data.raw_battery_2_temp_2,null_float,"B2 T 2 [Cts]",2,1,1};
//  db[17] = {"long",12,null_int,raw_telemetry_data.raw_inner_external_temp,null_float,"Inner Ext T [Cts]",2,1,0};
//  db[18] = {"long",12,null_int,raw_telemetry_data.raw_outter_external_temp,null_float,"Outer Ext T [Cts]",2,1,0};
//  db[19] = {"long",12,null_int,raw_telemetry_data.raw_internal_temp,null_float,"Int T [Cts]",2,1,1};
//  db[20] = {"int",8,Flag_RB.try_send_receive,null_long,null_float,"RB Flag",0,1,1};
//  db[21] = {"long",12,null_int,raw_telemetry_data.raw_analog_VIN_voltage,null_float,"VIN Voltage [Cts]",4,1,0};
//  db[22] = {"float",32,null_int,null_long,telemetry_data.battery_1_charge_current,"B1 Charge Current [A]",0,1,0};
//  db[23] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_batt1,"B1 Shunt V [V]",0,0,0};
//  db[24] = {"float",32,null_int,null_long,telemetry_data.busvoltage_batt1,"B1 Bus V [V]",0,1,1};
//  db[25] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_batt1,"B1 Load Voltage [V]",0,1,0};
//  db[26] = {"float",32,null_int,null_long,telemetry_data.battery_2_charge_current,"B2 Charge Current [A]",0,1,0};
//  db[27] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_batt2,"B2 Shunt V [V]",0,0,0};
//  db[28] = {"float",32,null_int,null_long,telemetry_data.busvoltage_batt2,"B2 Bus V [V]",0,1,1};
//  db[29] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_batt2,"B2 Load V [V]",0,1,0};
//  db[30] = {"float",32,null_int,null_long,telemetry_data.sa_current,"SA Current [A]",0,1,0};
//  db[31] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_sa,"SA Shunt V [V]",0,0,0};
//  db[32] = {"float",32,null_int,null_long,telemetry_data.busvoltage_sa,"SA Bus V [V]",0,1,0};
//  db[33] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_sa,"SA Load V [V]",0,1,0};
//  db[34] = {"float",32,null_int,null_long,telemetry_data.load_path_current,"Loadpath Current [A]",0,1,0};
//  db[35] = {"float",32,null_int,null_long,telemetry_data.shuntvoltage_load_path,"Loadpath Shunt V [V]",0,0,0};
//  db[36] = {"float",32,null_int,null_long,telemetry_data.busvoltage_load_path,"Loadpath Bus V [V]",0,1,0};
//  db[37] = {"float",32,null_int,null_long,telemetry_data.loadvoltage_load_path,"Loadpath Load V [V]",0,1,0};
//  db[38] = {"long",8,null_int,parameters.tlm_processing_period/1000,null_float,"Tel Proc Per [s]",1,1,0};
//  db[39] = {"float",32,null_int,null_long,gps_data.gps_processed_lat,"GPS Lat [deg]",0,1,1};
//  db[40] = {"float",32,null_int,null_long,gps_data.gps_processed_long,"GPS Long [deg]",0,1,1};
//  db[41] = {"float",32,null_int,null_long,gps_data.gps_altitude,"GPS Alt [m]",0,1,1};
//  db[42] = {"float",32,null_int,null_long,gps_data.max_gps_altitude,"Max GPS Alt [m]",0,1,0};
//  db[43] = {"float",32,null_int,null_long,gps_data.mean_gps_altitude,"Mean GPS Alt [m]",0,1,0};
//  db[44] = {"float",32,null_int,null_long,gps_data.min_gps_altitude,"Min GPS Alt [m]",0,1,0};  
//  db[45] = {"float",32,null_int,null_long,gps_data.gps_location_age,"GPS Loc Age [s]",0,1,0};
//  db[46] = {"float",32,null_int,null_long,gps_data.gps_altitude_age,"GPS Alt Age [s]",0,1,0};
//  db[47] = {"float",32,null_int,null_long,gps_data.gps_heading,"GPS Course [deg]",0,1,0};
//  db[48] = {"float",32,null_int,null_long,gps_data.gps_speed,"GPS Speed [km/h]",0,1,0}; 
//  db[49] = {"int",4,gps_data.gps_num_satellites,null_long,null_float,"GPS # of Sat [-]",0,1,0}; 
//  db[50] = {"long",32,null_int,gps_data.gps_date,null_float,"GPS Date [-]",0,1,1}; 
//  db[51] = {"long",32,null_int,gps_data.gps_time,null_float,"GPS Time [-]",0,1,1}; 
//  db[52] = {"int",12,gps_data.gps_hdop,null_long,null_float,"HDOP Value [?]",0,1,0};
//  db[53] = {"int",32,gps_data.gps_chars_processed,null_long,null_float,"GPS Chars Proc. [-]",0,0,0};
//  db[54] = {"int",16,gps_data.gps_sentances_with_fix,null_long,null_float,"GPS Sent with Fix [-]",0,1,0}; 
//  db[55] = {"int",8,gps_data.gps_failed_checksum,null_long,null_float,"GPS Failed Checksum [-]",0,1,0};             
//  db[56] = {"int",1,gps_data.gps_location_valid,null_long,null_float,"GPS Location Isvalid [-]",0,1,0};
//  db[57] = {"int",1,gps_data.gps_altitude_valid,null_long,null_float,"GPS Alt Isvalid [-]",0,1,0};
//  db[58] = {"int",1,gps_data.gps_heading_valid,null_long,null_float,"GPS Course Isvalid [-]",0,1,0};
//  db[59] = {"int",1,gps_data.gps_speed_valid,null_long,null_float,"GPS Speed Isvalid [-]",0,1,0};
//  db[60] = {"int",1,gps_data.gps_num_satellites_valid,null_long,null_float,"GPS Satellites Isvalid [-]",0,1,0};
//  db[61] = {"int",1,gps_data.gps_date_valid,null_long,null_float,"GPS Date Isvalid [-]",0,1,0};
//  db[62] = {"int",1,gps_data.gps_time_valid,null_long,null_float,"GPS Time Isvalid [-]",0,1,0};
//  db[63] = {"int",1,gps_data.gps_hdop_valid,null_long,null_float,"GPS HDOP Isvalid [-]",0,1,0};
//  db[64] = {"float",32,null_int,null_long,gyro.max_ax,"Max AX [g]",0,0,0};   
//  db[65] = {"float",32,null_int,null_long,gyro.min_ax,"Min AX [g]",0,0,0}; 
//  db[66] = {"float",32,null_int,null_long,gyro.mean_ax,"Mean AX [g]",0,1,0}; 
//  db[67] = {"float",32,null_int,null_long,gyro.max_ay,"Max AY [g]",0,0,0};   
//  db[68] = {"float",32,null_int,null_long,gyro.min_ay,"Min AY [g]",0,0,0}; 
//  db[69] = {"float",32,null_int,null_long,gyro.mean_ay,"Mean AY [g]",0,1,0};
//  db[70] = {"float",32,null_int,null_long,gyro.max_az,"Max AZ [g]",0,0,0};   
//  db[71] = {"float",32,null_int,null_long,gyro.min_az,"Min AZ [g]",0,0,0}; 
//  db[72] = {"float",32,null_int,null_long,gyro.mean_az,"Mean AZ [g]",0,1,0};
//  db[73] = {"float",32,null_int,null_long,gyro.max_gx,"Max GX [d/s]",0,0,0};   
//  db[74] = {"float",32,null_int,null_long,gyro.min_gx,"Min GX [d/s]",0,0,0}; 
//  db[75] = {"float",32,null_int,null_long,gyro.mean_gx,"Mean GX [d/s]",0,1,0}; 
//  db[76] = {"float",32,null_int,null_long,gyro.max_gy,"Max GY [d/s]",0,0,0};   
//  db[77] = {"float",32,null_int,null_long,gyro.min_gy,"Min GY [d/s]",0,0,0}; 
//  db[78] = {"float",32,null_int,null_long,gyro.mean_gy,"Mean GY [d/s]",0,1,0}; 
//  db[79] = {"float",32,null_int,null_long,gyro.max_gz,"Max GZ [d/s]",0,0,0};   
//  db[80] = {"float",32,null_int,null_long,gyro.min_gz,"Min GZ [d/s]",0,0,0}; 
//  db[81] = {"float",32,null_int,null_long,gyro.mean_gz,"Mean GZ [d/s]",0,1,0};     
//  db[82] = {"float",32,null_int,null_long,gyro.mx,"magX [G]",0,1,0}; 
//  db[83] = {"float",32,null_int,null_long,gyro.my,"magY [G]",0,1,0}; 
//  db[84] = {"float",32,null_int,null_long,gyro.mz,"magZ [G]",0,1,0};  
//  db[85] = {"float",32,null_int,null_long,gyro.gyro_temp,"Gyro T [C]",0,1,0};
//  db[86] = {"float",32,null_int,null_long,parameters.voltage_sanity_check_high,"Volt San Chck High [V]",0,1,0};
//  db[87] = {"float",32,null_int,null_long,parameters.voltage_sanity_check_low,"Volt San Chck Low [V]",0,1,0};
//  db[88] = {"float",32,null_int,null_long,parameters.charge_current_sanity_check_high,"Chrg Cur Sanity High [A]",0,1,0};
//  db[89] = {"float",32,null_int,null_long,parameters.charge_current_sanity_check_low,"Chrg Cur Sanity Low [A]",0,1,0};
//  db[90] = {"float",32,null_int,null_long,parameters.battery_1_recharge_ratio,"B1 Rech Ratio [-]",0,0,0};
//  db[91] = {"float",32,null_int,null_long,parameters.battery_1_amphrs_charging,"B1 AH Chrg [A]",0,1,0};
//  db[92] = {"float",32,null_int,null_long,parameters.battery_1_amphrs_discharging,"B1 AH Discharging [A]",0,1,0};
//  db[93] = {"float",32,null_int,null_long,parameters.battery_2_recharge_ratio,"B2 Rech Ratio [-]",0,0,0};
//  db[94] = {"float",32,null_int,null_long,parameters.battery_2_amphrs_charging,"B2 Amp Hours Charg [A]",0,1,0};
//  db[95] = {"float",32,null_int,null_long,parameters.battery_2_amphrs_discharging,"B2 Amp Hours Discharg [A]",0,1,0};
//  db[96] = {"float",32,null_int,null_long,parameters.sa_amphrs,"SA Total Amp Hrs [A.h]",0,1,0};
//  db[97] = {"float",32,null_int,null_long,parameters.load_amphrs,"Load Total Amp Hrs [A.h]",0,1,0};
//  db[98] = {"int",16,parameters.battery_temperature_limit_high+273,null_long,null_float,"B Active T Lim High [K]",5,1,0};
//  db[99] = {"int",16,parameters.battery_temperature_limit_low+273,null_long,null_float,"B Active T Lim Low [K]",5,1,0}; 
//  db[100] = {"int",16,parameters.battery_temperature_sanity_check_high+273,null_long,null_float,"B T Sanity Check High [K]",5,1,0};
//  db[101] = {"int",16,thresholds.normal_battery_temperature_limit_high+273,null_long,null_float,"B Norm T Lim High [K]",5,1,0};
//  db[102] = {"int",16,thresholds.normal_battery_temperature_limit_low+273,null_long,null_float,"B Norm T Lim Low [K]",5,1,0};
//  db[103] = {"int",16,thresholds.survival_battery_temperature_limit_high+273,null_long,null_float,"B Surv T Lim High [K]",5,1,0};
//  db[104] = {"int",16,thresholds.survival_battery_temperature_limit_low+273,null_long,null_float,"B Surv T Lim Low [K]",5,1,0};         
//  db[105] = {"int",16,parameters.battery_temperature_sanity_check_low+273,null_long,null_float,"B T San Check Low [K]",5,1,0};     
//  db[106] = {"float",32,null_int,null_long,parameters.low_voltage_limit_for_loadshed_entry,"Loadshed Entry Volt Lim [V]",0,1,0};      
//  db[107] = {"float",32,null_int,null_long,parameters.low_voltage_limit_for_auto_cutdown,"Auto Cut Volt Lim [V]",0,1,0};
//  db[108] = {"int",16,parameters.low_voltage_time_limit/1000,null_long,null_float,"Low Volt Time until Cut [s]",0,1,0};
//  db[109] = {"int",16,parameters.gps_altitude_limit_low,null_long,null_float,"GPS Alt Limit Low [m]",0,1,0};
//  db[110] = {"int",16,parameters.gps_altitude_sanity_check_low,null_long,null_float,"GPS Alt San Check Low [m]",0,1,0}; 
//  db[111] = {"int",16,parameters.gps_altitude_sanity_check_high,null_long,null_float,"GPS Alt San Check High [m]",0,1,0};
//  db[112] = {"int",16,parameters.alt_altitude_limit_low,null_long,null_float,"ALT Alt Limit Low [m]",0,1,0};
//  db[113] = {"int",16,parameters.alt_altitude_sanity_check_low,null_long,null_float,"ALT Alt San Check Low [m]",0,1,0}; 
//  db[114] = {"int",16,parameters.alt_altitude_sanity_check_high,null_long,null_float,"ALT Alt San Check High [m]",0,1,0};
//  db[115] = {"int",8,parameters.cutdown_pulse_width/1000,null_long,null_float,"Pyro Pulse Width [s]",0,1,0};
//  db[116] = {"int",16,parameters.camera_period/1000,null_long,null_float,"Cam Per [s]",0,1,0};
//  db[117] = {"int",16,parameters.camera_on_time/1000,null_long,null_float,"Cam On Time [s]",0,1,0};
//  db[118] = {"int",16,parameters.camera_delay_take_picture/1000,null_long,null_float,"Cam Delay [s]",0,1,0};
//  db[119] = {"int",1,parameters.battery_bus_low_voltage_flag,null_long,null_float,"Batt Bus Low V Flag",0,1,0};
//  db[120] = {"int",1,parameters.heater_state_1,null_long,null_float,"Heat State 1",0,1,0};
//  db[121] = {"int",1,parameters.heater_state_2,null_long,null_float,"Heat State 2",0,1,0};
//  db[122] = {"int",1,parameters.cutdown_event_flag,null_long,null_float,"Cut Event Flag",0,1,0};
//  db[123] = {"int",1,parameters.cutdown_1_status,null_long,null_float,"Cut 1 stat",0,1,0};
//  db[124] = {"int",1,parameters.cutdown_2_status,null_long,null_float,"Cut 2 stat",0,1,0};
//  db[125] = {"int",1,parameters.gps_alt_valid_flag,null_long,null_float,"GPS Alt Valid",0,1,0};
//  db[126] = {"int",1,parameters.altimeter_altitude_valid_flag,null_long,null_float,"ALT Alt Valid",0,1,0};  
//  db[127] = {"int",1,parameters.camera_enabled,null_long,null_float,"Cam Enabled",0,1,0};
//  db[128] = {"int",1,parameters.camera_status,null_long,null_float,"Cam Status",0,1,0};
//  db[129] = {"int",1,parameters.battery_1_temp_tlm_valid_flag,null_long,null_float,"B1 T TLM Val Flag",0,1,0};         
//  db[130] = {"int",1,parameters.battery_2_temp_tlm_valid_flag,null_long,null_float,"B2 T TLM Val Flag",0,1,0}; 
//  db[131] = {"int",1,parameters.battery_voltage_tlm_valid_flag,null_long,null_float,"Bus V TLM Val Flag",0,1,0};
//  db[132] = {"int",1,parameters.battery_1_current_tlm_valid_flag,null_long,null_float,"B1 Cur TLM Val Flag",0,1,0};
//  db[133] = {"int",1,parameters.battery_2_current_tlm_valid_flag,null_long,null_float,"B2 Cur TLM Val Flag",0,1,0};
//  db[134] = {"float",32,null_int,null_long,alt.altitude_in_meters,"Alt [m]",0,1,1};
//  db[135] = {"float",32,null_int,null_long,alt.max_altitude_in_meters,"Max Alt [m]",0,1,0}; 
//  db[136] = {"float",32,null_int,null_long,alt.min_altitude_in_meters,"Min Alt [m]",0,1,0}; 
//  db[137] = {"float",32,null_int,null_long,alt.mean_altitude_in_meters,"Mean Alt [m]",0,1,0}; 
//  db[138] = {"float",32,null_int,null_long,alt.temperature,"Alt T [C]",0,1,0};
//  db[139] = {"float",32,null_int,null_long,alt.pressure,"Alt P [Pa]",0,1,0};
//  db[140] = {"float",32,null_int,null_long,alt.max_pressure,"Max P [Pa]",0,1,0}; 
//  db[141] = {"float",32,null_int,null_long,alt.min_pressure,"Min P [Pa]",0,1,0};
//  db[142] = {"int",8,parameters.edm_flag_type,null_long,null_float,"EDM Flag Type",0,1,0};
//  db[143] = {"int",8,parameters.signal_quality_record,null_long,null_float,"SQ_Rec",0,1,0};
//  db[144] = {"int",8,parameters.signal_quality_error_record,null_long,null_float,"SQ_Err_Rec",0,1,0};
//  db[145] = {"int",8,parameters.num_rb_words_recieved,null_long,null_float,"RB Words Recd",0,1,0}; 
//  db[146] = {"int",8,parameters.invalid_command_recieved_count,null_long,null_float,"Invalid CMD Recd Flag",0,1,0};
}

void calculate_RB_out_mssg_size()
{
   int size_of_fmt_1 = 0;
   int size_of_fmt_2 = 0;
   
   for(int i=0; i<DB_SIZE; i++) {
      size_of_fmt_1 = size_of_fmt_1 +(db[i].format_1*db[i].bitsize);
      size_of_fmt_2 = size_of_fmt_2 +(db[i].format_2*db[i].bitsize);
   }

   if ((size_of_fmt_1 > 340))
   {
   Serial.println("************************** WARNING !!!!!!!!!!!!!!!!!*********************");
   Serial.print("************************** RB mssg in Format1 is too long *****************");
   Serial.println("************************** Mssg wont be sent ****************************");
   Serial.println("************************** WARNING !!!!!!!!!!!!!!!!!*********************");
   }
    
   if ((size_of_fmt_2 > 340))
   {
   Serial.println("************************** WARNING !!!!!!!!!!!!!!!!!*********************");
   Serial.print("************************** RB mssg in Format2 is too long *****************");
   Serial.println("************************** Mssg wont be sent ****************************");
   Serial.println("************************** WARNING !!!!!!!!!!!!!!!!!*********************");
   } 
   
} // end calculate_RB_out_mssg_size

void set_load_shed_mode()
{
  //Turn Camera Off
  digitalWrite(PIN_CAMERA_SWITCH, LOW);

  //Set Camera flag to false
  parameters.camera_status = false;

  //Set Heater Threshols to Survival settings
  parameters.battery_temperature_limit_high = thresholds.survival_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.survival_battery_temperature_limit_low;

  //Set Transmit Rate
  parameters.transmit_period = thresholds.load_shed_transmit_period;
  parameters.vehicle_mode = LOADSHED_MODE;

  //Write to SD Card
  write_telemetry_data_to_sd();
}

void set_normal_mode()
{
  //Set Camera flag to true
  parameters.camera_status = true;

  //Set Heater Threshols to Normal settings
  parameters.battery_temperature_limit_high = thresholds.normal_battery_temperature_limit_high;
  parameters.battery_temperature_limit_low = thresholds.normal_battery_temperature_limit_low;

  //Set Transmit Rate
  parameters.transmit_period = thresholds.normal_transmit_period;
  parameters.vehicle_mode = FLIGHT_MODE;

  //Write to SD Card
  write_telemetry_data_to_sd();
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

  //Write to SD Card
  write_telemetry_data_to_sd();
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
  
  //Write to SD Card
  write_telemetry_data_to_sd();
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

  //Write to SD Card
  write_telemetry_data_to_sd();
  
  //Fire Pyro
  cutdown_fire();
  Serial.println("Pyro fire command initiated!");
}



void cutdown_fire()
{
  //Enable Pyro pin
  //digitalWrite(PIN_CUTDOWN_ENABLE, HIGH);
  //parameters.cutdown_enable_state = true;

  //Set primary pin to high
  digitalWrite(PIN_CUTDOWN_1_FIRE, HIGH);
  parameters.cutdown_1_status = true;

  //Mark time that pyro was initiated
  parameters.cutdown_initiation_elapsed_time = 0;
}
