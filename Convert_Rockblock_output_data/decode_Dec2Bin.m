%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Script to convert rockblock message from web interface into understandable data 
%
% 1) load data from rockblock spreadsheet
% 2) convert the data into the original binary word
% 3) use TLM order to convert bin into data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all
close all
clc
format long

% define print path for output file: 
path_results='/Users/gnlacaz/PERSO/Balloon_project/Arduino/Flight1/Convert_Rockblock_output_data/';
% name of output file: 
name_file_result='data_from_balloon.txt';

%%%%%%%%%%%%%%%%% import data: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 cd ./
 filename = 'rockblock_data_example.txt';
 
fileID = fopen(filename);
mssg_rockblock = textscan(fileID,'%s');
fclose(fileID);

nb_words=ndims(mssg_rockblock);


%%%%%%%%%%%%%%%%% END import data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
% loop on words: 
%for lw = 1:nb_words
for lw = 1:1 % just the first word for testing
%%%%%%%%%%%%%%%%% Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mssg = mssg_rockblock{1}{lw};
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
tot_word_bin
%tot_word_dec

%%%%%%%%%%%%%%%%% END Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%% Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
size_bin_word=size(tot_word_bin);
size_bin_word=size_bin_word(2)


nb_var=95;

% initialization
var = zeros(nb_var,1);

