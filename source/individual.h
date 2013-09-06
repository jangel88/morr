#include <vector>       // std::vector
#include "domain.h"
#include <boost/serialization/access.hpp>
//#include <boost/serialization/vector.hpp>

class Individual  {
  private: 
    typedef int gene; 
    std::vector<gene> chromosome;
    double fitness;

    int log2(int n); 
    void swap_segment(bool mirror1, bool mirror2); 
    void cut_n_paste_segment(bool mirror); 
    void head_to_tail(); 
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) { 
      ar & fitness;
      ar & chromosome;
    }
  protected:
  public: 
    Individual(int size, bool shuffle=false); 
    Individual(const Individual& parent, bool shuffle=false); 
    Individual(const std::vector<gene>& parent_chromosome); 
    float get_fitness(){ return fitness;}
    int get_size() { return chromosome.size(); }
    void show(); 
    void show(char* s); //Prepend with this optional string
    size_t hash(); 
    void mutate();
};
//namespace boost{
//  namespace serialization{ 
//  }
//}
