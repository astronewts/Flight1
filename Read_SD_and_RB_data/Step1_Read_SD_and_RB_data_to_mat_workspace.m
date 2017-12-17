clear all
close all
clc
%
answer=input('Choose what you want to post process=> SD card [s], Rockblock [r] or Both [b] : ','s');
%
if (strcmp(answer,'b'))==1
    display('You are going to read a SD file and then a RB file');
    Read_SD_Card_to_mat_workspace;
    Read_RB_mssg_to_mat_workspace;
    if mean(SD_Num_RB)~=mean(mean(SD_Num_SD))
        display(' = = = WARNING: SOME DATA FROM THE RB DO NOT CORRESPOND TO THIS SD FILE');
    end
end
%
if (strcmp(answer,'s'))==1
    display('You are going to read a SD file only');
    Read_SD_Card_to_mat_workspace;
end
%
if (strcmp(answer,'r'))==1
    display('You are going to read a RB file only');
    Read_RB_mssg_to_mat_workspace;
end
%
clearvars -except *_RB *_SD Data Txt Names_* FileName_wo_ext PathName Date_message_RB_PST
