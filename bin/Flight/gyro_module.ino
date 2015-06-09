#include "Arduino.h"
#include <Wire.h>

//  DEVICE_TO_USE selects whether the IMU at address 0x68 (default) or 0x69 is used
//    0 = use the device at 0x68
//    1 = use the device at ox69
#define  DEVICE_TO_USE    0

MPU9150Lib dueMPU;   // the MPU object

//  MPU_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the sensor data and DMP output
#define MPU_UPDATE_RATE  (20)

//  MAG_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the magnetometer data
//  MAG_UPDATE_RATE should be less than or equal to the MPU_UPDATE_RATE
#define MAG_UPDATE_RATE  (10)

//  MPU_MAG_MIX defines the influence that the magnetometer has on the yaw output.
//  The magnetometer itself is quite noisy so some mixing with the gyro yaw can help
//  significantly. Some example values are defined below:

#define  MPU_MAG_MIX_GYRO_ONLY          0                   // just use gyro yaw
#define  MPU_MAG_MIX_MAG_ONLY           1                   // just use magnetometer and no gyro yaw
#define  MPU_MAG_MIX_GYRO_AND_MAG       10                  // a good mix value 
#define  MPU_MAG_MIX_GYRO_AND_SOME_MAG  50                  // mainly gyros with a bit of mag correction 


// Address definition of Temp Sensors ...? Maybe do this for the other too ... 
#define MPU9150_TEMP_OUT_H         0x41   // R  
#define MPU9150_TEMP_OUT_L         0x42   // R  
int MPU9150_I2C_ADDRESS = 0x68;

//  MPU_LPF_RATE is the low pas filter rate and can be between 5 and 188Hz

#define MPU_LPF_RATE   100

//  SERIAL_PORT_SPEED defines the speed to use for the debug serial port
#define  SERIAL_PORT_SPEED  9600

#define MIN_X 0      // Indexing values for calibration offset
#define MAX_X 1
#define MIN_Y 2
#define MAX_Y 3
#define MIN_Z 4
#define MAX_Z 5

int loopState;                                              // what code to run in the loop
double gyro_temp;

#define  LOOPSTATE_NORMAL  0                                // normal execution
#define  LOOPSTATE_MAGCAL  1                                // mag calibration
#define  LOOPSTATE_ACCELCAL  2                              // accel calibration

static CALLIB_DATA calData;                                 // the calibration data

long lastPollTime;                                          // last time the MPU-9150 was checked
long pollInterval;                                          // gap between polls to avoid thrashing the I2C bus

void magCalStart(void);
void magCalLoop(void);
void accelCalStart(void);
void accelCalLoop(void);

void mpuInit()
{
  dueMPU.selectDevice(DEVICE_TO_USE);                        // only really necessary if using device 1
  dueMPU.init(MPU_UPDATE_RATE, MPU_MAG_MIX_GYRO_AND_MAG, MAG_UPDATE_RATE, MPU_LPF_RATE);   // start the MPU
}

boolean duePoll()
{
  if ((millis() - lastPollTime) < pollInterval)
    return false;                                            // not time yet
  if (dueMPU.read()) {
    lastPollTime = millis();
    return true;
  } 
  return false;                                              // try again next time round
}

void gyro_setup()
{
  Wire.begin();
  mpuInit();
  loopState = LOOPSTATE_NORMAL;
  pollInterval = (1000 / MPU_UPDATE_RATE) - 1;              // a bit less than the minimum interval
  lastPollTime = millis();
}

int MPU9150_readSensor(int addrL, int addrH){
  Wire.beginTransmission(MPU9150_I2C_ADDRESS);
  Wire.write(addrL);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
  byte L = Wire.read();

  Wire.beginTransmission(MPU9150_I2C_ADDRESS);
  Wire.write(addrH);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU9150_I2C_ADDRESS, 1, true);
  byte H = Wire.read();

  return (int16_t)((H<<8)+L);
}


void get_gyro_data()
{  

  if (Serial.available()) {
    switch (Serial.read()) {
      case 'm':
      case 'M':
        magCalStart();
        return;
        
      case 'a':
      case 'A':
        accelCalStart();
        return;
    }
  }  

  switch (loopState) {
    case LOOPSTATE_NORMAL:
   
        dueMPU.read();  // get the latest data if ready yet
		Serial.print("Gyro - Quaternion: ");

        gyro_temp = ((double) MPU9150_readSensor(MPU9150_TEMP_OUT_L,MPU9150_TEMP_OUT_H) + 12412.0) / 340.0;
        
        Serial.print("Gyro - Temp Data: ");  
        Serial.print(gyro_temp);
        Serial.println();

        dueMPU.printQuaternion(dueMPU.m_rawQuaternion);       // print the raw quaternion from the dmp
        Serial.println();
		Serial.print("Gyro - Raw Mag Data: ");      
		dueMPU.printVector(dueMPU.m_rawMag);                  // print the raw mag data
        Serial.println();
		Serial.print("Gyro - Raw Accel Data: ");
        dueMPU.printVector(dueMPU.m_rawAccel);                // print the raw accel data
		Serial.println();
		Serial.print("Gyro - Euler angles from DMP: ");
        dueMPU.printAngles(dueMPU.m_dmpEulerPose);            // the Euler angles from the dmp quaternion
		Serial.println();
		Serial.print("Gyro - Calibrated Accel Data: ");
        dueMPU.printVector(dueMPU.m_calAccel);                // print the calibrated accel data
		Serial.println();
		Serial.print("Gyro - Calibrated Mag Data: ");
        dueMPU.printVector(dueMPU.m_calMag);                  // print the calibrated mag data
		Serial.println();
		Serial.print("Gyro - Data Fusion: ");
        dueMPU.printAngles(dueMPU.m_fusedEulerPose);          // print the output of the data fusion
		Serial.println();
		

      break;
        
    case LOOPSTATE_MAGCAL:
      magCalLoop();
      break;
      
    case LOOPSTATE_ACCELCAL:
      accelCalLoop();
      break;
 }
}

