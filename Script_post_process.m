clear all
close all
clc
%
[FileName,PathName,FilterIndex]=uigetfile('*.csv');
[Data,Txt]=xlsread(strcat(PathName,filesep,FileName));
%
for c=1:size(Data,2)
Name_variable=char(Txt(1,c)); 
Name_variable_mod=strrep(Name_variable,' ','_');
Name_variable_mod=strrep(Name_variable_mod,'_[-]','');
Name_variable_mod=strrep(Name_variable_mod,'[','');
Name_variable_mod=strrep(Name_variable_mod,']','');
Name_variable_mod=strrep(Name_variable_mod,'-','');
Name_variable_mod=strrep(Name_variable_mod,'/','');
Name_variable_mod=strrep(Name_variable_mod,'.','');
Name_variable_mod=strrep(Name_variable_mod,'#','');
Name_variable_mod=strrep(Name_variable_mod,'__','_');
Name_variable_mod=strrep(Name_variable_mod,'?','');
assignin('base',Name_variable_mod,Data(:,c));
end
%
Time=millis/1e3;
xlimits=[350 400];
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
Size_fig=[4 4 8 3];
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Acc_Z_g-1,'Color',[0 0.5 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Z Acceleration [g]')
saveas(h,strcat(Output_results_dir,'Acceleration_Z.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Acc_X_g,'Color',[0 0 0],'Marker','o')
line(Time,Acc_Y_g,'Color',[1 0 0],'Marker','o')
line(Time,Acc_Z_g-1,'Color',[0 0.5 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Acceleration [g]')
saveas(h,strcat(Output_results_dir,'Acceleration.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
title(title_str)
line(Time,Gyro_mag_X_G,'Color',[0 0 0],'Marker','o')
line(Time,Gyro_mag_Y_G,'Color',[1 0 0],'Marker','o')
line(Time,Gyro_mag_Z_G,'Color',[0 0.5 0],'Marker','o')
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Mag data [mG]')
saveas(h,strcat(Output_results_dir,'Mag_data.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
title(title_str)
line(Time,Gyro_X_degs,'Color',[0 0 0],'Marker','o')
line(Time,Gyro_Y_degs,'Color',[1 0 0],'Marker','o')
line(Time,Gyro_Z_degs,'Color',[0 0.5 0],'Marker','o')
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Gyro [deg]')
saveas(h,strcat(Output_results_dir,'Gyro.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(GPS_Long_deg,GPS_Lat_deg,'Color',[0 0 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Longitude [deg]')
ylabel('Lattitude [deg]')
saveas(h,strcat(Output_results_dir,'GPS_coord.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Alt_ft,'Color',[0 0 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Altitude [ft]')
saveas(h,strcat(Output_results_dir,'Alt_ft.png'),'png')
close(h)
%
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,GPS_Alt_m*100/30.48,'Color',[0 0 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Altitude from GPS [ft]')
saveas(h,strcat(Output_results_dir,'Alt_ft_from_GPS.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Command_Count,'Color',[0 0 0],'Marker','o')
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
line(Time,Cutdown_Enable,'Color',[0 0 0],'Marker','o')
line(Time,Cutdown_1_Fire_Status,'Color',[1 0 0],'Marker','o')
line(Time,Cutdown_2_Fire_Status,'Color',[0 0.5 0],'Marker','o')
title(title_str)
legend(Legend_txt,'Location','best')
legend('boxoff')
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Cutdown [-]')
saveas(h,strcat(Output_results_dir,'Cutdown.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Heater_State_1,'Color',[0 0 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Heater state [-]')
saveas(h,strcat(Output_results_dir,'Heater_state.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',Size_fig);
set(gca,'Box','on')
line(Time,Bat_1_Temp_1_C,'Color',[0 0 0],'Marker','o')
line(Time,Bat_1_Temp_2_C,'Color',[0 0.5 0],'Marker','o')
title(title_str)
grid on
xlim(xlimits)
xlabel('Time [s]')
ylabel('Battery 1 temperature [C]')
saveas(h,strcat(Output_results_dir,'Battery_temp.png'),'png')
close(h)
