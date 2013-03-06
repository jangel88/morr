#include"utils.h"
#include<math.h>
#include<limits.h>


#define XDIM 25 // Dimension of toroidal interconnect
#define YDIM 16
#define ZDIM 24
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) /* ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!*/


//typedef int nodeid; 
float distance_between_nodes(nodeid, nodeid); 
/* If statements are mostly structured as
 * if (cond) {command;} in one line for compactness*/



int* mutate(int elite[], int N, int mutant[],int id)
{



  double r1=rand()/(double)RAND_MAX;// coin flip 
  int z_range[2]={1,2};//,4};//,6,8,16};
  int z=z_range[rand_int_inclusive(0,1)];
  int x=rand_int_inclusive(0,N-1);
  int y=rand_int_inclusive(0,N-1);
  int* mut;
  if (r1>.500){
    mut=mutate_swap(elite,N,mutant);
//      printf("Swap\n");
    }else{// if(r1>.333){ 
    mut=mutate_inject(elite,N,mutant,x,y,z);
//      printf("Inject\n");
//  }else{
//    mut=mutate_mirror(elite,N,mutant,z);
//    printf("Mirror\n");
    }      
  return mut; 
}
int* copy_inject(int elite[], int N, int mutant[],int start, int stop, int z, int mirr_flag)
{
  int i;
  if(start>stop){
    printf("Error! Start > Stop\n");
    exit(1);
  }
  z=MIN(z,(N-stop));
//printf("z: %d\n",z); 
  for(i=0;i<N;i++){
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
  return &mutant[0];
}
int* mutate_inject(int elite[], int N, int mutant[],int x, int y, int z)
{

  int mirr_flag=rand_int_inclusive(0,1);
//  printf("Mirror flag:%d\n",mirr_flag);
//  mirr_flag=0;
  if (x<y){
    return copy_inject(elite,N,mutant,x,y,z,mirr_flag);
  }else{
    return copy_inject(elite,N,mutant,y,x,z,mirr_flag);
  } 
}



int* mutate_swap(int elite[], int N, int mutant[])
{
  int i,tmp,M,x,y;

  M=rand_int_inclusive(0,1);
  M=0;
  for(i=0;i<=M;i++){ 
    x=rand_int_inclusive(0,N-1);
    y=rand_int_inclusive(0,N-1);
    tmp=elite[x]; 
    for(i=0;i<N;i++){
      if(i==x) {mutant[i]=elite[y];}
      else if (i==y) {mutant[i]=tmp;}
      else {mutant[i]=elite[i];}
    } 
  }
  return &mutant[0]; 
 
}

int* mutate_mirror(int elite[], int N, int mutant[], int z)
{


  int i,j=0;
  int k=rand_int_inclusive(1,N-1);
  int m=MIN(k-z,0);
   
  for(i=0;i<N;i++){
    if(i<m){
      mutant[i]=elite[i];
    }else if(i>=m && i<=k){
      mutant[i]=elite[k-j];
      j++;
    }else{
      mutant[i]=elite[i];
    }
  }
  return &mutant[0];
}

int rand_int_inclusive(int min, int max)
{

  int r;
 
  while (1){ 
    r=(rand()/(double)RAND_MAX)*(max-min+1)+min;
    if(r>=min && r<=max){ // Make sure r is in bounds
      return r;
    }
  } 
}

double tournament(int N, int* pop[], int pop_size,const int xcoors[],const int ycoors[],const int zcoors[],
                int topology[],int elites[], int num_of_elites)
{

  double norm2(float cost[], int N);
  int compare(const void * a, const void * b);
  void computeCost(float cost[], int assignment[], int topology[],const int xcoors[],
                 const int ycoors[],const int zcoors[],  int N);

  double fit[pop_size],tmp;
  double *ind[pop_size]; 
  int i,j,k;
  float* cost=malloc(sizeof(float)*N*6);
  double b_fit=INT_MAX; 
  for(i=0;i<pop_size;i++){  // Compute fitness of each individual
    computeCost(cost,&pop[i][0],topology,xcoors,ycoors,zcoors,N);
    fit[i]=norm2(cost,N*6); 
    ind[i]=&fit[i];
    b_fit=(fit[i]<b_fit) ? fit[i] : b_fit; 
  } 
  free(cost);
  qsort(ind,pop_size,sizeof(double *),compare); // Sort by fit
  for(i=0;i<pop_size;i++){
    elites[i]=ind[i]-fit; // recover indices
  }

  return b_fit;
}


double roulette(int N, int* pop[], int pop_size,const int xcoors[],const int ycoors[],const int zcoors[],
                int topology[],int elites[], int num_of_elites)
{

  double norm2(float cost[], int N);
  int compare(const void * a, const void * b);
  void computeCost(float cost[], int assignment[], int topology[],const int xcoors[],
                 const int ycoors[],const int zcoors[],  int N);

  double fit[pop_size],scaled_fit[pop_size];
  double *ind[pop_size]; 
  int i,j,k,tmp;

  double total_fit=0.0;
  float* cost=malloc(sizeof(float)*N*6);
  double b_fit=INT_MAX; 
  for(i=0;i<pop_size;i++){  // Compute fitness of each individual
    computeCost(cost,&pop[i][0],topology,xcoors,ycoors,zcoors,N);
    fit[i]=norm2(cost,N*6);   
    total_fit+=fit[i];
    b_fit=(fit[i]<b_fit) ? fit[i] : b_fit; 
  } 
  free(cost);

  j=num_of_elites-1;
  i=0;
  double r1,partial_sum;
  int chosen_one=0;
  int* chosen=calloc(pop_size,sizeof(int));
  while(j>=0){ 
    partial_sum=0;
    chosen_one=0;
    r1=(rand()/(double)RAND_MAX)*total_fit;
    while(r1>partial_sum){
      partial_sum+=fit[chosen_one];
      chosen_one++;
    }
    elites[i]=chosen_one;
    chosen[chosen_one]=1;
    i++;
    j--;
  } 
  k=0;
  for(i=0;i<pop_size;i++){
    if(chosen[i]==0){
      elites[num_of_elites+k]=i;       
      k++;
    }
  }
  return b_fit;
}



int compare(const void * a, const void * b)
{
  double va=**(const double**) a;
  double vb=**(const double**) b;
 
  return (va>vb)-(va<vb);
}

void create_pop(int node_count, int nodes[], int pop_size, int* pop[],int id)
{
  int i,j;
  int r[node_count];
  void randperm(int a, int b, int r[],int id);
  for(i=0;i<pop_size;i++){
    randperm(0,node_count,r,id);
    pop[i]=malloc(node_count*sizeof(int)); 
    for(j=0;j<node_count;j++){
      pop[i][j]=nodes[r[j]];      
    } 
  }
}





void randperm(int a, int b, int r[],int id)
{


  int i,j;

  double tmp,rn; 
  for(j=0;j<(b-a+1);j++){
    r[j]=a+j;
  }

  for(i=a;i<b;i++){
     rn=round((b-a))*rand()/(double)RAND_MAX;
     tmp=r[i];
     r[i]=r[(int) rn];
     r[(int) rn]=tmp;
  }
}

double norm2(float cost[], int N)
{
  int i;
  double norm=0.0;
  for (i=0;i<N;i++){
    norm+=(double) cost[i]*(double) cost[i];
  }
  return sqrt(norm);

}

double avg_cost(float cost[], int N)
{
   int i;
   double tmp=0;
   for (i=0;i<N;i++){
     tmp+=cost[i];
   }
   return tmp/N;
}






void computeCost(float cost[], int assignment[], int topology[],const int xcoors[],
               const int ycoors[],const int zcoors[],  int N)
{
  int r,i,j,k;
  for (k=0; k<6; k++){
    for (r=0; r<N; r++){
      j=assignment[topology[r+N*k]];
      i=assignment[r];
      cost[r+N*k]=distance_between_nodes((nodeid) i,(nodeid) j);
    } 
  }
}                 

void get_subset(FILE *fp, int subset_nodes[], int node_count)
{
  
  int i; 
  for (i=0;i<node_count;i++)
  {
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







