#include "individual.h" 
#include <cstdlib>      // std::rand, std::srand

int main(int argc, char*argv[]) {

const int count=51; 
nodeid nodes[count]; 

std::srand(24); 

for (int i=0; i<count; i++) nodes[i]=20+i; 

Individual* gold = new Individual(count, nodes, false); 
gold->show_Individual(); 

for (int i=0; i<25; i++) {
  Individual* indv1 = new Individual(*gold);
  indv1->mutate(); 
  indv1->show_Individual(); 
  }

}
