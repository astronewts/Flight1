function Value_processed=convert_raw_data(Value_raw, Calib_type)
    Value_processed=Value_raw;
if Calib_type==0
    Value_processed=Value_raw;
end
if Calib_type==1
    Value_processed=Value_raw/1000;
end
if Calib_type==2
    Value_processed=convert_temperatures(Value_raw);
end
if Calib_type==3
    Value_processed=Value_raw;
end
if Calib_type==4
    Value_processed=convert_voltage(Value_raw);
end
if Calib_type==5
    Value_processed=Value_raw-273;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function T_in_C=convert_temperatures(counts)
%
TEMP_BREAK_COUNT=   160;
TEMP_CONSTANT_1_1=  91.31522494;
TEMP_CONSTANT_1_2=  -0.173536021;
TEMP_CONSTANT_1_3=  0.000162517;
TEMP_CONSTANT_1_4=  -8.30807E-08;
TEMP_CONSTANT_1_5=  2.04376E-11;
TEMP_CONSTANT_1_6=  -1.94001E-15;
TEMP_CONSTANT_2_1=  201.0543745;
TEMP_CONSTANT_2_2=  -4.089196897;
TEMP_CONSTANT_2_3=  0.071827729;
TEMP_CONSTANT_2_4=  -0.000719311;
TEMP_CONSTANT_2_5=  3.65125E-06;
TEMP_CONSTANT_2_6=  -7.29058E-09;
%
for c=1:size(counts,1)
    count=counts(c,1);
 if count~=0
    if (count > TEMP_BREAK_COUNT)
        T_in_C(c,1) = TEMP_CONSTANT_1_1+TEMP_CONSTANT_1_2*(count)+TEMP_CONSTANT_1_3*count^2+TEMP_CONSTANT_1_4*count^3+TEMP_CONSTANT_1_5*count^4+TEMP_CONSTANT_1_6*count^5;
    else
        T_in_C (c,1)= TEMP_CONSTANT_2_1+TEMP_CONSTANT_2_2*(count)+TEMP_CONSTANT_2_3*count^2+TEMP_CONSTANT_2_4*count^3+TEMP_CONSTANT_2_5*count^4+TEMP_CONSTANT_2_6*count^5;
    end
 else
     T_in_C (c,1)=NaN;
 end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%
function Voltage=convert_voltage(counts)
%
VOLTAGE_CONSTANT_1= 3.3;
VOLTAGE_CONSTANT_2= 4095.0;
VOLTAGE_CONSTANT_3= 5.0151;
VOLTAGE_CONSTANT_4= 0.6301;
Voltage = ((counts * VOLTAGE_CONSTANT_1)/VOLTAGE_CONSTANT_2) * VOLTAGE_CONSTANT_3 + VOLTAGE_CONSTANT_4;
