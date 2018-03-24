%% 1-Choose the file
display('**********************************************************');
display('1-Please choose a SD card file')
try
    [FileName,PathName,FilterIndex]=uigetfile('*.csv','SELECT A CSV FILE','D:\Astronewts\SD_data\CSV_files_SDHC_card_March_2018\Raw_CSV_files\');
    display(strcat('You selected the SD card file=>',FileName));
    FileName_wo_ext=strrep(FileName,'.csv','');
    FileName_wo_ext=strrep(FileName,'.CSV','');
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
    if c==1
        Name_variable='Elapsed Time [ms]';
        Name_variable_SD=strcat('Elapsed_Time_ms_SD');
        assignin('base',Name_variable_SD,Value_raw);
    else
        index_in_DB=find(strcmp(Name_variable,Names_Arduino_Database)==1);
        if isempty(index_in_DB)==0
            %
            Value_processed=convert_raw_data(Value_raw,var_calib(index_in_DB));
            %
            Names_after_conversion_for_figures(index_in_DB,1)=Adjust_names_after_conversion(Names_Arduino_Database(index_in_DB),var_calib(index_in_DB));
            %
            Name_variable_SD=strcat(char(Names_Variable_MATLAB(index_in_DB,1)),'_SD');
            assignin('base',Name_variable_SD,Value_processed);
        end
    end
end
display(char(strcat('=> Data import from ',{' '},FileName,' is now DONE')));