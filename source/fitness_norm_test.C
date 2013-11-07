#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "cart_space.h"


int main(int argc, char **argv){

//FILE* nodefile = fopen(argv[1], "r"); 
//if (nodefile==NULL) {
//  cout << "error opening file "<<argv[1] << "\n"; 
//  return 1; 
//}
FILE* nodefile = stdin;
std::vector<nodeid> nodelist;

int nodecount, max_i, max_j, max_k;

char string[100];
int line_count=0;
while( fgets(string,100,nodefile)!=NULL){
  if(string[0]=='#' || string[0]=='\n'){
    std::cout << string; 
    continue;
  }else{
    line_count+=1;
    //if(line_count==1){ 
    //  sscanf(string,"%d%d%d%d",&nodecount,&max_i,&max_j,&max_k); 
    //}else{
      nodelist.push_back((nodeid) atoi(string));
    //} 
  }
} 
//max_i=8; max_j=8; max_k=8; //for the 512-node S3D case
max_i=8; max_j=16; max_k=16; //for the 2048-node S3D case
if(max_i*max_j*max_k != nodelist.size()) {
  std::cout << "Error!: maxijk != nodelist size \n"; 
  return 1;
}

float s3d_fitness   =Domain(max_i, max_j, max_k).get_fitness(nodelist); 
float lammps_fitness=Domain(max_k, max_j, max_i).get_fitness(nodelist); 

//printf("%7.3f ", s3d_fitness); 
printf("%7.3f %7.3f ", s3d_fitness, lammps_fitness); 
} // end main

