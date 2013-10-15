#include "individual.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <algorithm>    // std::random_shuffle
 
extern Domain gampi_domain;
extern std::vector<nodeid> gampi_nodelist; 

/* ---------------------------------------------------------------------- */
Individual::Individual(bool shuffle) {
  int size=gampi_domain.get_size(); 
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
  assert(parent_chromosome.size()==gampi_domain.get_size()); 
  chromosome.resize(parent_chromosome.size()); 
  for(int i=0; i<chromosome.size(); i++) chromosome[i]=parent_chromosome[i]; 
  fitness=gampi_domain.get_fitness(gampi_nodelist, chromosome); 
  hash=ring_fnv_1a(); 
}

/* ---------------------------------------------------------------------- */
bool Individual::is_valid() const {
  assert(gampi_domain.get_size()==chromosome.size()); 
  float f=gampi_domain.get_fitness(gampi_nodelist, chromosome);
  assert(abs(f-fitness)<f*1e-6); //Allow for roundoff errors
  assert(hash==ring_fnv_1a()); 

  Individual a(this->chromosome); 
  std::sort(a.chromosome.begin(), a.chromosome.end()); 
  Individual b;//unshuffled
  return a==b; 
}

/* ---------------------------------------------------------------------- */
void Individual::show() {
  printf("%08x : %8.5f : ",hash, fitness); 
  for(int i=0; i<chromosome.size(); i++) printf("%3d ", chromosome[i]);
  printf("\n");
}

/* ---------------------------------------------------------------------- */
void Individual::show(char* s){
  printf("%s :", s); 
  this->show(); 
}

/* ---------------------------------------------------------------------- */
nodeid Individual::reordered_nodeid_at(int n) const {
  assert(n>=0 && n<chromosome.size()); 
  assert(gampi_domain.get_size()==chromosome.size()); 
  return gampi_nodelist.at(chromosome.at(n)); 
}

