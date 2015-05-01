%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Script to convert rockblock message from web interface into binaries 
%
% 1) load data from rockblock spreadsheet
% 2) convert the decimal data into the original binary word
% 3) use TLM order to retreave data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all
close all
clc
format long

%%%%%%%%%%%%%%%%% import data: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%mssg_rockblock = importdata(full_data);

% filename = './rockblock_data_example.xls';
% sheet = 1;
% xlRange = 'J6:J8';
% % note: xlRange does not work for excel on Mac:
% %mssg_rockblock = xlsread(filename,sheet,xlRange)
% mssg_rockblock = xlsread(filename);

mssg_rockblock=[12 34 123 43 56 32];
%%%%%%%%%%%%%%%%% END import data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    

%%%%%%%%%%%%%%%%% Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
nb_words = size(mssg_rockblock);
nb_words = nb_words(2);

output={'nb words in message:',num2str(nb_words)};
disp(output); 

bin_word='';
for m = 1:nb_words
    bin_word = strcat(bin_word,dec2bin(mssg_rockblock(m),8));
    disp(bin_word) ; 
end
%%%%%%%%%%%%%%%%% END Create bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%% Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
size_bin_word=size(bin_word);
size_bin_word=size_bin_word(2);

nb_var=95;

var_name{01} = 'hdr'            ; var_length(01) =8  ;
var_name{02} = 'mode'           ; var_length(02) =8  ;
var_name{03} = 'cmdcnt'	        ; var_length(03) =8  ;
var_name{04} = 'ratenorm'       ; var_length(04) =8  ;
var_name{05} = 'rateshed'       ; var_length(05) =8  ;
var_name{06} = 'ratedesc'       ; var_length(06) =8  ;
var_name{07} = 'rateemer'       ; var_length(07) =8  ;
var_name{08} = 'ratetest'       ; var_length(08) =8  ;
var_name{09} = 'ratewrite'      ; var_length(09) =8  ;
var_name{10} = 'batt1temp1'     ; var_length(10) =12 ;
var_name{11} = 'batt1temp2'     ; var_length(11) =12 ;
var_name{12} = 'batt2temp1'     ; var_length(12) =12 ;
var_name{13} = 'batt2temp2'     ; var_length(13) =12 ;
var_name{14} = 'exttemp1'       ; var_length(14) =12 ;
var_name{15} = 'exttemp2'       ; var_length(15) =12 ;
var_name{16} = 'inttemp1'       ; var_length(16) =12 ;
var_name{17} = 'extpr1'         ; var_length(17) =12 ;
var_name{18} = 'batt1vpri'      ; var_length(18) =12 ;
var_name{19} = 'batt1vred'      ; var_length(19) =12 ;
var_name{20} = 'batt1i1'        ; var_length(20) =13 ;
var_name{21} = 'batt1i2'        ; var_length(21) =13 ;
var_name{22} = 'spare1'         ; var_length(22) =32 ;
var_name{23} = 'spare2'         ; var_length(23) =32 ;
var_name{24} = 'spare3'         ; var_length(24) =32 ;
var_name{25} = 'spare4'         ; var_length(25) =32 ;
var_name{26} = 'gpslat'         ; var_length(26) =32 ;
var_name{27} = 'gpslong'        ; var_length(27) =32 ;
var_name{28} = 'gpsalt'         ; var_length(28) =32 ;
var_name{29} = 'gpslocage'      ; var_length(29) =32 ;
var_name{30} = 'gpsaltage'      ; var_length(30) =32 ;
var_name{31} = 'gpsdir'         ; var_length(31) =32 ;
var_name{32} = 'gpsvel'         ; var_length(32) =32 ;
var_name{33} = 'gpssatcnt'      ; var_length(33) =4  ;
var_name{34} = 'gpsdate'        ; var_length(34) =32 ;
var_name{35} = 'gpstime'        ; var_length(35) =32 ;
var_name{36} = 'gpshdopval'     ; var_length(36) =32 ;
var_name{37} = 'gpscharcnt'     ; var_length(37) =32 ;
var_name{38} = 'gpssentcnt'     ; var_length(38) =32 ;
var_name{39} = 'gpserrchk'      ; var_length(39) =32 ;
var_name{40} = 'gpserrpar'      ; var_length(40) =8  ;
var_name{41} = 'accx'           ; var_length(41) =32 ;
var_name{42} = 'accxmax'        ; var_length(42) =32 ;
var_name{43} = 'accy'           ; var_length(43) =32 ;
var_name{44} = 'accymax'        ; var_length(44) =32 ;
var_name{45} = 'accz'           ; var_length(45) =32 ;
var_name{46} = 'acczmax'        ; var_length(46) =32 ;
var_name{47} = 'magx'           ; var_length(47) =32 ;
var_name{48} = 'magxmax'        ; var_length(48) =32 ;
var_name{49} = 'magy'           ; var_length(49) =32 ;
var_name{50} = 'magymax'        ; var_length(50) =32 ;
var_name{51} = 'magz'           ; var_length(51) =32 ;
var_name{52} = 'magzmax'        ; var_length(52) =32 ;
var_name{53} = 'rawquat'        ; var_length(53) =32 ;
var_name{54} = 'eupose'         ; var_length(54) =32 ;
var_name{55} = 'fuseeupose'     ; var_length(55) =32 ;
var_name{56} = 'gyrotemp'       ; var_length(56) =32 ;
var_name{57} = 'sanbattvhi'     ; var_length(57) =8  ;
var_name{58} = 'sanbattvlo'     ; var_length(58) =8  ;
var_name{59} = 'sanbattihi'     ; var_length(59) =8  ;
var_name{60} = 'sanbattilo'     ; var_length(60) =8  ;
var_name{61} = 'batt1chgrat'    ; var_length(61) =32 ;
var_name{62} = 'batt1chg'       ; var_length(62) =32 ;
var_name{63} = 'batt1diss'      ; var_length(63) =32 ;
var_name{64} = 'batt1term'      ; var_length(64) =8  ;
var_name{65} = 'batt1init'      ; var_length(65) =8  ;
var_name{66} = 'batt1vterm'     ; var_length(66) =8  ;
var_name{67} = 'batt1vinit'     ; var_length(67) =8  ;
var_name{68} = 'batt2chgrat'    ; var_length(68) =32 ;
var_name{69} = 'batt2chg'       ; var_length(69) =32 ;
var_name{70} = 'batt2diss'      ; var_length(70) =32 ;
var_name{71} = 'batt2term'      ; var_length(71) =8  ;
var_name{72} = 'batt2init'      ; var_length(72) =8  ;
var_name{73} = 'batt2vterm'     ; var_length(73) =8  ;
var_name{74} = 'batt2vinit'     ; var_length(74) =8  ;
var_name{75} = 'setacttemphi'   ; var_length(75) =16 ;
var_name{76} = 'setacttemplo'   ; var_length(76) =16 ;
var_name{77} = 'sanbattthi'     ; var_length(77) =16 ;
var_name{78} = 'sethtrhi'       ; var_length(78) =16 ;
var_name{79} = 'sethtrlo'       ; var_length(79) =16 ;
var_name{80} = 'sethtrhished'   ; var_length(80) =16 ;
var_name{81} = 'sethtrloshed'   ; var_length(81) =16 ;
var_name{82} = 'sanbatttlo'     ; var_length(82) =16 ;
var_name{83} = 'shedv'          ; var_length(83) =8  ;
var_name{84} = 'cutv'           ; var_length(84) =8  ;
var_name{85} = 'cuttime'        ; var_length(85) =8  ;
var_name{86} = 'cutalt'         ; var_length(86) =16 ;
var_name{87} = 'sanaltlo'       ; var_length(87) =16 ;
var_name{88} = 'cutpw'          ; var_length(88) =8  ;
var_name{89} = 'camrate'        ; var_length(89) =12 ;
var_name{90} = 'camon'          ; var_length(90) =12 ;
var_name{91} = 'parbwrd'        ; var_length(91) =16 ;
var_name{92} = 'spare5'         ; var_length(92) =32 ;
var_name{93} = 'spare6'         ; var_length(93) =32 ;
var_name{94} = 'spare7'         ; var_length(94) =32 ;
var_name{95} = 'spare8'         ; var_length(95) =32 ;

index=1;
%for n = 1:nb_var
for n = 1:3
    for i=index:index+var_length(n)-1
       
    end
    index=index+var_length(n);
end
%%%%%%%%%%%%%%%%% END Decode bin word %%%%%%%%%%%%%%%%%%%%%%%%%%%%%