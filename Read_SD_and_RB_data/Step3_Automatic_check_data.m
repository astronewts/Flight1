display('**********************************************************');
display('***')
if (exist('Elapsed_Time_s_SD')==1 && exist('Elapsed_Time_s_RB')==1)
display('*** Going to check data from both a SD card and Rock-block')
end
if (exist('Elapsed_Time_s_SD')==1 && exist('Elapsed_Time_s_RB')==0)
display('*** Going to check data from a SD card')
end
if (exist('Elapsed_Time_s_SD')==1 && exist('Elapsed_Time_s_RB')==1)
display('*** Going to check data from the Rock-block!')
end
if (exist('Elapsed_Time_s_SD')==0 && exist('Elapsed_Time_s_RB')==0)
display('*** Man, you have no data, go back to Step1!')
return;
end
display('***')
%
Time_int=min(Elapsed_Time_ms_SD/1000):1:max(Elapsed_Time_ms_SD/1000);
%
display('0-GENERAL')
display(['Duration of the test [min]: ',num2str(max(round(10*Elapsed_Time_ms_SD/(1000*60))/10))]);
display(['Vehicle modes in this test: ',num2str(unique(Veh_Mode_SD)')])
display(['First SD card writting after ',num2str(min(Elapsed_Time_ms_SD/1000)),'s']);
%
display('***')
display('1-GYRO DATA')
display(['Mean vertical acceleration: ',num2str(round(100*mean(nonzeros(Mean_Acc_z_g_SD)))/100),'g'])
%
display('***')
display('2-GPS')
ind_GPS=1+min(find(diff(GPS_of_Sat_SD)>0));
if isempty(ind_GPS)~=1
    display(['GPS got first sattelite contact after ',num2str(mean(Elapsed_Time_ms_SD(min(ind_GPS)))/1000),'s'])
    display(['Mean GPS Lattitude: ',num2str(mean(nonzeros(round(10*GPS_Long_deg_SD)/10))),' deg'])
    display(['Mean GPS Longtitude: ',num2str(mean(nonzeros(round(10*GPS_Lat_deg_SD)/10))),' deg'])
    display(['Mean GPS Altitude: ',num2str(mean(nonzeros(round(10*GPS_Alt_m_SD/10)))),'m'])
else
    display('WARNING: No GPS data')
end
%
display('***')
display('3-ROCK-BLOCK AND COMMANDS')
if RB_Words_Received_SD>0 
    if RB_Words_Received_SD==1
    display('There was 1 RB Word received during this test')  
    else
        display(['There were ', num2str(int(RB_Words_Received_SD)),' RB words received during this test'])  
    end
else
display('No RB word was received during this test')    
end
if CMD_Count_SD>0 
    if CMD_Count_SD==1
    display('There was 1 command during this test')  
    else
        display(['There were ', num2str(int(CMD_Count_SD)),' commands during this test'])  
    end
else
display('No command during this test')    
end
%
display('***')
display('4- BATTERIES')

display(['Mean B1 voltage: ',num2str(mean(round(100*B1_Bus_V_V_SD)/100)),'V'])
display(['Mean B2 voltage: ',num2str(mean(round(10*B2_Bus_V_V_SD)/100)),'V'])
%
display('***')
display('5-ALTIMETER')
ind_Alt=find(Alt_m_SD~=0);
display(['Altimeter started to read non-zeros after ',num2str(mean(Elapsed_Time_ms_SD(min(ind_Alt)))/1000),'s'])
display(['Mean altitude: ',num2str(round(10*mean(Alt_m_SD(ind_Alt)))/10),'m'])
display(['Max altitude change: ',num2str(round(100*mean(gradient(Alt_m_SD(ind_Alt),Elapsed_Time_ms_SD(ind_Alt))*1000))/100),'m/s'])
%
display('***')
display('6-CUTDOWN')
if diff(Cut_Event_Flag_SD>0)
    display(['Cutdown was commanded after ',num2str(Elapsed_Time_ms_SD(diff(Cut_Event_Flag_SD>0))/1000),' s'])
else
display(['No cutdown during this test'])
end
%
display('***')
display('7-HEATERS')
Time_heater_1_on=sum(interp1(Elapsed_Time_ms_SD/1000,Heat_State_1_SD,Time_int));
Time_heater_2_on=sum(interp1(Elapsed_Time_ms_SD/1000,Heat_State_2_SD,Time_int));
display(['Heater 1 was ON during ',num2str(Time_heater_1_on),' s => ',num2str(round(10*Time_heater_1_on/size(Time_int,1))/10),'% of the time'])
display(['Heater 2 was ON during ',num2str(Time_heater_2_on),' s => ',num2str(round(10*Time_heater_2_on/size(Time_int,1))/10),'% of the time'])
display('***')
%
display('8-TEMPERATURES')
ind_Inner=find(isnan(Inner_Ext_T_C_SD)~=1);
ind_Outer=find(isnan(Outer_Ext_T_C_SD)~=1);
display(['Mean Gyro Temp was ',num2str(round(10*mean(Gyro_T_C_SD()))/10),' C with a max amplitude of ',num2str(round(10*(max(Gyro_T_C_SD)-min(Gyro_T_C_SD)))/10),' C'])
display(['Mean Inner Temp was ',num2str(round(10*mean(Inner_Ext_T_C_SD(ind_Inner)))/10),' C with a max amplitude of ',num2str( round(10*(max(Inner_Ext_T_C_SD(ind_Inner))-min(Inner_Ext_T_C_SD(ind_Inner))))/10),' C'])
display(['Mean Outer Temp was ',num2str(round(10*mean(Outer_Ext_T_C_SD(ind_Outer)))/10),' C with a max amplitude of ',num2str( round(10*(max(Outer_Ext_T_C_SD(ind_Outer))-min(Outer_Ext_T_C_SD(ind_Outer))))/10),' C'])
