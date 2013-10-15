#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "cart_space.h"

/* ---------------------------------------------------------------------- */
Domain::Domain(int max_i, int max_j, int max_k) {
  assert(max_i>0); 
  assert(max_j>0); 
  assert(max_k>0); 
  this->max_i=max_i;
  this->max_j=max_j;
  this->max_k=max_k;  
  if(max_k==1){
    if(max_j==1){
      period=1; 
    } else {
      period=max_i;
    }
  } else {
    period=max_i*max_j;
  }
  size=max_i*max_j*max_k; 
  topology.resize(size*6); 
  for(int i=0, N=size;i<N;i++){
    Subdomain q(i,max_i,max_j,max_k);
    std::vector<Subdomain> neigh=find_neighbors(q); 
    topology[i*6+0]=get_position(neigh[0]);
    topology[i*6+1]=get_position(neigh[1]);
    topology[i*6+2]=get_position(neigh[2]);
    topology[i*6+3]=get_position(neigh[3]);
    topology[i*6+4]=get_position(neigh[4]);
    topology[i*6+5]=get_position(neigh[5]);
  }  
}

/* ---------------------------------------------------------------------- */
std::vector<Subdomain> Domain::find_neighbors(Subdomain element) {
  int i,j,k;
  int coors[3];
  element.get_coors(coors);
  i=coors[0];
  j=coors[1];
  k=coors[2];
  assert(i<max_i); 
  assert(j<max_j);
  assert(k<max_k);
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

/* ---------------------------------------------------------------------- */
int Domain::get_position(Subdomain element) {
  int i,j,k;
  int coors[3];
  element.get_coors(coors);
  i=coors[0];  
  j=coors[1]; 
  k=coors[2];
  assert(i<max_i); 
  assert(j<max_j);
  assert(k<max_k);
  return max_j*max_i*k+max_i*j+i; 
}

/* ---------------------------------------------------------------------- */
float Domain::get_fitness(std::vector<nodeid> nodelist){
  assert(nodelist.size()==this->size); 

  int N=this->size; 
  std::vector<int> order(N); 
  for(int i=0;i<N;i++){
    order[i]=i; 
  }
  return get_fitness(nodelist, order); 
}

/* ---------------------------------------------------------------------- */
float Domain::get_fitness(std::vector<nodeid> nodelist, std::vector<int> reorder){
  assert(nodelist.size()==this->size); 
  assert( reorder.size()==this->size); 

  int N=this->size; 

  std::vector<nodeid> reorlist(N); 
  for(int i=0; i<N; i++){
    reorlist[i]=nodelist[reorder[i]]; 
  }
  
  float fitness=0.0; 
  for(int i=0;i<N;i++){
    nodeid n1=reorlist[i];
    for(int j=0;j<6;j++){
      nodeid n2=reorlist[topology[i*6+j]];
      float cost=distance_between_nodes(n1,n2); 
      fitness+=cost*cost; // L2 norm
      //fitness+=cost; //L1 norm
    }
  }
  fitness=sqrt(fitness);
  return fitness;
}

/* ---------------------------------------------------------------------- */
// Begin Subdomain class
/* ---------------------------------------------------------------------- */

Subdomain::Subdomain(int i, int j, int k) {
  this->i=i;
  this->j=j;
  this->k=k;
}

Subdomain::Subdomain(int position,int max_i, int max_j,int max_k) {
  assert(position<max_i*max_j*max_k); 
  i=position%max_i;
  j=((position-i)/max_i)%max_j; 
  k=(position-i-max_i*j)/(max_j*max_i);
}

void Subdomain::get_coors(int* coors) {
  coors[0]=i;
  coors[1]=j;
  coors[2]=k;
}

