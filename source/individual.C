#include "individual.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <algorithm>    // std::random_shuffle
 
extern Domain gampi_domain;
extern std::vector<nodeid> gampi_nodelist; 

/* ---------------------------------------------------------------------- */
Individual::Individual(int size, bool shuffle) {
  assert(gampi_domain.get_size()==size);
  chromosome.resize(size);
  for(int i=0; i<size; i++) chromosome[i]=i; 
  if(shuffle) std::random_shuffle(chromosome.begin(),chromosome.end()); 
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
  hash=ring_fnv_1a(); 
}

/* ---------------------------------------------------------------------- */
Individual::Individual(const Individual& parent, bool mutate) {
  chromosome=parent.chromosome;
  fitness=parent.fitness; 
  hash=parent.hash; 
  if(mutate) this->mutate(); 
}

/* ---------------------------------------------------------------------- */
Individual::Individual(const std::vector<gene>& parent_chromosome){
  chromosome.resize(parent_chromosome.size()); 
  for(int i=0; i<chromosome.size(); i++) chromosome[i]=parent_chromosome[i]; 
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
  hash=ring_fnv_1a(); 
}

/* ---------------------------------------------------------------------- */
bool Individual::is_valid() {

  assert(gampi_domain.get_size()==chromosome.size()); 
  assert(fitness==gampi_domain.get_fitness(gampi_nodelist, chromosome)); 
  assert(hash==ring_fnv_1a()); 

  Individual a(this->chromosome); 
  std::sort(a.chromosome.begin(), a.chromosome.end()); 
  Individual b(chromosome.size()); 
  return a==b; 
}

/* ---------------------------------------------------------------------- */
void Individual::show() {
  printf("%x : %8.5f : ",hash, fitness); 
  for(int i=0; i<chromosome.size(); i++) printf("%3d ", chromosome[i]);
  printf("\n");
}

/* ---------------------------------------------------------------------- */
void Individual::show(char* s){
  printf("%s :", s); 
  this->show(); 
}

/* ---------------------------------------------------------------------- */
uint32_t Individual::fnv_1a(uint8_t *bp, uint8_t *be){ //begin point and beyond end
  const uint32_t fnv_prime = 16777619u;
  const uint32_t fnv_offset_basis = 2166136261u;

  uint32_t hval = fnv_offset_basis; 
  while(bp<be) {
    hval ^= (uint32_t)*bp++; 
    hval *= fnv_prime; 
  }
  return hval; 
}

/* ---------------------------------------------------------------------- */
uint32_t Individual::ring_fnv_1a() {
  int period=gampi_domain.get_period(); 
  int num_periods=chromosome.size()/period; 
  uint8_t *bp, *be; 

  if(num_periods==1) {
    bp = (uint8_t *) &chromosome[0]; //begin point
    be = bp + chromosome.size()*sizeof(gene); //beyond end
    return fnv_1a(bp, be); 
  } else {
    std::vector<uint32_t> hval; 
    hval.resize(num_periods); 
    for(int i=0; i<num_periods; i++){
      bp = (uint8_t *) &chromosome[i*period]; //begin point
      be = bp + period*sizeof(gene); //beyond end
      hval[i]=fnv_1a(bp, be); 
    }
    std::vector<uint32_t>::iterator minloc=std::min_element(hval.begin(), hval.end()); 
    std::vector<uint32_t> temp(hval.begin(), minloc);
    hval.erase(hval.begin(),minloc); 
    hval.insert(hval.end(),temp.begin(),temp.end()); 

    assert(hval.size()==num_periods); 
    bp = (uint8_t *) &hval[0]; //begin point
    be = bp + num_periods*sizeof(uint32_t); //beyond end
    return fnv_1a(bp, be); 
  }
}

/* ---------------------------------------------------------------------- */
int Individual::log2(int n){
  int l=-1; 
  while(n!=0){
    l++; 
    n>>=1; 
  }
  return l; 
}

