#include"population.h"
#include"nodes.h"

int main(int argc, char **argv)
{
  int max_gen=500;

  Individual temp(4, nodes_48,true); 
  Population P(500,4,&temp);
//P.show_population(); 
  
  Domain space(2,2,1);
  std::vector<Individual> elites(2,temp);


//P.tournament(elites,&space);
//P.get_best_fitness();

//or(int i=0;i<max_gen;i++){
//  P.tournament(elites,&space);
//  P.get_best_fitness();
//}

//Individual best_map = P.get_best_map();
//best_map.show_zcoors(&space); 
//P.get_best_fitness();
//best_map.show_Individual();
  Individual gold(4,nodes_48,false);
  gold.show_Individual();
  Population Q(1,4,&gold);
  
  std::vector<Individual> elite(1,gold);
  Q.tournament(elite,&space);
  Q.get_best_fitness();
  return 0;
}
