#include<vector> // std::vector
#include"individual.h" // Individual class

// ---------------------------------------------------------------------- 

typedef std::pair<float,int> fit_pair;
bool comparator (const fit_pair& i, const fit_pair& j);


class Population {
  private:
    int p_size;
    int max_psize;
    int num_elites;
//  Individual* individuals;

  protected:
  public: 
    void set_elites(int num_elites) { this->num_elites=num_elites; }
    std::vector<Individual> individuals;
    Population(int max_psize,int individual_size, Individual* ancestors);
    Population(Population elite_population, int progeny_size);
    Population find_elites(int elite_size); 
    void populate_next_gen(std::vector<Individual>*,Domain*);
    float get_best_fitness(); 
    void show_population(); 
    void show_fitness_of_population();  
    void tournament(std::vector<Individual>*,Domain*);    
    Individual get_best_map(std::vector<int>*);
};



