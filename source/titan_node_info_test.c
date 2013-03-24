#include <stdbool.h>
#include <mpi.h>
#include "titan_node_info.h"

int main(int argc, char*argv[]) {
nodeid myid; 
bool validity; 

MPI_Init(&argc,&argv);
myid=query_nodeid(); 

MPI_Finalize();
return 0; 
}

