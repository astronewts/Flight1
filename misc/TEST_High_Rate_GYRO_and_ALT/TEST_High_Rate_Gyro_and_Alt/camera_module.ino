void process_camera_function()
{
   //Is it time to turn camera on/off?
   if(parameters.camera_enabled == true)
   {
      //Is the camera on?
      // TODO: FIX THIS !!!
      if(parameters.camera_status == true)
      {
         if(parameters.camera_on_elapsed_time > parameters.camera_on_time)
         {
            //Turn Camera Off
            digitalWrite(PIN_CAMERA_SWITCH, LOW);
            parameters.camera_status = false;
            
            //Reset the period elapsed time
            parameters.camera_period_elapsed_time = 0;
         }
      }
      else
      {
         if(parameters.camera_period_elapsed_time > (parameters.camera_period - parameters.camera_on_time))
         {
            //Turn Camera On
            digitalWrite(PIN_CAMERA_SWITCH, HIGH);
            parameters.camera_status = true;
            
            //Reset the camera on elapsed time
            parameters.camera_on_elapsed_time = 0;
          }
      }  
   }
}
