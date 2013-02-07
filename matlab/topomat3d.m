function [ topout ] = topomat3d( m,n,p )
%TOPOMAT3D Create topology matrix of six nearest neighbors in NxMxP domain with periodic boundaries .
% Scheme : topology(i,:)=[a b c d e f]               
%         b  f           ^   
%         | /          y |  / z   
%    c - (i) - a         | /          
%        /|              |/_____>            
%       e d              /     x     
%
%
topology=zeros(m*n*p,6);

for v=1:m*n*p
    [i j k] = getind(v);
  %   fprintf('subdomain %d has coords (%d, %d, %d)\n',getid(i,j,k),i, j,k)
    
    if k==1         % Front face
        if (j~=1 && j~=m)
            if (i~=n && i~=1)
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,j-1,k) ...
                    getid(i+1,j,k) getid(i,j,p) getid(i,j,k+1)];
            elseif i==1 %Front face, top row, not corner
                topology(v,:)=[getid(i,j+1,k) getid(n,j,k) getid(i,j-1,k) ...
                    getid(i+1,j,k) getid(i,j,p) getid(i,j,k+1)];
            elseif i==n % Front face, bottom row, not corner
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,j-1,k) ...
                    getid(1,j,k) getid(i,j,p) getid(i,j,k+1)];
            end
        elseif j==m % Front face, right column, not corner
            if (i~=1 && i~=n)
                topology(v,:)=[getid(i,1,k) getid(i-1,j,k) getid(i,j-1,k) ...
                    getid(i+1,j,k) getid(i,j,p) getid(i,j,k+1)];
                
            elseif i==1 %Front face, top right corner
                topology(v,:)=[getid(i,1,k) getid(n,j,k) getid(i,j-1,k) ...
                    getid(i+1,j,k) getid(i,j,p) getid(i,j,k+1)];
            elseif i==n
                topology(v,:)=[getid(i,1,k) getid(i-1,j,k) getid(i,j-1,k) ...
                    getid(1,j,k) getid(i,j,p) getid(i,j,k+1)];
                
            end
            
        elseif j==1
            if (i~=n && i~=1) % Front face, left column, not corner
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,m,k) ...
                    getid(i+1,j,k) getid(i,j,p) getid(i,j,k+1)];
            elseif i==1   % Front top left corner
                topology(v,:)=[getid(i,j+1,k) getid(n,j,k) getid(i,m,k) ...
                    getid(i+1,j,k) getid(i,j,p) getid(i,j,k+1)];
            elseif i==n
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,m,k) ...
                    getid(1,j,k) getid(i,j,p) getid(i,j,k+1)];
                
            end
        end
    elseif k==p
        if (j~=1 && j~=m)
            if (i~=1 && i~=n) % Rear face, not corner-edge
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,j-1,k) ...
                    getid(i+1,j,k) getid(i,j,k-1) getid(i,j,1)];
            elseif i==1 % Rear face, top row, not corner
                topology(v,:)=[getid(i,j+1,k) getid(n,j,k) getid(i,j-1,k) ...
                    getid(i+1,j,k) getid(i,j,k-1) getid(i,j,1)];
            elseif i==n  % Rear face, bottom row, not corner
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,j-1,k) ...
                    getid(1,j,k) getid(i,j,k-1) getid(i,j,1)];
            end
        elseif j==1
            if (i~=1 && i~=n) % Rear face, left column, not corner
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,m,k)...
                    getid(i+1,j,k) getid(i,j,k-1) getid(i,j,1)];
            elseif i==1   % Rear face, left column, top corner
                topology(v,:)=[getid(i,j+1,k) getid(n,j,k) getid(i,m,k)...
                    getid(i+1,j,k) getid(i,j,k-1) getid(i,j,1)];
            elseif i==n % Rear face, left column, bottom corner
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,m,k)...
                    getid(1,j,k) getid(i,j,k-1) getid(i,j,1)];
            end
        elseif j==m
            if (i~=1 && i~=n) % Rear face, right column, not corner
                topology(v,:)=[getid(i,1,k) getid(i-1,j,k) getid(i,j-1,k)...
                    getid(i+1,j,k) getid(i,j,k-1) getid(i,j,1)];
                
            elseif i==1  % Rear face, right column, top corner
                topology(v,:)=[getid(i,1,k) getid(n,j,k) getid(i,j-1,k)...
                    getid(i+1,j,k) getid(i,j,k-1) getid(i,j,1)];
            elseif i==n  % Rear face, right column, bottom corner
                topology(v,:)=[getid(i,1,k) getid(i-1,j,k) getid(i,j-1,k)...
                    getid(1,j,k) getid(i,j,k-1) getid(i,j,1)];
            end
        end
    elseif (k~=1 && k~=p)
        if (j~=1 && j~=m)
            if (i~=1 && i~=n) %Interior, no boundaries
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,j-1,k)...
                    getid(i+1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
            elseif i==1  %Top face, no corner edges
                topology(v,:)=[getid(i,j+1,k) getid(n,j,k) getid(i,j-1,k)...
                    getid(i+1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
                
            elseif i==n % Bottom face, no corner edges
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,j-1,k)...
                    getid(1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
                
            end
        elseif j==1
            if (i~=1 && i~=n) %Left Face, no corner edges
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,m,k)...
                    getid(i+1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
            elseif i==1  % Left face, top row, no corners
                topology(v,:)=[getid(i,j+1,k) getid(n,j,k) getid(i,m,k)...
                    getid(i+1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
            elseif i==n % Left face, bottom row, no corners
                topology(v,:)=[getid(i,j+1,k) getid(i-1,j,k) getid(i,m,k)...
                    getid(1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
            end
        elseif j==m
            if (i~=1 && i~=n)  %Right Face, no corner edges
                topology(v,:)=[getid(i,1,k) getid(i-1,j,k) getid(i,j-1,k)...
                    getid(i+1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
                
            elseif i==1   %Right Face, top row, no corners
                topology(v,:)=[getid(i,1,k) getid(n,j,k) getid(i,j-1,k)...
                    getid(i+1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
            elseif i==n   %Right Face, bottom row, no corners
                topology(v,:)=[getid(i,1,k) getid(i-1,j,k) getid(i,j-1,k)...
                    getid(1, j, k) getid(i,j,k-1) getid(i,j,k+1)];
            end
        end
    end
end

function [i j k] = getind(id)
        % calculate (i,j,k) indices based on size
        % of domain and id.
        i = rem(id,n);
        if i==0; i=n; end
        j = rem((id-i)/n+1+m,m);
         if j==0; j=m; end
        k = 1/m*((id-i)/n+1+m-j);
end


    function id = getid(i,j,k)
        % calculates id from position using
        % predetermined ordering scheme
        if p==1 
            if (n==1)
              id=j;
            elseif m==1
               id=i;
            else
              id=n*(j-1)+i;
            end
        elseif m==1
            if n==1
                id=k;
            elseif p==1
                id=i;
            else 
                id=n*(k-1)+i;
            end
        elseif n==1
            if m==1
                id=k;
            elseif p==1
                id=j;
            else
                id=m*(k-1)+j;
            end
        else
            id = m*n*(k-1)+n*(j-1)+i;
        end
end
topout=topology;
end