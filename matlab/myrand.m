function r=myrand(a,b)
% Produce unique random integers ranged from a to b
% linear random unique integers

%a=1; b=50000; %try this as an example
r=a:b; r=r';
for i=1:(b-a)+1
%    rn=round((a-2)*rand(1,1))+1+(b-a);
    rn=round((b-a-1)*rand(1,1))+1;
    tmp=r(i);
    r(i)=r(rn);
    r(rn)=tmp;
end
