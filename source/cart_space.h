#include <vector>
#include "node_info.h"

class Subdomain{
  private:
    int i,j,k;
  public:
    Subdomain(int i, int j, int k);
    Subdomain(int position, int max_i, int max_j, int max_k);    
    void get_coors(int* coors);
};


class Domain {
  private:
    int size; //Size of the domain, the number of subdomains
    int period; //period in which head to tail is valid, <=size
    int max_i,max_j,max_k;
    std::vector<int> topology;

    std::vector<Subdomain> find_neighbors(Subdomain element); 
    int get_position(Subdomain element); 
  public:
    Domain(int max_i, int max_j, int max_k);
    int get_size() { return size; }
    int get_period() { return period; } 
    Subdomain get_subdomain(int position){ return Subdomain(position, max_i, max_j, max_k); }
    float get_fitness(const std::vector<nodeid>& nodelist); 
    float get_fitness(const std::vector<nodeid>& nodelist, const std::vector<int>& reorder); 
};

