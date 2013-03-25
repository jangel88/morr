#include"population.h"

bool comparator (const fit_pair& i, const fit_pair& j)
{ 
  return i.first < j.first; 
}

Population::Population(int max_psize, int individual_size, Individual* ancestor)
{
//set_current_psize(1);
//set_max_psize(max_psize);
  this->p_size=0;
  this->max_psize=max_psize; 
  while(p_size<max_psize){
    Individual temp(ancestor,true);
    std::random_shuffle(temp.begin(),temp.end());
    individuals.push_back(temp);
    p_size+=1;  
  }
}

void Population::set_max_psize(int max_psize)
{
  max_psize=max_psize;
}

void Population::set_current_psize(int p_size)
{
  p_size=p_size;
}

void Population::show_population()
{
  for(int i=0;i<p_size;++i){
    individuals[i].show_Individual();
    std::cout << i << std::endl;
  }
}

void Population::tournament(std::vector<Individual> elites, std::vector<int>* topology)
{
  std::vector<fit_pair> ind_fit;
  float tmp_fit; 

  for(int i=0; i<p_size;i++){
    tmp_fit=(float)individuals[i].get_fitness(topology);
    ind_fit.push_back(std::make_pair(tmp_fit,i));
  }

  std::sort(ind_fit.begin(),ind_fit.end(),comparator);

  for(int i=0; i<elites.size();i++){
    elites[i]=(individuals[ind_fit[i].second]);
  }  
      
  for(int i=0;i<p_size;i++){
    if(i<elites.size()){
      individuals[i]=elites[i];
    }else{ 
      int elite_n=(int)(((float)rand()/RAND_MAX)*elites.size());
      Individual mutant(elites[elite_n]);
      mutant.mutate();
      individuals[i]=mutant;
    }
  }
}

Population Population::find_elites(int elite_size)
{

}

int Population::get_best_fitness()
{
  float best=1e6;
  for(int i=0;i<p_size;i++){
    if (individuals[i].give_fitness()< best)
      best=individuals[i].give_fitness();
  }
  std::cout << best << std::endl; 
}
