display('**********************************************************');
display('1-Please choose a file with RB messages: .txt, .xls or csv');
%
[FileName,PathName,FilterIndex]=uigetfile ({'*.xls','xls-files';'*.txt','txt-files';'*.csv','csv-files'});
display(char(strcat('You selected the file=>',FileName)));
FileName_wo_ext=strrep(FileName,'.txt','');
FileName_wo_ext=strrep(FileName,'.TXT','');
FileName_wo_ext=strrep(FileName,'.xls','');
FileName_wo_ext=strrep(FileName,'.XLS','');
FileName_wo_ext=strrep(FileName,'.xlsx','');
FileName_wo_ext=strrep(FileName,'.XLSX','');
FileName_wo_ext=strrep(FileName,'.csv','');
FileName_wo_ext=strrep(FileName,'.CSV','');
%
if strfind(FileName,'xls')
    [Data,Txt]=xlsread(strcat(PathName,FileName));
    c=0;
    for lw=5:size(Txt,1)
        if strcmp(char(Txt(lw,3)),'MO')==1
            c=c+1;
            mssg_rockblock{c,1}=Txt(lw,4);
            Date_mssg_rockblock(c,1)=Txt(lw,1);
        end
    end
end
%
if strfind(FileName,'csv')
    [Data,Txt]=xlsread(strcat(PathName,FileName));
    c=0;
    for lw=2:size(Txt,1)
        if strcmp(char(Txt(lw,3)),'MO')==1
            c=c+1;
            mssg_rockblock{c,1}=Txt(lw,4);
            Date_mssg_rockblock(c,1)=Txt(lw,1);
        end
    end
end
%
if strfind(FileName,'txt')
    fileID = fopen(strcat(PathName,FileName));
    mssg_rockblock = textscan(fileID,'%s');
    fclose(fileID);
    for lw=1:size(mssg_rockblock{1,1},1);
        mssg_rockblock_new{lw,1}=mssg_rockblock{1}{lw};
        Date_mssg_rockblock(lw,1)=cellstr('Date Unknown');
    end
    clear mssg_rockblock
    mssg_rockblock=mssg_rockblock_new;
end
%
display(char(strcat('=> There are ',{' '},num2str(size(mssg_rockblock,1)),{' '},'RB message(s) in this file')));
%
display('2-Starting the decoding');
for lw = 1:size(mssg_rockblock,1)
    %try
        %%%%%%%%%%%%%%%%% Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        mssg = char(mssg_rockblock{lw,1});
        length_words = length(mssg);
        nb_hex_words= length_words/2 ;
        tot_word_bin='';
        tot_word_dec=0;
        for m = 0:nb_hex_words-1
            word_hex=strcat(mssg(2*m+1),mssg(2*m+2));
            word_dec=hex2dec(word_hex);
            
            tot_word_dec = [tot_word_dec;word_dec];
            
            word_bin=dec2bin(word_dec,8);
            tot_word_bin = strcat(tot_word_bin,word_bin);
        end
        % print the binary word:
        tot_word_dec;
        
        %%%%%%%%%%%%%%%%% END Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        %%%%%%%%%%%%%%%%% Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        size_bin_word=size(tot_word_bin);
        size_bin_word=size_bin_word(2);
        
        Read_Database_from_Arduino;
        
        %%%%%%%%%%%%%%%%% prepare for output %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % print results to this file
        %     path_name_output=strcat(path_results,name_file_result);
        %     delete(path_name_output);
        %     file_result_ID = fopen(path_name_output,'w');
        %
        Format_type=bin2dec(tot_word_bin(9:16));
        %
        index=1; % initialization
        vector_out = 0.; % prepare for output
        for n = 1:nb_var
            
            if ((Format_type==1 & var_format1(n)==1)  || (Format_type==2 & var_format2(n)==1))
                variable='';
                for i=index:index+var_length(n)-1
                    if i>size(tot_word_bin,2)
                        display(char('You have a mismatch between the RB message and the database for message',{' '},num2str(lw)));
                    else
                        variable = strcat(variable,tot_word_bin(i));
                    end
                end
                index=index+var_length(n);
                %
                % formating of the output file:
                var_out_type_head='%2d %12s';
                var_out_type_tail=' %s \n';
                %
                if strcmp(var_type{n},'null')
                    var_out_type_core=' %-17d';
                    var(n) = str2num(variable);
                elseif strcmp(var_type{n},'header')
                    var_out_type_core=' %-17f';
                    temp1=str2num(variable);
                elseif strcmp(var_type{n},'unsigned')
                    var_out_type_core=' %-17d';
                    var(n) = bin2dec(variable);
                elseif strcmp(var_type{n},'float')
                    var_out_type_core=' %-17f';
                    var(n) = typecast(uint32(bin2dec(variable)),'single');
                elseif strcmp(var_type{n},'float temp')
                    var_out_type_core=' %-17f';
                    temp1=bin2dec(variable);
                    var(n) = Ctemp1val*(bin2dec(variable)-Ctemp2val);
                elseif strcmp(var_type{n},'int')
                    var_out_type_core=' %-17d';
                    var(n) = bin2dec(variable);
                elseif strcmp(var_type{n},'long')
                    var_out_type_core=' %-17d';
                    var(n) = bin2dec(variable);
                end
                %
                var_processed(n)=convert_raw_data(var(n),var_calib(n));
                %
                Big_matrix(lw,n)=var_processed(n);
                %
                assignin('base',strcat(var_name{n},'_RB'),Big_matrix(:,n));
            end
        end
        display(char(strcat('Success for RB message ',{' '},num2str(lw),{' -'},{' '},Date_mssg_rockblock(lw,1),{' '},'- Corresponds to SD CARD',{' '},num2str(SD_Num_RB(lw)))));
        %%%%%%%%%%%%%%%%% END Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %catch
%         display(char(strcat('FAILURE for RB message ',{' '},num2str(lw),{' -'},{' '},Date_mssg_rockblock(lw,1),{' '},'- Corresponds to SD CARD',{' '},num2str(SD_Num_RB(lw)))));
%         for n = 1:nb_var
%             var_processed(n)=NaN;
%             %
%             Big_matrix(lw,n)=var_processed(n);
%             %
%             assignin('base',strcat(var_name{n},'_RB'),Big_matrix(:,n));
%         end
   % end
end % end of loop on the words of the input file