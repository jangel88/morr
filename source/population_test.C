#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <boost/mpi.hpp>

#include "population.h"

Domain gampi_domain(1,1,1); 
std::vector<nodeid> gampi_nodelist;

using namespace std; 
namespace mpi = boost::mpi;

int main(int argc, char **argv){

mpi::environment env(argc, argv);
mpi::communicator world;
std::srand(135+2*world.rank()); 

int nodecount, max_i, max_j, max_k;
if(world.rank()==0) {
  cout << "usage:  ./population_test.exe testfilename\n";
  FILE* nodefile = fopen(argv[1], "r"); 
  if (nodefile==NULL) {
    cout << "error opening file "<<argv[1] << "\n"; 
    return 1; 
  }

  char oneline[100];
  int line_count=0;
  while( fgets(oneline,100,nodefile)!=NULL){
    if(oneline[0]=='#' || oneline[0]=='\n'){
      cout << oneline; 
      continue;
    }else{
      line_count+=1;
      if(line_count==1){ 
        sscanf(oneline,"%d%d%d%d",&nodecount,&max_i,&max_j,&max_k); 
      }else{
        gampi_nodelist.push_back((nodeid) atoi(oneline));
      } 
    }
  } 
}

broadcast(world, max_i, 0);
broadcast(world, max_j, 0);
broadcast(world, max_k, 0);
broadcast(world, gampi_nodelist, 0);

gampi_domain=Domain(max_i, max_j, max_k); 

if(world.rank()==0){
  Individual a;
  a.show((char*)"Ideal soln:"); 
}

Individual v(true);
Population a(v, 1); //ancestor population

time_t start=time(NULL); 
time_t lastprint=time(NULL);
int elapsed=0; 

do {
  const int subset_percent=5; //percentage of population that makes the subset for next gen
  Population p(a, 100000); //population expanded from ancestors

  //Select a subset to be used for creating next generation
  //a = p.elitist_selection(p.get_size()*1/100); 
  a = p.rank_selection(p.get_size()*subset_percent/100); 

  //Add someone else's elites to mine
  if(world.size()>1){
    int comm_offset;
    if(world.rank()==0) comm_offset=1+(rand()%(world.size()-1)); // goes from 1 to N-1
    broadcast(world, comm_offset, 0); 
    int src_rank=(world.size()+world.rank()+comm_offset)%world.size(); // I receive from
    int dst_rank=(world.size()+world.rank()-comm_offset)%world.size(); // I send to 

    mpi::request reqs[2];
    Population s=p.elitist_selection(p.get_size()*subset_percent/100/10); 
    Population r(v, 1);
    reqs[0]=world.isend(dst_rank, 307, s); 
    reqs[1]=world.irecv(src_rank, 307, r); 
    mpi::wait_all(reqs, reqs+2);  
    a+=r; 
  }

  time_t current=time(NULL); 
  elapsed=difftime(current, start); 
  broadcast(world, elapsed, 0); //So they all stop at the same iteration

  float my_best=a.get_best_fitness(); 
  float world_best;
  reduce(world, my_best, world_best, mpi::minimum<float>(), 0);
  int sinceprint=difftime(current, lastprint); 
  if(world.rank()==0) {
    if(sinceprint>=1){
      printf("%3d (secs): %8.4f\n", elapsed, world_best);
      lastprint=time(NULL); 
    }
  }
} while(elapsed<90); 

//printf("RSA printing the elites\n"); 
//if(world.rank()==0) {
//  for(int i=0; i<e.get_size(); i++){
//    Individual p = e.get_individual(i); 
//    p.show(); 
//  }
//}

} // end main
