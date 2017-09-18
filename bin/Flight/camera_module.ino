
void process_camera_function()
{
   //Is the camera enabled for the current mode?
   if(parameters.camera_enabled == true)
   {
      //Is the camera on?
      if(parameters.camera_status == true)
      {
         //Is it time to turn camera off?
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
         //Reset the camera on elapsed time
         parameters.camera_on_elapsed_time = 0;
        
         //Is it time to turn camera on?
         if(parameters.camera_period_elapsed_time > (parameters.camera_period - parameters.camera_on_time))
         {
            //Turn Camera On
            digitalWrite(PIN_CAMERA_SWITCH, HIGH);
            parameters.camera_status = true;
            // ID3 High to say we are going to control the camera, set this low when finished controlling the camera
            digitalWrite(GOPRO_ID3, HIGH); 
            //delay(1500);
          }
         // Is it time to take a picture?
         if(parameters.camera_period_elapsed_time > (parameters.camera_period - parameters.camera_on_time + parameters.camera_delay_take_picture))
         {
            // Get the current state of ID1
            int start = digitalRead(GOPRO_ID1);
            //TODO: WHAT LOGIC STATE INDICATES ITS ACTUALLY READY??? 
            
            // Low pulse on ID2 to tell the camera to get ready 
            digitalWrite(GOPRO_ID2, LOW);
            delayMicroseconds(340);    
            digitalWrite(GOPRO_ID2, HIGH);
            
            // Wait for ID 1 to change state to indicate the camera is ready
            while(digitalRead(GOPRO_ID1) != start) {}
            //TODO: MAKE THIS TIME OUT IF THIS TAKES TOO LONG!
            delay(62);    
        
            // Low pulse on TRIG to tell the camera to take a photo
            digitalWrite(GOPRO_TRIG, LOW);
            delayMicroseconds(340);    
            digitalWrite(GOPRO_TRIG, HIGH);

            if(debug.mode == 1)
            {
                Serial.println("*****************");
                Serial.println("* Photo taken!! *");
                Serial.println("*****************");
            } 
          }      
      }  
   }
}

