typedef struct domain 
{
  int max_i,max_j,max_k;
  int* topology;
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
int validate_subdomain(subdomain element, domain space);
int get_position(subdomain element,domain space);
void find_neighbors(domain space, subdomain element, subdomain* neighbor);

void topomat3d(domain* space);
subdomain get_domain_coord(int position,domain space);
//float cost_metric(nodeid* node_list,metric);  




