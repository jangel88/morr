#include"population.h"
#include"nodes.h"
int main(int argc, char **argv)
{
  int max_gen=2000;

  Individual temp(96, nodes,true); 
  Population P(1000,96,&temp);
//P.show_population(); 
  
  Domain space(24,4,1);
  std::vector<Individual> elites(2,temp);
  P.tournament(elites,&space);
  P.get_best_fitness();
 
  for(int i=0;i<max_gen;i++){
    P.tournament(elites,&space);
    P.get_best_fitness();
  }
 
  return 0;
}
