#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <boost/mpi.hpp>

#include "population.h"

Domain gampi_domain(1,1,1); 
std::vector<nodeid> gampi_nodelist;

namespace mpi = boost::mpi;

int main(int argc, char **argv){//MAIN MAIN MAIN
mpi::environment env(argc, argv);
mpi::communicator world;
std::srand(135+2*world.rank()); 

int nodecount, max_i, max_j, max_k;
if(world.rank()==0) {
  std::cout << "usage:  ./population_test.exe testfilename\n";
  FILE* nodefile = fopen(argv[1], "r"); 
  if (nodefile==NULL) {
    std::cout << "error opening file "<<argv[1] << "\n"; 
    return 1; 
  }

  char oneline[100];
  int line_count=0;
  while( fgets(oneline,100,nodefile)!=NULL){
    if(oneline[0]=='#' || oneline[0]=='\n'){
      std::cout << oneline; 
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
  a.show((char*)"Ideal soln"); 
}

Individual v(true);
Population a(v, 1); //ancestor population
Population e(v, 1); //store the elites

time_t start=time(NULL); 
time_t lastprint=time(NULL);
float currbest=v.get_fitness(); 
std::pair<float,int> fitrank, bestfitrank; 
int elapsed=0; 

const int pop_size=100000; 
const float maxruntime=300.0; // seconds
const float maxgentime=maxruntime/30.0; // seconds, max time for creating one generation

do {
  const int subset_percent=5; //percentage of population that makes the subset for next gen
  float lastbest=currbest;

  Population p(a, pop_size, maxgentime); //population expanded from ancestors

  //Select a subset to be used for creating next generation
  e=p.elitist_selection(std::max(1, p.get_size()*subset_percent/100/10)); 
  currbest=e.get_best_fitness(); 

  if(currbest>lastbest*0.99) {
    a=p.rank_selection   (std::max(1, p.get_size()*subset_percent/100)); 
  } else {
    a=p.elitist_selection(std::max(1, p.get_size()*subset_percent/100)); 
  }

  //Add someone else's elites to mine
  if(world.size()>1){
    int comm_offset;
    if(world.rank()==0) comm_offset=1+(rand()%(world.size()-1)); // goes from 1 to N-1
    broadcast(world, comm_offset, 0); 
    int src_rank=(world.size()+world.rank()+comm_offset)%world.size(); // I receive from
    int dst_rank=(world.size()+world.rank()-comm_offset)%world.size(); // I send to 

    mpi::request reqs[2];
    Population r(v, 1);
    reqs[0]=world.isend(dst_rank, 307, e); 
    reqs[1]=world.irecv(src_rank, 307, r); 
    mpi::wait_all(reqs, reqs+2);  
    a+=r; 
  }

  time_t current=time(NULL); 
  elapsed=difftime(current, start); 
  broadcast(world, elapsed, 0); //So they all stop at the same iteration

  fitrank=std::make_pair(currbest, world.rank()); 
  all_reduce(world, fitrank, bestfitrank, mpi::minimum<std::pair<float,int> >()); 

  if (world.rank()==0 && difftime(current, lastprint)>=1) {
      printf("%3d (secs): %8.5f\n", elapsed, bestfitrank.first);
      lastprint=time(NULL); 
  }
} while(elapsed<maxruntime); 

Individual best; 
if(bestfitrank.second==0 && world.rank()==0) {
  best=e.get_individual(0); 
  best.show((char*) "Best soln"); 
} else if(world.rank()==bestfitrank.second) {
  best=e.get_individual(0); 
  world.send(0, 816, best); 
} else if (world.rank()==0) {
  world.recv(bestfitrank.second, 816, best); 
  best.show((char*) "Best soln"); 
}

} // end MAIN MAIN MAIN
