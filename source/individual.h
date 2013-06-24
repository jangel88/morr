#include <vector>       // std::vector
#include "node_info.h"  //nodeid
#include "cart_space.h" //Domain
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

class Individual  {
  private: 
    double fitness;
    std::vector<nodeid> chromosome;
    void swap_segment(bool mirror1, bool mirror2); 
    void cut_n_paste_segment(bool mirror); 
    void head_to_tail(bool mirror,Domain* space); 
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    { 
      ar & fitness;
      ar & chromosome;
    }

  protected:
  public: 
    Individual() { }
    Individual(Individual* parent, bool shuffle); 
    Individual(int length, std::vector<nodeid> nodeids, bool shuffle); 
    void mutate(Domain*); 
    static void crossover 
      (Individual parent1, Individual parent2, Individual* offspring1, Individual* offspring2); 
    float get_fitness(std::vector<int>* topology); 
    void show_Individual(); 
    float give_fitness();
    size_t hash(); 
    void show_zcoors(Domain *space);
    void print_chromosome(Domain* space);
    int give_size() { return chromosome.size(); }
};
//namespace boost{
//  namespace serialization{ 
//  }
//}
