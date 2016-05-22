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
  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  //Serial.println("Measuring voltage and current with INA219 ...");
}

void print_battery_charge_current_data(void) 
{
  Serial.println("");
  Serial.print("Bus Voltage:   "); Serial.print(telemetry_data.busvoltage_batt1); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(telemetry_data.shuntvoltage_batt1); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(telemetry_data.loadvoltage_batt1); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(telemetry_data.battery_1_charge_current); Serial.println(" mA");
  Serial.println("");

  Serial.print("Bus Voltage:   "); Serial.print(telemetry_data.busvoltage_batt2); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(telemetry_data.shuntvoltage_batt2); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(telemetry_data.loadvoltage_batt2); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(telemetry_data.battery_2_charge_current); Serial.println(" mA");
  Serial.println("");
}

void collect_charge_current_data(void) 
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
