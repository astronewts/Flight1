clear all
close all
clc
%
[FileName,PathName,FilterIndex]=uigetfile('*.csv');
[Data,Txt]=xlsread(strcat(PathName,filesep,FileName));
%%
for c=1:min(size(Txt,2),size(Data,2))
Name_variable=char(Txt(1,c)); 
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
assignin('base',Name_variable_mod,Data(:,c));
end
%%
Time=millis/1e3;
xlimits=[9100 9120];
%xlimits=[min(Time) max(Time)];
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
Output_results_dir=strcat(PathName,Output_results,filesep);
if isdir(Output_results_dir)~=1
mkdir(Output_results_dir);
end
copyfile(strcat(PathName,filesep,FileName),Output_results_dir);
display(strcat('Results will be stored->',Output_results_dir))
%
Size_fig=[4 4 12 3];
Marker_type='none';
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Acc_Z_g-1,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Z Acceleration [g]')
saveas(h,strcat(Output_results_dir,'Acceleration_Z.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Acc_X_g,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Acc_Y_g,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Acc_Z_g-1,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Acceleration [g]')
saveas(h,strcat(Output_results_dir,'Acceleration.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
title(title_str)
line(Time,Gyro_mag_X_G,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Gyro_mag_Y_G,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Gyro_mag_Z_G,'Color',[0 0.5 0],'Marker',Marker_type)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Mag data [mG]')
saveas(h,strcat(Output_results_dir,'Mag_data.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
title(title_str)
line(Time,Gyro_X_degs,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Gyro_Y_degs,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Gyro_Z_degs,'Color',[0 0.5 0],'Marker',Marker_type)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Gyro [deg]')
saveas(h,strcat(Output_results_dir,'Gyro.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(GPS_Long_deg,GPS_Lat_deg,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlabel('Longitude [deg]')
ylabel('Lattitude [deg]')
saveas(h,strcat(Output_results_dir,'GPS_coord.png'),'png')
close(h)
end
%
try 
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Alt_ft,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Altitude [ft]')
saveas(h,strcat(Output_results_dir,'Alt_ft.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,GPS_Alt_m*100/30.48,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Altitude from GPS [ft]')
saveas(h,strcat(Output_results_dir,'Alt_ft_from_GPS.png'),'png')
close(h)
end
%
try
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
end
%
try
Legend_txt(1,1)=cellstr('Enable');
Legend_txt(2,1)=cellstr('Fire 1 status');
Legend_txt(3,1)=cellstr('Fire 2 status');
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Cutdown_Enable,'Color',[0 0 0],'Marker',Marker_type)
line(Time,Cutdown_1_Fire_Status,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Cutdown_2_Fire_Status,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
legend(Legend_txt,'Location','best')
legend('boxoff')
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Cutdown [-]')
saveas(h,strcat(Output_results_dir,'Cutdown.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Heater_State_1,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Heater state [-]')
saveas(h,strcat(Output_results_dir,'Heater_state.png'),'png')
close(h)
end
%
try 
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Bat_1_Temp_1_C,'Color',[1 0.2 0],'Marker',Marker_type)
line(Time,Bat_1_Temp_2_C,'Color',[1 0 0],'Marker',Marker_type)
line(Time,Bat_2_Temp_1_C,'Color',[0 0.6 0],'Marker',Marker_type)
line(Time,Bat_2_Temp_2_C,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery temperature [C]')
saveas(h,strcat(Output_results_dir,'Battery_temp.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Bat_1_Charge_Current_A,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 1 Current [mA]')
saveas(h,strcat(Output_results_dir,'Battery_1_Current.png'),'png')
close(h)
end
%
try 
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Load_Path_Current_A,'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Load Path Current [mA]')
saveas(h,strcat(Output_results_dir,'Load_Path_Current.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Bat_1_Volt_V,'Color',[1 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 1 Voltage [V]')
saveas(h,strcat(Output_results_dir,'Battery_1_Voltage.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,busvoltage_batt1_V,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Bus Battery 1 Voltage [V]')
saveas(h,strcat(Output_results_dir,'Bus_Battery_1_Voltage.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,loadvoltage_batt1_V,'Color',[1 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Load Battery 1 Voltage [V]')
saveas(h,strcat(Output_results_dir,'Load_Battery_1_Voltage.png'),'png')
close(h)
end
%
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,shuntvoltage_batt1_V,'Color',[0 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Shunt Battery 1 Voltage [V]')
saveas(h,strcat(Output_results_dir,'Shunt_Battery_1_Voltage.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Filtre_tri(busvoltage_batt1_V.*Bat_1_Charge_Current_A*1e-3,50),'Color',[0 0 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 1 Power [W]')
saveas(h,strcat(Output_results_dir,'Power_Battery_1.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,busvoltage_sa_V,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Bus SA Voltage [V]')
saveas(h,strcat(Output_results_dir,'Bus_SA_Voltage.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,loadvoltage_sa_V,'Color',[1 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Load SA Voltage [V]')
saveas(h,strcat(Output_results_dir,'Load_SA_Voltage.png'),'png')
close(h)
end
%
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,shuntvoltage_sa_V,'Color',[0 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Shunt SA Voltage [V]')
saveas(h,strcat(Output_results_dir,'Shunt_SA_Voltage.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,busvoltage_load_path_V,'Color',[0 0.5 0],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
ylim([10 20])
xlabel('Time [s]')
ylabel('Bus loadpath Voltage [V]')
saveas(h,strcat(Output_results_dir,'Bus_loadpath_Voltage.png'),'png')
close(h)
end
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,loadvoltage_load_path_V,'Color',[1 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Load loadpath Voltage [V]')
saveas(h,strcat(Output_results_dir,'Load_loadpath_Voltage.png'),'png')
close(h)
end
%
%
try
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,shuntvoltage_load_path_V,'Color',[0 0 0.5],'Marker',Marker_type)
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Shunt loadpath Voltage [V]')
saveas(h,strcat(Output_results_dir,'Shunt_loadpath_Voltage.png'),'png')
close(h)
end