#include"population.h"
#include"nodes.h"
#include<tr1/functional>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
  int max_gen=3500;
  int pop_size=50;
  int node_count, max_i, max_j, max_k; 
  nodeid *node_list;
  FILE *node_file = fopen("./test_suite/chester_cart_05.txt","r");
//FILE *node_file = fopen("./elite.out","r");
  node_list = get_nodes(node_file,node_list,&node_count,&max_i,&max_j,&max_k); 
  std::cout << node_count << max_i << max_j << max_k << std::endl;
  Domain space(max_i,max_j,max_k); 
  std::vector<int> topo=space.give_topo();
  Individual temp(node_count, node_list,true);
  temp.get_fitness(&topo); 

  Population P(pop_size,node_count,&temp);


  std::vector<Individual> elites(5,temp);

  std::srand(1337);
  P.tournament(elites,&space);


  for(int i=0;i<max_gen;i++){
    P.tournament(elites,&space); 
  }

  Individual best_map=P.get_best_map(&topo);
//for(int i=0; i < elites.size(); i++){
//  std::cout << P.individuals[i].give_fitness() << std::endl;
//}
  std::cout << best_map.give_fitness() << std::endl;
  best_map.show_Individual(); 
  return 0;
}
