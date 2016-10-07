clear all
close all
clc
%
[filename,PathName,FilterIndex]=uigetfile ({'*.txt','txt-files';'*.xls','xls-files'});
%
if strfind(filename,'xls')
[Data,txt]=xlsread(strcat(PathName,filename));
mssg_rockblock=txt(5:size(txt,1),4);
end
if strfind(filename,'txt')
fileID = fopen(strcat(PathName,filename));
mssg_rockblock = textscan(fileID,'%s');
fclose(fileID);
end
%
nb_words=ndims(mssg_rockblock);
%
Read_RB_mssg_to_mat_workspace