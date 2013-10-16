#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <mpi.h>
#include "node_info.h" 

int init_node_info(void){
  //Nothing to do here now. Node coords are initialized in the static variable
  return(0); 
}

bool valid_nodeid(nodeid nid) {
  if (nid>=0 && nid<COMPUTE_NODE_COUNT){
    return true;
  } else {
    return false;
  }
}

bool valid_nodeid_list(int N, nodeid* node_list) {
  for(int i=0;i<N;i++){
    if (valid_nodeid(node_list[i])!=true) return false;
  } 
  return true; 
}

nodeid query_nodeid(){
  int flag;
  char name[32];
  char* name_offset;
  int len;

  flag = MPI_Get_processor_name(name, &len);
  if(flag != MPI_SUCCESS) return(-1);
  name_offset=&name[3];
  //printf("%s\n", name_offset);
  return( atoi(name_offset));
}

float distance_between_nodes(nodeid n1, nodeid n2) {
  assert(valid_nodeid(n1)); 
  assert(valid_nodeid(n2)); 
  const float C_same_node=0, C_same_router=0.01; 
  const int C_x=1, C_y=2, C_z=1; 
  int x1,x2;
  int y1,y2;
  int z1,z2;
  int dx,dy,dz;
  float distance;

  if(n1==n2) return(C_same_node); 

  x1=compute_node_coords[n1][0];
  y1=compute_node_coords[n1][1];
  z1=compute_node_coords[n1][2];
  x2=compute_node_coords[n2][0];
  y2=compute_node_coords[n2][1];
  z2=compute_node_coords[n2][2];
  if(x1==x2 && 
     y1==y2 && 
     z1==z2){
    distance=(C_same_router);
  } else {
    dx=abs(x2-x1); dx=(dx>XDIM/2) ? (XDIM-dx) : dx;
    dy=abs(y2-y1); dy=(dy>YDIM/2) ? (YDIM-dy) : dy;
    dz=abs(z2-z1); dz=(dz>ZDIM/2) ? (ZDIM-dz) : dz;
    distance=(C_x*dx+C_y*dy+C_z*dz);
    assert(distance>0); 
  }
  //printf("%d (%d,%d,%d) - %d (%d,%d,%d) = %.3f\n", n1, x1, y1, z1, n2, x2, y2, z2, distance);
  return(distance); 
}

