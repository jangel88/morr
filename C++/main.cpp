#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<cmath>

#include"population.h"
#include"cart_space.h"
#include<titan_node_info.c>
#include"utils.cpp"

int main(int argc, char** argv)
{
  int i,j,k;
  int pop_size=5;
  int m,n,p;
  m=7;n=1;p=1;
  int n_elites=2,elites[pop_size];
  int max_gen=100;
  Individual best_ind; 
  float cost[6*n*m*p]; 
  float fit;
  float n_fit,b_fit=1e6;
  std::srand(10);

  Population P;
  P.init(pop_size,m*n*p); 
//P.show_pop();
  Domain space; 
  space.init_domain(m,n,p);
//space.show_topo();   
  for(i=0;i<max_gen;i++){
    n_fit=tournament(n*m*p,&P,&space,elites,n_elites);   
    if(n_fit<b_fit){
      b_fit=n_fit;
      best_ind=P.Pop[elites[0]];
      std::cout << b_fit << std::endl;
    } 
    for(j=0;j<pop_size;j++){
      if(j<n_elites){
        P.Pop[j]=P.Pop[elites[j]];
      }else{
        
      }  
    } 
  }
  P.Pop[0].show_chromosome();  
  std::cout<<  " "<< std::endl;
  mutate_swap(P.Pop[0].chromosome);
  P.Pop[0].show_chromosome();
  mutate_inject(P.Pop[0].chromosome);
  std::cout<<  " "<< std::endl;
  P.Pop[0].show_chromosome(); 
  return 0;
}
