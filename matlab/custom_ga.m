function [ stats PopOut BestAssignment BestFit ] = custom_ga
%   Detailed explanation goes here
clc



m=12;n=4;p=1;
tasks=n*m*p;
PopSize=1000;
NumOfProcs=tasks;
mutate_frac=.9;
numberOfElites=15;
MaxGen=1000;
D=load('/home/Spring13/ORNL/data/coords.txt');
xcoors=D(:,1);ycoors=D(:,2);zcoors=D(:,3);
rng(7919);
clear D
topology=topomat3d(m,n,p);
metric='2norm';
BestFit=Inf;
StopTime=240;
BestAssignment=zeros(NumOfProcs,1);
v=load('/home/Spring13/ORNL/data/nodes_1_1_24_1_2_1.txt');
%v=load('/home/Spring13/ORNL/data/maxnorm.txt');
%v=load('/home/Spring13/ORNL/matlab/tests/nodes.txt');
v=[v];%;v];
if strcmp(metric,'max')==1
    met=@(x) max(x(:));
elseif strcmp(metric,'avg')==1
    met=@(x) mean(x(:));
    
elseif strcmp(metric,'2norm')==1
    met=@(x) norm(x(:),2);
elseif strcmp(metric,'cust')==1
    met=@(x) -(.9*sum(x(:)==0)+0.1*sum(x(:)==1));
end
c=1;
NumOfMutants=floor(mutate_frac*PopSize);
InitPop=zeros(PopSize,NumOfProcs);

for j=1:PopSize
    InitPop(j,randperm(tasks))=v;
end

Pop=InitPop;
no_better=0;
tic
for u=1:MaxGen
    if (toc>StopTime )
        break
    end
    [elites nBfit nBassign ]=tournament(Pop,xcoors,ycoors,zcoors,topology,met,numberOfElites);
    
    if nBfit<BestFit
        no_better=0;
        BestFit=nBfit   
        BestAssignment=nBassign;
        fitV(c,:)=[toc BestFit];c=c+1;  
    else 
        no_better=no_better+1;
        if no_better > 200
            break
        end
    end
    NextGen=zeros(PopSize,NumOfProcs);
    NextGen(1:numberOfElites,:)=Pop(elites,:);
    
    for k=numberOfElites+1:numberOfElites+1+NumOfMutants
        NextGen(k,:)=mutate(NextGen(randi(numberOfElites),:));
    end
    r=myrand(1,numberOfElites);
    for q=numberOfElites+1+NumOfMutants+1:PopSize 
        NextGen(q,:)=shuffle_cross(Pop(elites(r(randi(numberOfElites))),:),Pop(elites(r(randi(numberOfElites))),:));
    end
    Pop=NextGen;
end
    function [child] = shuffle_cross(parent1,parent2)
        d=(parent1==parent2)==0;
        proto1=parent1(d);
        child=parent1;
        child(d)=proto1(randperm(length(proto1)));
    end

    function [elites bfit bassign ] = tournament(pop,xcoors,ycoors,zcoors,topology,metric,numberOfElites)
        [M N]=size(pop);
        fit=zeros(M,1);
        bFit=Inf;
        for i=1:M
            C=compCostMatrix2(xcoors,ycoors,zcoors,topology,pop(i,:));
            fit(i)=metric(C);
            if fit(i)<bFit
                bFit=fit(i);
                bAssign=pop(i,:);
            end 
        end
        [Y ind]=sort(fit);
        bfit=bFit;
        bassign=bAssign;
       elites(1)=ind(1);
       e=2;
       ii=2;
       while length(elites)<numberOfElites
           K=pop(elites,:)==repmat(pop(ind(ii),:),length(elites),1);
           
           if(sum(sum(K,2)==N)==0)
                elites(e)=ind(ii);
                e=e+1;
            end
            ii=ii+1;
        end
  %      elites=ind(1:numberOfElites);
    end

    function mutant=mutate_inject(elite)
        N=numel(elite);
        x=randi(N);
        y=randi(N);
        z_range=[0 1 3 7 15 23];
        z=z_range(randi(6));
        holding=elite;
        if x<y
            z=mod(z,N-y+1);
            mutant=[holding(1:(x-1)) holding(y:y+z) holding(x:(y-1))  holding((y+z+1):end)];
        else
            z=mod(z,N-x+1);
            mutant=[holding(1:(y-1)) holding(x:x+z) holding(y:(x-1))  holding((x+z+1):end)];
        end
    end

    function mutant=mutate_swap(elite)
        N=numel(elite);
        x=randi(N);
        y=randi(N);
        z_range=[0 1 3 7 ];%8];% 16 24];
        z=z_range(randi(4));
        holding=elite;
        if x<y
            z=min([z,N-y]);
            mutant=[holding(1:(x-1)) holding(y:(y+z)) holding((x+z+1):(y-1)) holding(x:(x+min([z y-x-1]))) holding((y+z+1):end)];
        elseif y<x
            z=min([z N-x]);
            mutant=[holding(1:(y-1)) holding(x:(x+z)) holding((y+z+1):(x-1)) holding(y:(y+min([z x-y-1]))) holding((x+z+1):end)];
        else
            mutant=elite;
        end
    end

    function mutant=mutate_head2tail(elite)
        z_range=0:n-1;%[2 4 6 8 16 24];
        z=z_range(randi(length(z_range)));
        t=randi([0,m-1]);
        start=t*n+1;
        stop=(t+1)*n;
        mutant=[elite(1:(start-1)) elite((start+z+1):stop) elite(start:start+z) elite((stop+1):end)];
    end
    function mutant=mutate_mirror(elite)
        z_range=[1 3 5 7 15 23];
        z=z_range(randi(6));
        start=randi(length(elite)-z-1);
        mutant=[elite(1:(start-1)) fliplr(elite(start:start+z)) elite((start+z+1):end)];
    end

    function mutant=mutate(elite)
      t=1;%randi(3);
      tmp=elite;
      while(t>0)
        t=t-1;
        g=rand;
        if g>.750
            tmp=mutate_swap(tmp);
        elseif g>.500
            tmp=mutate_inject(tmp);
        elseif g>.2500
            tmp=mutate_head2tail(tmp);
        else
            tmp=mutate_mirror(tmp); 
        end
      end
      mutant=tmp;
    end
PopOut=Pop;
stats=fitV;

end
