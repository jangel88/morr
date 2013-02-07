function [ Cost ] = compCostMatrix2( xcoors,ycoors,zcoors, topology, assignvec)
% Given a distance matrix, known communication topology,,
% and vector assiging nodes to spatial subdomains, compcost
% computes a matrix that stores the communication cost.
% The cost matrix has the same dimensions of topology input.

% assignvec(i) -> node assigned to SUBDOMAIN i
% topology(i,:) -> neighbors in SPATIAL domain
% DistMat(i,:) -> distance to NODES on torus from NODE i
[N M]=size(topology);




j=assignvec(topology);
i=assignvec;
dx=abs(xcoors(j+1)-repmat(xcoors(i+1),1,M)); dx(dx>25/2)=abs(dx(dx>25/2)-25);
dy=abs(ycoors(j+1)-repmat(ycoors(i+1),1,M)); dy(dy>16/2)=abs(dy(dy>16/2)-16);
dz=abs(zcoors(j+1)-repmat(zcoors(i+1),1,M)); dz(dz>24/2)=abs(dz(dz>24/2)-24);
%C=2*(1-(1/2).^dx)+2*2*(1-(1/2).^dy)+2*(1-(1/2).^dz);
C=dx+2*dy+dz;

Cost=C;
end

