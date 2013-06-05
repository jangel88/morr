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

void Population::tournament(std::vector<Individual> elites, Domain* space)
{
  std::vector<fit_pair> ind_fit;
  float tmp_fit; 
  std::vector<int> topo=space->give_topo();
  std::vector<bool> is_hashed(199,0);
  int i=0;
  int elite_count=0;
  // Create pairs of fitness and indices
  for(int i=0; i<p_size;i++){
    tmp_fit=(float)individuals[i].get_fitness(&topo);
    ind_fit.push_back(std::make_pair(tmp_fit,i));
  }
  // Sort based on fitness
  std::sort(ind_fit.begin(),ind_fit.end(),comparator);
  // Get elites.size() unique elites for next generation
  while(elite_count<elites.size()){ 
    std::vector<nodeid>::iterator begin=individuals[ind_fit[i].second].begin();
    std::vector<nodeid>::iterator end=individuals[ind_fit[i].second].end();
    int key =(int) individuals[ind_fit[i].second].hash(begin,end)%199;
    // key might be negative. This is a bad work around.
    key=abs(key); 
    
    if(!is_hashed[key]){
      elites[elite_count]=individuals[ind_fit[i].second];
      is_hashed[key]=true;
      elite_count++; 
    } 
    i++; 
  } 
  for(int i=0;i<p_size;i++){
  // Copy elites into next gen
    if(i<elites.size()){
      individuals[i]=elites[i];
    }else{ 
  // Fill remaining pop with mutants
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
