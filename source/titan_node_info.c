#include <stdlib.h>
#include <mpi.h>
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

int valid_node_list(int N, *nodeid node_list)
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

float distance_between_nodes(nodeid n1, nodeid n2){
const float C_same_node=0, C_same_router=1e-2, C_x=1.0, C_y=2.0, C_z=1.0; 


}
