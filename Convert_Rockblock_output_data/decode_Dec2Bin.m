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
%path_results = strcat(pwd,'/data/');
path_results = pwd;
%path_results='/Users/kevinmacko/Library/Mobile Documents/com~apple~CloudDocs/Documents/Astronewts/code/Flight1/Convert_Rockblock_output_data/';
%path_results='/Users/gnlacaz/PERSO/Balloon_project/Arduino/Flight1/Convert_Rockblock_output_data/';
% name of output file: 
name_file_result='/data_from_balloon.txt';

%%%%%%%%%%%%%%%%% import data: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Note: to get the word sent by the balloon do the following:
% 1) go on webpage for our rock block: https://core.rock7.com/Operations 
% log ad: astronewts

 cd ./
 filename = 'Test_RB.txt';
 
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
clear tot_word_bin
tot_word_bin='00011100100000001000000000111000001100000001100001100100001100000011001000011000100000010111001000010110111100010110111010010110111101111111111110010111010101011000010000100000101110000001110011011111001001100110011001100110110111100111101011100001010001111010000010010011101011100001010010100000100100111010111000000101010111111000000000000000000000000101111010010001111010111000010100011111101100011010100111111100000111111011000110101000101011001101101010000000011100011110011011100000000100101000111101011100001000010000000101000101101000100010000100000001010001000101000001011111011001100110011001100110110111101010011001100110011001101001111110101110100101111000110110011111101011101001010111101010010001000000000000000000000000000000000000000000000110000001110010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010111101000011000000000000000000010001100100000011100100000000000000000000000000000000000000000001000110010000001110010000000000101111010000110000000000000000000100011001000000111001000000000000000000000000000000000000000000010001100100000011100100000000000011111101111111110000000000000001000110010000001110010000000000000000000000000000000000000000000100011001000000111001000000000000000000000000000000000000000000010001100100000011100100000000000000000000000000000000000000000010111100011110100000000000000000101111010101101011000000000000000011110010111011100000000000000001000001111101100000011100111010110000000000000000000000000000000100000001000000000000000000000011000000000000000000000000000000010000000100000000000000000000000011111110001100110011001100110100000000000000000000000000000000000000000000000000000000000000001011110111001100110011001100110110111111000000000000000000000000010000010100100000000000000000000100000100100000000000000000000000111111100011001100110011001101000000000000000000000000000000000000000000000000000000000000000010111101110011001100110011001101101111110000000000000000000000000100000101001000000000000000000001000001001000000000000000000000000000000001010000000000000100100000000010010110000000000001010000000000000100100000000000010000000000000000111011111111101100000100000100001000000000000000000001000001000000000000000000000000000001110000100011111101111010000010010011111000111101000000111110000000011100010000110000000111100011000010101010100101011101011001010000011100111001100110011001100100011111000110100001000000000000000000';
tot_word_dec

%%%%%%%%%%%%%%%%% END Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%% Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
size_bin_word=size(tot_word_bin);
size_bin_word=size_bin_word(2)

