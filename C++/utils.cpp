#include"utils.h"


bool comparotor (const fit_pair& i, const fit_pair& j)
  { return i.first < j.first; }

float norm2(float cost[], int length)
{
  int i;
  float norm=0.0;
  for (i=0;i<length;i++){
    norm+=(float) cost[i]*(float) cost[i];
  }
  return sqrt(norm);
}

float tournament(int chromo_length,Population* pop, Domain* space,int elites[],int n_elites)
{
  float fit;
  std::vector<fit_pair> ind_fit;
  int i;
  float* cost=(float*)malloc(sizeof(float)*chromo_length*6);
  float b_fit=1e6;
  for(i=0;i<pop->p_size;i++){
    compute_cost(cost,pop->Pop[i].chromosome,space->topology,chromo_length);
    fit=norm2(cost,chromo_length*6);
    ind_fit.push_back(std::make_pair(fit,i));
  }
  std::sort(ind_fit.begin(),ind_fit.end(),comparotor);
  for(i=0;i<pop->p_size;i++){
    elites[i]=ind_fit[i].second;
  }
  return ind_fit[0].first;
}
void compute_cost(float cost[], std::vector<int> chromosome, std::vector<int> topology, int chromo_length)
{
  int r,i,j,k;
  for (k=0; k<6; k++){
    for (r=0; r<chromo_length; r++){
      j=chromosome[topology[r+chromo_length*k]];
      i=chromosome[r];
      cost[r+chromo_length*k]=distance_between_nodes((nodeid) i,(nodeid) j);
    } 
  }
}

void mutate_swap(std::vector<int>& mutant)
{
  int x,y,tmp;
  x=(rand()/(float)RAND_MAX)*mutant.size();
  y=(rand()/(float)RAND_MAX)*mutant.size(); 
  tmp=mutant[x];
  mutant[x]=mutant[y];
  mutant[y]=tmp; 
}

void mutate_inject(std::vector<int>& mutant)
{
  int z=2;
  std::swap_ranges(mutant.begin()+1,mutant.end()-1,mutant.begin()+2); 

}
