
clear all
close all
clc
format long

n=1;

% for nd and unsinged:
tot_word_bin = '10101010';

% for single:
%tot_word_bin = '11000000000111001111110111110100' ; 

%var_type{01} = 'nd';
var_type{01} = 'unsigned';
%var_type{01} = 'single';
%var_type{01} = 'integer';


if strcmp(var_type{n},'nd')
    var_length(01) =8;
elseif strcmp(var_type{n},'unsigned')
    var_length(01) =8;
elseif strcmp(var_type{n},'single')
    var_length(01) =32;
elseif strcmp(var_type{n},'integer')
    var_length(01) =8;
end    

index=1;
var(n) = 0;   
if strcmp(var_type{n},'nd')
    variable='';
    for i=index:index+var_length(n)-1
        variable = strcat(variable,tot_word_bin(i));
    end
    var(n) = str2num(variable);
elseif strcmp(var_type{n},'unsigned')
    variable='';
    for i=index:index+var_length(n)-1
        variable = strcat(variable,tot_word_bin(i));
    end
    var(n) = bin2dec(variable);
        
%     compt=1;
%     var(n) = 0;
%     for i=index:index+var_length(n)-1
%         var(n) = var(n) + str2num(tot_word_bin(i))* 2.0^(var_length(n) - compt);
%         compt=compt+1;
%     end
elseif strcmp(var_type{n},'single')

            variable='';
        for i=index:index+var_length(n)-1
            variable = strcat(variable,tot_word_bin(i));
        end
        
        var(n) = typecast(uint32(bin2dec(variable)),'single');
        
elseif strcmp(var_type{n},'integer')
        variable='';
        for i=index:index+var_length(n)-1
            variable = strcat(variable,tot_word_bin(i));
        end        
        %var(n) = typecast(uint8((variable, 'left-msb')),'int8');
        
    
end % if on var_type

var(n) 