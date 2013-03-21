// ---------------------------------------------------------------------- 
class Population {
  private:
    int p_size;
    int max_psize;
    Individual** individuals;
  protected:
  public: 
    Population(int max_psize,int individual_size, Individual* ancestors);
    Population(Population elite_population, int progeny_size);
    Population find_elites(int elite_size); 
    void set_max_psize(int max_psize);
    void set_current_psize(int p_size);
    int get_best_fitness(); 
    void show_population(); 
    void show_fitness_of_population();  
};
