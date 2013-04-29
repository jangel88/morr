#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#include "titan_node_info.h"

int main(int argc, char*argv[]) {

int i, j, k;
float d; 
int rank, size; 
nodeid myid, *allid; 
bool validity; 

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 
allid = (nodeid*) malloc(sizeof(nodeid)*size);
srand(232); 

myid=query_nodeid(); 
MPI_Gather(&myid , sizeof(nodeid), MPI_BYTE,
           allid, sizeof(nodeid), MPI_BYTE, 0, MPI_COMM_WORLD); 
if(rank==0) {
  if(valid_nodeid_list(size, allid)==false) printf("Invalid nodeid found\n"); 
  for(i=0; i<size; i++) printf("%5d ", allid[i]); 
  printf("\n\n"); 
  for(i=0; i<10; i++){
    j=rand()%size; 
    k=rand()%size; 
    d=distance_between_nodes(allid[j], allid[k]); 
    printf("node %d  - node %d = %.3f \n", allid[j], allid[k], d); 
  }
}


MPI_Finalize();
return 0; 
}

