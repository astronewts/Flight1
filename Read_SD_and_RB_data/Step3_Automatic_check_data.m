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
end
display('***')
    %
display('***')
display('1-GYRO DATA')
display(strcat('Mean vertical acceleration=>',num2str(round(100*mean(nonzeros(Mean_Acc_z_g_SD)))/100),'g'))
%
display('***')
display('2-GPS')
ind_GPS=1+min(find(diff(GPS_of_Sat_SD)>0));
if isempty(ind_GPS)~=1
    display(['GPS got first sattelite contact after ',num2str(mean(Elapsed_Time_ms_SD(min(ind_GPS)))/1000),'s'])
    display(['Mean GPS Lattitude: ',num2str(mean(nonzeros(round(10*GPS_Long_deg_SD)/10))),'degrees'])
    display(['Mean GPS Longtitude: ',num2str(mean(nonzeros(round(10*GPS_Lat_deg_SD)/10))),'degrees'])
    display(['Mean GPS Altitude: ',num2str(mean(nonzeros(round(10*GPS_Alt_m_SD/10)))),'m'])
else
    display('WARNING: No GPS data')
end
%
display('***')
display('3-COMMANDS')
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
%display(['Mean altitude: ',num2str(round(10*mean(nonzeros(Alt_m_SD)))/10),'m'])


% function Check_GPS(Data_type)
% end

