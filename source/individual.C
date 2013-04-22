
#include"individual.h"
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/

Individual::Individual(int length, nodeid* nodeids, bool shuffle)
{
  for(int i=0;i<length;i++){
    push_back(nodeids[i]);
  }
  if(shuffle){
    std::random_shuffle(begin(),end());
  }
}

float Individual::give_fitness()
{
  return this->fitness;
}

Individual::Individual(Individual* parent, bool shuffle)
{
  for(Individual::iterator it=parent->begin(); it!=parent->end(); ++it){
    push_back(*it);
  }
  if(shuffle){
    std::random_shuffle(begin(),end());
  }
}

void Individual::show_Individual()
{
  for(Individual::iterator it=begin();it!=end();++it){
    std::cout << *it << " " ;
  }
  std::cout << "\n";
}

float Individual::get_fitness(std::vector<int>* topology)
{
  int i,j;
  int N=size();
  float tmp=0.0;
  nodeid n1,n2;
  std::vector<float> cost(N*6);
  for(j=0;j<6;j++){
    for(i=0;i<N;i++){
      n1=at(topology->at(i+N*j));
      n2=at(i);
      cost[i+N*j]=distance_between_nodes(n1,n2);
    }
  }
  for(i=0;i<N*6;i++){
    tmp+=cost[i]*cost[i];
  }
  fitness=sqrt(tmp);
  return fitness;
}

void Individual::mutate()
{
  int r=rand()%2;
  int mirr1=rand()%2;
  int mirr2=rand()%2;
  if(r==0){
    cut_n_paste_segment(mirr1);
  }else{
    swap_segment(mirr1,mirr2);
  }
}

void Individual::swap_segment(bool mirror1, bool mirror2)
{
  int length_range[4]={1,2,4,8};
  int length=length_range[rand()%4];
  int N=size();
  int x=rand()%size();
  int y=rand()%size();
  while(x == y){ //unique segments
    x=rand()%size();
  }
  int start2=MAX(x,y); 
  int start1=MIN(x,y); 
  while((length > (size()-start2-1)) || //make sure we arent overshooting or overlapping
           (start1+length > start2)){
    length-=1;
  }
  std::vector<nodeid> temp(length);

  if(mirror1){
    std::reverse_copy(begin()+start2,begin()+start2+length,temp.begin());
  }else{
    std::copy(begin()+start2,begin()+start2+length,temp.begin());
  }
  if(mirror2){
    std::reverse_copy(begin()+start1,begin()+start1+length,begin()+start2);
  }else{
    std::copy(begin()+start1,begin()+start1+length,begin()+start2);
  } 
  std::copy(temp.begin(),temp.end(),begin()+start1);

  if (size()!=N){ 
    std::cout << "Error in swap! Vector changed length\n";
    exit(1);
  }
}

void Individual::cut_n_paste_segment(bool mirr)
{
  int N=size();
  int length_range[4]={1,2,4,8};
  int length=length_range[rand()%4];

  int dst=rand()%size();
  int src=rand()%size();
  while((src+length) > size() || //dont overshoot the end
        (dst+length) > size()){
    length-=1;
  }
  std::vector<nodeid> temp(length); 
  if(mirr){
    std::reverse_copy(begin() + src , begin() + src + length,temp.begin() ); 
  }else{
    std::copy(begin() + src , begin() + src + length,temp.begin() ); 
  }
  erase(begin() + src, begin() + src + length);
  insert(begin() + dst, temp.begin(), temp.end()); 

  if (size()!=N){ 
    std::cout << "Error in cut_n_paste! Vector changed length\n";
    exit(1);
  }
}
