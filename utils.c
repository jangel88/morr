#include<utils.h>
#include<math.h>
#include<limits.h>
#include<time.h>
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)
#define min(a, b) (((a) < (b)) ? (a) : (b)) // ONLY SAFE WITH NON-FUNCTION ARGUMENTS!!

int* mutate_inject(int elite[], int N, int mutant[])
{
  double ran3(long *idum);
  static long ijctidum=11;
  int x=(int)(ran3(&ijctidum)*N);
  int y=(int)(ran3(&ijctidum)*N);
  int i;
  for(i=0;i<N;i++){
    if (x<y){
      if (i<x){mutant[i]=elite[i];}
      else if(i>=x && i<y){mutant[i]=elite[i+1];}
      else if(i==y){mutant[i]=elite[x];}  
      else {mutant[i]=elite[i];}
   }else {
      if (i<y){mutant[i]=elite[i];}
      else if(i>=y && i<x) {mutant[i]=elite[i+1];}
      else if(i==x){mutant[i]=elite[y];}
      else {mutant[i]=elite[i];}
   } 
  } 

  return &mutant[0];  
}

int* mutate_swap(int elite[], int N, int mutant[])
{
  double ran3(long *idum);
  static long swpidum=10;

  int x=(int)(ran3(&swpidum)*N);
  int y=(int)(ran3(&swpidum)*N);

  int i,tmp=elite[x];
//  printf("x: %d y: %d\n",x,y);
  for(i=0;i<N;i++){
    if(i==x) {mutant[i]=elite[y];}
    else if (i==y) {mutant[i]=tmp;}
    else {mutant[i]=elite[i];}
  } 
  return &mutant[0]; 
 
}





double tournament(int N, int* Pop[], int PopSize, int xcoors[], int ycoors[], int zcoors[],int topology[],
                int elites[], int NumOfElites)
{

  double norm2(int cost[], int N);
  int compare(const void * a, const void * b);
  void computeCost(int cost[], int assignment[], int topology[], int xcoors[],
                 int ycoors[], int zcoors[],  int N);

  double fit[PopSize],tmp;
  double *ind[PopSize]; 
  int i,j,k,cost[N*6];;
  double bFit=INT_MAX; 
  for(i=0;i<PopSize;i++){
    computeCost(cost,&Pop[i][0],topology,xcoors,ycoors,zcoors,N);
    fit[i]=norm2(cost,N*6); 
    ind[i]=&fit[i];
    if(fit[i]<bFit){bFit=fit[i];}
  } 
  qsort(ind,PopSize,sizeof(double *),compare);
  for(i=0;i<NumOfElites;i++){
    elites[i]=ind[i]-fit;
  }

  return bFit;
}




int compare(const void * a, const void * b)
{
  double va=**(const double**) a;
  double vb=**(const double**) b;
 
  return (va>vb)-(va<vb);
}

void create_pop(int node_count, int nodes[], int PopSize, int* Pop[],int id)
{
  int i,j;
  int r[node_count];
  
  void randperm(int a, int b, int r[],int id);
  for(i=0;i<PopSize;i++){
    randperm(0,node_count,r,id);
    Pop[i]=malloc(node_count*sizeof(int)); 
    for(j=0;j<node_count;j++){
      Pop[i][j]=nodes[r[j]];      
    } 
  }
}





void randperm(int a, int b, int r[],int id)
{

  double ran3(long *idum);
  int i,j;
  long permidum=id; 
  double tmp,rn; 
  for(j=0;j<(b-a+1);j++){
    r[j]=a+j;
  }

  for(i=a;i<b;i++){
     rn=round((b-a))*ran3(&permidum);
     tmp=r[i];
     r[i]=r[(int) rn];
     r[(int) rn]=tmp;
  }
}

double norm2(int cost[], int N)
{
  int i;
  double norm=0.0;
  for (i=0;i<N;i++){
    norm+=(double) cost[i]*(double) cost[i];
  }
  return sqrt(norm);

}

double avg_cost(int cost[], int N)
{
   int i;
   double tmp=0;
   for (i=0;i<N;i++){
     tmp+=cost[i];
   }
   return tmp/N;
}




double ran3(long *idum)
/* ran3 from Numerical Recipes in C */
{
  static int inext,inextp;
  static long ma[56];
  static int iff=0;
  long mj,mk; 
  int i,ii,k;

  if(*idum<0 || iff==0) {
    iff=1;
    mj=labs(MSEED-labs(*idum));
    mj %= MBIG;
    ma[55]=mj;
    mk=1;
    for (i=1;i<=54;i++) {
       ii=(21*i)%55;
       ma[ii]=mk;
       mk=mj-mk;
       if(mk < MZ) mk += MBIG;
       mj=ma[ii];
    }
    for (k=1;k<=4;k++)
       for (i=1;i<=55;i++) {
          ma[i] -= ma[1+(i+30) % 55];
          if (ma[i] < MZ) ma[i] += MBIG;
        }
    inext=0;
    inextp=31;
    *idum=1;
   }
   if (++inext==56) inext=1;
   if (++inextp==56) inextp=1;
   mj=ma[inext]-ma[inextp];
   if (mj < MZ) mj += MBIG;
   ma[inext]=mj;
   return mj*FAC;

}

