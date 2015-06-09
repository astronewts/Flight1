#ifndef INTERSEMA_BARO_H
#define INTERSEMA_BARO_H

/*
I have found variations of this code all over.  None of them worked for me with arduino 1.x.  
I don't own this code and I make no promises whatsoever about it's functionality.  It is working for me  :)
Bill Garrett - 2014 - mrbill2uaa@hotmail.com
***********************************************************
Astronewts: Using this library with the SDA1/SCL1 I2C Bus on the Arduino Due.
We also modified the code to pass through Average Temp, Avg Normalized Pressure (temp compensated), 
and Avg Non-Normalized Pressure.  It also seems to work for us ;)
*/


#include <Wire.h>
#include "Arduino.h"

namespace Intersema
{
  
class BaroPressure
{
public:
    virtual void init() = 0;   
   
    int32_t getHeightCentiMeters(void)
    {
        return AcquireAveragedSampleCm(NUM_SAMP_FOR_AVG);
    }   
    
    int32_t getTemperatureCentigrade(void)
    {
        return AcquireAveragedSampleTemp(NUM_SAMP_FOR_AVG);
    } 
    
    int32_t getAvgNormPressurePascals(void)
    {
        return AcquireAveragedSampleNormPress(NUM_SAMP_FOR_AVG);
    }  
    /*  
    int32_t getAvgRawPressurePascals(void)
    {
        return AcquireAveragedSampleRawPress(NUM_SAMP_FOR_AVG);
    }  
    */
    
protected:
    virtual int32_t AcquireAveragedSampleCm(const uint8_t nSamples) = 0;
    
    virtual uint32_t ConvertPressureTemperature(uint32_t pressure, uint32_t temperature) = 0;
    
    virtual int32_t AcquireAveragedSampleNormPress(const uint8_t nSamples) = 0;
    
    //virtual int32_t AcquireAveragedSampleRawPress(const uint8_t nSamples) = 0;
    
    virtual int32_t AcquireAveragedSampleTemp(const uint8_t nSamples) = 0;
    
    virtual uint32_t CalibrateTemperature(uint32_t temperature) = 0;
    
    int32_t PascalToCentimeter(const int32_t pressurePa)
    {
        // Lookup table converting pressure in Pa to altitude in cm.
        // Each LUT entry is the altitude in cm corresponding to an implicit
        // pressure value, calculated as [PA_INIT - 1024*index] in Pa.
        // The table is calculated for a nominal sea-level pressure  = 101325 Pa.
        
        static const int32_t PZLUT_ENTRIES = 77;
        static const int32_t PA_INIT       = 104908;
        static const int32_t PA_DELTA      = 1024;

        static const int32_t lookupTable[PZLUT_ENTRIES] = {
       -29408, -21087, -12700,  -4244,   4279,
        12874,  21541,  30281,  39095,  47986,
        56953,  66000,  75126,  84335,  93628,
       103006, 112472, 122026, 131672, 141410,
       151244, 161174, 171204, 181335, 191570,
       201911, 212361, 222922, 233597, 244388,
       255300, 266334, 277494, 288782, 300204,
       311761, 323457, 335297, 347285, 359424,
       371719, 384174, 396795, 409586, 422552,
       435700, 449033, 462560, 476285, 490216,
       504360, 518724, 533316, 548144, 563216,
       578543, 594134, 609999, 626149, 642595,
       659352, 676431, 693847, 711615, 729752,
       748275, 767202, 786555, 806356, 826627,
       847395, 868688, 890537, 912974, 936037,
       959766, 984206};

        if(pressurePa > PA_INIT) 
             return lookupTable[0];
        else 
        {
           const int32_t inx = (PA_INIT - pressurePa) >> 10;      
           if(inx >= PZLUT_ENTRIES - 1) 
               return lookupTable[PZLUT_ENTRIES - 1];
           else 
           {
                const int32_t pa1 = PA_INIT - (inx << 10);
                const int32_t z1 = lookupTable[inx];
                const int32_t z2 = lookupTable[inx+1];
                return (z1 + (((pa1 - pressurePa) * (z2 - z1)) >> 10));
            }
        }
    }
    
    static const uint8_t NUM_SAMP_FOR_AVG = 5;