/* ---------------------------------------------------------------------- */
uint32_t Individual::fnv_1a(uint8_t *bp, uint8_t *be) const{ //begin point and beyond end
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
uint32_t Individual::ring_fnv_1a() const {
  int period=gampi_domain.get_period(); 
  int num_periods=chromosome.size()/period; 
  uint8_t *bp, *be; 

  if(num_periods==1) {
    bp = (uint8_t *) &chromosome[0]; //begin point
    be = bp + chromosome.size()*sizeof(gene); //beyond end
    return fnv_1a(bp, be); 
  } else {
    std::vector<uint32_t> hval(num_periods); 
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
  float roll=1;
  while((float) rand()/RAND_MAX < roll){
    if(rand()%2){
      cut_n_paste_segment();
    } else {  
      swap_segment(); 
    }
    roll *= (float) 4/5; 
  }
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
int Individual::operator - (const Individual& a) const {
  assert(a.chromosome.size()==chromosome.size());  
  int different=0; 
  for(int i=0; i<chromosome.size(); i++){
    int d=(a.chromosome[i]!=chromosome[i]); //1 if unequal
    different+=d;
  }
  return different;
}

/* ---------------------------------------------------------------------- */
bool Individual::operator == (const Individual& a) const {
  assert(chromosome.size()==a.chromosome.size());  
  return std::equal(chromosome.begin(), chromosome.end(), a.chromosome.begin()); 
}

/* ---------------------------------------------------------------------- */
Individual& Individual::operator <<= (int n) {
  assert(n>=0); 
  n=n%chromosome.size(); 
  std::vector<gene> segment(n); 
  std::copy(chromosome.begin(),chromosome.begin()+n,segment.begin());
  chromosome.erase(chromosome.begin(),chromosome.begin()+n);
  chromosome.insert(chromosome.end(),segment.begin(),segment.end());
  return *this;
}

/* ---------------------------------------------------------------------- */
const Individual  Individual::operator <<  (int n) const {
  return Individual(*this) <<= n;
}

/* ---------------------------------------------------------------------- */
Individual& Individual::operator >>= (int n) {
  assert(n>=0); 
  n=n%chromosome.size(); 
  std::vector<gene> segment(n); 
  std::copy(chromosome.end()-n,chromosome.end(),segment.begin());
  chromosome.erase(chromosome.end()-n,chromosome.end());
  chromosome.insert(chromosome.begin(),segment.begin(),segment.end());
  return *this;
}

/* ---------------------------------------------------------------------- */
const Individual  Individual::operator >>  (int n) const {
  return Individual(*this) >>= n;
}

/* ---------------------------------------------------------------------- */
void Individual::mindiff(const Individual& a) { 
  int period=gampi_domain.get_period(); 
  int num_periods=chromosome.size()/period; 
  if(num_periods==1) return;

  Individual b(*this); 
  float mindiff=a-b; 
  int minoffset=0; 
  for(int i=1; i<num_periods; i++) {
    b<<=period; 
    int diff=a-b; 
    if(diff<mindiff) {
      mindiff = diff; 
      minoffset=i; 
    }
  }
  (*this)<<=minoffset*period; 
  //hash and fitness do not need to be recomputed since we are shifting by n*period.
  assert(this->is_valid()); 
}


/* ---------------------------------------------------------------------- */
void Individual::crossover
        (const Individual& parent1, const Individual& pt2, Individual& child1, Individual& child2) {
  assert(parent1.is_valid()); 
  assert(pt2.is_valid());
  assert(parent1.hash!=pt2.hash); 
  Individual parent2(pt2); 
  parent2.mindiff(parent1); 
  Individual::ordered_crossover(parent1, parent2, child1, child2); 
  //Individual::cyclic_crossover(parent1, parent2, child1, child2); //poor results from cyclic
}

/* ---------------------------------------------------------------------- */
void Individual::ordered_crossover
        (const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2) {
  assert(parent1.hash!=parent2.hash); 
  int N=parent1.chromosome.size(); 
  int x=rand()%N;
  int y=rand()%N;
  while(x == y) x=rand()%N; //two locations should not coincide
  int begin=std::min(x,y);
  int end=std::max(x,y); 
  int len=end-begin; 
  if(len<0) len+=N; 

  std::vector<gene> segment1(len), segment2(len); //For searching
  //Copy the selected section
  for (int i=0; i<len; i++){
    int j=(begin+i)%N; 
    segment1[i]=child1.chromosome[j]=parent2.chromosome[j]; 
    segment2[i]=child2.chromosome[j]=parent1.chromosome[j]; 
  }

  int fs=-1;
  if(end<begin) fs+=end; 
  //Add the remaining elements in order
  for (int i=0, j=fs, k=fs; i<N; i++){
    std::vector<gene>::iterator ind1=std::find(segment1.begin(), segment1.end(), parent1.chromosome[i]); 
    std::vector<gene>::iterator ind2=std::find(segment2.begin(), segment2.end(), parent2.chromosome[i]); 
    if(ind1>=segment1.end()) {
      ++j%=N; 
      if(j==begin) j=end; 
      child1.chromosome[j]=parent1.chromosome[i]; 
    }
    if(ind2>=segment2.end()) {
      ++k%=N;
      if(k==begin) k=end; 
      child2.chromosome[k]=parent2.chromosome[i]; 
    }
  }
  //hash and fitness need to be recomputed; 
  child1.fitness=gampi_domain.get_fitness(gampi_nodelist, child1.chromosome); 
  child1.hash=child1.ring_fnv_1a(); 
  child2.fitness=gampi_domain.get_fitness(gampi_nodelist, child2.chromosome); 
  child2.hash=child2.ring_fnv_1a(); 
  assert(child1.is_valid());
  assert(child2.is_valid());
}

/* ---------------------------------------------------------------------- */
void Individual::cyclic_crossover
        (const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2) {
  assert(parent1.hash!=parent2.hash); 
  int N=parent1.chromosome.size(); 
  int indexstart;
  do { //Obtain a random starting index, but it cannot be a singleton
    indexstart=rand()%N; 
  } while (parent1.chromosome[indexstart] == parent2.chromosome[indexstart]);

  std::vector<int> cycleindex; 
  int index1=indexstart; 
  do { //Find the cycle starting from that index
    cycleindex.push_back(index1);  
    int val2=parent2.chromosome[index1]; 
    index1=std::find (parent1.chromosome.begin(),parent1.chromosome.end(),val2) - parent1.chromosome.begin(); 
  } while (index1!=indexstart); 

  child1=Individual(parent1); 
  child2=Individual(parent2); 
  for(int i=0; i<cycleindex.size(); i++){ //Swap the selected cycle
    int j=cycleindex[i];
    child1.chromosome[j]=parent2.chromosome[j];
    child2.chromosome[j]=parent1.chromosome[j];
  }
  //hash and fitness need to be recomputed; 
  child1.fitness=gampi_domain.get_fitness(gampi_nodelist, child1.chromosome); 
  child1.hash=child1.ring_fnv_1a(); 
  child2.fitness=gampi_domain.get_fitness(gampi_nodelist, child2.chromosome); 
  child2.hash=child2.ring_fnv_1a(); 

  assert(child1.is_valid());
  assert(child2.is_valid());
}

