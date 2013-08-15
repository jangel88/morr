#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "cart_space.h"

/* ---------------------------------------------------------------------- */
Domain::Domain(int max_i, int max_j, int max_k) {
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
    topology[i    ]=get_position(neigh[0]);
    topology[i+  N]=get_position(neigh[1]);
    topology[i+2*N]=get_position(neigh[2]);
    topology[i+3*N]=get_position(neigh[3]);
    topology[i+4*N]=get_position(neigh[4]);
    topology[i+5*N]=get_position(neigh[5]);
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
  return max_j*max_i*k+max_i*j+i; 
}

/* ---------------------------------------------------------------------- */
float Domain::get_fitness(std::vector<nodeid> nodelist){
  if(nodelist.size() != this->size) {
    std::cout<< "Error! nodelist size not same as domain size\n"; 
    exit(1);
  }

  int N=this->size; 
  std::vector<int> order; 
  order.resize(N); 
  for(int i=0;i<N;i++){
    order[i]=i; 
  }
  return get_fitness(nodelist, order); 
}

/* ---------------------------------------------------------------------- */
float Domain::get_fitness(std::vector<nodeid> nodelist, std::vector<int> reorder){
  if(nodelist.size() != this->size) {
    std::cout<< "Error! nodelist size not same as domain size\n"; 
    exit(1);
  }
  if(reorder.size() != this->size) {
    std::cout<< "Error! reorder size not same as domain size\n"; 
    exit(1);
  }
  
  int N=this->size; 
  nodeid n1,n2;
  std::vector<float> cost(N*6);

  for(int j=0;j<6;j++){
    for(int i=0;i<N;i++){
      n1=nodelist.at(reorder.at(topology.at(i+j*N)));
      n2=nodelist.at(reorder.at(i));
      cost[i+j*N]=distance_between_nodes(n1,n2); 
    }
  }

  float fitness=0.0; 
  for(int i=0;i<N*6;i++){ 
    fitness+=cost[i]*cost[i];
//  fitness+=cost[i];
  } 
  fitness=sqrt(fitness);
//fitness=fitness/(N*6);
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
  i=position%max_i;
  j=((position-i)/max_i)%max_j; 
  k=(position-i-max_i*j)/(max_j*max_i);
}

void Subdomain::get_coors(int* coors) {
  coors[0]=i;
  coors[1]=j;
  coors[2]=k;
}

