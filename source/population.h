#include <vector> 
#include "individual.h"
#include <boost/serialization/access.hpp>

class Population {
  private:
    std::vector<Individual> flock; 
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) { 
      ar & flock;
    }
  protected:
  public: 
    Population(int size); 
    Population(const Individual& ancestor, int size); 
    Population(const Population& ancestors, int size); 

          Population& operator += (const Population& a);
    const Population  operator +  (const Population& a) const; 

          Population& operator += (const Individual& a);
    const Population  operator +  (const Individual& a) const; 

    int get_size()const { return flock.size(); } 
    float get_best_fitness();
    Individual get_individual(int i);
    void show(); 
    void show(char* s); 

    Population get_random_subset(int count); 
    Population get_unique_elites(int count); 
};