Read_Database_from_Arduino;
% nb_var=95;
% 
% % initialization
% var = zeros(nb_var,1);
% 
% var_name{01} = 'hdr'            ; var_length(01) =8  ;     var_type{01} = 'nd'       ;    var_unit{01}='None';
% var_name{02} = 'mode'           ; var_length(02) =8  ;     var_type{02} = 'unsigned' ;    var_unit{02}='Enum';
% var_name{03} = 'cmdcnt'	        ; var_length(03) =8  ;     var_type{03} = 'unsigned' ;    var_unit{03}='count';
% var_name{04} = 'periodnorm'     ; var_length(04) =8  ;     var_type{04} = 'unsigned' ;    var_unit{04}='min';
% var_name{05} = 'periodshed'     ; var_length(05) =8  ;     var_type{05} = 'unsigned' ;    var_unit{05}='min';
% var_name{06} = 'perioddesc'     ; var_length(06) =8  ;     var_type{06} = 'unsigned' ;    var_unit{06}='min';
% var_name{07} = 'periodemer'     ; var_length(07) =8  ;     var_type{07} = 'unsigned' ;    var_unit{07}='min';
% var_name{08} = 'periodtest'     ; var_length(08) =8  ;     var_type{08} = 'unsigned' ;    var_unit{08}='min';
% var_name{09} = 'periodwrite'    ; var_length(09) =8  ;     var_type{09} = 'unsigned' ;    var_unit{09}='sec';
% %
% var_name{10} = 'batt1temp1'     ; var_length(10) =12 ;     var_type{10} = 'float temp' ;    var_unit{10}='deg C';
% var_name{11} = 'batt1temp2'     ; var_length(11) =12 ;     var_type{11} = 'float temp' ;    var_unit{11}='deg C';
% var_name{12} = 'batt2temp1'     ; var_length(12) =12 ;     var_type{12} = 'float temp' ;    var_unit{12}='deg C';
% var_name{13} = 'batt2temp2'     ; var_length(13) =12 ;     var_type{13} = 'float temp' ;    var_unit{13}='deg C';
% var_name{14} = 'exttemp1'       ; var_length(14) =12 ;     var_type{14} = 'float temp' ;    var_unit{14}='deg C';
% var_name{15} = 'exttemp2'       ; var_length(15) =12 ;     var_type{15} = 'float temp' ;    var_unit{15}='deg C';
% var_name{16} = 'inttemp1'       ; var_length(16) =12 ;     var_type{16} = 'float temp' ;    var_unit{16}='deg C';
% var_name{17} = 'extpr1'         ; var_length(17) =12 ;     var_type{17} = 'float temp' ;    var_unit{17}='deg C';
% var_name{18} = 'batt1vpri'      ; var_length(18) =12 ;     var_type{18} = 'unsigned' ;    var_unit{18}='cnt (V)';
% var_name{19} = 'batt1vred'      ; var_length(19) =12 ;     var_type{19} = 'unsigned' ;    var_unit{19}='cnt (V)';
% var_name{20} = 'batt1i1'        ; var_length(20) =12 ;     var_type{20} = 'unsigned' ;    var_unit{20}='A';  % changed from float to unsinged
% var_name{21} = 'batt1i2'        ; var_length(21) =12 ;     var_type{21} = 'unsigned' ;    var_unit{21}='A';  % changed from float to unsinged
% var_name{22} = 'tempconst1'     ; var_length(22) =32 ;     var_type{22} = 'float'    ;    var_unit{22}='None'; 
% var_name{23} = 'tempconst2'     ; var_length(23) =32 ;     var_type{23} = 'float'    ;    var_unit{23}='None'; 
% var_name{24} = 'spare3'         ; var_length(24) =32 ;     var_type{24} = 'float'    ;    var_unit{24}='None'; 
% var_name{25} = 'spare4'         ; var_length(25) =32 ;     var_type{25} = 'float'    ;    var_unit{25}='None'; 
% var_name{26} = 'gpslat'         ; var_length(26) =32 ;     var_type{26} = 'float'    ;    var_unit{26}='Deg'; 
% var_name{27} = 'gpslong'        ; var_length(27) =32 ;     var_type{27} = 'float'    ;    var_unit{27}='Deg'; 
% var_name{28} = 'gpsalt'         ; var_length(28) =32 ;     var_type{28} = 'float'    ;    var_unit{28}='m'; 
% var_name{29} = 'gpslocage'      ; var_length(29) =32 ;     var_type{29} = 'float'    ;    var_unit{29}='msec'; 
% var_name{30} = 'gpsaltage'      ; var_length(30) =32 ;     var_type{30} = 'float'    ;    var_unit{30}='msec'; 
% var_name{31} = 'gpsdir'         ; var_length(31) =32 ;     var_type{31} = 'float'    ;    var_unit{31}='Deg'; 
% var_name{32} = 'gpsvel'         ; var_length(32) =32 ;     var_type{32} = 'float'    ;    var_unit{32}='kmph'; 
% var_name{33} = 'gpssatcnt'      ; var_length(33) =4  ;     var_type{33} = 'unsigned' ;    var_unit{33}='#'; 
% var_name{34} = 'gpsdate'        ; var_length(34) =32 ;     var_type{34} = 'unsigned' ;    var_unit{34}='DDMMYY'; 
% var_name{35} = 'gpstime'        ; var_length(35) =32 ;     var_type{35} = 'unsigned' ;    var_unit{35}='HHMMSSCC';
% var_name{36} = 'gpshdopval'     ; var_length(36) =32 ;     var_type{36} = 'integer'  ;    var_unit{36}='???'; 
% var_name{37} = 'gpscharcnt'     ; var_length(37) =32 ;     var_type{37} = 'nd'       ;    var_unit{37}='#'; 
% var_name{38} = 'gpssentcnt'     ; var_length(38) =32 ;     var_type{38} = 'nd'       ;    var_unit{38}='#'; 
% var_name{39} = 'gpserrchk'      ; var_length(39) =32 ;     var_type{39} = 'nd'       ;    var_unit{39}='Enum'; 
% var_name{40} = 'gpserrpar'      ; var_length(40) =8  ;     var_type{40} = 'unsigned' ;    var_unit{40}='Enum'; 
% var_name{41} = 'accx'           ; var_length(41) =32 ;     var_type{41} = 'float'    ;    var_unit{41}='g'; 
% var_name{42} = 'accxmax'        ; var_length(42) =32 ;     var_type{42} = 'float'    ;    var_unit{42}='g'; 
% var_name{43} = 'accy'           ; var_length(43) =32 ;     var_type{43} = 'float'    ;    var_unit{43}='g'; 
% var_name{44} = 'accymax'        ; var_length(44) =32 ;     var_type{44} = 'float'    ;    var_unit{44}='g'; 
% var_name{45} = 'accz'           ; var_length(45) =32 ;     var_type{45} = 'float'    ;    var_unit{45}='g'; 
% var_name{46} = 'acczmax'        ; var_length(46) =32 ;     var_type{46} = 'float'    ;    var_unit{46}='g'; 
% var_name{47} = 'magx'           ; var_length(47) =32 ;     var_type{47} = 'float'    ;    var_unit{47}='Deg'; 
% var_name{48} = 'magxmax'        ; var_length(48) =32 ;     var_type{48} = 'float'    ;    var_unit{48}='Deg'; 
% var_name{49} = 'magy'           ; var_length(49) =32 ;     var_type{49} = 'float'    ;    var_unit{49}='Deg'; 
% var_name{50} = 'magymax'        ; var_length(50) =32 ;     var_type{50} = 'float'    ;    var_unit{50}='Deg'; 
% var_name{51} = 'magz'           ; var_length(51) =32 ;     var_type{51} = 'float'    ;    var_unit{51}='Deg'; 
% var_name{52} = 'magzmax'        ; var_length(52) =32 ;     var_type{52} = 'float'    ;    var_unit{52}='Deg'; 
% var_name{53} = 'rawquat'        ; var_length(53) =32 ;     var_type{53} = 'float'    ;    var_unit{53}='Deg/s'; 
% var_name{54} = 'eupose'         ; var_length(54) =32 ;     var_type{54} = 'float'    ;    var_unit{54}='Deg/s'; 
% var_name{55} = 'fuseeupose'     ; var_length(55) =32 ;     var_type{55} = 'float'    ;    var_unit{55}='Deg/s'; 
% var_name{56} = 'gyrotemp'       ; var_length(56) =32 ;     var_type{56} = 'float'    ;    var_unit{56}='Deg C'; 
% var_name{57} = 'sanbattvhi'     ; var_length(57) =8  ;     var_type{57} = 'unsigned' ;    var_unit{57}='V *'; 
% var_name{58} = 'sanbattvlo'     ; var_length(58) =8  ;     var_type{58} = 'unsigned' ;    var_unit{58}='V *'; 
% var_name{59} = 'sanbattihi'     ; var_length(59) =8  ;     var_type{59} = 'unsigned' ;    var_unit{59}='A *'; 
% var_name{60} = 'sanbattilo'     ; var_length(60) =8  ;     var_type{60} = 'unsigned' ;    var_unit{60}='A *'; 
% var_name{61} = 'batt1chgrat'    ; var_length(61) =32 ;     var_type{61} = 'float'    ;    var_unit{61}='None'; 
% var_name{62} = 'batt1chg'       ; var_length(62) =32 ;     var_type{62} = 'float'    ;    var_unit{62}='A-hr'; 
% var_name{63} = 'batt1diss'      ; var_length(63) =32 ;     var_type{63} = 'float'    ;    var_unit{63}='A-hr'; 
% var_name{64} = 'batt1term'      ; var_length(64) =8  ;     var_type{64} = 'unsigned' ;    var_unit{64}='A-hr *'; 
% var_name{65} = 'batt1init'      ; var_length(65) =8  ;     var_type{65} = 'unsigned' ;    var_unit{65}='A-hr *'; 
% var_name{66} = 'batt1vterm'     ; var_length(66) =8  ;     var_type{66} = 'unsigned' ;    var_unit{66}='V *'; 
% var_name{67} = 'batt1vinit'     ; var_length(67) =8  ;     var_type{67} = 'unsigned' ;    var_unit{67}='V *'; 
% var_name{68} = 'batt2chgrat'    ; var_length(68) =32 ;     var_type{68} = 'float'    ;    var_unit{68}='None'; 
% var_name{69} = 'batt2chg'       ; var_length(69) =32 ;     var_type{69} = 'float'    ;    var_unit{69}='A-hr'; 
% var_name{70} = 'batt2diss'      ; var_length(70) =32 ;     var_type{70} = 'float'    ;    var_unit{70}='A-hr'; 
% var_name{71} = 'batt2term'      ; var_length(71) =8  ;     var_type{71} = 'unsigned' ;    var_unit{71}='A-hr *'; 
% var_name{72} = 'batt2init'      ; var_length(72) =8  ;     var_type{72} = 'unsigned' ;    var_unit{72}='A-hr *'; 
% var_name{73} = 'batt2vterm'     ; var_length(73) =8  ;     var_type{73} = 'unsigned' ;    var_unit{73}='V *'; 
% var_name{74} = 'batt2vinit'     ; var_length(74) =8  ;     var_type{74} = 'unsigned' ;    var_unit{74}='V *'; 
% var_name{75} = 'setacttemphi'   ; var_length(75) =16 ;     var_type{75} = 'unsigned' ;    var_unit{75}='Deg K'; 
% var_name{76} = 'setacttemplo'   ; var_length(76) =16 ;     var_type{76} = 'unsigned' ;    var_unit{76}='Deg K'; 
% var_name{77} = 'sanbattthi'     ; var_length(77) =16 ;     var_type{77} = 'unsigned' ;    var_unit{77}='Deg K'; 
% var_name{78} = 'sethtrhi'       ; var_length(78) =16 ;     var_type{78} = 'unsigned' ;    var_unit{78}='Deg K'; 
% var_name{79} = 'sethtrlo'       ; var_length(79) =16 ;     var_type{79} = 'unsigned' ;    var_unit{79}='Deg K'; 
% var_name{80} = 'sethtrhished'   ; var_length(80) =16 ;     var_type{80} = 'unsigned' ;    var_unit{80}='Deg K'; 
% var_name{81} = 'sethtrloshed'   ; var_length(81) =16 ;     var_type{81} = 'unsigned' ;    var_unit{81}='Deg K'; 
% var_name{82} = 'sanbatttlo'     ; var_length(82) =16 ;     var_type{82} = 'unsigned' ;    var_unit{82}='Deg K'; 
% var_name{83} = 'shedv'          ; var_length(83) =8  ;     var_type{83} = 'unsigned' ;    var_unit{83}='V *'; 
% var_name{84} = 'cutv'           ; var_length(84) =8  ;     var_type{84} = 'unsigned' ;    var_unit{84}='V *'; 
% var_name{85} = 'cuttime'        ; var_length(85) =8  ;     var_type{85} = 'unsigned' ;    var_unit{85}='min'; 
% var_name{86} = 'cutalt'         ; var_length(86) =16 ;     var_type{86} = 'unsigned' ;    var_unit{86}='m'; 
% var_name{87} = 'sanaltlo'       ; var_length(87) =16 ;     var_type{87} = 'unsigned' ;    var_unit{87}='m'; 
% var_name{88} = 'cutpw'          ; var_length(88) =8  ;     var_type{88} = 'unsigned' ;    var_unit{88}='dsec'; 
% var_name{89} = 'camrate'        ; var_length(89) =12 ;     var_type{89} = 'unsigned' ;    var_unit{89}='sec';  
% var_name{90} = 'camon'          ; var_length(90) =12 ;     var_type{90} = 'unsigned' ;    var_unit{90}='sec';  
% var_name{91} = 'parbwrd'        ; var_length(91) =16 ;     var_type{91} = 'unsigned' ;    var_unit{91}='Enum'; 
% var_name{92} = 'altalt'         ; var_length(92) =32 ;     var_type{92} = 'float'    ;    var_unit{92}='feet'; 
% var_name{93} = 'alttemp'        ; var_length(93) =32 ;     var_type{93} = 'float'    ;    var_unit{93}='DegC'; 
% var_name{94} = 'altpress'       ; var_length(94) =32 ;     var_type{94} = 'float'    ;    var_unit{94}='Pa'; 
% var_name{95} = 'spare5'         ; var_length(95) =32 ;     var_type{95} = 'long'     ;    var_unit{95}='None';  



