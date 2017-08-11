# Arduino Camera Code

#define Camera_Power    22
#define GOPRO_TRIG      24
#define GOPRO_ID1       26
#define GOPRO_ID2       30
#define GOPRO_ID3       31

void setup() {
    // Serial output
    Serial.begin(38400);
  
    pinMode(GOPRO_ID1, INPUT);
    pinMode(GOPRO_ID2, OUTPUT);
    pinMode(GOPRO_ID3, OUTPUT);
    pinMode(GOPRO_TRIG, OUTPUT);
    
    // Set output lines to their default state
    digitalWrite(GOPRO_TRIG, HIGH);
    digitalWrite(GOPRO_ID2, HIGH);
    digitalWrite(GOPRO_ID3, LOW);     
    
    Serial.println("press any key once the camera is in photo mode");
    while (Serial.available() == 0) {}
    Serial.read();

    // ID3 High to say we are going to control the camera, set this low when finished controlling the camera
    digitalWrite(GOPRO_ID3, HIGH); 
    delay(1500);
    Serial.println("Beginning to take photos");
}

void loop() {
    // Get the current state of ID1
    int start = digitalRead(GOPRO_ID1);

    // Low pulse on ID2 to tell the camera to get ready 
    digitalWrite(GOPRO_ID2, LOW);
    delayMicroseconds(340);    
    digitalWrite(GOPRO_ID2, HIGH);
    
    // Wait for ID 1 to change state to indicate the camera is ready
    while(digitalRead(GOPRO_ID1) != start) {}
    delay(62);    

    // Low pulse on TRIG to tell the camera to take a photo
    digitalWrite(GOPRO_TRIG, LOW);
    delayMicroseconds(340);    
    digitalWrite(GOPRO_TRIG, HIGH);

    Serial.println("Photo taken");
    
    // 1.5 second delay between photos. Longer or shorter works fine.
    delay(1500);
