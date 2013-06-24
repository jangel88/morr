#include"population.h"
#include<float.h>
#include<cstdlib>
#include<algorithm>

bool comparator (const fit_pair& i, const fit_pair& j)
{ 
  return i.first < j.first; 
}

Population::Population(int max_psize, int individual_size, Individual* ancestor)
{
  this->p_size=0;
  this->max_psize=max_psize; 
  while(p_size<max_psize){
    Individual temp(ancestor,true);
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

void Population::tournament(std::vector<Individual>* elites, Domain* space)
{
  std::vector<fit_pair> ind_fit;
  float tmp_fit; 
  std::vector<int> topo=space->give_topo();

  int i;
  int elite_count=0;
  // Create pairs of fitness and indices
  for(int i=0; i<p_size;i++){
    tmp_fit=(float)individuals[i].get_fitness(&topo);
    ind_fit.push_back(std::make_pair(tmp_fit,i));
  }
  // Sort based on fitness
  std::sort(ind_fit.begin(),ind_fit.end(),comparator);

  for(int i=0, k=0; i<p_size; i++){
    if(k==elites->size()) break; 
    size_t this_hash = individuals[ind_fit[i].second].hash();
    bool repeat=false; 
    for(int j=0; j<k; j++){
      if(this_hash == elites->at(j).hash()) repeat=true; 
    }
    if(repeat == false){
      elites->at(k)=individuals[ind_fit[i].second];
      k++; 
    }
  }


}

void Population::populate_next_gen(std::vector<Individual>* elites, Domain* space)
{
    
  for(int i=0;i<p_size;i++){
  // Copy elites into next gen 
    if(i<num_elites){
      individuals[i]=elites->at(i); 
    }else{ 
  // Fill remaining individuals with mutants
      int elite_n = rand() % elites->size(); 
      Individual mutant(elites->at(elite_n));
      mutant.mutate(space);
      individuals[i]=mutant;
    }
  } 

}


Population Population::find_elites(int elite_size)
{

}

Individual Population::get_best_map(std::vector<int>* topology)
{
  float tmp,best=FLT_MAX;

  Individual *best_map = (Individual*) ::operator new (sizeof(Individual));
  for(int i = 0; i < p_size; i++){ 
    tmp=individuals[i].get_fitness(topology); 
    if(tmp < best){
      best=tmp; 
      best_map = &individuals[i]; 
    }
  }
  return *best_map; 
}

float Population::get_best_fitness()
{
  float best=FLT_MAX;
  for(int i=0;i<p_size;i++){
    if (individuals[i].give_fitness() < best)
      best=individuals[i].give_fitness();
  }
  std::cout << best << std::endl; 
  return best;
}
