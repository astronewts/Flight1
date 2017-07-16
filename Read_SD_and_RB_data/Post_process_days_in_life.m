close all
%
figure(1)
line(Elapsed_Time_s_SD/3600,Alt_m_SD,'Marker','none')
line(Elapsed_Time_s_SD/3600,GPS_Alt_m_SD,'Color',[1 0 0],'Marker','none')
grid on
legend([{'Altimeter'};{'GPS'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Altitude [m]')
saveas(gcf,'Altitude.png','png')
close(gcf)
%
figure(1)
line(Elapsed_Time_s_SD/3600,Cut_Event_Flag_SD,'Marker','none')
grid on
%legend([{'Altimeter'};{'GPS'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Cutdown Event [-]')
saveas(gcf,'Cutdown.png','png')
close(gcf)
%%
figure(1)
line(Elapsed_Time_s_SD/3600,VIN_Voltage_V_SD,'Marker','none','Color',[0 0 0])
line(Elapsed_Time_s_SD/3600,B1_Bus_V_V_SD,'Marker','none')
line(Elapsed_Time_s_SD/3600,B2_Bus_V_V_SD,'Marker','none','Color',[0 0.5 0])
grid on
legend([{'VIN'};{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Battery voltage [-]')
saveas(gcf,'Bus voltage.png','png')
close(gcf)
%%y66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666
figure(1)
line(Elapsed_Time_s_SD/3600,(B1_T_1_C_SD+B1_T_2_C_SD)/2,'Marker','none')
line(Elapsed_Time_s_SD/3600,(B2_T_1_C_SD+B2_T_2_C_SD)/2,'Marker','none','Color',[0 0.5 0])
grid on
legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Battery temperature [-]')
saveas(gcf,'Batt temperature.png','png')
close(gcf)
%%
figure(1)
line(Elapsed_Time_s_SD/3600,Heat_State_1_SD,'Marker','none')
line(Elapsed_Time_s_SD/3600,Heat_State_2_SD,'Marker','none','Color',[0 0.5 0])
grid on
legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Heater state [-]')
saveas(gcf,'Heater_state.png','png')
close(gcf)
%
figure(1)
line(Elapsed_Time_s_SD/3600,Veh_Mode_SD,'Marker','none','Color',[0 0 0])
grid on
%legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Veh mode [-]')
saveas(gcf,'Vehicule_mode.png','png')
close(gcf)
%
figure(1)
line(Elapsed_Time_s_SD/3600,Loadpath_Bus_V_V_SD,'Marker','none','Color',[0 0 0])
grid on
%legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Load path voltage [V]')
saveas(gcf,'Loadpath_V.png','png')
close(gcf)
%
figure(1)
line(Elapsed_Time_s_SD/3600,Loadpath_Current_A_SD,'Marker','none','Color',[0 1 0.5])
grid on
%legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Load path Current [A]')
saveas(gcf,'Loadpath_Current.png','png')
close(gcf)
%
%%
%%
figure(10)
line(Elapsed_Time_s_SD/3600,B1_Charge_Current_A_SD,'Marker','none')
line(Elapsed_Time_s_SD/3600,B2_Charge_Current_A_SD,'Marker','none','Color',[0 0.5 0])
grid on
legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Battery Current [-]')
saveas(gcf,'Batt_current.png','png')
%close(gcf)

%%
figure(1)
line(Elapsed_Time_s_SD/3600,Mean_AZ_g_SD,'Marker','none','Color',[0 0.8 0])
grid on
%legend([{'B1'};{'B2'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Mean Acc Z [g]')
saveas(gcf,'Mean_Acc_Z_g.png','png')
close(gcf)
%%
figure(1)
line(Elapsed_Time_s_SD/3600,Gyro_T_C_SD,'Marker','none','Color',[0 0.9 0])
line(Elapsed_Time_s_SD/3600,Inner_Ext_T_C_SD,'Marker','none','Color',[0 0 0])
line(Elapsed_Time_s_SD/3600,Outer_Ext_T_C_SD,'Marker','none','Color',[1 0 0])
line(Elapsed_Time_s_SD/3600,Alt_T_C_SD,'Marker','none','Color',[0 0 0.5])
grid on
legend([{'Gyro'};{'Inner'};{'Outer'};{'Altimeter'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Temperatures [C]')
saveas(gcf,'Temperatures.png','png')
close(gcf)
%
%%
figure(1)
line(Elapsed_Time_s_SD/3600,SA_Bus_V_V_SD,'Marker','none','Color',[0 0 0])
line(Elapsed_Time_s_SD/3600,SA_Current_A_SD,'Marker','none','Color',[1 0 0])
grid on
ylim([-5 50])
legend([{'Bus V'};{'Current'}],'Location','NorthWest')
xlabel('Time [hr]')
ylabel('Solar arrays [-]')
saveas(gcf,'Solar_Array.png','png')
close(gcf)