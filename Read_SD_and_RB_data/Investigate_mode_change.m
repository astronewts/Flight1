
%%
figure(1)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,Veh_Mode_SD)
xlabel('Time [s]')
ylabel('Veh mode')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,Veh_Mode_SD)
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Veh mode')
grid on

figure(2)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,[GPS_Alt_m_SD Alt_m_SD],'o')
xlabel('Time [s]')
ylabel('ALtitude [m]')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,[GPS_Alt_m_SD Alt_m_SD],'o')
xlim([4390 4400])
xlabel('Time [s]')
ylabel('ALtitude [m]')
grid on

figure(3)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,[B1_Bus_V_V_SD B2_Bus_V_V_SD])
xlabel('Time [s]')
ylabel('Bus voltage [V]')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,[B1_Bus_V_V_SD B2_Bus_V_V_SD])
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Bus voltage [V]')
grid on

figure(4)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,GPS_of_Sat_SD)
xlabel('Time [s]')
ylabel('Number of Sat [-]')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,GPS_of_Sat_SD)
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Number of Sat [-]')
grid on

figure(5)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,ALT_Alt_Valid_SD)
plot(Elapsed_Time_ms_SD/1000,GPS_Alt_Valid_SD)
plot(Elapsed_Time_ms_SD/1000,GPS_Alt_Isvalid_SD)
xlabel('Time [s]')
ylabel('Valid flags [-]')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,ALT_Alt_Valid_SD)
plot(Elapsed_Time_ms_SD/1000,GPS_Alt_Valid_SD)
plot(Elapsed_Time_ms_SD/1000,GPS_Alt_Isvalid_SD)
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Valid flags [-]')
grid on

figure(6)
subplot(211)
line(Elapsed_Time_ms_SD/1000,ALT_Alt_Valid_SD,'Color',[1 0 0])
line(Elapsed_Time_ms_SD/1000,GPS_Alt_Valid_SD,'Marker','o','Color',[0 0.5 0])
line(Elapsed_Time_ms_SD/1000,GPS_Alt_Isvalid_SD,'Marker','s','Color',[0 0 0])
xlabel('Time [s]')
ylabel('Valid Alt flags [-]')
grid on
subplot(212)
line(Elapsed_Time_ms_SD/1000,ALT_Alt_Valid_SD)
line(Elapsed_Time_ms_SD/1000,GPS_Alt_Valid_SD)
line(Elapsed_Time_ms_SD/1000,GPS_Alt_Isvalid_SD)
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Valid Alt flags [-]')
grid on

figure(7)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,Bus_V_TLM_Val_Flag_SD)
xlabel('Time [s]')
ylabel('Valid Bus V TLM flags [-]')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,Bus_V_TLM_Val_Flag_SD)
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Valid Bus V TLM flags [-]')
grid on


figure(8)
subplot(211)
plot(Elapsed_Time_ms_SD/1000,Cut_Event_Flag_SD)
xlabel('Time [s]')
ylabel('Cut event [-]')
grid on
subplot(212)
plot(Elapsed_Time_ms_SD/1000,Cut_Event_Flag_SD)
xlim([4390 4400])
xlabel('Time [s]')
ylabel('Cut event  [-]')
grid on