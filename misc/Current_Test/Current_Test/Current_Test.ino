

#define CHARGE_CONSTANT_1   3.3
#define CHARGE_CONSTANT_2   4095.0
#define CHARGE_CONSTANT_3   1.40
#define CHARGE_CONSTANT_4   20.37

#define PIN_BATTERY_1_CHARGE_CURRENT_1         A1
#define PIN_BATTERY_1_CHARGE_CURRENT_2         A2

int raw_val;

double battery_1_charge_current_1;
double battery_1_charge_current_2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Charge Current 1
  Serial.print("raw Charge Current 1: ");
  raw_val = analogRead(PIN_BATTERY_1_CHARGE_CURRENT_1);
  battery_1_charge_current_1 = (((raw_val * CHARGE_CONSTANT_1)/CHARGE_CONSTANT_2) - CHARGE_CONSTANT_3) * CHARGE_CONSTANT_4;
  Serial.println(battery_1_charge_current_1);
  delay(200);
  
  //Charge Current 2
  Serial.print("raw Charge Current 2: ");
  raw_val = analogRead(PIN_BATTERY_1_CHARGE_CURRENT_2);
  battery_1_charge_current_2 = (((raw_val * CHARGE_CONSTANT_1)/CHARGE_CONSTANT_2) - CHARGE_CONSTANT_3) * CHARGE_CONSTANT_4;
  Serial.println(battery_1_charge_current_2);
  delay(200);

}
