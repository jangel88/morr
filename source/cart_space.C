#include<cart_space.h>
Domain::Domain(int max_i, int max_j, int max_k)
{
  max_i=max_i;
  max_j=max_j;
  max_k=max_k;  
  int i,j; 
  
  int N=max_i*max_j*max_k; 
  for(i=0;i<N;i++){
    Subdomain q(i,max_i,max_j,max_k);
    std::vector<Subdomain> neigh;
    neigh=find_neighbors(q); 
    for(j=0;j<6;j++){
      topology.push_back(get_position(neigh[j])); 
    }
  }  
}

void Domain::show_topo()
{
  for(std::vector<int>::iterator it=topology.begin(); it!=topology.end();++it){
    std::cout << *it << std::endl;
  }
}

std::vector<int> Domain::give_topo()
{
  return topology;
}

std::vector<Subdomain> Domain::find_neighbors(Subdomain element) 
{
  int i,j,k;
  int coors[3];
  element.give_coors(coors);
  i=coors[0];
  j=coors[1];
  k=coors[2];
  std::vector<Subdomain> neighbor;

  Subdomain n1(i==0 ? max_i-1 : i-1,j,k);
  Subdomain n2(i==max_i-1 ? 0 : i+1,j,k);
  Subdomain n3(i,j==0 ? max_j-1 : j-1,k);
  Subdomain n4(i,j==max_j-1 ? 0 : j+1,k);
  Subdomain n5(i,j,k==0 ? max_k-1 : k-1);
  Subdomain n6(i,j,k==max_k-1 ? 0 : k+1);

  neighbor.push_back(n1);
  neighbor.push_back(n2);
  neighbor.push_back(n3);
  neighbor.push_back(n4);
  neighbor.push_back(n5);
  neighbor.push_back(n6);
  return neighbor;
}

int Domain::get_position(Subdomain element)
{
  int i,j,k;
  int coors[3];
  element.give_coors(coors);
  i=coors[0];  
  j=coors[1]; 
  k=coors[2];
  return max_j*max_i*k+max_i*j+i; 
}

Subdomain::Subdomain(int i, int j, int k)
{
  this->i=i;
  this->j=j;
  this->k=k;
}

Subdomain::Subdomain(int position,int max_i, int max_j,int max_k) 
{
  i=position%max_i;
  j=((position-i)/max_i)%max_j; 
  k=(position-i-max_i*j)/(max_j*max_i);
}

void Subdomain::give_coors(int* coors)
{
  coors[0]=i;
  coors[1]=j;
  coors[2]=k;
}
