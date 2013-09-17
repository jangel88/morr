#include <vector>       // std::vector
#include <stdint.h>
#include "domain.h"
#include <boost/serialization/access.hpp>

class Individual  {
  private: 
    typedef int gene; 
    std::vector<gene> chromosome;
    double fitness;
    uint32_t hash; 

    int log2(int n); 
    uint32_t fnv_1a(uint8_t *bp, uint8_t *be) const; //begin point and beyond end
    uint32_t ring_fnv_1a() const; //returns the hash of the vector treating it as a periodic ring using fnv_1a
    void swap_segment(); 
    void cut_n_paste_segment(); 
    static void ordered_crossover
           (const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2); 
    static void cyclic_crossover
           (const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2); 

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) { 
      ar & fitness;
      ar & hash;
      ar & chromosome;
    }
  protected:
  public: 
    Individual(bool shuffle=false); 
    Individual(const Individual& parent, bool mutate=false); 
    Individual(const std::vector<gene>& parent_chromosome); 
    float get_fitness(){ return fitness;}
    int get_size() { return chromosome.size(); }
    uint32_t get_hash() const { return hash; }
    bool is_valid() const; 
    void show(); 
    void show(char* s); //Prepend with this optional string
    void mutate();
    static void crossover
           (const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2); 
    void mindiff(const Individual& a); //Period transformation to minimize difference with Individual a
    int operator - (const Individual& a) const; //Counts the number of chromosome elements unequal
    bool operator == (const Individual& a) const; //Compares if the two chromosomes are equal
          Individual& operator <<= (int n);
    const Individual  operator <<  (int n) const;
          Individual& operator >>= (int n);
    const Individual  operator >>  (int n) const;
};