    unsigned int coefficients_[6];
};

class BaroPressure_MS5607 : public BaroPressure
{
public:
    BaroPressure_MS5607() { 
    }
    
    void init()
    {    
       ResetSensor();
       ReadCoefficients();
    }
    
private:

    static const uint8_t i2cAddr_ = 0x76;
    static const uint8_t cmdReset_   = 0x1E;
    static const uint8_t cmdAdcRead_ = 0x00;
    static const uint8_t cmdAdcConv_ = 0x40;
    static const uint8_t cmdAdcD1_   = 0x00;
    static const uint8_t cmdAdcD2_   = 0x10;
    static const uint8_t cmdAdc256_  = 0x00;
    static const uint8_t cmdAdc512_  = 0x02;
    static const uint8_t cmdAdc1024_ = 0x04;
    static const uint8_t cmdAdc2048_ = 0x06;
    static const uint8_t cmdAdc4096_ = 0x08;
    static const uint8_t cmdPromRd_  = 0xA0;

    void ResetSensor()
    {
        Wire1.begin();
        Wire1.beginTransmission(i2cAddr_);
        Wire1.write(cmdReset_);   
        Wire1.endTransmission(); 
        delay(3);
        Serial.println("Sensor Reset Sent");
    }

    void ReadCoefficients(void)
    {
        for(uint8_t i=0; i<6; ++i)
            coefficients_[i] = ReadCoefficient(i + 1);  
            

   for(uint8_t i=0; i<6; ++i)
        {
            Serial.print("Coefficient ");
            Serial.print(i + 1, DEC);
            Serial.print(" : ");
            Serial.println(coefficients_[i], DEC);
        }
    }

    uint16_t ReadCoefficient(const uint8_t coefNum)
    {
        uint16_t rC=0;
    
        Wire1.beginTransmission(i2cAddr_);
        Wire1.write(cmdPromRd_ + coefNum * 2); // send PROM READ command
        Wire1.endTransmission(); 
    
        Wire1.requestFrom(i2cAddr_, static_cast<uint8_t>(2));

        if(Wire1.available() >= 2)
        {
            uint16_t ret = Wire1.read();   // read MSB and acknowledge
            uint16_t rC  = 256 * ret;
            ret = Wire1.read();        // read LSB and not acknowledge
            rC  = rC + ret;
            return rC;
        }
        else
        {
       Serial.println("No data available in ReadCoefficient()");
        }
    
        return 0;
    }

    virtual int32_t AcquireAveragedSampleCm(const uint8_t nSamples)
    {
        int64_t pressAccum = 0;

        for(size_t n = nSamples; n; n--) 
        {
            const uint32_t temperature = ReadAdc(cmdAdcD2_ | cmdAdc4096_); // digital temperature value : typical 8077636
            const uint32_t pressure    = ReadAdc(cmdAdcD1_ | cmdAdc4096_); // digital pressure value : typical 6465444  
            const uint32_t pressConv   = ConvertPressureTemperature(pressure, temperature);                 
            pressAccum += pressConv;
        }
        
        const int32_t pressAvg = pressAccum / nSamples;        
        const int32_t AltCm = PascalToCentimeter(pressAvg);

        return AltCm;   
    }
    
    virtual int32_t AcquireAveragedSampleTemp(const uint8_t nSamples)
    {
        int64_t tempAccum = 0;

        for(size_t n = nSamples; n; n--) 
        {
            const uint32_t temperature = ReadAdc(cmdAdcD2_ | cmdAdc4096_); // digital temperature value : typical 8077636       
            tempAccum += temperature;
        }
        const int32_t tempAvg = tempAccum / nSamples;  
        const int32_t temp_calibrated = CalibrateTemperature(tempAvg);  
            
        return temp_calibrated;   
    }
    
    
    virtual int32_t AcquireAveragedSampleNormPress(const uint8_t nSamples)
    {
        int64_t pressAccum = 0;

        for(size_t n = nSamples; n; n--) 
        {
            const uint32_t temperature = ReadAdc(cmdAdcD2_ | cmdAdc4096_); // digital temperature value : typical 8077636
            const uint32_t pressure    = ReadAdc(cmdAdcD1_ | cmdAdc4096_); // digital pressure value : typical 6465444  
            const uint32_t pressConv   = ConvertPressureTemperature(pressure, temperature);                 
            pressAccum += pressConv;
        }
        const int32_t pressAvg = pressAccum / nSamples;        
        return pressAvg;   
    }
    
