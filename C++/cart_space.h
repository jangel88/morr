class Subdomain{
  public:
    int i,j,k;
};


class Domain {
  public:
    int max_i,max_j,max_k;
    std::vector<int> topology;

    void  init_domain(int max_i, int max_j, int max_k)
      {
      void find_neighbors(Subdomain element, Subdomain* neighbor,int max_i,int max_j, int max_k);
      int get_position(Subdomain element,int max_i,int max_j,int max_k);
      Subdomain get_domain_coord(int position,int max_i, int max_j,int max_k);
 
      max_i=max_i;
      max_j=max_j;
      max_k=max_k;  
      int i,j; 
      Subdomain q;
      int N=max_i*max_j*max_k; 
      for(i=0;i<N;i++){
        q=get_domain_coord(i,max_i,max_j,max_k);
        Subdomain neigh[6];
        find_neighbors(q,neigh,max_i,max_j,max_k);
        for(j=0;j<6;j++){
          topology.push_back(get_position(neigh[j],max_i,max_j,max_k));
        }
      }  
    }

    void  show_topo()
    {
      for(std::vector<int>::iterator it=topology.begin(); it!=topology.end();++it){
        std::cout << *it << std::endl;
      }

    }
};


void find_neighbors(Subdomain element, Subdomain* neighbor,int max_i,int max_j, int max_k)
{
   // -i 
  neighbor[0].i = element.i==0 ? max_i-1 : element.i-1;
  neighbor[0].j = element.j;
  neighbor[0].k = element.k;
  // +i
  neighbor[1].i = element.i==max_i-1 ? 0 : element.i+1;
  neighbor[1].j = element.j;
  neighbor[1].k = element.k;
  // -j 
  neighbor[2].i = element.i;
  neighbor[2].j = element.j==0 ? max_j-1 : element.j-1;
  neighbor[2].k = element.k;
  // +j
  neighbor[3].i = element.i;
  neighbor[3].j = element.j==max_j-1 ? 0 : element.j+1;
  neighbor[3].k = element.k;
  // -k 
  neighbor[4].i = element.i;
  neighbor[4].j = element.j;
  neighbor[4].k = element.k==0 ? max_k-1 : element.k-1;
  // +k
  neighbor[5].i = element.i;
  neighbor[5].j = element.j;
  neighbor[5].k = element.k==max_k-1 ? 0 : element.k+1; 
}

int get_position(Subdomain element,int max_i,int max_j,int max_k)
{
  return max_j*max_i*element.k+max_i*element.j+element.i;
}


Subdomain get_domain_coord(int position,int max_i, int max_j,int max_k) 
{
  Subdomain element;
  element.i=position%max_i;
  element.j=((position-element.i)/max_i)%max_j; 
  element.k=(position-element.i-max_i*element.j)/(max_j*max_i);
  return element; 
}




