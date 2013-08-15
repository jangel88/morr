#include "individual.h"
#include <stdio.h>
#include <math.h>
#include <algorithm>    // std::random_shuffle
 
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/

extern Domain gampi_domain;
extern std::vector<nodeid> gampi_nodelist; 

/* ---------------------------------------------------------------------- */
Individual::Individual(int size, bool shuffle) {
  chromosome.resize(size);
  for(int i=0; i<size; i++) chromosome[i]=i; 
  if(shuffle) std::random_shuffle(chromosome.begin(),chromosome.end()); 
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
}

/* ---------------------------------------------------------------------- */
Individual::Individual(const Individual& parent, bool shuffle) {
  chromosome.resize(parent.chromosome.size()); 
  for(int i=0; i<chromosome.size(); i++) chromosome[i]=parent.chromosome[i]; 
  if(shuffle) {
    std::random_shuffle(chromosome.begin(),chromosome.end());
    fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
  } else {
    fitness=parent.fitness; 
  }
}

/* ---------------------------------------------------------------------- */
Individual::Individual(const std::vector<gene>& parent_chromosome){
  chromosome.resize(parent_chromosome.size()); 
  for(int i=0; i<chromosome.size(); i++) chromosome[i]=parent_chromosome[i]; 
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
}

/* ---------------------------------------------------------------------- */
void Individual::show() {
  printf("%x : %8.5f : ",hash(), fitness); 
  for(int i=0; i<chromosome.size(); i++) printf("%3d ", chromosome[i]);
  printf("\n");
}

/* ---------------------------------------------------------------------- */
size_t Individual::hash() {
  size_t results = 2166136261U; 
  for ( int i=0; i<chromosome.size(); i++) results = 127 * results + static_cast<size_t>(chromosome[i]);
  return results; 
}

/* ---------------------------------------------------------------------- */
void Individual::mutate() {
  float roll=1;
  while((float) rand()/RAND_MAX < roll){
    int r=rand()%3;
    int mirr1=rand()%2;
    int mirr2=rand()%2;
    if(r==0){
      cut_n_paste_segment(mirr1); 
    }else if(r==1){
      swap_segment(mirr1,mirr2);
    }else{
      head_to_tail(); 
    }
    roll *= (float) 4/5; 
  }
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
}

/* ---------------------------------------------------------------------- */
void Individual::swap_segment(bool mirror1, bool mirror2) {
  int length_range[10]={1,2,4,8,16,32,64,128,256,512};
  int length=length_range[rand()%5];
  int x=rand()%chromosome.size();
  int y=rand()%chromosome.size();
  while(x == y){ //unique segments
    x=rand()%chromosome.size();
  }
  int start2=MAX(x,y); 
  int start1=MIN(x,y); 

  while( (length > (chromosome.size()-start2-1)) || //make sure we arent overshooting 
         (start1+length > start2)){   //or overlapping
    length-=1;
  }
  std::vector<gene> temp(length); 

  if(mirror1){
    std::reverse_copy(chromosome.begin()+start2,chromosome.begin()+start2+length,temp.begin());
  }else{
    std::copy        (chromosome.begin()+start2,chromosome.begin()+start2+length,temp.begin());
  }

  if(mirror2){
    std::reverse_copy(chromosome.begin()+start1,chromosome.begin()+start1+length,chromosome.begin()+start2);
  }else{
    std::copy        (chromosome.begin()+start1,chromosome.begin()+start1+length,chromosome.begin()+start2);
  } 
  std::copy(temp.begin(),temp.end(),chromosome.begin()+start1);
}

/* ---------------------------------------------------------------------- */
void Individual::cut_n_paste_segment(bool mirr) {
  int length_range[10]={1,2,4,8,16,32,64,128,256,512};
  int length=length_range[rand()%5]; 
  int dst=rand()%chromosome.size();
  int src=rand()%chromosome.size();

  while((src+length) > chromosome.size() || //dont overshoot the end
        (dst+length) > chromosome.size()){
    length-=1;
  }

  std::vector<gene> temp(length); 
  if(mirr){
    std::reverse_copy(chromosome.begin() + src , chromosome.begin() + src + length,temp.begin() ); 
  }else{
    std::copy(chromosome.begin() + src , chromosome.begin() + src + length,temp.begin() ); 
  } 
  chromosome.erase(chromosome.begin() + src, chromosome.begin() + src + length);
  chromosome.insert(chromosome.begin() + dst, temp.begin(), temp.end()); 
}

/* ---------------------------------------------------------------------- */
void Individual::head_to_tail() { 
  int period=gampi_domain.get_period(); 
  int num_periods=chromosome.size()/period; 
  if(num_periods==1) return; //nothing to do
  int num_to_cut=(rand()%num_periods)*period;

  std::vector<gene> temp(num_to_cut); 
  std::copy(chromosome.begin(),chromosome.begin()+num_to_cut,temp.begin()); 
  chromosome.erase(chromosome.begin(),chromosome.begin()+num_to_cut); 
  chromosome.insert(chromosome.end(),temp.begin(),temp.end()); 
}

