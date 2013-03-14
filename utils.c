#include"utils.h"
#include<math.h>
#include<limits.h>
#include<mpi.h>
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/

void mutate(  int elite[],   int chromo_length,   int mutant[],int id)
{
  float r1=rand()/(float)RAND_MAX;// coin flip 
  int z_range[2]={1,2};//,4};//,6,8,16};
  int z=z_range[rand_int_inclusive(0,1)];
  int x=rand_int_inclusive(0,chromo_length-1);
  int y=rand_int_inclusive(0,chromo_length-1);
  if(r1>.500){
    mutate_swap(elite,chromo_length,mutant);
  }else{
    mutate_inject(elite,chromo_length,mutant,x,y,z);
  }      
}

void copy_inject(  int elite[],   int chromo_length,   int mutant[],int start, int stop, int z, int mirr_flag)
{
  int i;
  if(start>stop){
    printf("Error! copy_inject: Start > Stop\n");
    exit(1);
  }
  z=MIN(z,(chromo_length-stop));

  for(i=0;i<chromo_length;i++){
    if (i<start){ 
      mutant[i]=elite[i];
    }else if(i>=start && i<stop){
      mutant[i]=elite[i+z];
    }else if(i>=stop && i<(stop+z)){
      mutant[i]=elite[start+(1-2*mirr_flag)*(i-stop)+mirr_flag*(z-1)];
    }else{
      mutant[i]=elite[i];
    }
  }
}

void mutate_inject(  int elite[],   int chromo_length,   int mutant[],int x, int y, int z)
{

  int mirr_flag=rand_int_inclusive(0,1);
  if (x<y){
    copy_inject(elite,chromo_length,mutant,x,y,z,mirr_flag);
  }else{
    copy_inject(elite,chromo_length,mutant,y,x,z,mirr_flag);
  } 
}

void mutate_swap(  int elite[],   int chromo_length,   int mutant[])
{
  int i,j,tmp,M,x,y;
  M=rand_int_inclusive(0,4);
//M=0;
  for(i=0;i<=M;i++){ 
    x=rand_int_inclusive(0,chromo_length-1);
    y=rand_int_inclusive(0,chromo_length-1);
    tmp=elite[x]; 
    for(j=0;j<chromo_length;j++){
      if(j==x){
        mutant[j]=elite[y];
      }else if(j==y){
        mutant[j]=tmp;
      }else{
        mutant[j]=elite[j];
      }
    } 
  }
}



int rand_int_inclusive(int min, int max)
{
  int r;
  while (1){ 
    r=(rand()/(float)RAND_MAX)*(max-min+1)+min;
    if(r>=min && r<=max){ // Make sure r is in bounds
      return r;
    }
  } 
}


float tournament(  int chromo_length,   int* pop[],   int pop_size,domain* space,  int elites[],   int n_elites)
{

  float norm2(float cost[],   int chromo_length);
  int compare(const void * a, const void * b);
  void computeCost(float cost[],   int assignment[],   int topology[],    int chromo_length);

  float fit[pop_size],tmp;
  float *ind[pop_size]; 
  int i,j,k;
  float* cost=malloc(sizeof(float)*chromo_length*6);
  float b_fit=INT_MAX; 
  for(i=0;i<pop_size;i++){  // Compute fitness of each individual
    computeCost(cost,&pop[i][0],space->topology,chromo_length);
    fit[i]=norm2(cost,chromo_length*6); 
    ind[i]=&fit[i];
    b_fit=(fit[i]<b_fit) ? fit[i] : b_fit; 
  } 
  free(cost);
  qsort(ind,pop_size,sizeof(float *),compare); // Sort by fit
  for(i=0;i<pop_size;i++){
    elites[i]=ind[i]-fit; // recover indices
  }

  return b_fit;
}



int compare(const void * a, const void * b)
{
  float va=**(const float**) a;
  float vb=**(const float**) b;
  return (va>vb)-(va<vb);
}


void create_pop(int node_count, int nodes[],   int pop_size,   int* pop[])
{
  int i,j;
  int r[node_count];
  void randperm(int a, int b, int r[]);
  for(i=0;i<pop_size;i++){
    randperm(0,node_count,r);
    pop[i]=malloc(node_count*sizeof(int)); 
    for(j=0;j<node_count;j++){
      pop[i][j]=nodes[r[j]];      
    } 
  }
}

void randperm(int a, int b, int r[])
{
  int i,j;
  float tmp,rn; 
  for(j=0;j<(b-a+1);j++){
    r[j]=a+j;
  }
  for(i=a;i<b;i++){
     rn=round((b-a))*rand()/(float)RAND_MAX;
     tmp=r[i];
     r[i]=r[(int) rn];
     r[(int) rn]=tmp;
  }
}

float norm2(float cost[],   int chromo_length)
{
  int i;
  float norm=0.0;
  for (i=0;i<chromo_length;i++){
    norm+=(float) cost[i]*(float) cost[i];
  }
  return sqrt(norm);
}

float avg_cost(float cost[],   int chromo_length)
{
   int i;
   float tmp=0;
   for (i=0;i<chromo_length;i++){
     tmp+=cost[i];
   }
   return tmp/chromo_length;
}

void computeCost(float cost[],   int assignment[],   int topology[],   int chromo_length)
{
  int r,i,j,k;
  for (k=0; k<6; k++){
    for (r=0; r<chromo_length; r++){
      j=assignment[topology[r+chromo_length*k]];
      i=assignment[r];
      cost[r+chromo_length*k]=distance_between_nodes((nodeid) i,(nodeid) j);
    } 
  }
}                 

void get_subset(FILE *fp, int subset_nodes[], int node_count)
{
  int i; 
  for (i=0;i<node_count;i++){
    fscanf(fp,"%d",&subset_nodes[i]);
  }
}

int count_lines(FILE *fp)
{
  int line_count=0;
  int ch;
  while ((ch=fgetc(fp))!=EOF){
    if (ch=='\n' || ch=='\r'){
       ++line_count;  
    }
  }
  return line_count; 
}



void print_solution(const int* zcoors,float b_fit,   int* b_assign, int node_count,int id, int np,domain* space)
{
  int i,k,m=space->max_i; 
  int* all_solutions=(int*)malloc(node_count*np*sizeof(int));
  float* all_fitness=calloc(np,sizeof(float)); 
  MPI_Gather(b_assign,node_count,MPI_INT,all_solutions,node_count,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Gather(&b_fit,1,MPI_FLOAT,all_fitness,1,MPI_FLOAT,0,MPI_COMM_WORLD);
  if(id==0){ 
    float global_best=INT_MAX; 
    int best_rank;
    for(k=0;k<np;k++){
      if(all_fitness[k]<global_best){
        global_best=all_fitness[k]; best_rank=k;
      }
    } 
    printf("%d found best: %f\n",best_rank,global_best); 
    for(i=0;i<m;i++){
      fprintf(stdout," %d  %d  %d  %d\n",zcoors[3*all_solutions[m*0+i+best_rank*node_count]],zcoors[3*all_solutions[m*1+i+best_rank*node_count]],zcoors[3*all_solutions[m*2+i+best_rank*node_count]],zcoors[3*all_solutions[m*3+i+best_rank*node_count]]);
    }
  } 
  free(all_solutions);
  free(all_fitness);
}



