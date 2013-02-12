#include<stdio.h>
#include<stdlib.h>
#include"utils.c"
#include<mpi.h>
/* Most if statements are structured as 
 * if (cond) {command;} in one line for
 * compactness*/


int main(int argc, char *argv[]) 

{

  MPI_Init(&argc,&argv); 
  int n,m,p;
  n=12;m=4;p=1;
  int PopSize=1000;
  int MaxGen=500;
  int N=n*m*p;
  FILE *node_file;
  FILE *subset_file;  
  int nid[19200];
  int xcoors[19200];
  int ycoors[19200];
  int zcoors[19200];
  int topology[6*n*m*p];
//  int topology2[6*n*m*p]; 
  int i,j,k;
  int NumOfElites=10,elites[PopSize]; 
  int node_count; 

  int id,np; 

/* Some code used to test functions in utils.c 
 * ------------------------------------------*/

/*  topomat3d(topology,n,m,p);
  topomat3d_new(topology2,n,m,p);
  printf("old\n");
  for(i=0;i<n;i++){
    printf(" %d %d %d %d %d %d\n",topology[i+0*N],topology[i+1*N],topology[i+2*N],topology[i+3*N],topology[i+4*N],topology[i+5*N]); 
  } 
  printf("new\n");
  for(i=0;i<n;i++){
    printf(" %d %d %d %d %d %d\n",topology2[i+0*N],topology2[i+1*N],topology2[i+2*N],topology2[i+3*N],topology2[i+4*N],topology2[i+5*N]); 
  } 
*/

/*  int test[10]={0,1,2,3,4,5,6,7,8,9};
  int mut=malloc(10*sizeof(int));
  int* p_mut;
  p_mut=mutate_inject(test,10,mut);
  for(i=0;i<10;i++){
    printf("mut[%d]: %d\n",i,p_mut[i]);
  }
*/
//#if 0

// Start program
// -------------
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&np);

  long idum=(long)id;

  if (id==0){
    node_file = fopen("/home/Spring13/ORNL/data/coords.txt","r");
    if (node_file==NULL){
      printf("Unable to open file");
      exit(1);
    }  
    get_coords(node_file,xcoors,ycoors,zcoors,nid); 
    fclose(node_file);
    subset_file = fopen("/home/Spring13/ORNL/data/nodes_1_2_24_1_2_1.txt","r");
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
    nFit=tournament(N,Pop,PopSize,xcoors,ycoors,zcoors,topology,elites,NumOfElites);
    if (nFit<bFit){bFit=nFit;bAssign=&Pop[elites[0]][0];} /*New best solution*/

    for(i=0;i<PopSize;i++){
      if(i<NumOfElites){ NextGen[i]=&Pop[elites[i]][0];} /* Elite*/
      else {free(&Pop[elites[i]][0]);}                  /* Unfit*/
    }
    for(i=NumOfElites;i<PopSize;i++){   
      mutants[i-NumOfElites]=malloc(node_count*sizeof(int)); /*Populate */
      double r1=ran3(&idum);
      double r2=ran3(&idum);
      int rn=(int)(r2*NumOfElites);
      if (r1>.500){ /* coin flip */
        NextGen[i]=mutate_swap(NextGen[rn],node_count,mutants[(i-NumOfElites)]); 
      }else{ 
        NextGen[i]=mutate_inject(NextGen[rn],node_count,mutants[(i-NumOfElites)]); 
      }  
    } 
    for(i=0;i<PopSize;i++){
      Pop[i]=&NextGen[i][0]; 
    }  
  } 
  int* all_solutions=(int*)malloc(node_count*np*sizeof(int));
  double* all_fitness=calloc(np,sizeof(double)); 
  MPI_Gather(bAssign,node_count,MPI_INT,all_solutions,node_count,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Gather(&bFit,1,MPI_DOUBLE,all_fitness,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  if(id==0){ 
    double global_best=INT_MAX; 
    int best_rank;
    for(k=0;k<np;k++){
      if(all_fitness[k]<global_best){global_best=all_fitness[k]; best_rank=k;}
    } 
    printf("%d found best: %f\n",best_rank,global_best); 
    for(i=0;i<n;i++){
      fprintf(stdout," %d  %d  %d  %d\n",zcoors[all_solutions[n*0+i+best_rank*node_count]],zcoors[all_solutions[n*1+i+best_rank*node_count]],zcoors[all_solutions[n*2+i+best_rank*node_count]],zcoors[all_solutions[n*3+i+best_rank*node_count]]);
    }
  } 

    
  
 MPI_Finalize();  
//#endif  
  return 0; 
}