void computeCost(int cost[], int assignment[], int topology[], int xcoors[],
                 int ycoors[], int zcoors[],  int N)
{
  int r,i,j,k;

  int dx,dy,dz;
  for (k=0; k<6; k++){
    for (r=0; r<N; r++){
      j=assignment[topology[r+N*k]];
      i=assignment[r];
      dx=fabs(xcoors[j]-xcoors[i]); if (dx>25.0/2.0) {dx=fabs(dx-25);}
      dy=fabs(ycoors[j]-ycoors[i]); if (dy>16.0/2.0) {dy=fabs(dy-16);}
      dz=fabs(zcoors[j]-zcoors[i]); if (dz>24.0/2.0) {dz=fabs(dz-24);}
      cost[r+N*k]=dx+2*dy+dz;

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

void get_coords(FILE *fp, int xcoors[], int ycoors[], int zcoors[], int nid[])
{
  int i,j,k,n,m; 
  int tmp;
  j=0;k=0;n=0;m=0; 
  for(i=0; i<19200*4;i++) 
    {
       
       fscanf(fp,"%d",&tmp);
       if (i % 4 == 0) 
         {
           xcoors[j]=tmp;
           j+=1;
         }
       else if(i % 4 == 1)
         {
           ycoors[k]=tmp;
           k+=1;
         }
       else if(i % 4 == 2)
         {
           zcoors[n]=tmp;
           n+=1;
         }
       else
         {
           nid[m]=tmp;
           m+=1;
         } 
     }
 
}
void get_indices(int id, int indices[], int n, int m, int p)
/* Return (i,j,k) indices based on domain size and id
 */
{ 
  int i,j,k;
  i=id%n; 
  j=(id-i)/n%m;
  k=(id-i-n*j)/(m*n);
  indices[0]=i;
  indices[1]=j;
  indices[2]=k;
//  printf("node: %d had indices: (%d,%d,%d)\n",id,indices[0],indices[1],indices[2]);
}

int get_id(int i, int j, int k, int n, int m, int p) 
{ 
  int id;
  if (p==1) { if (n==1){id=j;} else if (m==1) {id=i;} else {id=n*j+i; }}
  else if (m==1) {if (n==1) {id=k;} else if(p==1) {id=i;} else {id=n*k+i;}}
  else if (n==1) {if (m==1) {id=k;} else if(p==1) {id=j;} else {id=m*k+j;}}
  else id=m*n*k+n*j+i;
  
  return id;
}

void topomat3d(int topology[], int n, int m, int p)
{




  int indices[3];
  int id;
  int r,q;
  int i,j,k;
  int N=n*m*p;
  for (r=0;r<N;r++)
  {
    get_indices(r,indices,n,m,p);
    i=indices[0];j=indices[1];k=indices[2];
    if (k==0)
    {
      if (j!=0 && j!=m-1)
      { 
        if(i!=n-1 && i!=0) 
        {  
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
        else if(i==0) // Front Face, top row, not corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
       else if(i==n-1) // Front face, bottom row, not corner
       {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
  
       }
    }
      else if (j==m-1)
    {
         if(i!=n-1 && i!=0) //Front face, right column, not corner
       {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
       }
         else if(i==0) // Front face, top right corner 
       {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);

       }  
        else if(i==n-1) // Front face, bottom right corner 
        {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
      }
      else if(j==0)
      {
        if(i!=n-1 && i!=0) //Front face, left column, not corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
        else if(i==0) //Top corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);

        }
        else if(i==n-1) //Bottom corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,p-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);

        }
      }
    } 
    else if(k==p-1)
    {
      if (j!=0 && j!=m-1)
      { 
        if(i!=n-1 && i!=0) 
        {  
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);
        }
        else if(i==0) // Rear Face, top row, not corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);
        }
       else if(i==n-1) // Read face, bottom row, not corner
       {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);
  
       }
    }
      else if (j==m-1)
    {
         if(i!=n-1 && i!=0) //Rear face, right column, not corner
       {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);
       }
         else if(i==0) // Rear face, top right corner 
       {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);

       }  
        else if(i==n-1) // Rear face, bottom right corner 
        {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);
        }
      }
      else if(j==0)
      {
        if(i!=n-1 && i!=0) //Rear face, left column, not corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);
        }
        else if(i==0)
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);

        }
        else if(i==n-1)
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,0,n,m,p);

        }
      }
    } 
    else if(k!=0 && k!=p-1)
    {
       if (j!=0 && j!=m-1)
      { 
        if(i!=n-1 && i!=0) 
        {  
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
        else if(i==0) //
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
       else if(i==n-1) //
       {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
  
       }
    }
      else if (j==m-1)
    {
         if(i!=n-1 && i!=0) //Rear face, right column, not corner
       {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
       }
         else if(i==0) // Rear face, top right corner 
       {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);

       }  
        else if(i==n-1) // Rear face, bottom right corner 
        {
          topology[r]=get_id(i,0,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,j-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
      }
      else if(j==0)
      {
        if(i!=n-1 && i!=0) //Rear face, left column, not corner
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);
        }
        else if(i==0)
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(n-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(i+1,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);

        }
        else if(i==n-1)
        {
          topology[r]=get_id(i,j+1,k,n,m,p);
          topology[N+r]=get_id(i-1,j,k,n,m,p);
          topology[2*N+r]=get_id(i,m-1,k,n,m,p);
          topology[3*N+r]=get_id(0,j,k,n,m,p);
          topology[4*N+r]=get_id(i,j,k-1,n,m,p);
          topology[5*N+r]=get_id(i,j,k+1,n,m,p);

        }
      }     

    }
}
}
       
  
  







