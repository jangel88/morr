#include"individual.h"
#include<stdio.h>
#include<math.h>
#include <algorithm>    // std::random_shuffle
 
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/

Individual::Individual(int length, std::vector<nodeid> nodeids, bool shuffle)
{

  for(std::vector<nodeid>::iterator it = nodeids.begin(); it != nodeids.end(); ++it){
    this->chromosome.push_back(*it);
  }

  if(shuffle){
    std::random_shuffle(chromosome.begin(),chromosome.end());
  }
}

float Individual::give_fitness()
{
  return this->fitness;
}

Individual::Individual(Individual* parent, bool shuffle)
{
  for(std::vector<nodeid>::iterator it=parent->chromosome.begin(); it!=parent->chromosome.end(); ++it){
    this->chromosome.push_back(*it);
  }
  if(shuffle){
    std::random_shuffle(chromosome.begin(),chromosome.end());
  }
  fitness = parent->give_fitness();
}

void Individual::show_Individual()
{
  for(std::vector<nodeid>::iterator it=chromosome.begin();it!=chromosome.end();++it){
   std::cout << *it << " " ;
  }
  std::cout << "\n";
}

float Individual::get_fitness(std::vector<int>* topology)
{
  int i,j;
  int N=chromosome.size();
  float tmp=0.0;
  fitness=0.0; 
  nodeid n1,n2;
  std::vector<float> cost(N*6);

  for(j=0;j<6;j++){
    for(i=0;i<N;i++){
      n1=chromosome.at(topology->at(i+j*N));
      n2=chromosome.at(i);
      cost[i+N*j]=distance_between_nodes(n1,n2); 
    }
  }
 
  for(i=0;i<N*6;i++){ 
    fitness+=cost[i]*cost[i];
//  fitness+=cost[i];
  } 
  fitness=sqrt(fitness);
//fitness=fitness/(N*6);
  return fitness;
}

size_t Individual::hash()
{
    size_t results = 2166136261U; 
    for ( std::vector<nodeid>::iterator current = this->chromosome.begin(); current != this->chromosome.end(); ++ current ) {
        results = 127 * results + static_cast<size_t>( *current );
    }
    return results; 
}


void Individual::mutate(Domain* space)
{
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
      head_to_tail(mirr1,space);
    }
    roll *= (float) 4/5; 
  }
}

void Individual::swap_segment(bool mirror1, bool mirror2)
{
 
  int length_range[10]={1,2,4,8,16,32,64,128,256,512};
  int length=length_range[rand()%5];
  int x=rand()%chromosome.size();
  int y=rand()%chromosome.size();
  while(x == y){ //unique segments
    x=rand()%chromosome.size();
  }
  int N=chromosome.size();
  int start2=MAX(x,y); 
  int start1=MIN(x,y); 

  while((length > (chromosome.size()-start2-1)) || //make sure we arent overshooting 
           (start1+length > start2)){   //or overlapping
    length-=1;
  }
  std::vector<nodeid> temp(length); 

  if(mirror1){
    std::reverse_copy(chromosome.begin()+start2,chromosome.begin()+start2+length,temp.begin());
  }else{
    std::copy(chromosome.begin()+start2,chromosome.begin()+start2+length,temp.begin());
  }

  if(mirror2){
    std::reverse_copy(chromosome.begin()+start1,chromosome.begin()+start1+length,chromosome.begin()+start2);
  }else{
    std::copy(chromosome.begin()+start1,chromosome.begin()+start1+length,chromosome.begin()+start2);
  } 
  std::copy(temp.begin(),temp.end(),chromosome.begin()+start1);

  if (chromosome.size()!=N){ 
    std::cout << "Error in swap! Vector changed length\n";
    exit(1);
  }
}

void Individual::cut_n_paste_segment(bool mirr)
{
  int N=chromosome.size();
  int length_range[10]={1,2,4,8,16,32,64,128,256,512};
  int length=length_range[rand()%5]; 
  int dst=rand()%chromosome.size();
  int src=rand()%chromosome.size();

  while((src+length) > chromosome.size() || //dont overshoot the end
        (dst+length) > chromosome.size()){
    length-=1;
  }

  std::vector<nodeid> temp(length); 
  if(mirr){
    std::reverse_copy(chromosome.begin() + src , chromosome.begin() + src + length,temp.begin() ); 
  }else{
    std::copy(chromosome.begin() + src , chromosome.begin() + src + length,temp.begin() ); 
  } 
  chromosome.erase(chromosome.begin() + src, chromosome.begin() + src + length);
  chromosome.insert(chromosome.begin() + dst, temp.begin(), temp.end()); 

  if (chromosome.size()!=N){ 
    std::cout << "Error in cut_n_paste! Vector changed length\n";
    exit(1);
  }
}

void Individual::head_to_tail(bool mirr,Domain* space)
{ 
  int N=chromosome.size(); 
  int i=space->get_max_i();
  int j=space->get_max_j();
  int k=space->get_max_k(); 
  int subdomains=space->get_num_subdomain();
  int depth;
  int num_to_cut; 
  if(k==1){
    depth=rand()%j; 
    num_to_cut=depth*i;
  }else{ 
    depth=rand()%k; 
    num_to_cut=i*j*depth;
  }
  std::vector<nodeid> temp(num_to_cut); 
  std::copy(chromosome.begin(),chromosome.begin()+num_to_cut,temp.begin()); 
  chromosome.erase(chromosome.begin(),chromosome.begin()+num_to_cut); 
  chromosome.insert(chromosome.end(),temp.begin(),temp.end()); 

  if (chromosome.size()!=N){
    std::cout << "Error in head2tail! Vector changed length!\n";
    exit(1);
  } 
}





void Individual::print_chromosome(Domain* space, int format_flag)
{
  int i,j,k;

  int max_i = space->get_max_i();
  int max_j = space->get_max_j();
  int max_k = space->get_max_k();
  if(format_flag){
    std::cout << std::endl;
    if (max_k == 1){
      for(i=0;i<max_i;i++){
        for(j=0;j<=max_j;j++){
          if(j==max_j){
           std::cout << std::endl;
          }else{ 
            printf("%4d",chromosome.at(j*max_i+i));
          }
        }
      }
    }
  }else{
    for(int i=0;i<chromosome.size();i++){
      std::cout << chromosome[i] << std::endl;
    }
  }
}


