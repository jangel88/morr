#include"node_info.h"
class Individual {
  public:
    double fitness;
    std::vector<int> chromosome; 
    int chromo_length;

    void   set_chromosome(int length,int* nodes)
    {
      chromo_length=length;
      for(int i=0; i<length; i++){
        chromosome.push_back(nodes[i]);  
      }
      std::random_shuffle(chromosome.begin(),chromosome.end());  
    }

    void   show_chromosome()
    {
      for(std::vector<int>::iterator it=chromosome.begin(); it!=chromosome.end();++it){
        std::cout << *it << std::endl;
      }
    }
};

class Population {
  public:
    int p_size;
    std::vector<Individual> Pop;

    void init(int pop_size, int ind_length)
    {
      p_size=pop_size;
      for(int i=0;i<pop_size;i++){
        Individual A;
        A.set_chromosome(ind_length,compute_nodes);
        Pop.push_back(A);
      }
    }
    void show_pop()
    {
      for(int i=0;i<p_size;i++){
        std::cout << "Ind " << i << std::endl;
        Pop[i].show_chromosome();
      }
    }
};



