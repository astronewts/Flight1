if exist('FileName_wo_ext')==1
    Potential_arduino_location=strcat(PathName,FileName_wo_ext,'_Database_arduino.txt');
    if exist(Potential_arduino_location)==2
        Location_Database=Potential_arduino_location;
    else
        Location_Database='Database_arduino.txt';
        copyfile('Database_arduino.txt',Potential_arduino_location);
    end
else
    Location_Database='Database_arduino.txt';
end
disp(['Location of Arduino Database=>',Location_Database])
%
DB=importdata(Location_Database);
%
i=0;
for r=1:size(DB,1)
   if strfind(char(DB(r,:)),'i += 1')>0
   else
    Line=strrep(char(DB(r,:)),'&','');
    i=i+1;
    Commas=strfind(Line,',');
    Open_bracket=strfind(Line,'{');
    Close_bracket=strfind(Line,'}');
    var_type(i)=cellstr(Line(Open_bracket(1)+2:Commas(1)-2));
    var_length(i)=str2num(Line(Commas(1)+1:Commas(2)-1));
    var_calib(i)=str2num(Line(Commas(6)+1:Commas(7)-1));
    var_format1(i)=str2num(Line(Commas(7)+1:Commas(8)-1));
    var_format2(i)=str2num(Line(Commas(8)+1:Close_bracket(1)-1));
    %
    Name_variable=Line(Commas(5)+2:Commas(6)-2);
    %
    Name_variable_mod=strrep(Name_variable,' ','_');
    Name_variable_mod=strrep(Name_variable_mod,'_[-]','');
    Name_variable_mod=strrep(Name_variable_mod,'[','');
    Name_variable_mod=strrep(Name_variable_mod,']','');
    Name_variable_mod=strrep(Name_variable_mod,'-','');
    Name_variable_mod=strrep(Name_variable_mod,'/','');
    Name_variable_mod=strrep(Name_variable_mod,'.','');
    Name_variable_mod=strrep(Name_variable_mod,'#','');
    Name_variable_mod=strrep(Name_variable_mod,'?','');
    nb_double_underscore=size(strfind(Name_variable_mod,'__'),2);
    for u=1:nb_double_underscore
        Name_variable_mod=strrep(Name_variable_mod,'__','_');
    end
    %
    Names_Arduino_Database(i,1)=cellstr(Name_variable);
    Names_Variable_MATLAB(i,1)=cellstr(Name_variable_mod);
   end
end
nb_var=size(Names_Arduino_Database,1);