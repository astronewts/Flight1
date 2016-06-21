function Yf=Filtre_tri(Y,Filter_band);
%
Bandwidth=2*Filter_band+1;
for i=1:Bandwidth
Filtering(i,1)=i-(Filter_band+1);
if abs(Filtering(i,1))>Filter_band
Filtering(i,2)=0;
else Filtering(i,2)=-4*abs(Filtering(i,1))/(Bandwidth*Bandwidth)+2/Bandwidth;
end
end
Filtering(Filter_band+1,2)=2/Bandwidth-1/(Bandwidth*Bandwidth);
%
Yf=Y;
for i=(Filter_band+1):size(Y,1)-(Filter_band+1)   
Yf_temp=0;
for j=-Filter_band:Filter_band
Yf_temp=Yf_temp+Y(i+j,1)*Filtering(j+Filter_band+1,2);
end
Yf(i,1)=Yf_temp;
end