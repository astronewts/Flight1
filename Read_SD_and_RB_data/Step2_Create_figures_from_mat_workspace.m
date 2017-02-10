%% Create figures
% figure(1)
% line(Elapsed_Time_ms_SD/1000,Heater_State_1_SD*30,'Color',[0 0 1])
% line(Elapsed_Time_ms_SD/1000,Heater_State_2_SD*28,'Color',[0 0 0])
% line(Elapsed_Time_ms_SD/1000,(B_1_T_1_C_SD+B_1_T_2_C_SD)/2,'Color',[1 0 0])
% line(Elapsed_Time_ms_SD/1000,(B_2_T_1_C_SD+B_2_T_2_C_SD)/2,'Color',[0 0.5 0])
% ylim([20 32])
%
% figure(1)
% line(Elapsed_Time_ms_SD/1000,Veh_Mode_SD)
% line(Elapsed_Time_s_RB,Veh_Mode_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% %
% figure(2)
% line(Elapsed_Time_ms_SD/1000,Cam_Status_SD)
% line(Elapsed_Time_s_RB,Cam_Status_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% %
% %%
% figure(3)
% line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_High_C_SD)
% line(Elapsed_Time_s_RB,B_Active_T_Lim_High_C_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% %%
% figure(4)
% line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_Low_C_SD)
% line(Elapsed_Time_s_RB,B_Active_T_Lim_Low_C_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% %
% % %%
% % figure(5)
% % %line(Elapsed_Time_ms_SD/1000,Active_Trans_Per_s_SD)
% % line(Elapsed_Time_s_RB,Active_Trans_Per_s_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% % %%
% % % figure(6)
% % % line(Elapsed_Time_ms_SD/1000,Alt_ft_SD)
% % % line(Elapsed_Time_s_RB,Alt_ft_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% % %
% % %%
figure(1)
line(Elapsed_Time_ms_SD/1000,Acc)
line(Elapsed_Time_s_RB,Max_GPS_Alt_m_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
line(Elapsed_Time_s_RB,Min_GPS_Alt_m_RB,'Color',[0 0.5 0],'Marker','+','LineStyle','none')
line(Elapsed_Time_s_RB,Mean_GPS_Alt_m_RB,'Color',[0 0 0],'Marker','v','LineStyle','none')
% %%
% close all
% Read_Database_from_Arduino
% clearvars -except *_RB *_SD *var_name
% for v=131:140%size(var_name,2)
%     try
% Y_SD=evalin('base',strcat(char(var_name(1,v)),'_SD'));
%     end
%     try
% Y_RB=evalin('base',strcat(char(var_name(1,v)),'_RB'));
%     end
% %
% if (exist('Y_SD')==1 && exist('Y_RB')==1) 
% h=figure(v);
% set(gcf,'Visible','on')
% line(Elapsed_Time_ms_SD/1000,Y_SD)
% line(Elapsed_Time_s_RB,Y_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% xlabel('Time [s]')
% ylabel(strrep(char(var_name(1,v)),'_','-'))
% grid on
% pause(2)
% clear Y_RB Y_SD
% else
% display(strcat(char(var_name(1,v)),'=> DOES NOT EXIST'))    
% end
%end