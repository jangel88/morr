#include <stdlib.h>
#include <mpi.h>
#include "titan_node_info.h" 

int init_node_info(void){
  //Nothing to do here now. Node coords are initialized in the header file. 
  return(0); 
}


nodeid_validity valid_nodeid(int nodeid)
{
  if  (nodeid>-1 && nodeid<19200){
    return VALID;
  } else {
    return INVALID;
  }
}

nodeid_validity valid_node_list(int N, int* node_list)
/* Check that node_list of length N contains only 
 * valid node ids*/
{
  int i;
  for(i=0;i<N;i++){
    if (valid_nodeid(node_list[i])!=VALID){
      return INVALID;
    }
  } 
  return VALID; 
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

