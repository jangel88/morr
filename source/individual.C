
#include"individual.h"

Individual::Individual(int length, nodeid* nodeids, bool shuffle)
{
  for(int i=0;i<length;i++){
    push_back(nodeids[i]);
  }
}


Individual::Individual(Individual* parent, bool shuffle)
{
  for(Individual::iterator it=parent->begin(); it!=parent->end(); ++it){
    push_back(*it);
  }
}

void Individual::show_Individual()
{
  for(Individual::iterator it=begin();it!=end();++it){
    std::cout << *it << std::endl;
  }
}
