#include "individual.h" 
#include <cstdlib>      // std::rand, std::srand

int main(int argc, char*argv[]) {

  const int count=27; 
  std::vector<nodeid> nodes(count); 
  
  std::srand(24); 
  
  Domain space(3,3,3);  
  
  for (int i=0; i<count; i++) nodes[i]=i; 
  
  Individual* gold = new Individual(count, nodes, false); 
  gold->show_Individual(); 
  
  for (int i=0; i<25; i++) {
    Individual* indv1 = new Individual(*gold);
    indv1->mutate(&space);
    indv1->show_Individual(); 
  }

}
