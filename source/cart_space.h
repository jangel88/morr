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
  public:
    Domain(int max_i, int max_j, int max_k);
    void  show_topo();
    std::vector<int> give_topo();
    int get_position(Subdomain element); 
    std::vector<Subdomain> find_neighbors(Subdomain element); 
};



