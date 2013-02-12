typedef struct domain 
{
  int max_i,max_j,max_k;
} domain;
typedef struct subdomain
{
  int i,j,k;
} subdomain;

typedef enum metric{
  norm, avg, max
} metric; 

domain init_domain(int max_i, int max_j, int max_k);
int validate_domain_size(domain space, int node_count);
subdomain get_domain_coord(int position,domain space);
//float cost_metric(nodeid* node_list,metric);  