/* ---------------------------------------------------------------------- */
void Individual::mutate() {
  //float roll=1;
  //while((float) rand()/RAND_MAX < roll){
    if(rand()%2){
      cut_n_paste_segment();
    } else {  
      swap_segment(); 
    }
  //  roll *= (float) 4/5; 
  //}
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
  hash=ring_fnv_1a(); 
  assert(this->is_valid()); 
}

/* ---------------------------------------------------------------------- */
void Individual::swap_segment() {
  int N=chromosome.size(); 
  int l=log2(N); 
  assert(l>1); //Not programmed for trivial cases
  
  int x=rand()%N;
  int y=rand()%N;
  while(x == y) x=rand()%N; //two locations should not coincide
  int pos1=std::min(x,y);
  int pos2=std::max(x,y); 

  int len0= 1<<(rand()%l);  //length is a power of 2 in the range 1 to 2^(l-1)
  int len1=   pos2-pos1;
  int len2= N+pos1-pos2;
  int length=std::min(len0, std::min(len1, len2)); 

  std::vector<gene> twice(2*N,-1); 
  std::copy(chromosome.begin(), chromosome.end(), twice.begin()  ); 
  std::copy(chromosome.begin(), chromosome.end(), twice.begin()+N); 

  std::vector<gene> segment1(twice.begin()+pos1, twice.begin()+pos1+length); 
  std::vector<gene> segment2(twice.begin()+pos2, twice.begin()+pos2+length); 

  // Reverse the segments half the time
  if(rand()%2) std::reverse(segment1.begin(), segment1.end()); 
  if(rand()%2) std::reverse(segment2.begin(), segment2.end()); 

  std::copy(segment2.begin(), segment2.end(), chromosome.begin()+pos1); 
  if(pos2+length<N) { // does not wrap around
    std::copy(segment1.begin(), segment1.end(), chromosome.begin()+pos2); 
  } else { 
    int posw=N-pos2; 
    std::copy(segment1.begin(), segment1.begin()+posw, chromosome.begin()+pos2); 
    std::copy(segment1.begin()+posw, segment1.end(), chromosome.begin()); 
  }
}

/* ---------------------------------------------------------------------- */
void Individual::cut_n_paste_segment() {
  int N=chromosome.size(); 
  int l=log2(N); 
  assert(l>1); //Not programmed for trivial cases
  
  int src=rand()%N;
  int length= 1<<(rand()%l);  //length is a power of 2 in the range 1 to 2^(l-1)

  std::vector<gene> twice(2*N,-1); 
  std::copy(chromosome.begin(), chromosome.end(), twice.begin()  ); 
  std::copy(chromosome.begin(), chromosome.end(), twice.begin()+N); 

  std::vector<gene> segment(twice.begin()+src, twice.begin()+src+length); 

  // Reverse the segment half the time
  if(rand()%2) std::reverse(segment.begin(), segment.end()); 

  // Erase the segment from source
  if(src+length<N){
    chromosome.erase(chromosome.begin()+src, chromosome.begin()+src+length); 
  } else { 
    int posw=N-src;
    chromosome.erase(chromosome.begin()+src, chromosome.end()); 
    chromosome.erase(chromosome.begin(), chromosome.begin()+length-posw);
  }
  assert(chromosome.size()==N-length); 

  int dst=rand()%(N-length); //Calculate destination based on new length
  chromosome.insert(chromosome.begin()+dst, segment.begin(), segment.end()); 
}

/* ---------------------------------------------------------------------- */
float Individual::operator - (const Individual& a) const {
  assert(a.chromosome.size()==chromosome.size());  
  float distance=0; 
  for(int i=0; i<chromosome.size(); i++){
    float d=a.chromosome[i]-chromosome[i]; 
    distance+=d*d; //L2 euclidean norm
  }
  return distance; 
}

/* ---------------------------------------------------------------------- */
bool Individual::operator == (const Individual& a) const {
  assert(chromosome.size()==a.chromosome.size());  
  return std::equal(chromosome.begin(), chromosome.end(), a.chromosome.begin()); 
}

