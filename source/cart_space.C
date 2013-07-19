#include <iostream>
#include "cart_space.h"

Domain::Domain(int max_i, int max_j, int max_k) {
  this->max_i=max_i;
  this->max_j=max_j;
  this->max_k=max_k;  
  if(max_k==1){
    period=max_i;
  } else {
    period=max_i*max_j;
  }
  size=max_i*max_j*max_k; 
  topology.resize(size*6); 
  for(int i=0, N=size;i<N;i++){
    Subdomain q(i,max_i,max_j,max_k);
    std::vector<Subdomain> neigh=find_neighbors(q); 
    topology[i]=get_position(neigh[0]);
    topology[i+N]=get_position(neigh[1]);
    topology[i+2*N]=get_position(neigh[2]);
    topology[i+3*N]=get_position(neigh[3]);
    topology[i+4*N]=get_position(neigh[4]);
    topology[i+5*N]=get_position(neigh[5]);
  }  
}

std::vector<Subdomain> Domain::find_neighbors(Subdomain element) {
  int i,j,k;
  int coors[3];
  element.give_coors(coors);
  i=coors[0];
  j=coors[1];
  k=coors[2];
  std::vector<Subdomain> neighbor;

  Subdomain n1(i==0 ? max_i-1 : i-1,j,k);
  Subdomain n2(i==max_i-1 ? 0 : i+1,j,k);
  Subdomain n3(i,j==0 ? max_j-1 : j-1,k);
  Subdomain n4(i,j==max_j-1 ? 0 : j+1,k);
  Subdomain n5(i,j,k==0 ? max_k-1 : k-1);
  Subdomain n6(i,j,k==max_k-1 ? 0 : k+1);

  neighbor.push_back(n1);
  neighbor.push_back(n2);
  neighbor.push_back(n3);
  neighbor.push_back(n4);
  neighbor.push_back(n5);
  neighbor.push_back(n6);
  return neighbor;
}

int Domain::get_position(Subdomain element) {
  int i,j,k;
  int coors[3];
  element.give_coors(coors);
  i=coors[0];  
  j=coors[1]; 
  k=coors[2];
  return max_j*max_i*k+max_i*j+i; 
}

Subdomain::Subdomain(int i, int j, int k) {
  this->i=i;
  this->j=j;
  this->k=k;
}

Subdomain::Subdomain(int position,int max_i, int max_j,int max_k) {
  i=position%max_i;
  j=((position-i)/max_i)%max_j; 
  k=(position-i-max_i*j)/(max_j*max_i);
}

void Subdomain::give_coors(int* coors) {
  coors[0]=i;
  coors[1]=j;
  coors[2]=k;
}

