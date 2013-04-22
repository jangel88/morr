
typedef int nodeid; 
int rand_int_inclusive(int min, int max);
void copy_inject(  int elite[],  int N,   int mutant[],int start, int stop, int length, int mirr_flag);
void mutate(  int elite[],   int N,   int mutant[],int id);
void mutate_inject(  int elite[],   int N,   int mutant[],int x, int y,int length);
void mutate_mirror(  int elite[],   int N,   int mutant[], int length);
void mutate_swap(  int elite[],   int N,   int mutant[]);
void create_pop(int node_count, int nodes[],   int pop_size,   int* pop[]);
float tournament(  int N,   int* pop[],   int pop_size,domain* space,  int elites[],   int n_elites);
void randperm(int a, int b, int r[]);
float norm2(float cost[],   int N);

int compare(const void * a, const void * b);

float avg_cost(float cost[],   int N);

void copy_best(int node_count, int* pop[],int elites[],int* b_assign );

void alloc_pop(int node_count, int *pop[], int pop_size);


void compute_cost(float cost[],   int assignment[],   int topology[],  int N,int print_flag);

void get_subset(FILE *fp, int subset_nodes[], int node_count);

int count_lines(FILE *fp);

void print_solution(const int* zcoors,float b_fit,   int* b_assign, int node_count,int id, int np,domain*);
//void get_coords(FILE *fp,const int xcoors[],const int ycoors[],const int zcoors[], int nid[]);

void free_pop(int pop_size, int *pop[]);

void get_indices(int id, int indices[], int n, int m, int p);

int get_id(int i, int j, int k, int n, int m, int p);

void print_cost_matrix(float cost[], int assignment[],int topology[], int chromo_length);


//void topomat3d(int topology[], int n, int m, int p); 
// Using topomat3d from source/struct_cart_domain.c
