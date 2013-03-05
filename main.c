#include<stdio.h>
#include<stdlib.h>
#include"utils.c"
#include<mpi.h>
#include<time.h>
#include<titan_node_info.c>
#include<struct_cart_domain.c>

/* Most if statements are structured as 
 * if (cond) {command;} in one line for
 * compactness*/


int main(int argc, char *argv[]) 

{
 
  MPI_Init(&argc,&argv); 

  int n,m,p;
  n=4;m=24;p=1;
  int pop_size=2000;
  int max_gen=2000;
  int N=n*m*p;
  FILE *node_file;
  FILE *subset_file;  
  int nid[19200];  
  const int* xcoors;
  const int* ycoors;
  const int* zcoors;
  double r1,r2;
  int rn;
  int topology[6*N];
  int i,j,k,q;
  int number_of_runs=5;
  int num_of_elites=800,elites[pop_size]; 
  int node_count; 
  domain space;
  int id,np; 
//xcoors=&titan_node_coords[0][0];
//ycoors=&titan_node_coords[0][1];
//zcoors=&titan_node_coords[0][2];
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&np);





//int A[10]={0,1,2,3,4,5,6,7,8,9};
//int* A_p=malloc(sizeof(int)*10);
//A_p=mutate(A,10,A_p,id);
//for(i=0;i<10;i++){
//  printf("A_p[%d]: %d\n",i,A_p[i]);
//}  
/*  
  for(i=0;i<15;i++){
      printf("x[%d]: %d y[%d]: %d z[%d]: %d\n",i,titan_node_coords[i][0],i,titan_node_coords[i][1],i,titan_node_coords[i][2]);
    }
  printf("Pointers\n");
  for(i=0;i<15;i++){
    printf("x[%d]: %d y[%d]: %d z[%d]: %d\n",i,xcoors[3*i],i,ycoors[3*i],i,zcoors[3*i]);
  }
*/ 
/* 
  topomat3d(topology,n,m,p);
  for(i=0;i<N;i++){
    printf("%d %d %d %d %d %d\n",topology[i],topology[i+N],topology[i+2*N],topology[i+3*N],topology[i+4*N],topology[i+5*N]);
  }
*/

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
  long idum=(long)2*id;

  if (id==0){
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
  MPI_Bcast((void *)subset_nodes,node_count,MPI_INT,0,MPI_COMM_WORLD);
  space=init_domain(m,n,p);
  
  topomat3d(topology,space);
  
  int *pop[pop_size]; 
  

  int* b_assign;
  double b_fit=INT_MAX,n_fit;
  int* mutants[pop_size-num_of_elites];  
  int* next_gen[pop_size]; 

  for(q=0;q<number_of_runs;q++){
    create_pop(node_count,subset_nodes,pop_size,pop,id*(q+1)); 
    for(j=0;j<max_gen;j++){
      n_fit=tournament(N,pop,pop_size,xcoors,ycoors,zcoors,topology,elites,num_of_elites);
      if (n_fit<b_fit){b_fit=n_fit;b_assign=&pop[elites[0]][0];} /*New best solution*/
  
      for(i=0;i<pop_size;i++){
        if(i<num_of_elites){ 
          next_gen[i]=&pop[elites[i]][0]; /* Elite*/
        }else{
          free(&pop[elites[i]][0]); /* Unfit*/
        }                 
      }
      for(i=num_of_elites;i<pop_size;i++){   
        mutants[i-num_of_elites]=malloc(node_count*sizeof(int)); /*populate */       
        r2=ran3(&idum);
        rn=(int)(r2*num_of_elites);
        next_gen[i]=mutate(next_gen[rn],node_count,mutants[(i-num_of_elites)],id); 
      } 
      for(i=0;i<pop_size;i++){
        pop[i]=&next_gen[i][0]; 
      }  
    } 
    int* all_solutions=(int*)malloc(node_count*np*sizeof(int));
    double* all_fitness=calloc(np,sizeof(double)); 
    MPI_Gather(b_assign,node_count,MPI_INT,all_solutions,node_count,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Gather(&b_fit,1,MPI_DOUBLE,all_fitness,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    if(id==0){ 
      double global_best=INT_MAX; 
      int best_rank;
      for(k=0;k<np;k++){
        if(all_fitness[k]<global_best){global_best=all_fitness[k]; best_rank=k;}
      } 
      printf("%d found best: %f\n",best_rank,global_best); 
      for(i=0;i<m;i++){
        fprintf(stdout," %d  %d  %d  %d\n",zcoors[3*all_solutions[m*0+i+best_rank*node_count]],zcoors[3*all_solutions[m*1+i+best_rank*node_count]],zcoors[3*all_solutions[m*2+i+best_rank*node_count]],zcoors[3*all_solutions[m*3+i+best_rank*node_count]]);
      }
    } 
   free(all_solutions);
   free(all_fitness);
 }
 MPI_Barrier(MPI_COMM_WORLD);
 

  
 
//#endif  
  MPI_Finalize(); 
  return 0; 
}


