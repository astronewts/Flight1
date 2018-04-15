
#include <elapsedMillis.h>
#include <Wire.h>
#include <SPI.h>

//#define DEFAULT_CAMERA_PERIOD                10000
//#define DEFAULT_CAMERA_ON_TIME               5000
#define DEFAULT_CAMERA_DELAY_TAKE_PICTURE     30000

#define PIN_CAMERA_SWITCH                     22
#define GOPRO_TRIG                            24
#define GOPRO_ID1                             26
#define GOPRO_ID2                             30
#define GOPRO_ID3                             31

bool camera_enabled;
bool camera_status;
unsigned int camera_period;
unsigned int camera_on_time;
unsigned int camera_delay_take_picture;

elapsedMillis time_since_start;
elapsedMillis camera_period_elapsed_time;
elapsedMillis camera_on_elapsed_time;

void setup() {
  Serial.begin(115200);

  Serial.println("*************************");
  Serial.println("* Setting Up The Camera *");
  Serial.println("*************************");
  camera_enabled = true;
  camera_status = false;

//  camera_period = DEFAULT_CAMERA_PERIOD;
//  camera_on_time = DEFAULT_CAMERA_ON_TIME;
  camera_delay_take_picture = DEFAULT_CAMERA_DELAY_TAKE_PICTURE;
  
  pinMode(GOPRO_ID1, INPUT);
  pinMode(GOPRO_ID2, OUTPUT);
  pinMode(GOPRO_ID3, OUTPUT);
  pinMode(GOPRO_TRIG, OUTPUT);
  
  digitalWrite(GOPRO_TRIG, HIGH);
  digitalWrite(GOPRO_ID2, HIGH);
  digitalWrite(GOPRO_ID3, LOW);  
}

void loop() 
{
  delay(camera_delay_take_picture * 0.5);

  //Turn Camera On
  // ID3 High to say we are going to control the camera, set this low when finished controlling the camera
  digitalWrite(GOPRO_ID3, HIGH); 

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

  Serial.println("*****************");
  Serial.println("* Photo taken!! *");
  Serial.println("*****************");

  delay(1000);
  //Turn Camera Off
  // ID3 High to say we are going to control the camera, set this low when finished controlling the camera
  digitalWrite(GOPRO_ID3, LOW); 

  delay(camera_delay_take_picture * 0.5);
}
