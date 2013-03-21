#include<list>

class Individual : public std::list<nodeid> {
  private:
    double fitness;
    void swap_segment(bool mirror1, bool mirror2); 
    void cut_n_paste_segment(bool mirror); 
    void head_to_tail(bool mirror); 
  protected:
  public: 
    Individual(Individual* parent, bool shuffle); 
    Individual(int length, nodeid* nodeids, bool shuffle); 
    void mutate(); 
    static void crossover 
      (Individual parent1, Individual parent2, Individual* offspring1, Individual* offspring2); 
    int get_fitness(); 
    void show_Individual(); 
};

