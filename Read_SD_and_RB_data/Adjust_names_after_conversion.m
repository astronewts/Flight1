function Name_after_Conversion=Adjust_names_after_conversion(Name_before_Conversion, Calib_type)
Name_after_Conversion=Name_before_Conversion;
if Calib_type==0
    Name_after_Conversion=Name_before_Conversion;
end
if Calib_type==1
    Name_after_Conversion=Name_before_Conversion;
end
if Calib_type==2
    Name_after_Conversion=strrep(Name_before_Conversion,'Cts','C');
    Name_after_Conversion=strrep(Name_before_Conversion,'Counts','C');
    Name_after_Conversion=strrep(Name_before_Conversion,'counts','C');
end
if Calib_type==3
    Name_after_Conversion=Name_before_Conversion;
end
if Calib_type==4
    Name_after_Conversion=strrep(Name_before_Conversion,'Cts','V');
end
if Calib_type==5
    Name_after_Conversion=strrep(Name_before_Conversion,'K','C');
end
