%% 1- Open the file of interest
clear all
close all
clc
%
addpath('..\Convert_Rockblock_output_data\')
%
Read_Database_from_Arduino;
%
[FileName,PathName,FilterIndex]=uigetfile('*.csv','SELECT A CSV FILE');
[Data,Txt]=xlsread(strcat(PathName,filesep,FileName));
%% 2- Read the csv file header
for c=1:min(size(Txt,2),size(Data,2))
%
Value_raw=Data(:,c);
Name_variable=char(Txt(1,c)); 
%
if c==1
Name_variable='Elapsed Time [ms]';
end
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
index_in_DB=find(strcmp(Name_variable_mod,var_name)==1);
if isempty(index_in_DB)==0
Value_processed=convert_raw_data(Value_raw,var_calib(index_in_DB));
else
display(strcat(Name_variable_mod,'=> not in the database'))    
Value_processed=Value_raw;
end
%
Name_variable_SD=strcat(Name_variable_mod,'_SD');
assignin('base',Name_variable_SD,Value_processed);
end
