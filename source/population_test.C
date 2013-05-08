#include"population.h"
#include"nodes.h"
#include<tr1/functional>
int main(int argc, char **argv)
{
  int max_gen=1000;

  Individual temp(96, nodes_96,true); 
  Population P(500,96,&temp);
//P.show_population(); 
  
  Domain space(24,4,1);
  std::vector<Individual> elites(4,temp);

  std::srand(10);
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
//Individual gold(48,nodes_48,false);
//gold.show_Individual();
//Population Q(1,48,&gold);
//
//std::vector<Individual> elite(1,gold);
//Q.tournament(elite,&space);
//Q.get_best_fitness();
  return 0;
}
