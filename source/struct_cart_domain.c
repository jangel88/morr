#include"struct_cart_domain.h"
#include<stdio.h>

#define TRUE 1
#define FALSE 0

 domain init_domain(int max_i, int max_j, int max_k)

{
  domain space;
  space.max_i=max_i;
  space.max_j=max_j;
  space.max_k=max_k;
  space.topology=malloc(sizeof(int)*max_i*max_j*max_k*6);
  topomat3d(&space);
  
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

int get_position(subdomain element,domain space)
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

int find_neighbors(domain space, subdomain element, subdomain* neighbor)
{
  // -i 
  neighbor[0].i = element.i==0 ? space.max_i-1 : element.i-1;
  neighbor[0].j = element.j;
  neighbor[0].k = element.k;
  // +i
  neighbor[1].i = element.i==space.max_i-1 ? 0 : element.i+1;
  neighbor[1].j = element.j;
  neighbor[1].k = element.k;
  // -j 
  neighbor[2].i = element.i;
  neighbor[2].j = element.j==0 ? space.max_j-1 : element.j-1;
  neighbor[2].k = element.k;
  // +j
  neighbor[3].i = element.i;
  neighbor[3].j = element.j==space.max_j-1 ? 0 : element.j+1;
  neighbor[3].k = element.k;
  // -k 
  neighbor[4].i = element.i;
  neighbor[4].j = element.j;
  neighbor[4].k = element.k==0 ? space.max_k-1 : element.k-1;
  // +k
  neighbor[5].i = element.i;
  neighbor[5].j = element.j;
  neighbor[5].k = element.k==space.max_k-1 ? 0 : element.k+1;
}


void topomat3d(domain* space)
{
  subdomain q;
  int n,m,p;
  m=space->max_j;
  n=space->max_i; 
  p=space->max_k;
  int r,s;
  int i,j,k;
  int N=n*m*p;

  for(r=0;r<N;r++){
    q=get_domain_coord(r,*space);
    i=q.i;j=q.j;k=q.k;
    subdomain neigh[6];
    find_neighbors(*space,q,neigh);
    for(s=0;s<6;s++){ 
      space->topology[s*N+r]=get_position(neigh[s],*space);
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

#if 0
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
//  subdomain p=get_domain_coord(i,space);
//  printf("%d has coors (%d,%d,%d)\n",get_position(p,space),p.i,p.j,p.k);
    printf("%d has neigh: %d %d %d %d %d %d\n",i,topology[i],topology[i+N],topology[i+2*N],topology[i+3*N],topology[i+4*N],topology[i+5*N]);
  }
  
}
#endif
