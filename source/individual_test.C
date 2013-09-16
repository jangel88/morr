#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<boost/mpi.hpp>

#include "individual.h"

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
if(world.rank()==0) a.show(); 

Individual b(true); 
if(world.rank()==0) b.show(); 
if(world.rank()==0) b.mutate(); 
if(world.rank()==0) b.show(); 
broadcast(world, b, 0); 

char s[16];
sprintf(s,"bcst %d",world.rank()); 
b.show(s); 

Individual c(b, true); 
sprintf(s,"mutt %d",world.rank()); 
c.show(s); 

if(world.rank()==0) {
cout << "Begin crossover test \n"; 

Individual parent1(true); 
parent1.show((char*) "parent1"); 

Individual parent2(true); 
parent2.show((char*) "parent2"); 

Individual pt2mind=parent2; 
pt2mind.mindiff(parent1); 
pt2mind.show((char*) "pt2mind"); 

Individual child1, child2;
Individual::crossover(parent1, parent2, child1, child2); 
child1.show((char*) "child1 "); 
child2.show((char*) "child2 "); 
}

} // end main

