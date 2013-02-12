#include"struct_cart_domain.h"
#include<stdio.h>

domain init_domain(int max_x, int max_y, int max_z)
{
  domain space;
  space.max_x=max_x;
  space.max_y=max_y;
  space.max_z=max_z;
  return space; 
}

int validate_domain_size(domain space, int node_count)
{
  return (node_count==(space.max_x*space.max_y*space.max_z));
}

int get_position(int i, int j, int k, domain space) 
{ 
  int position;
  if (space.max_z==1) { 
    if (space.max_x==1){position=i;} 
    else if (space.max_y==1) {position=j;} 
    else {position=space.max_y*j+i; }
  }else if (space.max_y==1) {
    if (space.max_x==1) {position=k;} 
    else {position=space.max_x*k+i;}
  }else if (space.max_x==1) {
     position=space.max_y*k+j;
  }else{
     position=space.max_x*space.max_y*k+space.max_y*j+i;
  }
  return position;
}

subdomain get_domain_coord(int position, domain space)
{
  subdomain element;
  element.x=position%space.max_y;
  element.y=((position-element.x)/space.max_y)%space.max_x; 
  element.z=(position-element.x-space.max_y*element.y)/(space.max_x*space.max_y);
  return element;
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

  int m=1,n=5,p=1;
  int i;
  int N=m*n*p;
  domain space=init_domain(m,n,p);
  for(i=0;i<N;i++){
    subdomain p=get_domain_coord(i,space);
    printf("%d has coors (%d,%d,%d)\n",get_position(p.x,p.y,p.z,space),p.x,p.y,p.z);
  }
}   
#endif     
