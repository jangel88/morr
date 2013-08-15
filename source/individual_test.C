#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "individual.h"

Domain gampi_domain(1,1,1); 
std::vector<nodeid> gampi_nodelist;

using namespace std; 

int main(int argc, char **argv){

cout << "usage:  ./individual_test.exe testfilename\n";

FILE* nodefile = fopen(argv[1], "r"); 
if (nodefile==NULL) {
  cout << "error opening file "<<argv[1] << "\n"; 
  return 1; 
}

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
      gampi_nodelist.push_back((nodeid) atoi(string));
    } 
  }
} 
gampi_domain=Domain(max_i, max_j, max_k); 

Individual a(gampi_domain.get_size()); 
a.show(); 

Individual b(gampi_domain.get_size(), true); 
b.show(); 

Individual c(a, true); 
c.show(); 

c.mutate(); 
c.show(); 

} // end main

