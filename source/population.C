#include"population.h"

#include<float.h>
bool comparator (const fit_pair& i, const fit_pair& j)
{ 
  return i.first < j.first; 
}

Population::Population(int max_psize, int individual_size, Individual* ancestor)
{
  this->p_size=0;
  this->max_psize=max_psize; 
  while(p_size<max_psize){
    Individual temp(ancestor,false);
    individuals.push_back(temp);
    p_size+=1;  
  }
}

void Population::show_population()
{
  for(int i=0;i<p_size;++i){
    individuals[i].show_Individual(); 
  }
}

void Population::tournament(std::vector<Individual> elites, Domain* space)
{
  std::vector<fit_pair> ind_fit;
  float tmp_fit; 
  std::vector<int> topo=space->give_topo();
  for(int i=0; i<p_size;i++){
    tmp_fit=(float)individuals[i].get_fitness(&topo);
    ind_fit.push_back(std::make_pair(tmp_fit,i));
  }

  std::sort(ind_fit.begin(),ind_fit.end(),comparator);

  for(int i=0; i<elites.size();i++){
    elites[i]=individuals[ind_fit[i].second];
  }  
      
  for(int i=0;i<p_size;i++){
    if(i<elites.size()){
      individuals[i]=elites[i];
    }else{ 
      int elite_n=(int)(((float)rand()/RAND_MAX)*elites.size());
      Individual mutant(elites[elite_n]);
      mutant.mutate(space);
      individuals[i]=mutant;
    }
  }
}

Population Population::find_elites(int elite_size)
{

}

Individual Population::get_best_map()
{
  float best=FLT_MAX;
  Individual *best_map = (Individual*) ::operator new (sizeof(Individual));
  for(int i = 0; i < p_size; i++){
    if(individuals[i].give_fitness() < best)
      best_map = &individuals[i];
  }
  return *best_map; 
}

float Population::get_best_fitness()
{
  float best=FLT_MAX;
  for(int i=0;i<p_size;i++){
    if (individuals[i].give_fitness()< best)
      best=individuals[i].give_fitness();
  }
  std::cout << best << std::endl; 
  return best;
}
