

#include "titan_node_info.h" 

#define TRUE 1
#define FALSE 0

int init_node_info(void){
  //Nothing to do here now. Node coords are initialized in the header file. 
  return(0); 
}


int valid_nodeid(nodeid nid)
{
  if  (nid>-1 && nid<19200){
    return TRUE;
  } else {
    return FALSE;
  }
}

int valid_node_list(int N, nodeid* node_list)
/* Check that node_list of length N contains only 
 * valid node ids*/
{
  int i;
  for(i=0;i<N;i++){
    if (valid_nodeid(node_list[i])!=TRUE){
      return FALSE;
    }
  } 
  return TRUE; 
}
#if 0
nodeid query_nodeid(){
  int flag;
  char name[32];
  char* name_offset;
  int len;

  flag = MPI_Get_processor_name(name, &len);
  if(flag != MPI_SUCCESS) return(-1);
  name_offset=&name[3];
  printf("%s\n", name_offset);
  return(atoi(name_offset));
}
#endif
float distance_between_nodes(nodeid n1, nodeid n2)
{
  const float C_same_node=0, C_same_router=0.01, C_x=1.0, C_y=2.0, C_z=1.0; 
  int x1,x2;
  int y1,y2;
  int z1,z2;
  float dx,dy,dz;
  x1=titan_node_coords[n1][0];
  x2=titan_node_coords[n2][0];
  y1=titan_node_coords[n1][1];
  y2=titan_node_coords[n2][1];
  z1=titan_node_coords[n1][2];
  z2=titan_node_coords[n2][2];

  if (n1==n2){
    return (float)(C_same_node);
  }else if (x1==x2 && 
            y1==y2 && 
            z1==z2){
    return (float)(C_same_router);
  }else{
    dx=fabs(x2-x1); dx=(dx>(double)XDIM/2.0) ? fabs(dx-XDIM) : dx;
    dy=fabs(y2-y1); dy=(dy>(double)YDIM/2.0) ? fabs(dy-YDIM) : dy;
    dz=fabs(z2-z1); dz=(dz>(double)ZDIM/2.0) ? fabs(dz-ZDIM) : dz;
    return (float)(C_x*dx+C_y*dy+C_z*dz);
  }
 

}
