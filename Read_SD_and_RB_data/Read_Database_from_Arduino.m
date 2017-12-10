Potential_arduino_location=strcat(PathName,FileName_wo_ext,'_Database_arduino.txt');
%
if exist(Potential_arduino_location)==2
    Location_Database=Potential_arduino_location;
else
    Location_Database='Database_arduino.txt';
    copyfile('Database_arduino.txt',Potential_arduino_location);
end
disp(['Location of Arduino Database=>',Location_Database])
%
DB=importdata(Location_Database);
%
for r=1:size(DB,1)
Line=strrep(char(DB(r,:)),'&','');
Commas=strfind(Line,',');
Open_bracket=strfind(Line,'{');
Close_bracket=strfind(Line,'}');
Open_crochet=strfind(Line,'[');
Close_crochet=strfind(Line,']');
var_ind=1+str2num(Line(Open_crochet(1)+1:Close_crochet(1)-1));
var_ind_list(r)=cellstr(strcat(num2str(var_ind-1)));
if var_ind<100
var_ind_list(r)=cellstr(strcat('0',num2str(var_ind-1)));
end
if var_ind<10
var_ind_list(r)=cellstr(strcat('00',num2str(var_ind-1)));
end
var_type(var_ind)=cellstr(Line(Open_bracket(1)+2:Commas(1)-2));
var_length(var_ind)=str2num(Line(Commas(1)+1:Commas(2)-1));
var_calib(var_ind)=str2num(Line(Commas(6)+1:Commas(7)-1));
var_format1(var_ind)=str2num(Line(Commas(7)+1:Commas(8)-1));
var_format2(var_ind)=str2num(Line(Commas(8)+1:Close_bracket(1)-1));
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
var_name(var_ind)=cellstr(Name_variable_mod);
%
Names_Arduino_Database(var_ind,1)=cellstr(Name_variable);
Names_Variable_MATLAB(var_ind,1)=cellstr(Name_variable_mod);
Names_after_conversion_for_figures(var_ind,1)=cellstr(Name_variable);
end
%
nb_var=size(DB,1);
%