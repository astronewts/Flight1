%
[filename,PathName,FilterIndex]=uigetfile ({'*.txt','txt-files';'*.xls','xls-files'});
%
if strfind(filename,'xls')
[Data,Txt]=xlsread(strcat(PathName,filename));
c=0;
for lw=5:size(Txt,1) 
if strcmp(char(Txt(lw,3)),'MO')==1
    c=c+1;
mssg_rockblock{c,1}=Txt(lw,4);
end
end
end
if strfind(filename,'txt')
fileID = fopen(strcat(PathName,filename));
mssg_rockblock = textscan(fileID,'%s');
fclose(fileID);
for lw=1:size(mssg_rockblock{1,1},1);
mssg_rockblock_new{lw,1}=mssg_rockblock{1}{lw};
end
clear mssg_rockblock
mssg_rockblock=mssg_rockblock_new;
end
%
nb_words=ndims(mssg_rockblock);
%
Read_RB_mssg_to_mat_workspace