var_name{01} = 'hdr'            ; var_length(01) =8  ;     var_type{01} = 'nd'       ; 
var_name{02} = 'mode'           ; var_length(02) =8  ;     var_type{02} = 'unsigned' ;
var_name{03} = 'cmdcnt'	        ; var_length(03) =8  ;     var_type{03} = 'unsigned' ;
var_name{04} = 'periodnorm'     ; var_length(04) =8  ;     var_type{04} = 'unsigned' ;
var_name{05} = 'periodshed'     ; var_length(05) =8  ;     var_type{05} = 'unsigned' ;
var_name{06} = 'perioddesc'     ; var_length(06) =8  ;     var_type{06} = 'unsigned' ;
var_name{07} = 'periodemer'     ; var_length(07) =8  ;     var_type{07} = 'unsigned' ;
var_name{08} = 'periodtest'     ; var_length(08) =8  ;     var_type{08} = 'unsigned' ;
var_name{09} = 'periodwrite'    ; var_length(09) =8  ;     var_type{09} = 'unsigned' ;
var_name{10} = 'batt1temp1'     ; var_length(10) =12 ;     var_type{10} = 'unsigned' ;
var_name{11} = 'batt1temp2'     ; var_length(11) =12 ;     var_type{11} = 'unsigned' ;
var_name{12} = 'batt2temp1'     ; var_length(12) =12 ;     var_type{12} = 'unsigned' ;
var_name{13} = 'batt2temp2'     ; var_length(13) =12 ;     var_type{13} = 'unsigned' ;
var_name{14} = 'exttemp1'       ; var_length(14) =12 ;     var_type{14} = 'unsigned' ;
var_name{15} = 'exttemp2'       ; var_length(15) =12 ;     var_type{15} = 'unsigned' ;
var_name{16} = 'inttemp1'       ; var_length(16) =12 ;     var_type{16} = 'unsigned' ;
var_name{17} = 'extpr1'         ; var_length(17) =12 ;     var_type{17} = 'unsigned' ;
var_name{18} = 'batt1vpri'      ; var_length(18) =12 ;     var_type{18} = 'unsigned' ;
var_name{19} = 'batt1vred'      ; var_length(19) =12 ;     var_type{19} = 'unsigned' ;
var_name{20} = 'batt1i1'        ; var_length(20) =32 ;     var_type{20} = 'float'    ;
var_name{21} = 'batt1i2'        ; var_length(21) =32 ;     var_type{21} = 'float'    ; 
var_name{22} = 'spare1'         ; var_length(22) =32 ;     var_type{22} = 'float'    ; 
var_name{23} = 'spare2'         ; var_length(23) =32 ;     var_type{23} = 'float'    ; 
var_name{24} = 'spare3'         ; var_length(24) =32 ;     var_type{24} = 'float'    ; 
var_name{25} = 'spare4'         ; var_length(25) =32 ;     var_type{25} = 'float'    ; 
var_name{26} = 'gpslat'         ; var_length(26) =32 ;     var_type{26} = 'float'    ; 
var_name{27} = 'gpslong'        ; var_length(27) =32 ;     var_type{27} = 'float'    ; 
var_name{28} = 'gpsalt'         ; var_length(28) =32 ;     var_type{28} = 'float'    ; 
var_name{29} = 'gpslocage'      ; var_length(29) =32 ;     var_type{29} = 'float'    ; 
var_name{30} = 'gpsaltage'      ; var_length(30) =32 ;     var_type{30} = 'float'    ; 
var_name{31} = 'gpsdir'         ; var_length(31) =32 ;     var_type{31} = 'float'    ; 
var_name{32} = 'gpsvel'         ; var_length(32) =32 ;     var_type{32} = 'float'    ; 
var_name{33} = 'gpssatcnt'      ; var_length(33) =4  ;     var_type{33} = 'unsigned' ; 
var_name{34} = 'gpsdate'        ; var_length(34) =32 ;     var_type{34} = 'unsigned' ; 
var_name{35} = 'gpstime'        ; var_length(35) =32 ;     var_type{35} = 'unsigned' ;
var_name{36} = 'gpshdopval'     ; var_length(36) =32 ;     var_type{36} = 'integer'  ; 
var_name{37} = 'gpscharcnt'     ; var_length(37) =32 ;     var_type{37} = 'nd'       ; 
var_name{38} = 'gpssentcnt'     ; var_length(38) =32 ;     var_type{38} = 'nd'       ; 
var_name{39} = 'gpserrchk'      ; var_length(39) =32 ;     var_type{39} = 'nd'       ; 
var_name{40} = 'gpserrpar'      ; var_length(40) =8  ;     var_type{40} = 'unsigned' ; 
var_name{41} = 'accx'           ; var_length(41) =32 ;     var_type{41} = 'float'    ; 
var_name{42} = 'accxmax'        ; var_length(42) =32 ;     var_type{42} = 'float'    ; 
var_name{43} = 'accy'           ; var_length(43) =32 ;     var_type{43} = 'float'    ; 
var_name{44} = 'accymax'        ; var_length(44) =32 ;     var_type{44} = 'float'    ; 
var_name{45} = 'accz'           ; var_length(45) =32 ;     var_type{45} = 'float'    ; 
var_name{46} = 'acczmax'        ; var_length(46) =32 ;     var_type{46} = 'float'    ; 
var_name{47} = 'magx'           ; var_length(47) =32 ;     var_type{47} = 'float'    ; 
var_name{48} = 'magxmax'        ; var_length(48) =32 ;     var_type{48} = 'float'    ; 
var_name{49} = 'magy'           ; var_length(49) =32 ;     var_type{49} = 'float'    ; 
var_name{50} = 'magymax'        ; var_length(50) =32 ;     var_type{50} = 'float'    ; 
var_name{51} = 'magz'           ; var_length(51) =32 ;     var_type{51} = 'float'    ; 
var_name{52} = 'magzmax'        ; var_length(52) =32 ;     var_type{52} = 'float'    ; 
var_name{53} = 'rawquat'        ; var_length(53) =32 ;     var_type{53} = 'float'    ; 
var_name{54} = 'eupose'         ; var_length(54) =32 ;     var_type{54} = 'float'    ; 
var_name{55} = 'fuseeupose'     ; var_length(55) =32 ;     var_type{55} = 'float'    ; 
var_name{56} = 'gyrotemp'       ; var_length(56) =32 ;     var_type{56} = 'float'    ; 
var_name{57} = 'sanbattvhi'     ; var_length(57) =8  ;     var_type{57} = 'unsigned' ; 
var_name{58} = 'sanbattvlo'     ; var_length(58) =8  ;     var_type{58} = 'unsigned' ; 
var_name{59} = 'sanbattihi'     ; var_length(59) =8  ;     var_type{59} = 'unsigned' ; 
var_name{60} = 'sanbattilo'     ; var_length(60) =8  ;     var_type{60} = 'unsigned' ; 
var_name{61} = 'batt1chgrat'    ; var_length(61) =32 ;     var_type{61} = 'float'    ; 
var_name{62} = 'batt1chg'       ; var_length(62) =32 ;     var_type{62} = 'float'    ; 
var_name{63} = 'batt1diss'      ; var_length(63) =32 ;     var_type{63} = 'float'    ; 
var_name{64} = 'batt1term'      ; var_length(64) =8  ;     var_type{64} = 'unsigned' ; 
var_name{65} = 'batt1init'      ; var_length(65) =8  ;     var_type{65} = 'unsigned' ; 
var_name{66} = 'batt1vterm'     ; var_length(66) =8  ;     var_type{66} = 'unsigned' ; 
var_name{67} = 'batt1vinit'     ; var_length(67) =8  ;     var_type{67} = 'unsigned' ; 
var_name{68} = 'batt2chgrat'    ; var_length(68) =32 ;     var_type{68} = 'float'    ; 
var_name{69} = 'batt2chg'       ; var_length(69) =32 ;     var_type{69} = 'float'    ; 
var_name{70} = 'batt2diss'      ; var_length(70) =32 ;     var_type{70} = 'float'    ; 
var_name{71} = 'batt2term'      ; var_length(71) =8  ;     var_type{71} = 'unsigned' ; 
var_name{72} = 'batt2init'      ; var_length(72) =8  ;     var_type{72} = 'unsigned' ; 
var_name{73} = 'batt2vterm'     ; var_length(73) =8  ;     var_type{73} = 'unsigned' ; 
var_name{74} = 'batt2vinit'     ; var_length(74) =8  ;     var_type{74} = 'unsigned' ; 
var_name{75} = 'setacttemphi'   ; var_length(75) =16 ;     var_type{75} = 'unsigned' ; 
var_name{76} = 'setacttemplo'   ; var_length(76) =16 ;     var_type{76} = 'unsigned' ; 
var_name{77} = 'sanbattthi'     ; var_length(77) =16 ;     var_type{77} = 'unsigned' ; 
var_name{78} = 'sethtrhi'       ; var_length(78) =16 ;     var_type{78} = 'unsigned' ; 
var_name{79} = 'sethtrlo'       ; var_length(79) =16 ;     var_type{79} = 'unsigned' ; 
var_name{80} = 'sethtrhished'   ; var_length(80) =16 ;     var_type{80} = 'unsigned' ; 
var_name{81} = 'sethtrloshed'   ; var_length(81) =16 ;     var_type{81} = 'unsigned' ; 
var_name{82} = 'sanbatttlo'     ; var_length(82) =16 ;     var_type{82} = 'unsigned' ; 
var_name{83} = 'shedv'          ; var_length(83) =8  ;     var_type{83} = 'unsigned' ; 
var_name{84} = 'cutv'           ; var_length(84) =8  ;     var_type{84} = 'unsigned' ; 
var_name{85} = 'cuttime'        ; var_length(85) =8  ;     var_type{85} = 'unsigned' ; 
var_name{86} = 'cutalt'         ; var_length(86) =16 ;     var_type{86} = 'unsigned' ; 
var_name{87} = 'sanaltlo'       ; var_length(87) =16 ;     var_type{87} = 'unsigned' ; 
var_name{88} = 'cutpw'          ; var_length(88) =8  ;     var_type{88} = 'unsigned' ; 
var_name{89} = 'camrate'        ; var_length(89) =12 ;     var_type{89} = 'unsigned' ;  
var_name{90} = 'camon'          ; var_length(90) =12 ;     var_type{90} = 'unsigned' ;  
var_name{91} = 'parbwrd'        ; var_length(91) =16 ;     var_type{91} = 'unsigned' ; 
var_name{92} = 'spare5'         ; var_length(92) =32 ;     var_type{92} = 'long'     ; 
var_name{93} = 'spare6'         ; var_length(93) =32 ;     var_type{93} = 'long'     ; 
var_name{94} = 'spare7'         ; var_length(94) =32 ;     var_type{94} = 'long'     ; 
var_name{95} = 'spare8'         ; var_length(95) =32 ;     var_type{95} = 'long'     ;  


