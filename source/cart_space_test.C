#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "cart_space.h"

using namespace std; 

int main(int argc, char **argv){

cout << "usage:  ./cart_space_test.exe testfilename\n";

FILE* nodefile = fopen(argv[1], "r"); 
if (nodefile==NULL) {
  cout << "error opening file "<<argv[1] << "\n"; 
  return 1; 
}
std::vector<nodeid> nodelist; 
int nodecount, max_i, max_j, max_k;

char string[100];
int line_count=0;
while( fgets(string,100,nodefile)!=NULL){
  if(string[0]=='#' || string[0]=='\n'){
    cout << string; 
    continue;
  }else{
    line_count+=1;
    if(line_count==1){ 
      sscanf(string,"%d%d%d%d",&nodecount,&max_i,&max_j,&max_k); 
    }else{
      nodelist.push_back((nodeid) atoi(string));
    } 
  }
} 

Domain testdomain(max_i, max_j, max_k); 
cout << "Domain size is " << testdomain.get_size() << endl; 
cout << "Domain period is " << testdomain.get_period() << endl; 
float fitness= testdomain.get_fitness(nodelist);
cout << "Domain ideal fitness is " << fitness << endl; 

srand(233); 
int i=rand()%(max_i*max_j*max_k); 
printf("position i is %d\n", i); 
Subdomain a(i, max_i, max_j, max_k); 
int c[3], d[3]; 
a.get_coors(c); 
printf("coords 1st method: %d %d %d\n", c[0], c[1], c[2]); 
Subdomain b=testdomain.get_subdomain(i); 
b.get_coors(d); 
printf("coords 2nd method: %d %d %d\n", d[0], d[1], d[2]); 

} // end main

