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
if (counts > TEMP_BREAK_COUNT)   
    T_in_C = TEMP_CONSTANT_1_1+TEMP_CONSTANT_1_2*(counts)+TEMP_CONSTANT_1_3*counts^2+TEMP_CONSTANT_1_4*counts^3+TEMP_CONSTANT_1_5*counts^4+TEMP_CONSTANT_1_6*counts^5;    
else
    T_in_C = TEMP_CONSTANT_2_1+TEMP_CONSTANT_2_2*(counts)+TEMP_CONSTANT_2_3*counts^2+TEMP_CONSTANT_2_4*counts^3+TEMP_CONSTANT_2_5*counts^4+TEMP_CONSTANT_2_6*counts^5;
end