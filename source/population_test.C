#include"population.h"
#include<tr1/functional>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cstdlib>

void print_map(Domain* space, Individual* map)
{
  int i,j,k;

  int max_i = space->get_max_i();
  int max_j = space->get_max_j();
  int max_k = space->get_max_k();
  std::cout << std::endl;
  if (max_k == 1){
    for(i=0;i<max_i;i++){
      for(j=0;j<=max_j;j++){
        if(j==max_j){
         std::cout << std::endl;
        }else{
          printf("%4d",map->at(j*max_i+i));
        }
      }
    }
  }

}


nodeid* get_nodes(FILE *node_file, nodeid* node_list, int *node_count, int *max_i, int *max_j, int *max_k)
{
// Read in number of cores, dimensions of simulations, and node IDs assuming
// format:
// # comments
// num_of_cores max_i max_j max_k
// nid0
// nid1
// ...
// nidN

  int tmp;
  char string[100];
  int i = 0;
  int line_count=0;
  while( fgets(string,100,node_file)!=NULL){
    if(string[0]=='#' || string[0]=='\n'){
      continue;
    }else{
      line_count+=1;
      if(line_count==1){ 
        sscanf(string,"%d%d%d%d",node_count,max_i,max_j,max_k);
        node_list = (nodeid*) malloc(sizeof(nodeid)*(*node_count));
      }else{
        node_list[i]=(nodeid) atoi(string);
        i++;
      } 
    }
  } 

  return node_list; 
}


int main(int argc, char **argv)
{
  int max_gen=15000;
  int pop_size=150;
  int num_elites=25;
  int node_count, max_i, max_j, max_k; 
  nodeid *node_list;
  FILE *node_file = fopen("./test_suite/chester_cart_05.txt","r");

  node_list = get_nodes(node_file,node_list,&node_count,&max_i,&max_j,&max_k); 
  Domain space(max_i,max_j,max_k); 
  std::vector<int> topo=space.give_topo();
  Individual temp(node_count, node_list,true);
  temp.get_fitness(&topo); 

  Population P(pop_size,node_count,&temp);


  std::vector<Individual> elites(num_elites,temp);

  std::srand(1337);
  P.tournament(&elites,&space);


  for(int i=0;i<max_gen;i++){
    P.tournament(&elites,&space); 
    printf("%d %f %f\n",i,elites[0].give_fitness(),elites[num_elites-1].give_fitness()); 
  }

  Individual best_map=P.get_best_map(&topo);
  print_map(&space,&best_map);

  return 0;
}
