function [ child ] = ox_cross( parent1,parent2 )
%OX_CROSS Summary of this function goes here
%   Detailed explanation goes here
if length(parent1)~=length(parent2)
    error('cross:sizeChk','Parents not same length!')
end
N=numel(parent1);

start=randi(N);
stop=randi([start, N]);
proto=zeros(1,N);
proto(start:stop)=parent1(start:stop);
trim=parent2(ismember(parent2,proto)==0);
proto(proto==0)=trim;
child=proto;
end
% l=0;
% for p=1:numel(proto)
%     if proto(p)==0
%         l=l+1;
%     end
% end
%    
% par2trim=trimpar(proto,parent2);
% q=1;t=1;
% ToRemove=zeros(l,1);
% while l>0
%     
%     if par2trim(q)==0
%         ToRemove(t)=q;
%         t=t+1;
%         l=l-1;
%     end
%     q=q+1;
%    
% end
% 
% par2trim(ToRemove)=[];   
% proto=[par2trim(1:length(1:start-1)) proto];
% proto=[proto par2trim(length(1:start-1)+1:end)];
% child=proto;
% 
% 
% 
%     function trimmed=trimpar(proto,par)
%         n=numel(proto);m=numel(par);
%         tpar=par;
%         
%         for j=1:n
%             for k=1:m
%                 if proto(j)==par(k) && proto(j)~=0
%                     tpar=tpar(tpar~=par(k));
%                 
%                
%                 end
%                 
%             end
%         end
%  
%         trimmed=tpar; 
% 
% 
%                 
%         end
%             
%         
%     end
