#include<vector>
#include<iostream>
class Subdomain{
  private:
    int i,j,k;
  public:
    Subdomain();
    Subdomain(int i, int j, int k);
    Subdomain(int position, int max_i, int max_j, int max_k);    
    void give_coors(int* coors);
};


class Domain {
  private:
    int max_i,max_j,max_k;
    std::vector<int> topology;
    int period;
  public:
    Domain(int max_i, int max_j, int max_k);
    int get_period() 
    {   
      return period; 
    } 
    int get_max_i()
    {
      return max_i; 
    }   
    int get_max_j() 
    {
      return max_j; 
    }   
    int get_max_k()
    { 
      return max_k; 
    }
    int get_num_subdomain()
    {
      return max_i*max_j*max_k;
    }
    void  show_topo();
    std::vector<int> give_topo() 
    {   
      return topology; 
    }
    int get_position(Subdomain element); 
    std::vector<Subdomain> find_neighbors(Subdomain element); 
};



