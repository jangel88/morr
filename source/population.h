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
    Population(const Individual& ancestor, int size, float timeout=5); 
    Population(const Population& ancestors, int size, float timeout=5); 

          Population& operator += (const Population& a);
    const Population  operator +  (const Population& a) const; 

          Population& operator += (const Individual& a);
    const Population  operator +  (const Individual& a) const; 

    int get_size()const { return flock.size(); } 
    bool is_valid() const;
    float get_best_fitness();
    Individual get_individual(int i);
    void show(); 
    void show(char* s); 

    Population random_selection(int count); 
    Population elitist_selection(int count); 
    Population rank_selection(int count); 
};

