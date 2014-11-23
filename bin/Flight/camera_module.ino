void process_camera_function()
{
   //Is it time to turn camera on/off?
   if(parameters.camera_flag == true)
   {
      //Is the camera on?
      if(digitalRead(PIN_CAMERA_SWITCH) == HIGH)
      {
         if(parameters.camera_on_elapsed_time > parameters.camera_on_time)
         {
            //Turn Camera Off
            digitalWrite(PIN_CAMERA_SWITCH, LOW);
            
            //Reset the period elapsed time
            parameters.camera_period_elapsed_time = 0;
         }
      }
      else
      {
         if(parameters.camera_period_elapsed_time > parameters.camera_period)
         {
            //Turn Camera On
            digitalWrite(PIN_CAMERA_SWITCH, HIGH);
            
            //Reset the camera on elapsed time
            parameters.camera_on_elapsed_time = 0;
          }
      }  
   }
}
