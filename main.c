
/*
 * Copyright (C) 2013 Jordan Angel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU
 * General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>. */
#include<stdio.h>
#include<stdlib.h>
#include"utils.c"
#include<mpi.h>
#include<time.h>
int main(int argc, char *argv[]) 

{

  MPI_Init(&argc,&argv); 
  int n,m,p;
  n=12;m=4;p=1;
  int PopSize=1500;
  int MaxGen=300;
  int N=n*m*p;
  FILE *node_file;
  FILE *subset_file;  
  int nid[19200];
  int xcoors[19200];
  int ycoors[19200];
  int zcoors[19200];
  int topology[6*n*m*p];
  int i,j,k;
  int NumOfElites=100,elites[NumOfElites]; 
  int node_count; 

  int id,np; 

  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
//  printf("Init!\n");
  long idum=(long)(time(NULL)*id);
  if (id==0){
    node_file = fopen("/home/Spring13/ORNL/data/coords.txt","r");
    if (node_file==NULL){
      printf("Unable to open file");
      exit(1);
    }  
    get_coords(node_file,xcoors,ycoors,zcoors,nid); 
    fclose(node_file);
    subset_file = fopen("/home/Spring13/ORNL/data/nodes_1_1_24_1_2_1.txt","r");
    if (subset_file == NULL){ 
      printf("Unable to open file");
      exit(1);
    } 
    node_count=count_lines(subset_file); 
  }
  MPI_Bcast((void *)&node_count,1,MPI_INT,0,MPI_COMM_WORLD);  
  int subset_nodes[node_count];
  if (id==0){ 
    rewind(subset_file);
    get_subset(subset_file,subset_nodes,node_count); 
    fclose(subset_file); 
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Bcast((void *)xcoors,19200,MPI_INT,0,MPI_COMM_WORLD); 
  MPI_Bcast((void *)ycoors,19200,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast((void *)zcoors,19200,MPI_INT,0,MPI_COMM_WORLD); 
  MPI_Bcast((void *)subset_nodes,node_count,MPI_INT,0,MPI_COMM_WORLD);

  topomat3d(topology,n,m,p);
  
  int *Pop[PopSize]; 
  create_pop(node_count,subset_nodes,PopSize,Pop,id); 

  int* bAssign;
  double bFit=INT_MAX,nFit;
  int* mutants[PopSize-NumOfElites];  
  int* NextGen[PopSize]; 
  for(j=0;j<MaxGen;j++){
//  for(i=0;i<PopSize;i++){
//    NextGen[i]=malloc(node_count*sizeof(int));
//  }
 
    nFit=tournament(N,Pop,PopSize,xcoors,ycoors,zcoors,topology,elites,NumOfElites);
    if (nFit<bFit){bFit=nFit;bAssign=&Pop[elites[0]][0];}

    for(i=0;i<NumOfElites;i++){
      NextGen[i]=&Pop[elites[i]][0];
//      for(k=0;k<node_count;k++){
//        NextGen[i][k]=Pop[elites[i]][k];
//      }
    }
    for(i=NumOfElites;i<PopSize;i++){   
      mutants[i-NumOfElites]=malloc(node_count*sizeof(int));
      double r1=ran3(&idum);
      double r2=ran3(&idum);
      int rn=(int)(r2*NumOfElites);
      if (r1>.500){ 
        NextGen[i]=mutate_swap(NextGen[rn],node_count,mutants[(i-NumOfElites)]); 
      }else{ 
        NextGen[i]=mutate_inject(NextGen[rn],node_count,mutants[(i-NumOfElites)]); 
      }  
    } 
    for(i=0;i<PopSize;i++){
//    for(k=0;k<node_count;k++){ 
//      Pop[i][k]=NextGen[i][k];   
//    }
//      free(NextGen[i]);
      Pop[i]=&NextGen[i][0];
    } 
    
  } 
  
  char out[20];
  sprintf(out,"%d.out",id);
  FILE* outfile=fopen(out,"w");
  fprintf(outfile,"%d's bFit:%f \n",id,bFit);
  for(i=0;i<n;i++){
    fprintf(outfile," %d  %d  %d  %d\n",zcoors[bAssign[n*0+i]],zcoors[bAssign[n*1+i]],zcoors[bAssign[n*2+i]],zcoors[bAssign[n*3+i]]);
  } 
  fclose(outfile);



  MPI_Finalize();    
  return 0; 
}


