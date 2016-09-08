function Value_processed=convert_raw_data(Value_raw, Calib_type);
    Value_processed=Value_raw;
if Calib_type==1
    Value_processed=Value_raw/1000;
end
if Calib_type==2
    Value_processed=convert_temperatures(Value_raw);
end
if Calib_type==4
    Value_processed=convert_voltage(Value_raw);
end