bool updateMinMax(const short val, short *minimum, short *maximum) {
  bool updated = (val < *minimum) || (val > *maximum);
  *minimum = min(*minimum, val);
  *maximum = max(*maximum, val);
  return updated;
}

void callLoop(const char *type, short vec[3], short cal[6]) {
  boolean changed;
  
  if (duePoll()) {                                          // get the latest data
    changed = false;
    changed |= updateMinMax(vec[VEC3_X], &cal[MIN_X], &cal[MAX_X]);
    changed |= updateMinMax(vec[VEC3_Y], &cal[MIN_Y], &cal[MAX_Y]);
    changed |= updateMinMax(vec[VEC3_Z], &cal[MIN_Z], &cal[MAX_Z]);
 
    if (changed) {
      Serial.println("-------");
      Serial.print("minX: "); Serial.print(cal[MIN_X]);
      Serial.print(" maxX: "); Serial.print(cal[MAX_X]); Serial.println();
      Serial.print("minY: "); Serial.print(cal[MIN_Y]);
      Serial.print(" maxY: "); Serial.print(cal[MAX_Y]); Serial.println();
      Serial.print("minZ: "); Serial.print(cal[MIN_Z]);
      Serial.print(" maxZ: "); Serial.print(cal[MAX_Z]); Serial.println();
    }
  }
  
  if (Serial.available()) {
    switch (Serial.read()) {
      case 's':
      case 'S':
        calData.accelValid = true;
        calLibWrite(DEVICE_TO_USE, &calData);
        Serial.print(type);
        Serial.print(" cal data saved for device "); Serial.println(DEVICE_TO_USE);
        break;
        
      case 'x':
      case 'X':
        loopState = LOOPSTATE_NORMAL;
        Serial.println("\n\n *** restart to use calibrated data ***");
       break;
    }
  }
}

void magCalStart(void)
{
  calLibRead(DEVICE_TO_USE, &calData);                     // pick up existing accel data if there   

  calData.magValid = false;
  calData.magMinX = 0x7fff;                                // init mag cal data
  calData.magMaxX = 0x8000;
  calData.magMinY = 0x7fff;                              
  calData.magMaxY = 0x8000;
  calData.magMinZ = 0x7fff;                             
  calData.magMaxZ = 0x8000; 
 
  Serial.print("\n\nEntering mag calibration mode for device: "); Serial.println(DEVICE_TO_USE); 
  loopState = LOOPSTATE_MAGCAL;
}

void magCalLoop()
{
  short cal[6];
  cal[MIN_X] = calData.magMinX;
  cal[MAX_X] = calData.magMaxX;
  cal[MIN_Y] = calData.magMinY;
  cal[MAX_Y] = calData.magMaxY;
  cal[MIN_Z] = calData.magMinZ;
  cal[MAX_Z] = calData.magMaxZ;
  callLoop("Magnetometer", dueMPU.m_rawMag, cal);
}

void accelCalStart(void)
{
  calLibRead(DEVICE_TO_USE, &calData);                     // pick up existing accel data if there   

  calData.accelValid = false;
  calData.accelMinX = 0x7fff;                              // init accel cal data
  calData.accelMaxX = 0x8000;
  calData.accelMinY = 0x7fff;                              
  calData.accelMaxY = 0x8000;
  calData.accelMinZ = 0x7fff;                             
  calData.accelMaxZ = 0x8000;
 
  Serial.print("\n\nEntering accel calibration mode for device: "); Serial.println(DEVICE_TO_USE); 
  loopState = LOOPSTATE_ACCELCAL;
  dueMPU.disableAccelCal();
}

void accelCalLoop()
{
  short cal[6];
  cal[MIN_X] = calData.accelMinX;
  cal[MAX_X] = calData.accelMaxX;
  cal[MIN_Y] = calData.accelMinY;
  cal[MAX_Y] = calData.accelMaxY;
  cal[MIN_Z] = calData.accelMinZ;
  cal[MAX_Z] = calData.accelMaxZ;
  callLoop("Acceleration", dueMPU.m_rawAccel, cal);
}

