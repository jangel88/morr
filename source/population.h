// ---------------------------------------------------------------------- 
class Population {
  private
    int p_size;
    Individual* individuals; 
  protected:
  public: 
    Population(int individual_size, Individual* ancestors);
    Population(Population elite_population, int progeny_size);
    Population find_elites(int elite_size); 
    int get_best_fitness(); 
    void show_population(); 
    void show_fitness_of_population();  
  }