%%%%%%%%%%%%%%%%% prepare for output %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% print results to this file
path_name_output=strcat(path_results,name_file_result);
delete(path_name_output);
file_result_ID = fopen(path_name_output,'w');

Ctemp1str='';
for i = 216:216+32
    Ctemp1str = strcat(Ctemp1str,tot_word_bin(i));
end
Ctemp1val = typecast(uint32(bin2dec(Ctemp1str)),'single');

Ctemp2str='';
for i = 248:248+32
    Ctemp2str = strcat(Ctemp2str,tot_word_bin(i));
end
Ctemp2val = typecast(uint32(bin2dec(Ctemp2str)),'single');

index=1; % initialization
vector_out = 0.; % prepare for output
for n = 1:nb_var
%for n = 1:3
    variable='';
    for i=index:index+var_length(n)-1
        variable = strcat(variable,tot_word_bin(i));
    end
    index=index+var_length(n);
    
    % formating of the output file: 
    var_out_type_head='%2d %12s';
    var_out_type_tail=' %s \n';
    
    if strcmp(var_type{n},'null')
        var_out_type_core=' %-17d';
        var(n) = str2num(variable);
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
    var_processed(n)=convert_raw_data(var(n),var_calib(n));
    assignin('base',var_name{n},var_processed(n));
    
    % finalize the formating of the output data:
    var_out_type=strcat(var_out_type_head,var_out_type_core,var_out_type_tail);
    
     %Print output data to file: 
     fprintf(file_result_ID,var_out_type,n-1,var_name{n},var(n));

end
    
% output check
%var(1)   
end_message=strcat('result printed in ./',name_file_result);
%disp('result printed')
disp(end_message)

%%%%%%%%%%%%%%%%% END Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end % end of loop on the words of the input file

fclose(file_result_ID);