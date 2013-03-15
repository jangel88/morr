#include"node_info.h"

class Individual : std::List <nodeid> {
  private:
    double fitness=-1;
    void swap_segment(bool mirror1, bool mirror2); 
    void cut_n_paste_segment(bool mirror); 
    void head_to_tail(bool mirror); 
  protected:
  public: 
    Individual(Individual parent, bool shuffle); 
    Individual(int length, nodeid* nodeids, bool shuffle); 
    void mutate(){
      fitness=-1; 
    } 
    int get_fitness(){
      if(fitness==-1) recalculate;
      return(fitness); 
    }
    void show_Individual(); 
  }

