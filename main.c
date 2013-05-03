#include<struct_pop.c>
#include<struct_cart_domain.c>
#include<titan_node_info.c>
#include"utils.c"
#include<mpi.h>
#include<time.h>



int main(int argc, char *argv[]) 

{
 
  MPI_Init(&argc,&argv); 

  double t_start,t_stop;
  int n,m,p;
  n=4;m=24;p=1;
  int pop_size=1000;
  int max_gen=1000;
  int n_elites=2,elites[pop_size]; 
  int chromo_length=n*m*p;
  int *pop[pop_size]; 
  int *next_gen[pop_size]; 
  int *mutants[pop_size-n_elites];  
  int *b_assign;
  float b_fit=FLT_MAX,n_fit;

  FILE *subset_file;  

  const int *xcoors=&titan_node_coords[0][0];
  const int *ycoors=&titan_node_coords[0][1];
  const int *zcoors=&titan_node_coords[0][2];

  float r1;
  int rn;
  int num_runs=1;
  int i,j,k;

  int node_count; 
  domain space=init_domain(m,n,p);
  int id,np; 

  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&np);

  srand(id);

// Start program
// -------------


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
  b_assign = malloc(node_count*sizeof(int));

  MPI_Barrier(MPI_COMM_WORLD);
  t_start=MPI_Wtime();

  MPI_Bcast((void *)subset_nodes,node_count,MPI_INT,0,MPI_COMM_WORLD);

  alloc_pop(node_count,pop,pop_size); 

  for(k=0;k<num_runs;k++){
    create_pop(node_count,subset_nodes,pop_size,pop); 
    for(j=0;j<max_gen;j++){
      n_fit=tournament(chromo_length,pop,pop_size,&space,elites,n_elites);
      if (n_fit<b_fit){ /*New best solution*/
        b_fit=n_fit;
        copy_best(node_count,pop,elites,b_assign); 
      }
      for(i=0;i<pop_size;i++){
        if(i<n_elites){ 
          next_gen[i]=pop[elites[i]]; /* Elite*/
        }else{
          free(pop[elites[i]]); /* Unfit*/
        }                 
      }
      for(i=n_elites;i<pop_size;i++){   /*populate */       
        mutants[i-n_elites]=malloc(node_count*sizeof(int)); 
        r1=rand()/(float)RAND_MAX;
        rn=(int)(r1*n_elites);
        mutate(next_gen[rn],node_count,mutants[(i-n_elites)],id); 
        next_gen[i]=mutants[i-n_elites];
      } 
      for(i=0;i<pop_size;i++){
        pop[i]=next_gen[i]; 
      }  
    } 
  } 
  free_pop(pop_size,pop);
  print_solution(zcoors,b_fit,b_assign,node_count,id,np,&space);
  MPI_Barrier(MPI_COMM_WORLD);
  t_stop=MPI_Wtime(); 
  free(b_assign);

//if(id==0){
//  printf("Elasped time: %f\n",t_stop-t_start);
//} 

  MPI_Finalize(); 
  return 0; 
}


