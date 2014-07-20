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

struct telemetry_data_struct telemetry_data;
struct satellite_data_struct satellite_data;
struct parameter_struct parameters;

int ret_val = 0;

void setup() 
{
   Serial.begin(9600); 
   Serial.println("Flight1 starting up");
   parameters.loop_sleep = 1000;
}

void loop() 
{
   // Check if there is any data waiting for us from ROCKBLOCK
   Serial.println("Calling read");
   ret_val = read_satellite_data(&satellite_data);
   
   if(ret_val == COMMANDS_TO_PROCESS)
   {
     //Process satellite commands from ROCKBLOCK, blocking unless it's time to make a loop cycle
     // 1 - Cord Snap
     // 2 - Turn on/off feed from solar panels
     // 3 - Set hi/low temperature thresholds 
     // 4 - Set sampling rate
     // 5 - Set camera rate
     // 6 - Set data format
     process_satellite_data(&satellite_data);
   }
  
   //Collect telemetry
   colect_telemetry(&telemetry_data);
   
   //Is there anything to act on from tolemety read?
   // 1 - Turn heating element on/off if temperature breaks threshold()
   // 2 - Check voltage time() to check if cutdown is needed
   // 3 - Mode change (Low/Normal/High)
   process_telemetry(&telemetry_data);
   
   //Is it time to turn camera on/off?
   process_camera_function();
 
   //Either sleep or continue looping, based on battery draw tests
   delay(parameters.loop_sleep);
}
