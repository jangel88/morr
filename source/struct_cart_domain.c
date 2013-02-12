#include"struct_cart_domain.h"
#include<stdio.h>

domain init_domain(int max_i, int max_j, int max_k)
{
  domain space;
  space.max_i=max_i;
  space.max_j=max_j;
  space.max_k=max_k;
  return space; 
} 
int validate_domain_size(domain space, int node_count)
{
  return (node_count==(space.max_i*space.max_j*space.max_k));
}

int validate_subdomain(subdomain element, domain space)
{
  if( element.i < 0 || element.i >= space.max_i || 
      element.j < 0 || element.j >= space.max_j || 
      element.k < 0 || element.k >= space.max_k    ) {
    return FALSE; 
  } else { 
    return TRUE; 
  }

}

int get_position(subdomain element,domain space)//int i, int j, int k,  
{  
  return space.max_j*space.max_i*element.k+space.max_i*element.j+element.i;
}

subdomain get_domain_coord(int position, domain space)
{
  subdomain element;
  element.i=position%space.max_i;
  element.j=((position-element.i)/space.max_i)%space.max_j; 
  element.k=(position-element.i-space.max_i*element.j)/(space.max_j*space.max_i);
  return element;
}

void topomat3d(int topology[],domain space)
{

  subdomain q;
  int n,m,p;
  m=space.max_j;
  n=space.max_i; 
  p=space.max_k;
  int r,s;
  int i,j,k;;
  int N=n*m*p;
  
  for(r=0;r<N;r++){
    q=get_domain_coord(r,space);
    i=q.i;j=q.j;k=q.k;
    subdomain neigh[6]={{i,(j+1)%m,k},{(i-1)%n+((i-1)%n<0)*n,j,k},{i,(j-1)%m+((j-1)%m<0)*m,k},{(i+1)%n,j,k},{i,j,(k-1)%p+((k-1)%p<0)*p},{i,j,(k+1)%p}}; //initialize neighbors inline
 
    for(s=0;s<6;s++){
      topology[s*N+r]=get_position(neigh[s],space);
    } 
  }
 
}

/*float cost_metric(nodeid* node_list, metric, domain space){
  int i,j,k;
  int me;
  int neigh[6];
  for(i=0;i<space.max_x;i++){
    for(j=0;j<space.max_y;j++){
      for(k=0;k<space.max_z;k++){
        neigh[0]=get_position(i,j+1,k) 
*/


int main(int argc, char** argv)
{
  int m=3,n=3,p=3;
  int i;
  int N=m*n*p;
  int topology[N*6];



  subdomain elements[N]; 
  domain space=init_domain(m,n,p);
  topomat3d(topology,space);

  for(i=0;i<N;i++){
    subdomain p=get_domain_coord(i,space);
    printf("%d has coors (%d,%d,%d)\n",get_position(p,space),p.i,p.j,p.k);
  }




  
}
