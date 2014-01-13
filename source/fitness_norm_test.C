#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "cart_space.h"

int main(int argc, char **argv){

int max_i, max_j, max_k;
int opt;

std::cout << "Usage: fitness_norm.exe  -X max_i -Y max_j -Z max_k \n"; 
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

FILE* nodefile = stdin;
std::vector<nodeid> nodelist;

char string[100];
int line_count=0;
while( fgets(string,100,nodefile)!=NULL){
  if(string[0]=='#' || string[0]=='\n'){
    std::cout << string; 
    continue;
  }else{
    line_count+=1;
      nodelist.push_back((nodeid) atoi(string));
  }
} 
if(max_i*max_j*max_k != nodelist.size()) {
  std::cout << "Error!: maxijk != nodelist size \n"; 
  return 1;
}

float s3d_fitness   =Domain(max_i, max_j, max_k).get_fitness(nodelist); 

printf("%7.3f \n", s3d_fitness); 
} // end main

