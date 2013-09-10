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
  cout << "usage:  ./individual_test.exe testfilename\n";
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

Individual a;
a.show((char*)"Ideal soln:"); 
Individual b(true);
Population e(b, 1); 

time_t start=time(NULL); 
int elapsed=0; 

do {
  Population p(e, 10000); 
  e = p.get_unique_elites(0.01*p.get_size()); 

  time_t current=time(NULL); 
  elapsed=difftime(current, start); 
  if(world.rank()==0) 
    printf("%3d (secs): %8.4f %8x %8.4f %8x\n", elapsed, 
      e.get_individual(0).get_fitness(), e.get_individual(0).get_hash(),
      e.get_individual(e.get_size()-1).get_fitness(), e.get_individual(e.get_size()-1).get_hash()
      ); 
} while(elapsed<600.0);

printf("RSA printing the elites\n"); 
for(int i=0; i<e.get_size(); i++){
  Individual p = e.get_individual(i); 
  p.show(); 
}


} // end main
