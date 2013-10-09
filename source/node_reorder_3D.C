#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <boost/mpi.hpp>

#include "population.h"

/* ---------------------------------------------------------------------- */
// GLOBAL VARIABLES
Domain gampi_domain(1,1,1); 
std::vector<nodeid> gampi_nodelist;
/* ---------------------------------------------------------------------- */

namespace mpi = boost::mpi;

int main(int argc, char *argv[]) { //MAIN MAIN MAIN
mpi::environment env(argc, argv);
mpi::communicator world;
std::srand(135+2*world.rank());

bool err=true, werr=true; 
int max_i, max_j, max_k;
int opt;

if(world.rank()==0) 
  std::cout << "Usage: node_reorder_3D.exe  -X max_i -Y max_j -Z max_k \n"; 
while ((opt = getopt(argc, argv, "X:Y:Z:")) != -1) {
  switch (opt) {
  case 'X':
    max_i = atoi(optarg);
    break;
  case 'Y':
    max_j = atoi(optarg);
    break;
  case 'Z':
    max_k = atoi(optarg);
    break;
  default: /* '?' */
    fprintf(stderr, "Usage: %s [-X max_i -Y max_j -Z max_k ]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}
if (optind < argc) {
  fprintf(stderr, "Expected argument after options\n"); 
  exit(EXIT_FAILURE);
}

nodeid myid=query_nodeid(); 
std::vector<nodeid> allid; 
all_gather(world, myid, allid);

std::vector<nodeid>::iterator it; 
std::sort(allid.begin(), allid.end()); 
it=std::unique(allid.begin(), allid.end()); 
gampi_nodelist.resize(std::distance(allid.begin(), it)); 
std::copy(allid.begin(), it, gampi_nodelist.begin()); 

err=true; 
if(gampi_nodelist.size()==max_i*max_j*max_k) err=false;
all_reduce(world, err, werr, std::logical_or<bool>());
if(werr) {
  if(world.rank()==0) 
    fprintf(stderr, "nodecount mismatch: %d %d %d %d\n", gampi_nodelist.size(), max_i, max_j, max_k); 
  exit(EXIT_FAILURE); 
} else {
  if(world.rank()==0) 
    printf("nodes=%d max_i=%d max_j=%d max_k=%d\n", gampi_nodelist.size(), max_i, max_j, max_k);
}
err=!valid_nodeid_list(gampi_nodelist.size(), &gampi_nodelist.at(0));
all_reduce(world, err, werr, std::logical_or<bool>());
if(werr) {
  if(world.rank()==0) fprintf(stderr, "invalid nodeid list\n");
  exit(EXIT_FAILURE); 
}
gampi_domain=Domain(max_i, max_j, max_k);

if(world.rank()==0){
  Individual a;
  a.show((char*)"Start soln"); 
  FILE* nodefile = fopen("startnodelist.txt", "w"); 
  printf("Start Nodelist: "); 
  for(int i=0; i<gampi_nodelist.size(); i++) {
    fprintf(nodefile, "%5d\n",gampi_nodelist.at(i));
    printf("%d,",gampi_nodelist.at(i));
  }
  printf("\n");
  fclose(nodefile);
}

Individual v(false); //Dont shuffle
Population a(v, 1); //ancestor population
Population e(v, 1); //store the elites

time_t start=time(NULL); 
time_t lastprint=time(NULL);
float currbest=v.get_fitness(); 
std::pair<float,int> fitrank, bestfitrank; 
int elapsed=0; 

do {
  const int subset_percent=5; //percentage of population that makes the subset for next gen
  float lastbest=currbest;

  Population p(a, 100000); //population expanded from ancestors

  //Select a subset to be used for creating next generation
  e=p.elitist_selection(std::max(1, p.get_size()*subset_percent/100/10)); 
  currbest=e.get_best_fitness(); 

  if(currbest>lastbest*0.99) {
    a=p.rank_selection   (std::max(1, p.get_size()*subset_percent/100)); 
  } else {
    a=p.elitist_selection(std::max(1, p.get_size()*subset_percent/100)); 
  }

  //Add someone else's elites to mine
  if(world.size()>1){
    int comm_offset;
    if(world.rank()==0) comm_offset=1+(rand()%(world.size()-1)); // goes from 1 to N-1
    broadcast(world, comm_offset, 0); 
    int src_rank=(world.size()+world.rank()+comm_offset)%world.size(); // I receive from
    int dst_rank=(world.size()+world.rank()-comm_offset)%world.size(); // I send to 

    mpi::request reqs[2];
    Population r(v, 1);
    reqs[0]=world.isend(dst_rank, 307, e); 
    reqs[1]=world.irecv(src_rank, 307, r); 
    mpi::wait_all(reqs, reqs+2);  
    a+=r; 
  }

  time_t current=time(NULL); 
  elapsed=difftime(current, start); 
  broadcast(world, elapsed, 0); //So they all stop at the same iteration

  fitrank=std::make_pair(currbest, world.rank()); 
  all_reduce(world, fitrank, bestfitrank, mpi::minimum<std::pair<float,int> >()); 

  if (world.rank()==0 && difftime(current, lastprint)>=5) {
      printf("%3d (secs): %8.5f\n", elapsed, bestfitrank.first);
      lastprint=time(NULL); 
  }
} while(elapsed<300); 

Individual best; 
if(bestfitrank.second==0 && world.rank()==0) {
  best=e.get_individual(0); 
} else if(world.rank()==bestfitrank.second) {
  best=e.get_individual(0); 
  world.send(0, 816, best); 
} else if (world.rank()==0) {
  world.recv(bestfitrank.second, 816, best); 
}

if(world.rank()==0) {
  best.show((char*) "Best soln"); 
  FILE* nodefile = fopen("bestnodelist.txt", "w"); 
  printf("Best Nodelist: "); 
  for(int i=0; i<gampi_nodelist.size(); i++) {
    fprintf(nodefile, "%5d\n",best.reordered_nodeid_at(i));
    printf("%d,",best.reordered_nodeid_at(i));
  }
  printf("\n");
  fclose(nodefile);
  /* ---------------------------------------------------------------------- */
  FILE* lammpsfile = fopen("nodelist_lammps.txt", "w"); 
  for(int i=0; i<gampi_nodelist.size(); i++) {
    Subdomain sd=gampi_domain.get_subdomain(i); 
    int coors[3]; 
    sd.get_coors(coors);
    fprintf(lammpsfile, "%d %d %d %d\n",best.reordered_nodeid_at(i),coors[0], coors[1], coors[2]);
  }
  fclose(lammpsfile);
}

world.barrier(); 
} //end MAIN MAIN MAIN
