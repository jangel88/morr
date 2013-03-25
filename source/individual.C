
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
    std::cout << *it<< " " ;
  }
  std::cout << "\n";
}

float Individual::get_fitness(std::vector<int>* topology)
{
  int i,j;
  int N=size();
  float tmp=0.0;
  nodeid n1,n2;
  std::vector<float> cost(N*6,0);
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
    cut_n_paste_segment(false);
//}else if(r==1){
  }else{
    swap_segment(mirr1,mirr2);
//}else{
//  head_to_tail(mirr1);
//}
  }
}

void Individual::swap_segment(bool mirror1, bool mirror2)
{
  int z_range[4]={1,2,4,8};
  int z=z_range[rand()%4];
  int N=size();
  int x=rand()%size();
  int y=rand()%size();
  int stop=MAX(x,y); 
  int start=MIN(x,y);
  std::vector<nodeid>::iterator itx=begin()+x;
  std::vector<nodeid>::iterator ity=begin()+y;
  std::vector<nodeid> temp1(z);
  std::vector<nodeid> temp2(z); 
  z=MIN(z,N-stop);
  z=MIN(z,(stop-start)-1);
  for(int i=0; i<z; i++){
    temp1[i]=(*(itx+i));
    temp2[i]=(*(ity+i)); 
  }
  for(int i=0;i<z;i++){
    at(x+i)=temp2[i+mirror2*(z-2*i-1)];
    at(y+i)=temp1[i+mirror1*(z-2*i-1)];
  }
  if (size()!=N){ 
    std::cout << "Error in swap! Vector changed length\n";
    exit(1);
  }
}

void Individual::cut_n_paste_segment(bool mirr)
{
  int N=size();
  int z_range[4]={1,2,4,8};
  int length=z_range[rand()%2];
  length=1;
  int dst=rand()%size();
  int start=rand()%size();

  const size_t final_dst = dst > start ? dst - length : dst;

  std::vector<nodeid> tmp(begin() + start, begin() + start + length);
  erase(begin() + start, begin() + start + length);
  insert(begin() + final_dst, tmp.begin(), tmp.end()); 
  if (size()!=N){ 
    std::cout << "Error in cut_n_paste! Vector changed length\n";
    exit(1);
  }
}
