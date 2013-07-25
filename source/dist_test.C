#include"population.h" 
#include<stdio.h>
#include<stdlib.h>
#include<boost/mpi.hpp>
#include<boost/serialization/vector.hpp>


std::vector<nodeid> get_nodes(FILE *node_file,std::vector<nodeid> node_list,int *proc_count,int *max_i,int *max_j,int *max_k)
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
        sscanf(string,"%d%d%d%d",proc_count,max_i,max_j,max_k); 
      //node_list = (nodeid*) malloc(sizeof(nodeid)*(*proc_count)); 
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
    printf("./dist_test N max_gen max_i max_j max_k\n");
    printf("N - population size\n");
    printf("max_gen - population size\n"); 
    printf("max_i - size of i dimension\n");
    printf("max_j - size of j dimension\n");
    printf("max_k - size of k dimension\n");
    printf("where simulation elements are referenced\n");
    printf("as (i,j,k)\n");
    exit(1);
  }
  
  mpi::environment env(argc, argv);
  mpi::communicator world; 

  int pop_size=(int) atoi(argv[1]);
  int max_gen=(int) atoi(argv[2]);
  int num_elites=0.1*pop_size;
//int num_elites=5;



  int proc_count=world.size();
 
  int max_i = atoi(argv[3]);
  int max_j = atoi(argv[4]);
  int max_k = atoi(argv[5]);

  std::vector<nodeid> node_list;
  nodeid nid=query_nodeid();
  all_gather(world,nid,node_list);

  srand(133*world.rank());

  Individual temp(proc_count,node_list,false); 
  Domain space(max_i,max_j,max_k);


  std::vector<int> topo = space.give_topo();
  temp.get_fitness(&topo);

  Population P(pop_size,proc_count,&temp);
  P.set_elites(num_elites);

  std::vector<Individual> local_elites(num_elites,temp);
  std::vector<Individual> global_elites;

  P.tournament(&local_elites,&space);
  if(world.rank()<16){
    for(int i=0;i<max_gen;i++){
      P.tournament(&local_elites,&space); 
      all_gather(world,&local_elites[0],1,global_elites);
      P.populate_next_gen(&global_elites,&space); 
//    P.populate_next_gen(&local_elites,&space); 
    }
  }



  if(world.rank()==0){
    P.individuals[0].print_chromosome(&space,0);
  }
  return 0;
}