%%%%%%%%%%%%%%%%% prepare for output %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% print results to this file
path_name_output=strcat(path_results,name_file_result);
delete(path_name_output);
file_result_ID = fopen(path_name_output,'w');

% header='';
% for n = 1:nb_var
%     header = strcat(header,var_name{n},', ');
% end    
% fprintf(file_result_ID,'%s',header);


index=1; % initialization
vector_out = 0.; % prepare for output
for n = 1:nb_var
%for n = 1:3
    variable='';
    for i=index:index+var_length(n)-1
        variable = strcat(variable,tot_word_bin(i));
    end
    index=index+var_length(n);
    
    if strcmp(var_type{n},'nd')
        var_out_type='%d %s %d \n';
        var(n) = str2num(variable);
    elseif strcmp(var_type{n},'unsigned')
        var_out_type='%d %s %d \n';
        var(n) = bin2dec(variable);
    elseif strcmp(var_type{n},'float')
        var_out_type='%d %s %f \n';
        var(n) = typecast(uint32(bin2dec(variable)),'single');
    elseif strcmp(var_type{n},'integer') 
        var_out_type='%d %s %d \n';
        % problem with the following:
        %var(n) = typecast(uint8(bi2de(variable, 'left-msb')), 'int8')
        var(n) = bin2dec(variable);
    elseif strcmp(var_type{n},'long') 
        var_out_type='%d %s %d \n';
        var(n) = bin2dec(variable);     
    end
    
    % output
    %vector_out = [vector_out;var(n)];
    % direct output

     %display(var(n),var_name{n})
     fprintf(file_result_ID,var_out_type,n,var_name{n},var(n));

end
    
% output check
%var(1) 
%fprintf(file_result_ID,'%s %f \n',var_name{n},var(n));
disp('result printed')





%%%%%%%%%%%%%%%%% END Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end % end of loop on the words of the input file