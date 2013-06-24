#include"population.h"
#include<tr1/functional>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cstdlib>



std::vector<nodeid> get_nodes(FILE *node_file, std::vector<nodeid> node_list, int *node_count, int *max_i, int *max_j, int *max_k)
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
      }else{
        node_list.push_back((nodeid) atoi(string));
        i++;
      } 
    }
  } 

  return node_list; 
}


int main(int argc, char **argv)
{


  int max_gen=1500;
  int pop_size=atoi(argv[1]);
  int num_elites=(int) pop_size*0.1;
  int node_count, max_i, max_j, max_k; 
  std::vector<nodeid> node_list;
  FILE *node_file = fopen("./test_suite/chester_cart_05.txt","r");


  node_list = get_nodes(node_file,node_list,&node_count,&max_i,&max_j,&max_k); 
  Domain space(max_i,max_j,max_k); 
  std::vector<int> topo=space.give_topo();
  Individual temp(node_count, node_list,true);
  temp.get_fitness(&topo); 

  Population P(pop_size,node_count,&temp);
  P.set_elites(num_elites);

  std::vector<Individual> elites(num_elites,temp);

  std::srand(1327);
  P.tournament(&elites,&space);


  for(int i=0;i<max_gen;i++){
    P.tournament(&elites,&space); 
//  printf("%d %f %f\n",i,elites[0].give_fitness(),elites[num_elites-1].give_fitness()); 
    P.populate_next_gen(&elites,&space);
  }

  Individual best_map=P.get_best_map(&topo);

  printf("%d %f\n",pop_size,P.individuals[0].give_fitness());
  P.individuals[0].print_chromosome(&space);
  return 0;
}
