#include"population.h"

#include<stdio.h>
#include<stdlib.h>
#include<boost/mpi.hpp>
#include<boost/serialization/vector.hpp>


std::vector<nodeid> get_nodes(FILE *node_file,std::vector<nodeid> node_list,int *node_count,int *max_i,int *max_j,int *max_k)
{

//Read in number of cores, dimensions of simulations, and node IDs assuming
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
      //node_list = (nodeid*) malloc(sizeof(nodeid)*(*node_count)); 
      }else{ 
        node_list.push_back((nodeid) atoi(string));
        i++; 
      } 
    } 
  } 

  return node_list; 
}


namespace mpi = boost::mpi;



int main(int argc, char **argv)
{

  if(argc < 2){
    printf("usage:\n");
    printf("./dist_test N\n");
    printf("N - population size\n");
    exit(1);
  }
  mpi::environment env(argc, argv);
  mpi::communicator world;

  int rank,np; 
  int max_gen=1500;
  int pop_size=(int) atoi(argv[1]);
  int num_elites=0.1*pop_size;



  int node_count,max_i,max_j,max_k;
  std::vector<nodeid> node_list;

  srand(133*world.rank());

  if(world.rank()==0){
    FILE *node_file=fopen("./test_suite/chester_cart_05.txt","r");
  
    node_list=get_nodes(node_file,node_list,&node_count,&max_i,&max_j,&max_k);
    printf("%4d %4d %4d %4d \n",node_count,max_i,max_j,max_k);

  }

  broadcast(world,node_count,0); 
  broadcast(world,node_list,0);
  broadcast(world,max_i,0);
  broadcast(world,max_j,0);
  broadcast(world,max_k,0); 

  Individual temp(node_count,node_list,true); 
  Domain space(max_i,max_j,max_k);
  std::vector<int> topo = space.give_topo();

  temp.get_fitness(&topo);

  Population P(pop_size,node_count,&temp);
  P.set_elites(num_elites);

  std::vector<Individual> local_elites(num_elites,temp);
  std::vector<Individual> global_elites;

  P.tournament(&local_elites,&space);

  for(int i=0;i<max_gen;i++){
    P.tournament(&local_elites,&space); 
    all_gather(world,&local_elites[0],num_elites,global_elites); 
    P.populate_next_gen(&global_elites,&space); 
    printf("%d %f %f\n",i,local_elites[0].give_fitness(),local_elites[num_elites-1].give_fitness());
  }

  P.individuals[0].print_chromosome(&space);
  std::cout << P.individuals[0].give_fitness();

  return 0; 
}
