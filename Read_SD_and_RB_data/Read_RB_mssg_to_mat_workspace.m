for lw = 1:size(mssg_rockblock,1)
    lw% just the first word for testing
    %%%%%%%%%%%%%%%%% Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    mssg = char(mssg_rockblock{lw,1})
    length_words = length(mssg)
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
    tot_word_dec
    
    %%%%%%%%%%%%%%%%% END Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%% Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    size_bin_word=size(tot_word_bin);
    size_bin_word=size_bin_word(2)
    
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
                variable = strcat(variable,tot_word_bin(i));
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
            
            % finalize the formating of the output data:
            %    var_out_type=strcat(var_out_type_head,var_out_type_core,var_out_type_tail);
            
            %Print output data to file:
            %     fprintf(file_result_ID,var_out_type,n-1,var_name{n},var(n));
                    assignin('base',strcat(var_name{n},'_RB'),Big_matrix(:,n));
        end

    end
    
    % output check
    %var(1)
%    end_message=strcat('result printed in ./',name_file_result);
    %disp('result printed')
    %disp(end_message)
    
    %%%%%%%%%%%%%%%%% END Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end % end of loop on the words of the input file