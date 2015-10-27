

#define RESOLUTION_PRESSURE_SENSOR   12
//Constants
#define TEMP_CONSTANT_1     0.0497
#define TEMP_CONSTANT_2     54.794

#define PIN_EXTERNAL_OUTTER_TEMP               A5
#define PIN_INTERNAL_TEMP                      A6
#define PIN_EXTERNAL_INNER_TEMP                A7
#define PIN_BATTERY1_1_TEMP                    A8
#define PIN_BATTERY1_2_TEMP                    A9
#define PIN_BATTERY2_1_TEMP                    A10
#define PIN_BATTERY2_2_TEMP                    A11


int raw_val;

double battery_1_temp_1;
double battery_1_temp_2;
double battery_2_temp_1;
double battery_2_temp_2;
double internal_temp;
double inner_external_temp;
double outter_external_temp;

void setup() {
  // put your setup code here, to run once:
//  pinMode(PIN_EXTERNAL_OUTTER_TEMP, INPUT);
//  pinMode(PIN_INTERNAL_TEMP, INPUT);
//  pinMode(PIN_EXTERNAL_INNER_TEMP, INPUT);
//  pinMode(PIN_BATTERY1_1_TEMP, INPUT);
//  pinMode(PIN_BATTERY1_2_TEMP, INPUT);
//  pinMode(PIN_BATTERY2_1_TEMP, INPUT);
//  pinMode(PIN_BATTERY2_2_TEMP, INPUT);  
//  pinMode(A0, OUTPUT); 
//  pinMode(A1, OUTPUT); 
//  pinMode(A2, OUTPUT); 
//  pinMode(A3, OUTPUT); 
//  pinMode(A4, OUTPUT); 
//  pinMode(A7, OUTPUT); 
//  pinMode(A8, OUTPUT); 
//  pinMode(A9, OUTPUT); 
//  pinMode(A10, OUTPUT); 
//  pinMode(A11, OUTPUT); 
  
  
  
  analogReadResolution(RESOLUTION_PRESSURE_SENSOR);
  
  Serial.begin(9600); 

}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println("-----------TEMP Telemetry---------------");
  //Battery 1-1 Temp
  Serial.print("raw Battery 1-1 Temp: ");
  raw_val = analogRead(PIN_BATTERY1_1_TEMP);
  Serial.println(raw_val);
  Serial.print("Battery 1-1 Temp: ");
  battery_1_temp_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(battery_1_temp_1);
  delay(100);
  
  //Battery 1-2 Temp
  Serial.print("raw Battery 1-2 Temp: ");
  raw_val = analogRead(PIN_BATTERY1_2_TEMP);
  Serial.println(raw_val);
  Serial.print("Battery 1-2 Temp: ");
  battery_1_temp_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(battery_1_temp_2);
  delay(100);

  //Battery 2-1 Temp
  Serial.print("raw Battery 2-1 Temp: ");
  raw_val = analogRead(PIN_BATTERY2_1_TEMP);
  Serial.println(raw_val);
  Serial.print("Battery 2-1 Temp: ");
  battery_2_temp_1 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(battery_2_temp_1);
  delay(100);
  
  //Battery 2-2 Temp
  Serial.print("raw Battery 2-2 Temp: ");
  raw_val = analogRead(PIN_BATTERY2_2_TEMP);
  Serial.println(raw_val);
  Serial.print("Battery 2-2 Temp: ");
  battery_2_temp_2 = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(battery_2_temp_2);
  delay(100);
  
  //Internal Temp
  Serial.print("raw Internal Temp: ");
  raw_val = analogRead(PIN_INTERNAL_TEMP);
  Serial.println(raw_val);
  Serial.print("Internal Temp: ");
  internal_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(internal_temp);
  delay(100);
  
  //External Inner Temp
  Serial.print("raw External Inner Temp: ");
  raw_val = analogRead(PIN_EXTERNAL_INNER_TEMP);
  Serial.println(raw_val);
  Serial.print("External Inner Temp: ");
  inner_external_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(inner_external_temp);
  delay(100);
  
  //External Outer Temp
  Serial.print("raw External Outter Temp: ");
  raw_val = analogRead(PIN_EXTERNAL_OUTTER_TEMP);
  Serial.println(raw_val);
  Serial.print("External Outter Temp: ");
  outter_external_temp = TEMP_CONSTANT_1 * (raw_val - TEMP_CONSTANT_2);
  Serial.println(outter_external_temp);
  delay(100);

}
