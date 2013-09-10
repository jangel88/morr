#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<boost/mpi.hpp>

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
Individual b(true); 

Population p(a, 10); 
if(world.rank()==0) p.show((char*)"ordrd1"); 
Population q(a, 10); 
if(world.rank()==0) q.show((char*)"ordrd2"); 
Population r(b, 10); 
if(world.rank()==0) r.show((char*)"shffld"); 

q=q+p; 
r+=q; 
if(world.rank()==0) r.show((char*)"additn"); 

Population s=r.get_unique_elites(5); 
if(world.rank()==0) s.show((char*)"elites"); 

} // end main
