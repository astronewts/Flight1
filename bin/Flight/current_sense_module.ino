#include <Wire.h>

//#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
//   #define Serial SerialUSB
//#endif

void current_sense_setup(void) 
{
  //#ifndef ESP8266
  //  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  //#endif
  
  uint32_t currentFrequency;
    
  //Serial.begin(115200);
  //Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  
  ina219_1.begin(); // Initialize first board (default address 0x40)
  ina219_2.begin(); // Initialize second board with the address 0x41
  ina219_3.begin(); // Initialize third board with the address 0x44
  ina219_4.begin(); // Initialize fourth board with the address 0x45
  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219_1.setCalibration_16V_400mA();
  //ina219_2.setCalibration_16V_400mA();
  //ina219_3.setCalibration_16V_400mA();
  //ina219_4.setCalibration_16V_400mA();

  //Serial.println("Measuring voltage and current with INA219 ...");
}

void print_battery_charge_current_data() 
{
  // Current Sensor #1
  Serial.println("");
  Serial.print("Battery 1 Bus Voltage:     "); Serial.print(telemetry_data.busvoltage_batt1); Serial.println(" V");
  Serial.print("Battery 1 Shunt Voltage:   "); Serial.print(telemetry_data.shuntvoltage_batt1); Serial.println(" mV");
  Serial.print("Battery 1 Load Voltage:    "); Serial.print(telemetry_data.loadvoltage_batt1); Serial.println(" V");
  Serial.print("Battery 1 Charge Current:  "); Serial.print(telemetry_data.battery_1_charge_current); Serial.println(" mA");
  Serial.println("");

  // Current Sensor #2
  Serial.print("Battery 2 Bus Voltage:     "); Serial.print(telemetry_data.busvoltage_batt2); Serial.println(" V");
  Serial.print("Battery 2 Shunt Voltage:   "); Serial.print(telemetry_data.shuntvoltage_batt2); Serial.println(" mV");
  Serial.print("Battery 2 Load Voltage:    "); Serial.print(telemetry_data.loadvoltage_batt2); Serial.println(" V");
  Serial.print("Battery 2 Charge Current:  "); Serial.print(telemetry_data.battery_2_charge_current); Serial.println(" mA");
  Serial.println("");
}

void print_low_rate_current_data() 
{
  // Current Sensor #3
  Serial.println("");
  Serial.print("Solar Array Bus Voltage:   "); Serial.print(telemetry_data.busvoltage_sa); Serial.println(" V");
  Serial.print("Solar Array Shunt Voltage: "); Serial.print(telemetry_data.shuntvoltage_sa); Serial.println(" mV");
  Serial.print("Solar Array Load Voltage:  "); Serial.print(telemetry_data.loadvoltage_sa); Serial.println(" V");
  Serial.print("Solar Array Current:       "); Serial.print(telemetry_data.sa_current); Serial.println(" mA");
  Serial.println("");

  // Current Sensor #4
  Serial.print("Load Path Bus Voltage:     "); Serial.print(telemetry_data.busvoltage_load_path); Serial.println(" V");
  Serial.print("Load Path Shunt Voltage:   "); Serial.print(telemetry_data.shuntvoltage_load_path); Serial.println(" mV");
  Serial.print("Load Path Voltage:         "); Serial.print(telemetry_data.loadvoltage_load_path); Serial.println(" V");
  Serial.print("Load Path Current:         "); Serial.print(telemetry_data.load_path_current); Serial.println(" mA");
  Serial.println("");  
}

void collect_charge_current_data() 
{
  telemetry_data.shuntvoltage_batt1 = ina219_1.getShuntVoltage_mV();
  telemetry_data.busvoltage_batt1 = ina219_1.getBusVoltage_V();
  telemetry_data.battery_1_charge_current = ina219_1.getCurrent_mA();
  telemetry_data.loadvoltage_batt1 = telemetry_data.busvoltage_batt1 + (telemetry_data.shuntvoltage_batt1 / 1000);

  telemetry_data.shuntvoltage_batt2 = ina219_2.getShuntVoltage_mV();
  telemetry_data.busvoltage_batt2 = ina219_2.getBusVoltage_V();
  telemetry_data.battery_2_charge_current = ina219_2.getCurrent_mA();
  telemetry_data.loadvoltage_batt2 = telemetry_data.busvoltage_batt2 + (telemetry_data.shuntvoltage_batt2 / 1000);
}

void collect_low_rate_current_data() 
{
  telemetry_data.shuntvoltage_sa = ina219_3.getShuntVoltage_mV();
  telemetry_data.busvoltage_sa = ina219_3.getBusVoltage_V();
  telemetry_data.sa_current = ina219_3.getCurrent_mA();
  telemetry_data.loadvoltage_sa = telemetry_data.busvoltage_sa + (float)(telemetry_data.shuntvoltage_sa/1000);

  telemetry_data.shuntvoltage_load_path = ina219_4.getShuntVoltage_mV();
  telemetry_data.busvoltage_load_path = ina219_4.getBusVoltage_V();
  telemetry_data.load_path_current = ina219_4.getCurrent_mA();
  telemetry_data.loadvoltage_load_path = telemetry_data.busvoltage_load_path + (float)(telemetry_data.shuntvoltage_load_path / 1000);

}

