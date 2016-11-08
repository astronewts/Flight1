%% 1-Choose the file
display('**********************************************************');
display('1-Please choose a SD card file')
try
    [FileName,PathName,FilterIndex]=uigetfile('*.csv','SELECT A CSV FILE');
    display(strcat('You selected the SD card file=>',FileName));
catch
    display(strcat('You did not select a valid .csv file'));
    return;
end
[Data,Txt]=xlsread(strcat(PathName,filesep,FileName));
display('2- Data from the csv file has been sucessfully imported')
%
%% 2- Read the database
Read_Database_from_Arduino;
display('3- Database information is now loaded')
%
%% 3- Read the csv file header
display('4- Start to populate your matlab workspace')
for c=1:min(size(Txt,2),size(Data,2))
    %
    Value_raw=Data(:,c);
    Name_variable=char(Txt(1,c));
    %
   % display(char(Name_variable));
    %
    if c==1
        Name_variable='Elapsed Time [ms]';
    end
    %
    %Clean variable name
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
    index_in_DB=find(strcmp(Name_variable_mod,var_name)==1);
    if isempty(index_in_DB)==0
        Value_processed=convert_raw_data(Value_raw,var_calib(index_in_DB));
    else
        Value_processed=Value_raw;
        if c>1 %Because Elpased Tme is not in database
            display(strcat(Name_variable_mod,'=> not in database'))
        end
    end
    %
    Name_variable_SD=strcat(Name_variable_mod,'_SD');
    assignin('base',Name_variable_SD,Value_processed);
end
display(char(strcat('=> Data import from ',{' '},FileName,' is now DONE')));