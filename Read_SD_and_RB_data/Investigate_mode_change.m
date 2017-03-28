load 
figure(1)
plot(Elapsed_Time_ms_SD/1000,Veh_Mode_SD)
xlim([4390 4400])

figure(2)
plot(Elapsed_Time_ms_SD/1000,[GPS_Alt_m_SD Alt_m_SD])
xlim([4390 4400])

figure(3)
plot(Elapsed_Time_ms_SD/1000,[B1_Bus_V_V_SD B2_Bus_V_V_SD])
xlim([4390 4400])


figure(4)
plot(Elapsed_Time_ms_SD/1000,[B1_Bus_V_V_SD B2_Bus_V_V_SD])
xlim([4390 4400])
