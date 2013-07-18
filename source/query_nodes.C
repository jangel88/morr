#include"population.h" 
#include<stdio.h>
#include<stdlib.h>
#include<boost/mpi.hpp>
#include<boost/serialization/vector.hpp>


namespace mpi = boost::mpi;

int main(int argc, char **argv)
{

  mpi::environment env(argc, argv);
  mpi::communicator world;



  std::vector<nodeid> node_list;
  nodeid nid=query_nodeid();
  all_gather(world,nid,node_list);
  if(world.rank()==0){
    for(int i=0;i<world.size();i++){
      std::cout << node_list[i] << std::endl;
    }
  }

  return 0;  
} 
