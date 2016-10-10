clear all
close all
clc
%
Read_SD_Card_to_mat_workspace
decode_Dec2Bin
%clearvars -except _RB* _SD*
%%
figure(1)
line(Elapsed_Time_ms_SD/1000,Veh_Mode_SD)
line(Elapsed_Time_s_RB,Veh_Mode_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
%
figure(2)
line(Elapsed_Time_ms_SD/1000,Cam_Status_SD)
line(Elapsed_Time_s_RB,Cam_Status_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
%
%%
figure(3)
line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_High_C_SD)
line(Elapsed_Time_s_RB,B_Active_T_Lim_High_C_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')

figure(4)
line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_Low_C_SD)
line(Elapsed_Time_s_RB,B_Active_T_Lim_Low_C_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
%
%%
figure(5)
%line(Elapsed_Time_ms_SD/1000,Active_Trans_Per_s_SD)
line(Elapsed_Time_s_RB,Active_Trans_Per_s_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')