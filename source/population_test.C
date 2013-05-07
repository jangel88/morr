#include"population.h"
#include"nodes.h"

int main(int argc, char **argv)
{
  int max_gen=500;

  Individual temp(48, nodes_48,true); 
  Population P(500,48,&temp);
//P.show_population(); 
  
  Domain space(12,4,1);
  std::vector<Individual> elites(2,temp);


  P.tournament(elites,&space);
  P.get_best_fitness();

 for(int i=0;i<max_gen;i++){
    P.tournament(elites,&space);
//  P.get_best_fitness();
  }

  Individual best_map = P.get_best_map();
  best_map.show_zcoors(&space); 
  P.get_best_fitness();
  best_map.show_Individual();
  return 0;
}
