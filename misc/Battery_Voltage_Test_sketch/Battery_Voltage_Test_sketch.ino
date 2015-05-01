

//Constants
#define PRESSURE_CONSTANT   0.0036140
#define TEMP_CONSTANT_1     0.0497
#define TEMP_CONSTANT_2     54.794
#define VOLTAGE_CONSTANT_1  3.3
#define VOLTAGE_CONSTANT_2  4095.0
#define VOLTAGE_CONSTANT_3  3.83
#define CHARGE_CONSTANT_1   3.3
#define CHARGE_CONSTANT_2   26.0
#define CHARGE_CONSTANT_3   4095.0
#define MS_IN_SEC           1000.0
#define SECS_IN_HOUR        3600.0


//Analog Pins
#define PIN_PRESSURE_SENSOR                    A0
#define PIN_BATTERY_1_CHARGE_CURRENT_1         A1
#define PIN_BATTERY_1_CHARGE_CURRENT_2         A2
#define PIN_BATTERY_1_VOLTAGE_1                A3
#define PIN_BATTERY_1_VOLTAGE_2                A4
#define PIN_EXTERNAL_OUTTER_TEMP               A5
#define PIN_INTERNAL_TEMP                      A6
#define PIN_EXTERNAL_INNER_TEMP                A7
#define PIN_BATTERY1_1_TEMP                    A8
#define PIN_BATTERY1_2_TEMP                    A9
#define PIN_BATTERY2_1_TEMP                    A10
#define PIN_BATTERY2_2_TEMP                    A11


double raw_val;
double Xraw_val;
double Yraw_val;
double actual_val;
double battery_1_voltage_1;
double battery_1_voltage_2;


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600); 
  //analogReference(INTERNAL2V56);
  
  analogReadResolution(10);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  
  read_voltage();
}



void read_voltage()
{
  Xraw_val = analogRead(PIN_BATTERY_1_VOLTAGE_1);
  battery_1_voltage_1 = ((Xraw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
    //Battery Voltage 1
  Xraw_val = analogRead(PIN_BATTERY_1_VOLTAGE_1);
  battery_1_voltage_1 = ((Xraw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  delay(1000);

  
  //Battery Voltage 2
  Yraw_val = analogRead(PIN_BATTERY_1_VOLTAGE_2);
  battery_1_voltage_2 = ((Yraw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  Yraw_val = analogRead(PIN_BATTERY_1_VOLTAGE_2);
  battery_1_voltage_2 = ((Yraw_val * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3;
  delay(100);
  
  Serial.println("");
  Serial.print("Battery Voltage 1: ");
  Serial.println(battery_1_voltage_1);   
  Serial.print("Battery Voltage 1 RAW: ");
  Serial.println(Xraw_val);
  delay(100);
  Serial.print("Battery Voltage 2: ");
  Serial.println(battery_1_voltage_2);   
  Serial.print("Battery Voltage 2 RAW: ");
  Serial.println(Yraw_val);
  Serial.println("");
}