    /*
    virtual int32_t AcquireAveragedSampleRawPress(const uint8_t nSamples)
    {
        int64_t pressAccum = 0;

        for(size_t n = nSamples; n; n--) 
        {
            const uint32_t pressure    = ReadAdc(cmdAdcD1_ | cmdAdc4096_); // digital pressure value : typical 6465444       
            pressAccum += pressure;
        }
        const int32_t pressAvg = pressAccum / nSamples;        
        return pressAvg;   
    }
    */
    
    int32_t ReadAdc(const uint8_t cmd)
    {             
        Wire1.beginTransmission(i2cAddr_);
        Wire1.write(cmdAdcConv_ | cmd); // send conversion command
        Wire1.endTransmission(); 

        // wait necessary conversion time
        switch(cmd & 0x0f) 
        {
        case cmdAdc256_: 
            delay(1);
            break;
        case cmdAdc512_: 
            delay(3);
            break;
        case cmdAdc1024_: 
            delay(4);
            break;
        case cmdAdc2048_: 
            delay(6);
            break;
        case cmdAdc4096_: 
            delay(10); 
            break;
        }

        Wire1.beginTransmission(i2cAddr_);
        Wire1.write(cmdAdcRead_);
        Wire1.endTransmission();
    
        Wire1.requestFrom(i2cAddr_, static_cast<uint8_t>(3));

        if(Wire1.available() >= 3)
        {
            uint16_t ret  = Wire1.read(); // read MSB and acknowledge
            uint32_t temp = 65536 * ret;
            ret  = Wire1.read();      // read byte and acknowledge
            temp = temp + 256 * ret;
            ret  = Wire1.read();  // read LSB and not acknowledge
            temp = temp + ret;
                
            return temp;
        }

        else
        {
           Serial.println(Wire1.available());
        }

        return 0;
    }

    uint32_t ConvertPressureTemperature(uint32_t pressure, uint32_t temperature)
    {      
        const uint64_t C1 = static_cast<uint64_t>(coefficients_[0]);
        const uint64_t C2 = static_cast<uint64_t>(coefficients_[1]);
        const uint64_t C3 = static_cast<uint64_t>(coefficients_[2]);
        const uint64_t C4 = static_cast<uint64_t>(coefficients_[3]);
        const uint64_t C5 = static_cast<uint64_t>(coefficients_[4]);
        const uint64_t C6 = static_cast<uint64_t>(coefficients_[5]);
        
        // calcualte 1st order pressure and temperature (MS5607 1st order algorithm)
        const int32_t dT    = temperature - C5 * 256;                   // difference between actual and reference temperature
        const int32_t temp  = 2000 + (dT * C6) / pow(2, 23) ; //        // actual temperature
        const int64_t OFF   = (C2 * pow(2, 17)) + ((C4 * dT) / pow(2, 6)); // offset at actual temperature
        const int64_t SENS  = (C1 * pow(2, 16)) + ((C3 * dT) / pow(2, 7)); // sensitivity at actual temperature
        const int32_t press = ((pressure * SENS / pow(2, 21) - OFF) / pow(2, 15)); // / 100;      // temperature compensated pressure

        return press; 
    }
    
    uint32_t CalibrateTemperature(uint32_t temperature)
    {
        const uint64_t C1 = static_cast<uint64_t>(coefficients_[0]);
        const uint64_t C2 = static_cast<uint64_t>(coefficients_[1]);
        const uint64_t C3 = static_cast<uint64_t>(coefficients_[2]);
        const uint64_t C4 = static_cast<uint64_t>(coefficients_[3]);
        const uint64_t C5 = static_cast<uint64_t>(coefficients_[4]);
        const uint64_t C6 = static_cast<uint64_t>(coefficients_[5]);
        
        const int32_t dT    = temperature - C5 * 256;                   // difference between actual and reference temperature
        const int32_t temp  = 2000 + (dT * C6) / pow(2, 23); //         // actual temperature
        
        return temp;
    }
    
};

} // namespace Intersema
#endif