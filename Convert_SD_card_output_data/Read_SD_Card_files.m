%% 1- Open the file of interest
clear all
close all
clc
%
addpath('..\Convert_Rockblock_output_data\')
%
Read_Database_from_Arduino;
%
[FileName,PathName,FilterIndex]=uigetfile('*.csv');
[Data,Txt]=xlsread(strcat(PathName,filesep,FileName));
%% 2- Read the csv file header
for c=1:min(size(Txt,2),size(Data,2))
%
Value_raw=Data(:,c);
Name_variable=char(Txt(1,c)); 
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
index_in_DB=find(strcmp(Name_variable_mod,var_name)==1)
%
if isempty(index_in_DB)==0
Value_processed=convert_raw_data(Value_raw,var_calib(index_in_DB));
else
Value_processed=Value_raw;
end
%
assignin('base',Name_variable_mod,Value_processed);
end
%% 3- Create figures
%
Time=millis/1e3;
%xlimits=[9100 9120];
xlimits=[min(Time) max(Time)];
%
Rate=1./diff(Time);
Rate(size(Rate,1)+1,1)=mean(Rate);
%
Mode=Veh_Mode(1);
%
if Mode==1
Mode_str='Flight mode';
end
if Mode==7
Mode_str='Terminal';
end
if Mode==6
Mode_str='Cut-down';
end
if Mode==8
Mode_str='Signal test';
end
if Mode==9
Mode_str='Flight with debug';
end
if Mode==10
Mode_str='Flight without RB';
end
%
title_str=strcat(FileName,{'  '},Mode_str);
%
Output_results=strrep(FileName,'.CSV','_results');
Output_results_dir=strcat('..\..\Results_CSV_files\',Output_results,filesep);
if isdir(Output_results_dir)~=1
mkdir(Output_results_dir);
end
display(strcat('Results will be stored->',Output_results_dir))
%
Size_fig=[4 4 12 3];
Marker_type='none';
%

Legend_txt(1,1)=cellstr('Batt1 Temp 1');
Legend_txt(2,1)=cellstr('Batt1 Temp 2');
Legend_txt(3,1)=cellstr('Heater 1');
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B_1_T_1_C,'Color',[0 0.5 0],'Marker',Marker_type)
line(Time,B_1_T_2_C,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Heater_State_1*max(max(B_1_T_1_C,B_1_T_2_C)),'Color',[0 0 0],'LineWidth',2,'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
legend(Legend_txt,'Location','Best')
legend('boxoff')
clear Legend_txt;
xlabel('Time [s]')
ylabel('Battery 1 temperature [C]')
saveas(h,strcat(Output_results_dir,'Battery_1_Temperature.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Batt2 Temp 1');
Legend_txt(2,1)=cellstr('Batt2 Temp 2');
Legend_txt(3,1)=cellstr('Heater 2');
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B_2_T_1_C,'Color',[0 0.5 0],'Marker',Marker_type)
line(Time,B_2_T_2_C,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Heater_State_2*max(max(B_2_T_1_C,B_2_T_2_C)),'Color',[0 0 0],'LineWidth',2,'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
legend(Legend_txt,'Location','Best')
legend('boxoff')
clear Legend_txt;
xlabel('Time [s]')
ylabel('Battery 2 temperature [C]')
saveas(h,strcat(Output_results_dir,'Battery_2_Temperature.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Altimeter T');
Legend_txt(2,1)=cellstr('Gyro T');
Legend_txt(3,1)=cellstr('In T');
Legend_txt(4,1)=cellstr('Inner Ext T');
Legend_txt(5,1)=cellstr('Outer Ext T');
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Alt_T_C,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Gyro_T_C,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Int_T_C,'Color',[0 0 0.5],'Marker',Marker_type)
line(Time,Inner_Ext_T_C,'Color',[0 0.5 0],'Marker',Marker_type)
line(Time,Outer_Ext_T_C,'Color',[1 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
legend(Legend_txt,'Location','Best')
legend('boxoff')
clear Legend_txt;
xlabel('Time [s]')
ylabel('Temperatures [C]')
saveas(h,strcat(Output_results_dir,'All_temperatures.png'),'png')
ylim([10 40])
saveas(h,strcat(Output_results_dir,'All_temperatures_zoom.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Altimer [ft]');
Legend_txt(2,1)=cellstr('GPS [converted to ft]');
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Alt_ft,'Color',[0 0 0],'Marker',Marker_type)
line(Time,GPS_Alt_m*100/30.48,'Color',[1 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Altitude [ft]')
legend(Legend_txt,'Location','Best')
legend('boxoff')
clear Legend_txt;
saveas(h,strcat(Output_results_dir,'Altitude.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
title(title_str)
line(Time,Rate,'Color',[0 0 0],'Marker',Marker_type)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Data acqusition rate [Hz]')
saveas(h,strcat(Output_results_dir,'Rate.png'),'png')
close(h)
%
% Legend_txt(1,1)=cellstr('X');
% Legend_txt(2,1)=cellstr('Y');
% Legend_txt(3,1)=cellstr('Z');
% h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
% set(gca,'Box','on')
% title(title_str)
% line(Time,Gyro_X_degs,'Color',[0 0 0],'Marker',Marker_type)
% line(Time,Gyro_Y_degs,'Color',[1 0 0],'Marker',Marker_type)
% line(Time,Gyro_Z_degs,'Color',[0 0.5 0],'Marker',Marker_type)
% grid on
% xlim(xlimits)
% legend(Legend_txt,'Location','Best')
% legend('boxoff')
% clear Legend_txt;
% xlabel('Time [s]')
% ylabel('Gyro [deg]')
% saveas(h,strcat(Output_results_dir,'Gyro.png'),'png')
% close(h)
% %
% Legend_txt(1,1)=cellstr('X');
% Legend_txt(2,1)=cellstr('Y');
% Legend_txt(3,1)=cellstr('Z');
% h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
% set(gca,'Box','on')
% title(title_str)
% line(Time,Acc_X_g,'Color',[0 0 0],'Marker',Marker_type)
% line(Time,Acc_Y_g,'Color',[1 0 0],'Marker',Marker_type)
% line(Time,Acc_Z_g-1,'Color',[0 0.5 0],'Marker',Marker_type)
% grid on
% xlim(xlimits)
% legend(Legend_txt,'Location','Best')
% legend('boxoff')
% clear Legend_txt;
% xlabel('Time [s]')
% ylabel('Acceleration [g]')
% saveas(h,strcat(Output_results_dir,'Acceleration.png'),'png')
% close(h)
% %
% Legend_txt(1,1)=cellstr('X');
% Legend_txt(2,1)=cellstr('Y');
% Legend_txt(3,1)=cellstr('Z');
% h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
% set(gca,'Box','on')
% title(title_str)
% line(Time,Gyro_mag_X_G,'Color',[0 0 0],'Marker',Marker_type)
% line(Time,Gyro_mag_Y_G,'Color',[1 0 0],'Marker',Marker_type)
% line(Time,Gyro_mag_Z_G,'Color',[0 0.5 0],'Marker',Marker_type)
% grid on
% xlim(xlimits)
% legend(Legend_txt,'Location','Best')
% legend('boxoff')
% clear Legend_txt;
% xlabel('Time [s]')
% ylabel('Mag [-]')
% saveas(h,strcat(Output_results_dir,'Mag.png'),'png')
% close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,GPS_Lat_deg,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlabel('Time [s]')
ylabel('Lattitude [deg]')
saveas(h,strcat(Output_results_dir,'GPS_Lat.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,GPS_Long_deg,'Color',[1 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlabel('Time [s]')
ylabel('Longitude [deg]')
saveas(h,strcat(Output_results_dir,'GPS_Long.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(GPS_Long_deg,GPS_Lat_deg,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlabel('Longitude [deg]')
ylabel('Lattitude [deg]')
saveas(h,strcat(Output_results_dir,'GPS_coord.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Command_Count,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Command count [-]')
saveas(h,strcat(Output_results_dir,'Command_count.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Enable');
Legend_txt(2,1)=cellstr('Fire 1 status');
Legend_txt(3,1)=cellstr('Fire 2 status');
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Cutdown_Event_Flag,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Cutdown_1_Fire_Status,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Cutdown_2_Fire_Status,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
legend(Legend_txt,'Location','best')
legend('boxoff')
clear Legend_txt
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Cutdown [-]')
saveas(h,strcat(Output_results_dir,'Cutdown.png'),'png')
close(h)
clear Legend_txt
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B1_Charge_Current_A,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Filtre_tri(B1_Charge_Current_A,50),'Color',[1 0 0],'Marker',Marker_type,'LineWidth',2)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 1 Current [mA]')
saveas(h,strcat(Output_results_dir,'Battery_1_Current.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B2_Charge_Current_A,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Filtre_tri(B2_Charge_Current_A,50),'Color',[1 0 0],'Marker',Marker_type,'LineWidth',2)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 2 Current [mA]')
saveas(h,strcat(Output_results_dir,'Battery_2_Current.png'),'png')
close(h)
%%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Load_Path_Current_A,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Filtre_tri(Load_Path_Current_A,300),'Color',[1 0 0],'Marker',Marker_type,'LineWidth',2)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Load Path Current [mA]')
saveas(h,strcat(Output_results_dir,'Load_Path_Current.png'),'png')
close(h)
%%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,SA_Current_A,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Filtre_tri(SA_Current_A,50),'Color',[1 0 0],'Marker',Marker_type,'LineWidth',2)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('SA Current [mA]')
saveas(h,strcat(Output_results_dir,'SA_Current.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Bus Battery 1');
Legend_txt(2,1)=cellstr('Bus Battery 2');
%Legend_txt(3,1)=cellstr('Bus SA');
Legend_txt(3,1)=cellstr('Bus loadpath');
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B1_Bus_V_V,'Color',[1 0 0],'Marker',Marker_type)
line(Time,B2_Bus_V_V,'Color',[0 0.5 0],'Marker',Marker_type)
%line(Time,SA_busV_V,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Loadpath_Bus_V_V,'Color',[0 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
legend(Legend_txt,'Location','best')
legend('boxoff')
clear Legend_txt
xlabel('Time [s]')
ylabel('Voltage [V]')
saveas(h,strcat(Output_results_dir,'All_Voltages.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Charging');
Legend_txt(2,1)=cellstr('Discharging');    
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B_1_AH_Charging_A,'Color',[0 0 0],'Marker',Marker_type)
line(Time,B_1_AH_Discharging_A,'Color',[1 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 1 Charge/Disch [Ah]')
saveas(h,strcat(Output_results_dir,'Battery_1_Charge_Discharge.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Charging');
Legend_txt(2,1)=cellstr('Discharging');    
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,B_2_Amp_Hours_Charging_A,'Color',[0 0 0],'Marker',Marker_type)
line(Time,B_2_Amp_Hours_Discharging_A,'Color',[1 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 2 Charge/Disch [Ah]')
saveas(h,strcat(Output_results_dir,'Battery_2_Charge_Discharge.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Battery_Bus_Low_V_Flag,'Color',[1 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
ylim([-0.1 1.1])
xlabel('Time [s]')
ylabel('Battery Bus Low V Flag [-]')
saveas(h,strcat(Output_results_dir,'Battery_bus_Low_V_Flag.png'),'png')
close(h)
%
Legend_txt(1,1)=cellstr('Bus Voltage TLM');
Legend_txt(2,1)=cellstr('Current Batt1 TLM');
Legend_txt(3,1)=cellstr('Current Batt2 TLM');
Legend_txt(4,1)=cellstr('Temp Batt1 TLM');
Legend_txt(5,1)=cellstr('Temp Batt2 TLM');
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Bus_V_TLM_Val_Flag,'Color',[0 0 0],'Marker',Marker_type)
line(Time,B_1_Cur_TLM_Val_Flag,'Color',[1 0 0],'Marker',Marker_type)
line(Time,B_2_Cur_TLM_Val_Flag_,'Color',[0 0.5 0],'Marker',Marker_type)
line(Time,B_1_T_TLM_Val_Flag,'Color',[1 0.8 0],'Marker',Marker_type)
line(Time,B_2_T_TLM_Val_Flag,'Color',[0 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
ylim([-0.1 1.1])
xlabel('Time [s]')
ylabel('TLM Valid Flags [-]')
legend(Legend_txt,'Location','best')
legend('boxoff')
clear Legend_txt
saveas(h,strcat(Output_results_dir,'Battery_TLM_flags.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,GPS_Isvalid_Congl,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('GPS is valid [-]')
saveas(h,strcat(Output_results_dir,'GPS_is_valid.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,GPS_of_Sat,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Number of Sat. [-]')
saveas(h,strcat(Output_results_dir,'GPS_nb_sat.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,RB_Words_Recieved,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('RB words received [-]')
saveas(h,strcat(Output_results_dir,'RB_words_received.png'),'png')
close(h)
