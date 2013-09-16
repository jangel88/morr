#include "population.h"
#include <stdio.h>
#include <assert.h>
#include <algorithm>

/* ---------------------------------------------------------------------- */
Population::Population(int size) {
  assert(size>0);
  flock.resize(size);
}

/* ---------------------------------------------------------------------- */
Population::Population(const Individual& ancestor, int size) {
  assert(size>0);
  assert(ancestor.is_valid()); 
  flock.resize(size); 
  flock[0]=ancestor;
  for(int i=1; i<size; i++)
    flock[i]=Individual(ancestor, true);
}

/* ---------------------------------------------------------------------- */
Population::Population(const Population& ancestors, int size) {
  int ansize=ancestors.get_size();
  assert(size>=ansize);
  assert(ancestors.is_valid()); 
  flock.resize(size); 
  for(int i=0; i<ansize; i++)
    flock[i]=ancestors.flock[i];
  for(int i=ansize; i<size; ) {
    //Obtain 1/4th of the Individuals through crossover
    if(rand()%4==0 && ansize>1 && i<size-1) {
      int p1=rand()%ansize; 
      int p2;
      do {
        p2=rand()%ansize; 
      } while (p1==p2); //p1 and p2 cannot be the same for crossover
      Individual::crossover(ancestors.flock[p1], ancestors.flock[p2], flock[i], flock[i+1]); 
      //flock[i]=Individual(ancestors.flock[p1], true);
      //flock[i+1]=Individual(ancestors.flock[p2], true);
      i+=2; 
    } else {
      flock[i]=Individual(ancestors.flock[rand()%ansize], true);
      i++; 
    }
  }
}

/* ---------------------------------------------------------------------- */
Population& Population::operator += (const Population& a){
  for(int i=0; i<a.get_size(); i++){
    (*this)+=a.flock[i];
  }
  return *this;
}

/* ---------------------------------------------------------------------- */
const Population  Population::operator +  (const Population& a) const {
  return Population(*this) += a;
}

/* ---------------------------------------------------------------------- */
Population& Population::operator += (const Individual& a){
  bool present=false; 
  for(int i=0; i<get_size(); i++){
    //Check if this individual's hash is already in flock
    if(flock[i].get_hash()==a.get_hash()) present=true; 
    if(present) break; //out of i loop
  }
  if(!present) flock.insert(flock.end(), a);
  return *this;
}

/* ---------------------------------------------------------------------- */
const Population  Population::operator +  (const Individual& a) const {
  return Population(*this) += a;
}

/* ---------------------------------------------------------------------- */
Population Population::get_random_subset(int count) {
  assert(count>0);
  Population p(count);
  for(int i=0; i<count; i++)
    p.flock[i]=this->flock[rand()%this->get_size()];
  return p;
}

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
typedef std::pair<int,float> indexfitness_pair; //index and fitness pair for efficient sorting
static bool comparator (const indexfitness_pair& a, const indexfitness_pair& b){
  return a.second < b.second;
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
Population Population::get_unique_elites(int count){
  assert(count>0);
  int size=this->get_size();

  std::vector<indexfitness_pair> indfit(size);
  for(int i=0; i<size; i++) 
    indfit[i]=std::make_pair(i, flock[i].get_fitness());
  std::sort(indfit.begin(), indfit.end(), comparator);

  Population e(flock[indfit[0].first],1); 
  for(int i=1; i<size; i++){
    if(e.get_size()>=count) break; //out of i loop
    e+=flock[indfit[i].first]; 
  }
  return e;
}

/* ---------------------------------------------------------------------- */
float Population::get_best_fitness() {
  int size=this->get_size();

  std::vector<float> fit(size);
  for(int i=0; i<size; i++) { fit[i]=flock[i].get_fitness(); }
  return *(std::min_element(fit.begin(), fit.end())); 
}

/* ---------------------------------------------------------------------- */
Individual Population::get_individual(int i) {
  assert(i>=0); 
  assert(i<this->get_size()); 
  return flock[i];
}

/* ---------------------------------------------------------------------- */
bool Population::is_valid() const {
  int size=get_size();
  assert(size>=0);
  for(int i=0; i<size; i++) {
    if(!flock[i].is_valid()) return false; 
  }
  return true; 
}

/* ---------------------------------------------------------------------- */
void Population::show() {
  for(int i=0; i<this->get_size(); i++)
    printf("%x:%-8.3f ", flock[i].get_hash(),flock[i].get_fitness());
  printf("\n");
}

/* ---------------------------------------------------------------------- */
void Population::show(char* s) {
  printf("%s :", s);
  this->show();
}
