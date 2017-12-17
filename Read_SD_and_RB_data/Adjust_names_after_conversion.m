function Name_after_Conversion=Adjust_names_after_conversion(Name_before_Conversion,Calib_type)
Name_after_Conversion=char(Name_before_Conversion);
if Calib_type==2
    Name_after_Conversion=strrep(Name_after_Conversion,'Cts','C');
    Name_after_Conversion=strrep(Name_after_Conversion,'Counts','C');
    Name_after_Conversion=strrep(Name_after_Conversion,'counts','C');
end
if Calib_type==4
    Name_after_Conversion=strrep(Name_after_Conversion,'Cts','V');
end
if Calib_type==5
    Name_after_Conversion=strrep(Name_after_Conversion,'K','C');
end
Name_after_Conversion=cellstr(Name_after_Conversion);