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
%
Output_results=strrep(FileName,'.CSV','_results');
Output_results_dir=strcat(PathName,Output_results,filesep);
if isdir(Output_results_dir)~=1
mkdir(Output_results_dir);
end
copyfile(strcat(PathName,filesep,FileName),Output_results_dir);
display(strcat('Results will be stored->',Output_results_dir))
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',[6 4 6 4]);
set(gca,'Box','on')
line(Time,Acc_X_g,'Color',[0 0 0])
line(Time,Acc_Y_g,'Color',[1 0 0])
line(Time,Acc_Z_g-1,'Color',[0 0.5 0])
grid on
xlabel('Time [s]')
ylabel('Acceleration [g]')
saveas(h,strcat(Output_results_dir,'Acceleration.png'),'png')
close(h)
%
h=figure('Visible','off','PaperUnits','inches','PaperPosition',[6 4 6 4]);
set(gca,'Box','on')
line(GPS_Long_deg,GPS_Lat_deg,'Color',[0 0 0],'Marker','o')
grid on
xlabel('Longitude [deg]')
ylabel('Lattitude [deg]')
saveas(h,strcat(Output_results_dir,'GPS_coord.png'),'png')
close(h)
