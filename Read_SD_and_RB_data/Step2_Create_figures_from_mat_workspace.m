if exist('SD_Num_SD')==1
    ind=find(isnan(SD_Num_SD)==0);
    SD_number=mean(SD_Num_SD(ind));
    clear ind
else
    if exist('SD_Num_RB')==1
         ind=find(isnan(SD_Num_RB)==0);
        SD_number=mean(SD_Num_RB(ind));
        clear(ind)
    end
end
%
extra='';
%
if exist('SD_number')==1
    Output_dir=strcat('D:\Astronewts\Postprocessed_data\','Results_',num2str(SD_number),extra,'\');
else
    Output_dir=strcat('D:\Astronewts\Postprocessed_data\Buffer\');
end
if exist(Output_dir,'dir')==1
else
mkdir(Output_dir);    
end
%%
close all
Read_Database_from_Arduino
clearvars -except *_RB *_SD Output_dir Names_*
for v=1:size(Names_Arduino_Database,1)
    try
        Y_SD=evalin('base',strcat(char(Names_Variable_MATLAB(v,1)),'_SD'));
    end
    try
        Y_RB=evalin('base',strcat(char(Names_Variable_MATLAB(v,1)),'_RB'));
    end
    %
    h=figure(v);
    set(gcf,'Visible','on')
    if exist('Y_SD')==1
        line(Elapsed_Time_ms_SD/1000,Y_SD)
    end
    if exist('Y_RB')==1
        line(Elapsed_Time_s_RB,Y_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
    end
    xlabel('Time [s]')
    ylabel(char(Names_after_conversion_for_figures(v,1)))
    grid on
    clear Y_RB Y_SD
    saveas(h,strcat(Output_dir,char(Names_Variable_MATLAB(v,1)),'.png'),'png')
    close(h)
end

%% Create figures
% figure(1)
% line(Elapsed_Time_ms_SD/1000,Heat_State_1_SD*30,'Color',[0 0 0])
% line(Elapsed_Time_ms_SD/1000,(B1_T_1_C_SD+B1_T_2_C_SD)/2,'Color',[0 0.5 0])
% line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_Low_K_SD,'Color',[1 0 0])
% line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_High_K_SD,'Color',[1 0.8 0])
% line(Elapsed_Time_ms_SD/1000,10+Veh_Mode_SD,'Color',[0 0 0.5])
% ylim([10 32])
% %
% figure(2)
% line(Elapsed_Time_ms_SD/1000,Heat_State_2_SD*30,'Color',[0 0 0])
% line(Elapsed_Time_ms_SD/1000,(B2_T_1_C_SD+B2_T_2_C_SD)/2,'Color',[0 0.5 0])
% line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_Low_K_SD,'Color',[1 0 0])
% line(Elapsed_Time_ms_SD/1000,B_Active_T_Lim_High_K_SD,'Color',[1 0.8 0])
% line(Elapsed_Time_ms_SD/1000,10+Veh_Mode_SD,'Color',[0 0 0.5])
% ylim([10 32])
% close all
% figure(1)
% line(Elapsed_Time_ms_SD/1000,Alt_m_SD,'Marker','o')
% line(Elapsed_Time_ms_SD/1000,GPS_Alt_m_SD,'Color',[1 0 0],'Marker','+')
% grid on
% legend([{'Altimeter'};{'GPS'}],'Location','NorthWest')
% xlabel('Time [s]')
% ylabel('Altitude [m]')
% saveas(gcf,'Altitude.png','png')
% close(gcf)
% %
% figure(2)
% line(Elapsed_Time_ms_SD/1000,Veh_Mode_SD)
% grid on
% xlabel('Time [s]')
% ylabel('Veh mode')
% saveas(gcf,'Veh_mode.png','png')
% close(gcf)
% %
% figure(3)
% line(GPS_Long_deg_SD,GPS_Lat_deg_SD)
% grid on
% ylim([37 40])
% xlim([-123 -119])
% xlabel('Longitude [deg]')
% ylabel('Lattitude [deg]')
% saveas(gcf,'Lat_long.png','png')
% close(gcf)
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
% figure(1)
% line(Elapsed_Time_ms_SD/1000,Acc)
% line(Elapsed_Time_s_RB,Max_GPS_Alt_m_RB,'Color',[1 0 0],'Marker','+','LineStyle','none')
% line(Elapsed_Time_s_RB,Min_GPS_Alt_m_RB,'Color',[0 0.5 0],'Marker','+','LineStyle','none')
% line(Elapsed_Time_s_RB,Mean_GPS_Alt_m_RB,'Color',[0 0 0],'Marker','v','LineStyle','none')